//Engine.cpp

#include "stdafx.h"
#include "Engine.hpp"
#include "ServiceLocator.hpp"
#include "Map.hpp"
#include "Pathfinder.hpp"
#include "Object.hpp"
#include "Visibility.hpp"
#include "Physics.hpp"
#include "Settings.hpp"

#include "ClanManager.hpp"
#include "Clan.hpp"
#include "Scout.hpp"
#include "Axeman.hpp"

#include <Windows.h>

namespace bjoernligan
{
	namespace system
	{
		Engine::Engine()
			: m_physics(nullptr)
		{
			m_xDrawManager = nullptr;
			m_xSpriteManager = nullptr;
			m_xUIManager = nullptr;
			m_xKeyboard = nullptr;
			m_xMouse = nullptr;
			m_xUtility = nullptr;
		}

		Engine::~Engine()
		{
		}

		bool Engine::Initialize()
		{
			m_xDrawManager = DrawManager::Create();
			m_xSpriteManager = SpriteManager::Create();
			m_xKeyboard = input::Keyboard::Create();
			m_xMouse = input::Mouse::Create();
			m_xUtility = Utility::Create();
			m_xUIManager = UIManager::Create();

			ServiceLocator<DrawManager>::SetService(m_xDrawManager.get());
			ServiceLocator<SpriteManager>::SetService(m_xSpriteManager.get());
			ServiceLocator<UIManager>::SetService(m_xUIManager.get());
			ServiceLocator<input::Keyboard>::SetService(m_xKeyboard.get());
			ServiceLocator<input::Mouse>::SetService(m_xMouse.get());
			ServiceLocator<Utility>::SetService(m_xUtility.get());
			ServiceLocator<Physics>::SetService(m_physics.get());
			ServiceLocator<Map>::SetService(m_map.get());

			if (!m_xDrawManager->Initialize())
				return false;

			if (!m_xUIManager->Initialize(m_xDrawManager->getWindow()))
				return false;

			float fSpacing = 80.0f;
			m_xUIManager->AddSlider("Social", 1.0f, sf::Vector2f((float)Settings::m_xWindowSize.x - 300.0f, (float)Settings::m_xWindowSize.y - fSpacing*3.0f), 240.0f, 0.0f, 100.0f);
			m_xUIManager->AddSlider("Brave", 1.0f, sf::Vector2f((float)Settings::m_xWindowSize.x - 300.0f, (float)Settings::m_xWindowSize.y - fSpacing*2.0f), 240.0f, 0.0f, 100.0f);
			m_xUIManager->AddSlider("Agressive", 1.0f, sf::Vector2f((float)Settings::m_xWindowSize.x - 300.0f, (float)Settings::m_xWindowSize.y - fSpacing*1.0f), 240.0f, 0.0f, 100.0f);

			m_clanManager = std::make_unique<ClanManager>();
			m_map = std::make_unique<Map>("../data/");
			m_map->load("map.tmx");
			m_physics = std::make_unique<Physics>(0.f, 0.f, m_xDrawManager->getWindow());
			m_pathFinder = std::make_unique<Pathfinder>(m_map->getSize());
			m_visibility = std::make_unique<Visibility>();

			// PATHFINDER
			for (int x = 0; x < m_map->getWidth(); ++x)
			{
				for (int y = 0; y < m_map->getHeight(); ++y)
				{
					Map::Tile* tile = m_map->getLayer("objects")->getTile(x, y);
					if (tile != nullptr)
					{
						m_pathFinder->getGrid().setWalkableAt(x, y, tile->hasProperty("walkable"));
						if (!tile->hasProperty("walkable"))
						{
							sf::Vector2f tileSize = m_map->getTileSize();
							Physics::Params xParams;
							xParams.m_xFixtureDef.friction = 0.2f;
							xParams.m_xFixtureDef.density = 1.0f;
							xParams.m_xFixtureDef.restitution = 1.0f;

							xParams.m_eShapeType = Physics::Box;
							xParams.m_xShapeSize.m_xBox.x = tileSize.x;
							xParams.m_xShapeSize.m_xBox.y = tileSize.y;

							Physics::Body* body = m_physics->createBody(xParams);
							body->setPosition(x * tileSize.x + tileSize.x * 0.5f, y * tileSize.y + tileSize.y * 0.5f);
						}
					}
				}
			}

			// VISIBILITY
			std::vector<Map::Object*> objects = m_map->getObjectGroup("light_segments")->getObjects();
			for (std::size_t i = 0; i < objects.size(); ++i)
			{
				std::vector<sf::Vector2f> points = objects[i]->m_points;
				for (std::size_t k = 0; k < points.size(); ++k)
				{
					if (k != points.size() - 1)
					{
						sf::Vector2f p0(points[k]);
						sf::Vector2f p1(points[k + 1]);
						m_visibility->addSegment(p0, p1);
					}
				}
			}
			m_visibility->create(sf::Vector2f(100, 100), sf::Color::Red);
			
			// CLANS
			std::vector<Map::Object*> spawns = m_map->getObjectGroup("spawns")->getObjects();
			for (std::size_t i = 0; i < spawns.size(); ++i)
			{
				Map::Object* object = spawns[i];
				if (object->hasProperty("team"))
				{
					int teamNumber = std::stoi(object->getProperty("team"));
					teamNumber;
				}
			}

			Clan* clan = m_clanManager->createClan("MacDonald");
			clan->createMember<Scout>();
			clan->createMember<Scout>();
			clan->createMember<Axeman>();
			clan->createMember<Axeman>();
			clan->createMember<Axeman>();

			//// Find a spawn position
			//ClanMemberDef clanMemberDef;
			//clanMemberDef.startPos = m_map->getLayer("spawns")
			//clan->createMember(Clan::SCOUT);

			return m_bRunning = true;
		}

		void Engine::CleanUp()
		{
		}

		void Engine::RunLoop()
		{
			while (m_bRunning && m_xDrawManager->getWindow()->isOpen())
			{
				PollEvents();
				UpdateDeltaTime();

				if (m_xKeyboard->IsDown(sf::Keyboard::Escape))
					m_bRunning = false;

				//Updates
				m_physics->update(m_fDeltaTime);
				m_visibility->update();
				m_xUIManager->Update(m_fDeltaTime);

				//Draw
				m_xDrawManager->ClearScr();
				m_xDrawManager->Draw(m_map.get());
				m_xDrawManager->Draw(m_visibility.get());
				m_physics->draw();
				m_xUIManager->DrawElements();
				m_xDrawManager->Display();

				::Sleep(2);
			}
		}

		void Engine::UpdateDeltaTime()
		{
			m_fDeltaTime = m_xDeltaClock.getElapsedTime().asSeconds();
			if (m_fDeltaTime > 0.02f)
				m_fDeltaTime = 0.02f;
			m_xDeltaClock.restart();
		}

		void Engine::PollEvents()
		{
			sf::Event p_xEvent;
			while (m_xDrawManager->getWindow()->pollEvent(p_xEvent))
			{
				if (p_xEvent.type == sf::Event::Closed)
					m_xDrawManager->getWindow()->close();

				if (p_xEvent.type == sf::Event::MouseMoved)
				{
					m_xMouse->m_xPos = sf::Mouse::getPosition(*m_xDrawManager->getWindow());
				}

				//keyboard keys pressed or released
				if (p_xEvent.type == sf::Event::KeyPressed)
					m_xKeyboard->SetCurrent(p_xEvent.key.code, true);
				if (p_xEvent.type == sf::Event::KeyReleased)
					m_xKeyboard->SetCurrent(p_xEvent.key.code, false);

				//mouse buttons pressed or released
				if (p_xEvent.type == sf::Event::MouseButtonPressed)
					m_xMouse->SetCurrent(p_xEvent.mouseButton.button, true);
				if (p_xEvent.type == sf::Event::MouseButtonReleased)
					m_xMouse->SetCurrent(p_xEvent.mouseButton.button, false);
			}
		}
	}
}
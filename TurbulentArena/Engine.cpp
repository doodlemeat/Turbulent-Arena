//Engine.cpp

#include "stdafx.h"
#include "Engine.hpp"
#include "ServiceLocator.hpp"
#include "Map.hpp"
#include "Pathfinder.hpp"
#include "Object.hpp"
#include "Visibility.hpp"
#include "ClanManager.hpp"
#include "Clan.hpp"
#include "Scout.hpp"
#include "Physics.hpp"
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

			ServiceLocator<DrawManager>::SetService(m_xDrawManager.get());
			ServiceLocator<SpriteManager>::SetService(m_xSpriteManager.get());
			ServiceLocator<input::Keyboard>::SetService(m_xKeyboard.get());
			ServiceLocator<input::Mouse>::SetService(m_xMouse.get());
			ServiceLocator<Utility>::SetService(m_xUtility.get());
			ServiceLocator<Physics>::SetService(m_physics.get());
			ServiceLocator<Map>::SetService(m_map.get());

			if (!m_xDrawManager->Initialize())
				return false;

			m_clanManager = std::make_unique<ClanManager>();
			m_map = std::make_unique<Map>("../data/map.txt");
			m_physics = std::make_unique<Physics>(0.f, 0.f, m_xDrawManager->m_xWindow);
			m_pathFinder = std::make_unique<Pathfinder>(m_map->getSize());
			m_visibility = std::make_unique<Visibility>();

			// PATHFINDER
			for (int x = 0; x < m_map->getWidth(); ++x)
			{
				for (int y = 0; y < m_map->getHeight(); ++y)
				{
					Map::Tile* tile = m_map->getTopmostTile(x, y);
					if (tile != nullptr)
					{
						m_pathFinder->getGrid().setWalkableAt(x, y, tile->isWalkable());
						if (!tile->isWalkable())
						{
							float tileSize = static_cast<float>(m_map->getTileSize());
							Physics::Params xParams;
							xParams.m_xFixtureDef.friction = 0.2f;
							xParams.m_xFixtureDef.density = 1.0f;
							xParams.m_xFixtureDef.restitution = 1.0f;

							xParams.m_eShapeType = Physics::Box;
							xParams.m_xShapeSize.m_xBox.x = static_cast<int32>(tileSize);
							xParams.m_xShapeSize.m_xBox.y = static_cast<int32>(tileSize);

							Physics::Body* body = m_physics->createBody(xParams);
							body->setPosition(static_cast<float>(x)* tileSize + tileSize * 0.5f, static_cast<float>(y)* tileSize + tileSize * 0.5f);
							tile->setPhysicsBody(body);
						}
					}
				}
			}

			// VISIBILITY
			Map::Layer* layer = m_map->getLayer("objects");
			if (layer != nullptr)
			{
				float tileSize = static_cast<float>(m_map->getTileSize());

				// Looping all static objects in map
				for (int x = 0; x < m_map->getWidth(); ++x)
				{
					for (int y = 0; y < m_map->getHeight(); ++y)
					{
						Map::Tile* tile = layer->getTile(x, y);
						if (tile != nullptr)
						{
							sf::Vector2f tilePos = sf::Vector2f(tile->getPosition());
							m_visibility->addSegment(sf::Vector2f(tilePos.x * tileSize, tilePos.y * tileSize), sf::Vector2f((tilePos.x + 1.f) * tileSize, tilePos.y * tileSize));
							m_visibility->addSegment(sf::Vector2f((tilePos.x + 1.f) * tileSize, tilePos.y * tileSize), sf::Vector2f((tilePos.x + 1.f) * tileSize, (tilePos.y + 1.f) * tileSize));
							m_visibility->addSegment(sf::Vector2f((tilePos.x + 1.f) * tileSize, (tilePos.y + 1.f) * tileSize), sf::Vector2f(tilePos.x * tileSize, (tilePos.y + 1.f) * tileSize));
							m_visibility->addSegment(sf::Vector2f(tilePos.x * tileSize, (tilePos.y + 1.f) * tileSize), sf::Vector2f(tilePos.x * tileSize, tilePos.y * tileSize));
						}
					}
				}
			}
			
			// CLANS

			{
				Clan* clan = m_clanManager->createClan("MacDonald");
				clan;
			}

			return m_bRunning = true;
		}

		void Engine::CleanUp()
		{
		}

		void Engine::RunLoop()
		{
			while (m_bRunning && m_xDrawManager->m_xWindow->isOpen())
			{
				PollEvents();
				UpdateDeltaTime();

				if (m_xKeyboard->IsDown(sf::Keyboard::Escape))
					m_bRunning = false;

				//Updates
				m_physics->update(m_fDeltaTime);
				m_visibility->update();

				//Draw
				m_xDrawManager->ClearScr();
				//insert stuff to draw
				m_xDrawManager->Draw(m_map.get());
				m_xDrawManager->Draw(m_visibility.get());
				m_physics->getWorld()->DrawDebugData();
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
			while (m_xDrawManager->m_xWindow->pollEvent(p_xEvent))
			{
				if (p_xEvent.type == sf::Event::Closed)
					m_xDrawManager->m_xWindow->close();

				if (p_xEvent.type == sf::Event::MouseMoved)
				{
					m_xMouse->m_xPos = sf::Mouse::getPosition(*m_xDrawManager->m_xWindow);
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
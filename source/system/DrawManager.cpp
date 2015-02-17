//DrawManager.cpp

#include "stdafx.h"
#include <Settings.hpp>
#include <system\DrawManager.hpp>
#include <string>

namespace bjoernligan
{
	namespace system
	{
		DrawManager::DrawManager()
		{
			m_xWindow = nullptr;
		}

		DrawManager::~DrawManager()
		{
			delete m_xWindow;
			m_xWindow = nullptr;
		}

		DrawManager::Ptr DrawManager::Create()
		{
			return Ptr(new DrawManager);
		}

		bool DrawManager::Initialize()
		{
			m_xWindow = new sf::RenderWindow(sf::VideoMode(Settings::m_xWindowSize.x, Settings::m_xWindowSize.y), "Turbulent Arena");
			if (m_xWindow == nullptr)
			{
				return false;
			}

			m_xFont.loadFromFile("../data/fonts/arial.ttf");

			return true;
		}

		void DrawManager::ClearScr()
		{
			m_xWindow->clear(sf::Color(80, 80, 80));
		}

		void DrawManager::Draw(sf::Drawable* p_xDrawTarget)
		{
			if (p_xDrawTarget == nullptr)
			{
				return;
			}
			m_xWindow->draw(*p_xDrawTarget);
		}

		void DrawManager::Display()
		{
			m_xWindow->display();
		}

		sf::Font* DrawManager::GetFont()
		{
			return &m_xFont;
		}
	}
}
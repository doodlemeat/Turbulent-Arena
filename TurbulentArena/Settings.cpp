//Settings.cpp

#include "stdafx.h"
#include "Settings.hpp"

namespace bjoernligan
{
	sf::Vector2i Settings::m_xWindowSize = sf::Vector2i(1600, 800);
	std::string Settings::m_sSpritePath = "../data/sprites/";
	sf::Vector2i Settings::m_xSliderSize = sf::Vector2i(16, 32);
}
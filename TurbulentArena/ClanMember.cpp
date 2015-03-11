#include "stdafx.h"
#include "ClanMember.hpp"
#include "Agent.hpp"
#include "Map.hpp"

namespace bjoernligan
{
	ClanMember::ClanMember(ai::Sense* sense)
	{
		m_sprite = std::make_unique<sf::Sprite>();
		m_xAgent = new ai::Agent(this, sense);
	}

	ClanMember::~ClanMember()
	{
		if (m_xAgent)
		{
			delete m_xAgent;
			m_xAgent = nullptr;
		}
	}

	void ClanMember::update(float deltatime)
	{
		Object::update(deltatime);

		if (m_sprite)
			m_sprite->setPosition(m_xPos);
		if (m_xAgent)
			m_xAgent->Update(deltatime);
	}
	
	void ClanMember::setBody(Physics::Body* body)
	{
		Object::setBody(body);
		InitSteeringStats();
		
	}
	void ClanMember::InitSteeringStats()
	{
		if (m_xAgent)
		{
			//Init Steering, with Body and Movementstats
			m_xAgent->InitializeSteering(m_xPhysicsBody->m_body, &m_MovementStats);
		}
	}
	void ClanMember::draw(sf::RenderTarget& target, sf::RenderStates states) const
	{
		target.draw(*m_sprite.get(), states);

		if (m_drawPathfinder)
		{
			if (!m_xAgent->m_CurrentPath.isDone())
			{
				sf::Vector2f size = m_xAgent->m_map->getTileSize();
				sf::RectangleShape nodeShape;
				nodeShape.setSize(sf::Vector2f(size.x - 2, size.y - 2));
				nodeShape.setOutlineThickness(1);
				
				for (int i = 0; i < m_xAgent->m_CurrentPath.length; ++i)
				{
					if (i < m_xAgent->m_CurrentPath.currentNode)
					{
						nodeShape.setFillColor(sf::Color(0, 255, 0, 130));
						nodeShape.setOutlineColor(sf::Color(0, 255, 0, 200));
					}
					else
					{
						nodeShape.setFillColor(sf::Color(255, 0, 0, 130));
						nodeShape.setOutlineColor(sf::Color(255, 0, 0, 200));
					}
					nodeShape.setPosition(m_xAgent->m_CurrentPath.nodes[i].x * size.x, m_xAgent->m_CurrentPath.nodes[i].y * size.y);
					target.draw(nodeShape, states);
				}
			}
		}
	}

	sf::Sprite* ClanMember::getSprite()
	{
		return m_sprite.get();
	}

	ai::Agent* ClanMember::getAgent() const
	{
		return m_xAgent;
	}

	void ClanMember::drawPathfinder(bool value)
	{
		m_drawPathfinder = value;
	}
}
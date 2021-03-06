#include "stdafx.h"
#include "Clan.hpp"
#include "AudioManager.hpp"

#include "Scout.hpp"
#include "Axeman.hpp"

namespace bjoernligan
{
	Clan::Clan(const std::string& name, const sf::Color &p_xTeamColor)
		: m_name(name)
		, m_color(p_xTeamColor)
	{
	}

	void Clan::Update(const float &p_fDeltaTime)
	{
		{
			auto itr = m_clanMembers.begin();
			while (itr != m_clanMembers.end())
			{
				if (!(*itr)->m_xCombatStats.Alive())
				{
					RemoveEnlightenedFriend((*itr)->getAgent());
					ServiceLocator<ai::Sense>::GetService()->removeAgent((*itr)->getAgent());
					ServiceLocator<Physics>::GetService()->destroyBody((*itr)->getBody());

					if ((*itr)->GetClass() == ClanMember::EClass::EClassAxeman)
						ServiceLocator<system::AudioManager>::GetService()->PlaySoundFromGroup("Death", 0.6f);
					else if ((*itr)->GetClass() == ClanMember::EClass::EClassScout)
						ServiceLocator<system::AudioManager>::GetService()->PlaySoundFromGroup("DeathGirly", 0.6f);

					itr = m_clanMembers.erase(itr);
					continue;
				}
				(*itr)->update(p_fDeltaTime);
				++itr;
			}
		}
	}

	void Clan::draw(sf::RenderTarget& target, sf::RenderStates states) const
	{
		for (std::size_t i = 0; i < m_clanMembers.size(); ++i)
		{
			target.draw(*m_clanMembers[i], states);
		}
	}

	void Clan::drawHpBars(sf::RenderTarget& target, sf::RenderStates states) const
	{
		for (std::size_t i = 0; i < m_clanMembers.size(); ++i)
		{
			m_clanMembers[i]->drawHpBar(target, states);
		}
	}

	sf::Color Clan::getColor() const
	{
		return m_color;
	}

	std::vector<ClanMember*> Clan::getMembers() const
	{
		std::vector<ClanMember*> members;
		for (std::size_t i = 0; i < m_clanMembers.size(); ++i)
		{
			if (m_clanMembers[i] && m_clanMembers[i]->GetCombat()->Alive())
				members.push_back(m_clanMembers[i].get());
			else
				members.push_back(nullptr);
		}
		return members;
	}

	bool Clan::IsAlive()
	{
		return !m_clanMembers.empty();
	}

	void Clan::SetSocial(const float &p_fNewValue)
	{
		if (!m_selectedAgents.empty())
		{
			for (uint32_t k = 0; k < m_selectedAgents.size(); ++k)
			{
				m_selectedAgents[k]->GetCombat()->setSocial(p_fNewValue);
			}
		}
		else
		{
			for (uint32_t k = 0; k < m_clanMembers.size(); ++k)
			{
				m_clanMembers[k]->GetCombat()->setSocial(p_fNewValue);
			}
		}
	}

	void Clan::SetBrave(const float &p_fNewValue)
	{
		if (!m_selectedAgents.empty())
		{
			for (uint32_t k = 0; k < m_selectedAgents.size(); ++k)
			{
				m_selectedAgents[k]->GetCombat()->setBrave(p_fNewValue);
			}
		}
		else
		{
			for (uint32_t k = 0; k < m_clanMembers.size(); ++k)
			{
				m_clanMembers[k]->GetCombat()->setBrave(p_fNewValue);
			}
		}
	}

	void Clan::SetAgression(const float &p_fNewValue)
	{
		if (!m_selectedAgents.empty())
		{
			for (uint32_t k = 0; k < m_selectedAgents.size(); ++k)
			{
				m_selectedAgents[k]->GetCombat()->setAgression(p_fNewValue);
			}
		}
		else
		{
			for (uint32_t k = 0; k < m_clanMembers.size(); ++k)
			{
				m_clanMembers[k]->GetCombat()->setAgression(p_fNewValue);
			}
		}
	}

	void Clan::SetMoodValues(const ai::Mood::EMoodType &p_eMoodType, const float &p_fNewValue)
	{
		if (!m_selectedAgents.empty())
		{
			for (uint32_t k = 0; k < m_selectedAgents.size(); ++k)
			{
				m_selectedAgents[k]->SetMoodValue(p_eMoodType, p_fNewValue);
			}
		}
		else
		{
			for (uint32_t k = 0; k < m_clanMembers.size(); ++k)
			{
				m_clanMembers[k]->SetMoodValue(p_eMoodType, p_fNewValue);
			}
		}
	}

	void Clan::RemoveEnlightenedFriend(ai::Agent* p_xAgent)
	{
		if (p_xAgent->getOwner()->GetClass() == ClanMember::EClass::EClassScout)
			return;

		auto itr = m_clanMembers.begin();
		while (itr != m_clanMembers.end())
		{
			if ((*itr)->GetClass() == ClanMember::EClass::EClassScout)
				static_cast<Scout*>((*itr).get())->RemoveEnlightenedFriend(p_xAgent);
			++itr;
		}
	}

	const std::string &Clan::GetName() const
	{
		return m_name;
	}

	void Clan::SelectAgentsInRect(const sf::FloatRect &p_xShape)
	{
		m_selectedAgents.clear();
		for (uint32_t k = 0; k < m_clanMembers.size(); ++k)
		{
			if (m_clanMembers[k]->getSprite() && m_clanMembers[k]->getSprite()->getGlobalBounds().intersects(p_xShape))
			{
				m_clanMembers[k]->SetSelection(true);
				m_selectedAgents.push_back(m_clanMembers[k].get());
			}
			else
				m_clanMembers[k]->SetSelection(false);
		}
	}

	void Clan::DamageRandomMember(const float &p_iAmount)
	{
		if (m_clanMembers.empty())
			return;

		m_clanMembers[random::random(0, m_clanMembers.size() - 1)]->m_xCombatStats.TakeDamage(p_iAmount);
	}
}
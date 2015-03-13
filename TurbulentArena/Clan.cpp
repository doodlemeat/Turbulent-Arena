#include "stdafx.h"
#include "Clan.hpp"

#include "Scout.hpp"
#include "Axeman.hpp"

namespace bjoernligan
{
	Clan::Clan(const std::string& name, const sf::Color &p_xTeamColor)
		: m_name(name)
		, m_color(p_xTeamColor)
	{
		m_xRandomDeathSound.AddSoundName("Death1");
		m_xRandomDeathSound.AddSoundName("Death2");
		m_xRandomDeathSound.AddSoundName("Death3");
	}

	void Clan::Update(const float &p_fDeltaTime)
	{
		auto itr = m_clanMembers.begin();
		while (itr != m_clanMembers.end())
		{
			if (!(*itr)->m_xCombatStats.Alive())
			{
				itr = m_clanMembers.erase(itr);
				m_xRandomDeathSound.PlayRandomSound();
				continue;
			}
			(*itr)->update(p_fDeltaTime);
			++itr;
		}
	}

	void Clan::draw(sf::RenderTarget& target, sf::RenderStates states) const
	{
		for (std::size_t i = 0; i < m_clanMembers.size(); ++i)
		{
			target.draw(*m_clanMembers[i], states);
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
			members.push_back(m_clanMembers[i].get());
		}
		return members;
	}

	bool Clan::IsAlive()
	{
		return !m_clanMembers.empty();
	}

	void Clan::SetSocial(const float &p_fNewValue)
	{
		p_fNewValue; // <-- for compile

		for (uint32_t k = 0; k < m_clanMembers.size(); ++k)
		{
			//m_members[k]->SetSocial(p_fNewValue);
		}
	}

	void Clan::SetBrave(const float &p_fNewValue)
	{
		p_fNewValue; // <-- for compile

		for (uint32_t k = 0; k < m_clanMembers.size(); ++k)
		{
			//m_members[k]->SetSocial(p_fNewValue);
		}
	}
	void Clan::SetAgression(const float &p_fNewValue)
	{
		p_fNewValue; // <-- for compile

		for (uint32_t k = 0; k < m_clanMembers.size(); ++k)
		{
			//m_members[k]->SetSocial(p_fNewValue);
		}
	}
	const std::string &Clan::GetName() const
	{
		return m_name;
	}
	void Clan::DamageRandomMember(const int32_t &p_iAmount)
	{
		if (m_clanMembers.empty())
			return;

		m_clanMembers[random::random(0, m_clanMembers.size() - 1)]->m_xCombatStats.TakeDamage(p_iAmount);
	}
}
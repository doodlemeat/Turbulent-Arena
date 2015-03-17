#pragma once

namespace bjoernligan
{
	class Clan;
	class ClanManager : public sf::Drawable
	{
	public:
		ClanManager();
		~ClanManager();

		Clan* createClan(const std::string& name, const sf::Color &p_xTeamColor, const uint32_t &p_iAxemanCount, const uint32_t &p_iScoutCount);
		std::size_t getAmount() const;

		void Update(const float &p_fDeltaTime);
		void draw(sf::RenderTarget& target, sf::RenderStates states) const;
		std::vector<Clan*> getClans() const;

		void SetDrawHealthBars(const bool &p_bValue);

	private:
		std::vector<std::unique_ptr<Clan>> m_clans;
		bool m_bDrawHealthBars;
	};
}
//MainMenuState.hpp

#pragma once
#include "GameState.hpp"

namespace bjoernligan
{
	class UIManager;

	namespace input
	{
		class Keyboard;
		class Mouse;
	}

	namespace system
	{
		class AudioManager;
	}

	class MainMenuState : public GameState
	{
	public:
		MainMenuState(const std::string &p_sName, const bool &p_bExclusive = false);

		void Enter();
		void Exit();
		bool Update(const float &p_fDeltaTime);
		void draw(sf::RenderTarget &target, sf::RenderStates states) const;

	private:
		void Play(const bool &p_bButtonValue);
		void Options(const bool &p_bButtonValue);
		void Quit(const bool &p_bButtonValue);

		system::AudioManager* m_xAudioManager;
		UIManager* m_xUIManager;
		input::Keyboard* m_xKeyboard;
	};
}
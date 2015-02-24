//UIManager.hpp

#pragma once
#include <memory>
#include <vector>
#include <SFML\Graphics.hpp>
#include "UIBase.hpp"
#include "UISlider.hpp"
#include <functional>

namespace bjoernligan
{
	class UISlider;

	class UIManager : public sf::Drawable
	{
	private:
		UIManager();
		UIManager(const UIManager&);
		UIManager& operator=(const UIManager&);

	public:
		typedef std::unique_ptr<UIManager> Ptr;
		static Ptr Create();

		void Update(const float &p_fDeltaTime);
		void draw(sf::RenderTarget& target, sf::RenderStates states) const;

		template<class T>
		UIBase* AddElement(const float &p_fDepth)
		{
			++m_iElementCount;

			uint32_t iPos = 0;
			auto itr = m_axElements.begin();
			while (itr != m_axElements.end())
			{
				if ((*itr)->m_fDepth > p_fDepth)
					++iPos;

				++itr;
			}

			m_axElements.insert(m_axElements.begin() + iPos, T::Create(p_fDepth));
			return m_axElements[iPos].get();
		}

		UISlider* AddSlider(const UISlider::SliderDef &p_xDefinition, const sf::Vector2f &p_xPos, const float &p_fDepth);

	private:
		sf::View m_view;
		std::vector<UIBase::Ptr> m_axElements;
		uint32_t m_iElementCount;
	};
}
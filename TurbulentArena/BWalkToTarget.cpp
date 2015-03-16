//BSetWanderTarget.cpp

#include "stdafx.h"
#include "BWalkToTarget.hpp"
#include "Agent.hpp"

namespace bjoernligan
{
	namespace ai
	{
		BWalkToTarget::BWalkToTarget()
		{

		}

		EBNodeStatus BWalkToTarget::Process()
		{
			if (!m_xAgent)
				return EBNodeStatus::Invalid;

			std::vector<SenseAgentData*> enemies = m_xAgent->getSense()->getVisibleEnemies();
			if (!enemies.empty())
			{
				if (m_xAgent->getPathToVisibleTarget(enemies[0]))
				{
					return EBNodeStatus::Success;

				}
			}

			return EBNodeStatus::Success;
		}
	}
}
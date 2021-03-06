#include "stdafx.h"
#include "Explore.h"
#include "Agent.hpp"
#include "ClanMember.hpp"

namespace bjoernligan
{
	namespace ai
	{
		Explore::Explore()
		{

		}

		EBNodeStatus Explore::Process()
		{
			if (!m_xAgent)
				return EBNodeStatus::Invalid;

			m_xAgent->getOwner()->GetMovementStats().SetMaxVelocity(350.f);
			m_xAgent->ChooseWanderPos(false, 80);

			return EBNodeStatus::Success;
		}
	}
}

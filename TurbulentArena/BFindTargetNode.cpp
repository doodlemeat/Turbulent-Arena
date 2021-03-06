#include "stdafx.h"
#include "BFindTargetNode.hpp"
#include "Agent.hpp"

namespace bjoernligan
{
	namespace ai
	{
		BFindTargetNode::BFindTargetNode()
		{

		}

		EBNodeStatus BFindTargetNode::Process()
		{
			if (!m_xAgent)
				return EBNodeStatus::Invalid;

			if (m_xAgent->canFindTarget())
			{
				// Make agent run here
				return EBNodeStatus::Success;
			}
			else
				return EBNodeStatus::Fail;
		}

	}
}

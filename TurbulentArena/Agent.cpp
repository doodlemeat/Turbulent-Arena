//Agent.cpp

#include "stdafx.h"
#include "Agent.hpp"
#include "BehaviorTree.hpp"

namespace bjoernligan
{
	namespace ai
	{
		Agent::Agent()
		{
			m_xBT = nullptr;
		}

		Agent::~Agent()
		{

		}

		void Agent::Sense()
		{

		}

		void Agent::Decide()
		{
			m_xBT->Process();
		}

		void Agent::OnNotify(/*add parameters*/)
		{

		}

		void Agent::SetBehaviorTree(BehaviorTree* p_xBT)
		{
			m_xBT = p_xBT;
		}
	}
}
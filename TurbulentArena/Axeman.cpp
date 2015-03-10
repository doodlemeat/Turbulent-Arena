//Axeman.cpp

#include "stdafx.h"
#include "Axeman.hpp"
#include "Agent.hpp"
#include "BehaviorTree.hpp"
#include "BNodesInclude.hpp"
#include "BFindTargetNode.hpp"

namespace bjoernligan
{
	Axeman::Axeman(ai::Sense* sense)
		: ClanMember(sense)
	{

	}

	void Axeman::initiate()
	{
		m_xCombatStats.Initiate(3, 4);

		m_MovementStats.Initiate(sf::Vector2f(300.0f, 300.0f), 0.1f, 2.0f);

		ai::BehaviorTree* xBT = m_xAgent->getBehaviorTree();

		ai::BSelectorNode* xRootSelector = xBT->CreateRoot<ai::BSelectorNode>();

		ai::BSequenceNode* xCombatSequence = xRootSelector->AddChild<ai::BSequenceNode>();
		xCombatSequence->AttachAgent(m_xAgent);
		ai::BSequenceNode* xWanderSequence = xRootSelector->AddChild<ai::BSequenceNode>();
		xWanderSequence->AttachAgent(m_xAgent);

		xCombatSequence->AddChild<ai::BFindTargetNode>()->AttachAgent(m_xAgent);
		
		xWanderSequence->AddChild<ai::BSetWanderTarget>()->AttachAgent(m_xAgent);
	}

	void Axeman::update(float deltatime)
	{
		ClanMember::update(deltatime);
	}
}
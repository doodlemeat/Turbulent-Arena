//Agent.cpp

#include "stdafx.h"
#include "Agent.hpp"
#include "BehaviorTree.hpp"
#include "ClanMember.hpp"
#include "ServiceLocator.hpp"
#include "Utility.hpp"
#include "Sense.hpp"
#include "Map.hpp"

#define AGENT_SENSE_TIMER 0.3f
#define AGENT_DECIDE_TIMER 1.0f

namespace bjoernligan
{
	namespace ai
	{
		Agent::Agent(ClanMember* p_xOwner, ai::Sense* sense)
			: m_xSenseTimer(AGENT_SENSE_TIMER)
			, m_xDecideTimer(AGENT_DECIDE_TIMER)
			, m_xOwner(p_xOwner)
			, m_xBT(nullptr)
			, m_Steering(nullptr)
			//, m_Pathfinder(nullptr)
			, m_Utility(nullptr)
		{
			m_Steering = std::make_unique<SteeringManager>();
			m_xBT = std::make_unique<BehaviorTree>();
			m_senseData = std::make_unique<SenseData>(this, sense, 32.f);
			m_xSenseTimer.SetOneTimeMax(random::random(0.0f, AGENT_SENSE_TIMER));
			m_xDecideTimer.SetOneTimeMax(random::random(0.0f, AGENT_DECIDE_TIMER));


			//Pathfinding stuff
			m_Utility = ServiceLocator<Utility>::GetService();
			//m_Pathfinder = ServiceLocator<Pathfinder>::GetService();
			//m_Pathfinder->getGrid();

		}

		Agent::~Agent()
		{
		}

		void Agent::Update(const float &p_fDeltaTime)
		{
			m_xSenseTimer.Update(p_fDeltaTime);
			m_xDecideTimer.Update(p_fDeltaTime);

			//SENSE
			if (m_xSenseTimer.Done())
			{
				m_xSenseTimer.Reset();
				Sense();
			}
			//DECIDE
			if (m_xDecideTimer.Done())
			{
				m_xDecideTimer.Reset();
				Decide();
			}
			//ACT
			Act();

		}

		void Agent::Sense()
		{

			m_senseData->update();
		}

		void Agent::Decide()
		{
			m_xBT->Process();
		}

		void Agent::Act()
		{
			/*if (!m_xPath.nodes.empty())
			{*/
			m_Steering->Seek(m_xMoveTarget);
			////L�GG TILL NODE TA BORT NODE mellan steering
			//}
			m_Steering->Update();
		}
		void Agent::OnNotify(/*add parameters*/)
		{

		}
		SenseData* Agent::getSense() const
		{
			return m_senseData.get();
		}

		ClanMember* Agent::getOwner() const
		{
			return m_xOwner;
		}

		BehaviorTree* Agent::getBehaviorTree()
		{
			return m_xBT.get();
		}

		void Agent::InitializeSteering(b2Body* p_CurrentBody, MovementStats* p_MovementStats)
		{
			m_Steering->Initialize();
			m_Steering->SetCurrentBody(p_CurrentBody, p_MovementStats->GetMaxVelocity(), p_MovementStats->GetSlowDownRadius());
		}

		//tomas BT-methods (bad solution)
		int32_t Agent::SensedEnemyCount()
		{
			return 0;
		}

		void Agent::ChooseWanderPos()
		{
			//m_xMoveTarget = sf::Vector2f(140.0f,140.0f);
			//m_xMoveTarget = sf::Vector2f(random::random(64.0f, 1040.0f), random::random(64.0f, 1040.0f));

			sf::Vector2i xTargetPos;
			Map* xMap = ServiceLocator<Map>::GetService();
			Pathfinder* xPathFinder = ServiceLocator<Pathfinder>::GetService();

			xMap, xPathFinder;

			//do
			//{
			//	xTargetPos = sf::Vector2i(random::random(0, xMap->getSize().x), random::random(0, xMap->getSize().y));
			//	/*xPathFinder->setStart();
			//	xPathFinder->setGoal();*/
			//}while (xPathFinder->findPath(m_CurrentPath, ,) != PathfinderInfo::PathResult::PATHRESULT_FAILED);
		}

		void Agent::MoveToTargetPos()
		{
				//insert pathfinding here
				//m_Pathfinder->findPath(m_CurrentPath, Pathfinder::Options());

				//m_Steering->Seek(m_xMoveTarget); // <- temporary
		}

		bool Agent::AtMoveTarget()
		{
			return false;
		}

		bool Agent::canFindTarget()
		{
			if (m_senseData->getVisibleAgents().size() > 0)
			{
				return true;
			}
			else
				return false;
		}

		bool Agent::getPathToVisibleTarget(Agent* agent)
		{
			Map* map = ServiceLocator<Map>::GetService();
			Pathfinder* pathfinder = ServiceLocator<Pathfinder>::GetService();
			pathfinder->setStart(map->getTilePosition(m_xOwner->getSprite()->getPosition()));
			pathfinder->setGoal(map->getTilePosition(agent->getOwner()->getSprite()->getPosition()));

			Pathfinder::Options options;
			options.algorithm = PathfinderInfo::ALGORITHM_ASTAR;
			options.diagonal = PathfinderInfo::DIAGONAL_NO_OBSTACLES;
			options.heuristic = PathfinderInfo::HEURISTIC_DIAGONAL;

			PathfinderInfo::PathResult result = pathfinder->findPath(m_CurrentPath, options);
			if (result == PathfinderInfo::PATHRESULT_SUCCEEDED)
				return true;
			return false;
		}

		bool Agent::getPathToRandomVisibleTarget()
		{
			std::vector<Agent*> visibleAgents = m_senseData->getVisibleAgents();
			std::size_t randomAgentIndex = random::random(0, visibleAgents.size() - 1);
			return getPathToVisibleTarget(visibleAgents[randomAgentIndex]);
		}
	}
}
//BMoveToNode.hpp

#pragma once
#include "BNode.hpp"

namespace bjoernligan
{
	namespace ai
	{
		class BMoveToNode : public BNode
		{
		public:
			BMoveToNode();

			EBNodeStatus Process();
		};
	}
}
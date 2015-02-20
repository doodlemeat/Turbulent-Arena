//Axeman.hpp

#pragma once
#include "Class.hpp"

namespace bjoernligan
{
	class Axeman : public Class
	{
	public:
		Axeman();
		~Axeman();

		virtual void Initiate();
		virtual void Update(const float &p_fDeltaTime);
	};
}
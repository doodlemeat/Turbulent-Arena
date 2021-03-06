#pragma once

namespace bjoernligan
{
	class ClanMember;
	struct B2UserData
	{
		enum B2UserDataType
		{
			CLANMEMBER,
			NATURE
		};

		B2UserData(B2UserDataType _type) :
			type(_type)
		{

		}

		virtual ~B2UserData()
		{

		}

		B2UserDataType type;
	};

	struct ClanMemberUD : public B2UserData
	{
		ClanMemberUD(ClanMember* _clanMember) :
			B2UserData(CLANMEMBER),
			clanMember(_clanMember)

		{
			type = CLANMEMBER;
		}


		~ClanMemberUD()
		{

		}

		ClanMember* clanMember;
	};

	struct NatureUD : public B2UserData
	{
		NatureUD(bool see_through) :
			B2UserData(NATURE),
			m_see_through(see_through)
		{

		}

		~NatureUD()
		{

		}

		bool m_see_through;
	};
}
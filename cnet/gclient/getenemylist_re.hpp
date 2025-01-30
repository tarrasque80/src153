
#ifndef __GNET_GETENEMYLIST_RE_HPP
#define __GNET_GETENEMYLIST_RE_HPP

#include "rpcdefs.h"
#include "callid.hxx"
#include "state.hxx"

#include "genemylist"

namespace GNET
{

class GetEnemyList_Re : public GNET::Protocol
{
	#include "getenemylist_re"

	void Process(Manager *manager, Manager::Session::ID sid)
	{
		// TODO
	}
};

};

#endif


#ifndef __GNET_GETPLAYERFACTIONRELATION_HPP
#define __GNET_GETPLAYERFACTIONRELATION_HPP

#include "rpcdefs.h"
#include "callid.hxx"
#include "state.hxx"


namespace GNET
{

class GetPlayerFactionRelation : public GNET::Protocol
{
	#include "getplayerfactionrelation"

	void Process(Manager *manager, Manager::Session::ID sid)
	{
		// TODO
	}
};

};

#endif

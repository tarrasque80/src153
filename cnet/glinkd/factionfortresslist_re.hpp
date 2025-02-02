
#ifndef __GNET_FACTIONFORTRESSLIST_RE_HPP
#define __GNET_FACTIONFORTRESSLIST_RE_HPP

#include "rpcdefs.h"
#include "callid.hxx"
#include "state.hxx"

#include "gfactionfortressbriefinfo"

namespace GNET
{

class FactionFortressList_Re : public GNET::Protocol
{
	#include "factionfortresslist_re"

	void Process(Manager *manager, Manager::Session::ID sid)
	{
		// TODO
		GLinkServer::GetInstance()->Send(localsid,this);
	}
};

};

#endif

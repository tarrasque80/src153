
#ifndef __GNET_NOTIFYFACTIONFORTRESSID_HPP
#define __GNET_NOTIFYFACTIONFORTRESSID_HPP

#include "rpcdefs.h"
#include "callid.hxx"
#include "state.hxx"
#include "factiondb.h"


namespace GNET
{

class NotifyFactionFortressID : public GNET::Protocol
{
	#include "notifyfactionfortressid"

	void Process(Manager *manager, Manager::Session::ID sid)
	{
		// TODO
		for(std::vector<int>::iterator it=factionids.begin(); it!=factionids.end(); ++it)
			Factiondb::GetInstance()->ObtainFactionInfo(*it);
	}
};

};

#endif

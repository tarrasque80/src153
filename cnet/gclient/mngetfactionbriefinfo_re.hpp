
#ifndef __GNET_MNGETFACTIONBRIEFINFO_RE_HPP
#define __GNET_MNGETFACTIONBRIEFINFO_RE_HPP

#include "rpcdefs.h"
#include "callid.hxx"
#include "state.hxx"

#include "mnfactionbriefinfo"

namespace GNET
{

class MNGetFactionBriefInfo_Re : public GNET::Protocol
{
	#include "mngetfactionbriefinfo_re"

	void Process(Manager *manager, Manager::Session::ID sid)
	{
		// TODO
	}
};

};

#endif

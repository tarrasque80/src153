
#ifndef __GNET_MNGETPLAYERLASTENTERINFO_RE_HPP
#define __GNET_MNGETPLAYERLASTENTERINFO_RE_HPP

#include "rpcdefs.h"
#include "callid.hxx"
#include "state.hxx"


namespace GNET
{

class MNGetPlayerLastEnterInfo_Re : public GNET::Protocol
{
	#include "mngetplayerlastenterinfo_re"

	void Process(Manager *manager, Manager::Session::ID sid)
	{
		// TODO
	}
};

};

#endif

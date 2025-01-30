
#ifndef __GNET_BATTLEENTERNOTICE_HPP
#define __GNET_BATTLEENTERNOTICE_HPP

#include "rpcdefs.h"
#include "callid.hxx"
#include "state.hxx"

namespace GNET
{

class BattleEnterNotice : public GNET::Protocol
{
	#include "battleenternotice"

	void Process(Manager *manager, Manager::Session::ID sid)
	{
	}
};

};

#endif

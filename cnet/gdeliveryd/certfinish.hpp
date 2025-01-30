
#ifndef __GNET_CERTFINISH_HPP
#define __GNET_CERTFINISH_HPP

#include "rpcdefs.h"
#include "callid.hxx"
#include "state.hxx"


namespace GNET
{

class CertFinish : public GNET::Protocol
{
	#include "certfinish"

	void Process(Manager *manager, Manager::Session::ID sid)
	{
		// TODO
	}
};

};

#endif

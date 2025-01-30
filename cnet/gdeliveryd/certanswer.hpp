
#ifndef __GNET_CERTANSWER_HPP
#define __GNET_CERTANSWER_HPP

#include "rpcdefs.h"
#include "callid.hxx"
#include "state.hxx"


namespace GNET
{

class CertAnswer : public GNET::Protocol
{
	#include "certanswer"

	void Process(Manager *manager, Manager::Session::ID sid)
	{
		// TODO
	}
};

};

#endif


#ifndef __GNET_WEBTRADEGETDETAIL_RE_HPP
#define __GNET_WEBTRADEGETDETAIL_RE_HPP

#include "rpcdefs.h"
#include "callid.hxx"
#include "state.hxx"

#include "gwebtradedetail"

namespace GNET
{

class WebTradeGetDetail_Re : public GNET::Protocol
{
	#include "webtradegetdetail_re"

	void Process(Manager *manager, Manager::Session::ID sid)
	{
		// TODO
		GLinkServer * lsm = GLinkServer::GetInstance();
		if(!lsm->IsRoleOnGame(localsid))
		{
			lsm->ChangeState(localsid,&state_GSelectRoleServer);
		}
		lsm->Send(localsid,this);
	}
};

};

#endif

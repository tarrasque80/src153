
#ifndef __GNET_WEBTRADEPRECANCELPOST_RE_HPP
#define __GNET_WEBTRADEPRECANCELPOST_RE_HPP

#include "rpcdefs.h"
#include "callid.hxx"
#include "state.hxx"


namespace GNET
{

class WebTradePreCancelPost_Re : public GNET::Protocol
{
	#include "webtradeprecancelpost_re"

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

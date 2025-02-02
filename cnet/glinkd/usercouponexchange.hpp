
#ifndef __GNET_USERCOUPONEXCHANGE_HPP
#define __GNET_USERCOUPONEXCHANGE_HPP

#include "rpcdefs.h"
#include "callid.hxx"
#include "state.hxx"


namespace GNET
{

class UserCouponExchange : public GNET::Protocol
{
	#include "usercouponexchange"

	void Process(Manager *manager, Manager::Session::ID sid)
	{
		if (!GLinkServer::GetInstance()->ValidUser(sid,userid)) return;
		this->localsid=sid;
		GDeliveryClient::GetInstance()->SendProtocol(this);
	}
};

};

#endif

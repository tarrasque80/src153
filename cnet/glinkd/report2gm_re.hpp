
#ifndef __GNET_REPORT2GM_RE_HPP
#define __GNET_REPORT2GM_RE_HPP

#include "rpcdefs.h"
#include "callid.hxx"
#include "state.hxx"
#include "glinkserver.hpp"

#include "statusannounce.hpp"
namespace GNET
{

class Report2GM_Re : public GNET::Protocol
{
	#include "report2gm_re"

	void Process(Manager *manager, Manager::Session::ID sid)
	{
		// TODO
		unsigned int tmp=localsid;
		localsid=0;
		GLinkServer::GetInstance()->Send(tmp,this);
	}
};

};

#endif

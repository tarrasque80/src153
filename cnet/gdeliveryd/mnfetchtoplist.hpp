
#ifndef __GNET_MNFETCHTOPLIST_HPP
#define __GNET_MNFETCHTOPLIST_HPP

#include "rpcdefs.h"
#include "callid.hxx"
#include "state.hxx"


namespace GNET
{

class MNFetchTopList : public GNET::Protocol
{
	#include "mnfetchtoplist"

	void Process(Manager *manager, Manager::Session::ID sid)
	{
		DEBUG_PRINT("file=%s\tfunc=%s\tline=%d\n", __FILE__, __FUNCTION__, __LINE__);
		CDS_MNToplistMan::GetInstance()->SendTopList(zoneid);
	}
};

};

#endif

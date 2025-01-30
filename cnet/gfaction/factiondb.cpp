#include "factiondb.h"
#include "roleid"
#include "factionid"
#include "gfactiondbclient.hpp"
#include "getuserfaction.hrp"
#include "addfaction.hrp"
#include "delfaction.hrp"
#include "addmember.hrp"
#include "delmember.hrp"
#include "delmemberschedule.hrp"
#include "getfactioninfo.hrp"
#include "getfactiondetail.hrp"
#include "factionlistmember_re.hpp"
#include "updateuserfaction.hrp"
#include "updatefaction.hrp"
#include "dbfactionupgrade.hrp"
#include "dbfactionpromote.hrp"
#include "log.h"
#include "matcher.h"
#include "gfs_io.h"

#include "dbfactionallianceapply.hrp"
#include "dbfactionalliancereply.hrp"
#include "dbfactionhostileapply.hrp"
#include "dbfactionhostilereply.hrp"
#include "dbfactionremoverelationapply.hrp"
#include "dbfactionremoverelationreply.hrp"
#include "dbfactionrelationtimeout.hrp"
#include "dbfactionrelationapplytimeout.hrp"
#include "factionlistrelation_re.hpp"
#include "getplayerfactionrelation_re.hpp"
#include "worldchat.hpp"
#include "gproviderserver.hpp"
#include "gfactionserver.hpp"
#include "factionmemberupdate.hpp"
#include "playernameupdate.hpp"

namespace
{
	template <typename V>
		inline void Add(V& v, int type, int fid, int end_time)
		{
			v.push_back(typename V::value_type(type,fid,end_time));
		}
	template <typename V>
		inline bool Remove(V& v, int type, int fid)
		{
			for(typename V::iterator it = v.begin(); it != v.end(); ++it)
				if(it->type == type && it->fid == fid)
				{
					v.erase(it);
					return true;
				}
			return false;
		}
	template <typename V>
		inline void Add(V& v, int fid, int end_time)
		{
			v.push_back(typename V::value_type(fid,end_time));
		}
	template <typename V>
		inline bool Remove(V& v, int fid)
		{
			for(typename V::iterator it = v.begin(); it != v.end(); ++it)
				if(it->fid == fid)
				{
					v.erase(it);
					return true;
				}
			return false;
		}
	template <typename V>	
		inline bool Find(V& v, int type, int fid)
		{
			for(typename V::iterator it = v.begin(); it != v.end(); ++it)
				if(it->type == type && it->fid == fid) return true;
			return false;
		}
	template <typename V>	
		inline int Get(V& v, int fid)
		{
			for(typename V::iterator it = v.begin(); it != v.end(); ++it)
				if(it->fid == fid) return it->end_time;
			return -1;
		}
}

namespace {

void NotifyMemberUpdate(int operation, unsigned int fid, int title, int roleid, const Octets &name = Octets()) {
	FactionMemberUpdate update;
	update.operation = operation;
	update.titleid = title;
	update.factionid.ftype = GT_FACTION_TYPE;
	update.factionid.factionid = (int64_t)fid;
	RoleBean role;
	role.info.roleid = (int64_t)roleid;
	role.info.rolename = name;
	update.roles.push_back(role);
	GFactionServer *server = GFactionServer::GetInstance();
	if(!server->DispatchProtocol(0, update)) {
		Log::log(LOG_ERR, "FactionDB Send FactionInfoUpdate failed.");
	}
}

}

namespace GNET
{
	bool Factiondb::InitFactiondb(/*bool ig_asp*/)
	{
//		ignore_announce_space = ig_asp;
		IntervalTimer::Attach( this,FACTIONDB_UPDATE_INTERVAL/IntervalTimer::Resolution() );	
		last_delayupdate_time = Timer::GetTime();
		pvp_status = 0;
		return true;
	}

	bool Factiondb::Update()
	{
		Thread::RWLock::RDScoped l(locker);
		
		time_t now = Timer::GetTime();
		for(RelationMap::iterator it=relation_map.begin(); it!=relation_map.end(); ++it)
		{
			if(now < it->second.end_time) continue;
			faction_relation & r = it->second; 
			switch(r.type)
			{
				case ALLIANCE:
				case HOSTILE:
				{
					Rpc * rpc = Rpc::Call(RPC_DBFACTIONRELATIONTIMEOUT,
								DBFactionRelationTimeoutArg(r.type, r.fid1, r.fid2));
					GFactionDBClient::GetInstance()->SendProtocol(rpc);
				}
				break;
				
				case ALLIANCE_HALF:
				case HOSTILE_HALF:
				case REMOVE_RELATION_HALF:
				{
					Rpc * rpc = Rpc::Call(RPC_DBFACTIONRELATIONAPPLYTIMEOUT,
								DBFactionRelationApplyTimeoutArg(r.type, r.fid1, r.fid2, 
									r.type==HOSTILE_HALF?Timer::GetTime()+RELATION_DURATION:0));
					GFactionDBClient::GetInstance()->SendProtocol(rpc);
				}
				break;
			
				default:
				break;
			}
		}

		DelayExpelTick(now);
		return true;	
	}

	// 建立帮派
	int Factiondb::CreateFaction(unsigned int fid,Octets& name, unsigned int rid, OperWrapper::wref_t oper)
	{
		if(name.size()>MAX_FACTION_NAME_SIZE || Matcher::GetInstance()->Match((char*)name.begin(),name.size())!=0)
			return ERR_FC_INVALIDNAME;

		AddFactionArg arg(name, rid, fid);
		AddFaction* rpc = (AddFaction*) Rpc::Call(RPC_ADDFACTION,&arg);
		rpc->oper = oper;
		GFactionDBClient::GetInstance()->SendProtocol(rpc);
		return 0;
	}

	// 删除帮派
	int Factiondb::RemoveFaction(unsigned int fid, OperWrapper::wref_t oper)
	{
		FactionId arg(fid);
		DelFaction* rpc = (DelFaction*) Rpc::Call(RPC_DELFACTION,&arg);
		rpc->oper = oper;
		GFactionDBClient::GetInstance()->SendProtocol(rpc);
		return 0;
	}

	// 接收成员
	int Factiondb::AcceptJoin(unsigned int fid, unsigned int rid, OperWrapper::wref_t oper)
	{
		AddMemberArg arg(fid,rid);
		AddMember* rpc = (AddMember*) Rpc::Call(RPC_ADDMEMBER,&arg);
		rpc->oper = oper;
		GFactionDBClient::GetInstance()->SendProtocol(rpc);
		return 0;
	}

	// 驱逐帮众
	int Factiondb::DismissMember(unsigned int fid, unsigned int rid, OperWrapper::wref_t oper)
	{
		DelMemberArg arg(fid,rid);
		DelMember* rpc = (DelMember*) Rpc::Call(RPC_DELMEMBER,&arg);
		rpc->oper = oper;
		GFactionDBClient::GetInstance()->SendProtocol(rpc);
		return 0;
	}

	int Factiondb::DelayExpelMember(unsigned int fid, unsigned int rid, OperWrapper::wref_t oper)
	{
		DelMemberScheduleArg arg(fid,rid,FC_DELAYEXPEL_EXECUTE,Timer::GetTime()+DELAY_EXPEL_TIME);
		DelMemberSchedule* rpc = (DelMemberSchedule*) Rpc::Call(RPC_DELMEMBERSCHEDULE,&arg);
		rpc->oper = oper;
		GFactionDBClient::GetInstance()->SendProtocol(rpc);
		return 0;
	}

	int Factiondb::CancelDelayExpel(unsigned int fid, unsigned int rid, OperWrapper::wref_t oper)
	{
		DelMemberScheduleArg arg(fid,rid,FC_DELAYEXPEL_CANCEL);
		DelMemberSchedule* rpc = (DelMemberSchedule*) Rpc::Call(RPC_DELMEMBERSCHEDULE,&arg);
		rpc->oper = oper;
		GFactionDBClient::GetInstance()->SendProtocol(rpc);
		return 0;
	}

	int Factiondb::DelayExpelTimeout(unsigned int fid, unsigned int rid)
	{
		DelMemberScheduleArg arg(fid,rid,FC_DELAYEXPEL_TIMEOUT);
		DelMemberSchedule* rpc = (DelMemberSchedule*) Rpc::Call(RPC_DELMEMBERSCHEDULE,&arg);
		GFactionDBClient::GetInstance()->SendProtocol(rpc);
		return 0;
	}

	#define CONTRIB_EXPEL_LIMIT 5000
	bool Factiondb::NeedDelayExpel(unsigned int fid,unsigned int rid)
	{
		FMemberInfo info;
		
		if(!GetMemberInfo(fid,rid,info))
		    return false;
		
		return info.contrib >= CONTRIB_EXPEL_LIMIT;
	}
	bool Factiondb::IsSpecialMember(unsigned int fid, unsigned int rid,unsigned char frole)
	{
		FMemberInfo info;
		
		if(!GetMemberInfo(fid,rid,info))
		    return false;
		
		return info.froleid == frole || 255 == frole;
	}
	bool Factiondb::AlreadyDelayExpel(unsigned int rid)
	{
		return GetDelayExpelTime(rid) != 0;
	}

	bool Factiondb::GetMemberInfo(unsigned int fid, unsigned int rid, FMemberInfo& info)
	{
		Thread::RWLock::RDScoped l(locker);
		Map::iterator it = factions.find(fid);
		
		if(!IsReady(fid, it))
			return false;
		
		FactionDetailInfo* pf = it->second;

		for(FMemberInfoVector::iterator it=pf->info.member.begin(); it!=pf->info.member.end();it++)
		{
			if(it->roleid==rid)
			{
			    info = *it;
			    return true;
			}
		}

		return false;
	}
	
	bool Factiondb::GetMemberList(unsigned int fid, unsigned int rid, std::vector<int>& list)
	{
		Thread::RWLock::RDScoped l(locker);
		Map::iterator it = factions.find(fid);
		
		if(!IsReady(fid, it))
			return false;
		
		FactionDetailInfo* pf = it->second;

		bool valid = false;
		for(FMemberInfoVector::iterator it=pf->info.member.begin(); it!=pf->info.member.end();it++)
		{
			if(it->roleid==rid)
			{
				valid = true;
			}
			else
			{
				list.push_back(it->roleid);
			}
		}

		return valid;
	}

	bool Factiondb::CheckMemberList(unsigned int fid, unsigned int rid, std::vector<int>& list)
	{
		Thread::RWLock::RDScoped l(locker);
		Map::iterator it = factions.find(fid);
		
		if(!IsReady(fid, it))
			return false;
		
		for(std::vector<int>::iterator it=list.begin(); it!=list.end();it++)
		{
			unsigned int roleid = (unsigned int)*it;
			if(roleid == rid)
				return false;  // self no member
			else if(!IsSpecialMember(fid,roleid))
				return false;
		}

		return true;
	}

	// 更改职位
	int Factiondb::UpdateRole(unsigned int fid, int superior, int roleid, char suprole, char role, OperWrapper::wref_t oper)
	{
		DBFactionPromoteArg arg(fid, superior, roleid, suprole, role);
		arg.max =  FactionConfig::MaxRole(role);
		int count = 0;

		{
			Thread::RWLock::RDScoped l(locker);
			Map::iterator it = factions.find(fid);
			if(!IsReady(fid, it))
				return ERR_FC_OUTOFSERVICE;
			FactionDetailInfo* pf = it->second;

			for(FMemberInfoVector::iterator it=pf->info.member.begin(); it!=pf->info.member.end();it++)
			{
				if(it->froleid==role)
					count++;
			}
		}
		if(role!=_R_MASTER && count>=arg.max)
			return ERR_FC_FULL;

		DBFactionPromote* rpc = (DBFactionPromote*) Rpc::Call(RPC_DBFACTIONPROMOTE,&arg);
		rpc->oper = oper;
		GFactionDBClient::GetInstance()->SendProtocol(rpc);
		return 0;
	}

	int Factiondb::UpdateNickname(unsigned int fid, unsigned int rid, Octets& nick, OperWrapper::wref_t oper)
	{
		if(nick.size()>16 || Matcher::GetInstance()->Match((char*)nick.begin(),nick.size())!=0)
			return ERR_FC_INVALIDNAME;
		UpdateUserFactionArg arg;
		arg.fid = fid;
		arg.rid = rid;
		arg.operation = 1;
		arg.nickname = nick;

		UpdateUserFaction* rpc = (UpdateUserFaction*) Rpc::Call(RPC_UPDATEUSERFACTION,&arg);
		rpc->oper = oper;
		GFactionDBClient::GetInstance()->SendProtocol(rpc);
		return 0;
	}

	// 更改宣言
	int Factiondb::UpdateAnnounce(unsigned int fid, Octets& announce, OperWrapper::wref_t oper)
	{
		Octets an_filter = announce;
		
		if(true/*ignore_announce_space*/)
		{
			if(an_filter.size()%2 != 0) return ERR_FC_INVALIDNAME;// only for usc2
			short* pChar = (short*)an_filter.begin();
			while(*pChar != 0 && pChar != an_filter.end())
			{
				if(*pChar == 32) an_filter.erase((void*)pChar,(void*)(pChar+1)); // remove space
				else ++pChar;
			}
		}
		
		if(announce.size()>160 || Matcher::GetInstance()->Match((char*)an_filter.begin(),an_filter.size())!=0)
			return ERR_FC_INVALIDNAME;
		UpdateFactionArg arg;
		arg.fid = fid;
		arg.type = 0;
		arg.announce = announce;
		UpdateFaction* rpc = (UpdateFaction*) Rpc::Call(RPC_UPDATEFACTION,&arg);
		rpc->oper = oper;
		GFactionDBClient::GetInstance()->SendProtocol(rpc);
		return 0;
	}

	void Factiondb::ClearLoadFlag(unsigned int fid)
	{
		Map::iterator it = factions.find(fid);
		if(it!=factions.end())
		{
			it->second->waitload = false;
		}
	}

	void Factiondb::PutFactionCache(unsigned int fid, GFactionDetail& info)
	{
		FactionDetailInfo* detail;
		Thread::RWLock::RDScoped l(locker);
		Map::iterator it = factions.find(fid);
		if(it==factions.end())
		{
			detail = new FactionDetailInfo;
			factions.insert(std::make_pair(fid,detail));
		}else
		{
			detail = it->second;
		}

		for(FMemberInfoVector::iterator it=info.member.begin(); it!=info.member.end();it++)
		{
			if (GFactionServer::GetInstance()->IsPlayerOnline(it->roleid))
				detail->online.add(it->roleid);
			if (it->delayexpel)
			   	AddDelayExpel(fid,it->roleid,it->expeltime);
		}
		detail->status = 1;
		detail->version = 1;
		detail->maxbonus = 0;
		detail->updatetime = 0;
		detail->info.fid = info.fid;
		detail->info.unifid = info.unifid;
		detail->info.name.swap(info.name);
		detail->info.level = info.level;
		detail->info.master = info.master;
		detail->info.announce.swap(info.announce);
		detail->info.member.swap(info.member);
		detail->info.last_op_time = info.last_op_time;
		detail->info.alliance.swap(info.alliance);
		detail->info.hostile.swap(info.hostile);
		detail->info.apply.swap(info.apply);
		//处理帮派外交数据
		for(size_t i=0; i<detail->info.apply.size(); i++)
		{
			GFactionRelationApply & apply = detail->info.apply[i];
			faction_pair p(detail->info.fid,apply.fid);
			RelationMap::iterator it = relation_map.find(p);
			if(it != relation_map.end())
			{
				if(apply.type == ALLIANCE_FROM_OTHER || apply.type == ALLIANCE_TO_OTHER)
				{
					if(it->second.type != ALLIANCE_HALF)
					{
						//LOG_ERR
						Log::log(LOG_ERR, "FactionRelation not consistent. old:type(%d)end_time(%d)fid1(%d)fid2(%d), new:fid(%d)type(%d)end_time(%d)",
								it->second.type,it->second.end_time,it->second.fid1,it->second.fid2,info.fid,ALLIANCE_HALF,apply.end_time);
					}
				}
				else if(apply.type == HOSTILE_FROM_OTHER || apply.type == HOSTILE_TO_OTHER)
				{
					if(it->second.type != HOSTILE_HALF)
					{
						//LOG_ERR
						Log::log(LOG_ERR, "FactionRelation not consistent. old:type(%d)end_time(%d)fid1(%d)fid2(%d), new:fid(%d)type(%d)end_time(%d)",
								it->second.type,it->second.end_time,it->second.fid1,it->second.fid2,info.fid,HOSTILE_HALF,apply.end_time);
					}
				}
				else if(apply.type == REMOVE_RELATION_FROM_OTHER || apply.type == REMOVE_RELATION_TO_OTHER)
				{
					if(it->second.type != REMOVE_RELATION_HALF)
					{
						//LOG_ERR
						Log::log(LOG_ERR, "FactionRelation not consistent. old:type(%d)end_time(%d)fid1(%d)fid2(%d), new:fid(%d)type(%d)end_time(%d)",
								it->second.type,it->second.end_time,it->second.fid1,it->second.fid2,info.fid,REMOVE_RELATION_HALF,apply.end_time);
					}
				}
				continue;
			}
			if(apply.type == ALLIANCE_FROM_OTHER || apply.type == ALLIANCE_TO_OTHER)
				relation_map.insert(std::make_pair(p,faction_relation(ALLIANCE_HALF,apply.end_time,detail->info.fid,apply.fid)));
			else if(apply.type == HOSTILE_FROM_OTHER || apply.type == HOSTILE_TO_OTHER)
				relation_map.insert(std::make_pair(p,faction_relation(HOSTILE_HALF,apply.end_time,detail->info.fid,apply.fid)));
			else if(apply.type == REMOVE_RELATION_FROM_OTHER || apply.type == REMOVE_RELATION_TO_OTHER)
				relation_map.insert(std::make_pair(p,faction_relation(REMOVE_RELATION_HALF,apply.end_time,detail->info.fid,apply.fid)));
		}
		for(size_t i=0; i<detail->info.alliance.size(); i++)
		{
			GFactionAlliance & alliance = detail->info.alliance[i];
			faction_pair p(detail->info.fid,alliance.fid);
			RelationMap::iterator it = relation_map.find(p);
			if(it != relation_map.end())
			{
				if(it->second.type != ALLIANCE && it->second.type != REMOVE_RELATION_HALF)
				{
					//LOG_ERR
					Log::log(LOG_ERR, "FactionRelation not consistent. old:type(%d)end_time(%d)fid1(%d)fid2(%d), new:fid(%d)type(%d)end_time(%d)",
							it->second.type,it->second.end_time,it->second.fid1,it->second.fid2,info.fid,ALLIANCE,alliance.end_time);
				}
				continue;
			}
			relation_map.insert(std::make_pair(p,faction_relation(ALLIANCE,alliance.end_time,detail->info.fid,alliance.fid)));
		}
		for(size_t i=0; i<detail->info.hostile.size(); i++)
		{
			GFactionHostile & hostile = detail->info.hostile[i];
			faction_pair p(detail->info.fid,hostile.fid);
			RelationMap::iterator it = relation_map.find(p);
			if(it != relation_map.end())
			{
				if(it->second.type != HOSTILE && it->second.type != REMOVE_RELATION_HALF)
				{
					//LOG_ERR
					Log::log(LOG_ERR, "FactionRelation not consistent. old:type(%d)end_time(%d)fid1(%d)fid2(%d), new:fid(%d)type(%d)end_time(%d)",
							it->second.type,it->second.end_time,it->second.fid1,it->second.fid2,info.fid,HOSTILE,hostile.end_time);
				}
				continue;
			}
			relation_map.insert(std::make_pair(p,faction_relation(HOSTILE,hostile.end_time,detail->info.fid,hostile.fid)));
		}
	}

	bool Factiondb::FindFactionName(unsigned int fid, Octets& name)
	{
		Thread::RWLock::RDScoped l(locker);
		Map::iterator it = factions.find(fid);
		if(!IsReady(fid, it))
			return false;
		name = it->second->info.name;
		return true;
	}

	bool Factiondb::FindFactionInCache(FactionBriefInfo& info)
	{
		Thread::RWLock::RDScoped l(locker);
		Map::iterator it = factions.find(info.fid);
		if(!IsReady(info.fid, it,false))
			return false;
		info.name = it->second->info.name;
		info.level = it->second->info.level;
		info.mem_num = it->second->info.member.size();
		return true;
	}

	int Factiondb::FindUserFaction(unsigned int rid, GUserFaction& user, int reason) 
	{
		UserFactionArg arg(reason, rid);
		GetUserFaction* rpc = (GetUserFaction*) Rpc::Call(RPC_GETUSERFACTION,&arg);
		GFactionDBClient::GetInstance()->SendProtocol(rpc);
		return 0;
	}   

	bool Factiondb::ObtainFactionInfo(unsigned int fid)
	{
		Thread::RWLock::RDScoped l(locker);
		Map::iterator it = factions.find(fid);
		return IsReady(fid, it);
	}

	int Factiondb::ListMember(unsigned int fid, unsigned int rid, int version)
	{
		FactionListMember_Re re;

		Thread::RWLock::RDScoped l(locker);
		Map::iterator it = factions.find(fid);
		
		if(!IsReady(fid, it))
			return ERR_FC_OUTOFSERVICE;

		FactionDetailInfo* pf = it->second;
		re.roleid = rid;
		re.handle = version;
		if(version==0)
		{
			re.member_list = pf->info.member;
			re.proclaim = pf->info.announce;
		}
		re.online_list = pf->online;

		for(FMemberInfoVector::iterator iter = re.member_list.begin(),
			iend = re.member_list.end(); iter != iend; ++iter)
		{
			if(iter->delayexpel)
			    iter->expeltime = GetDelayExpelTime(iter->roleid);
		}
		

		GFactionServer::Player player;
		GFactionServer* gfs = GFactionServer::GetInstance();
		if (gfs->GetPlayer(rid,player))
		{
			re.localsid = player.localsid;
			gfs->DispatchProtocol(player.linkid, re);
		}
		return 0;
	}

	int Factiondb::ListRelation(unsigned int fid, unsigned int rid)
	{
		Thread::RWLock::RDScoped l(locker);
		Map::iterator it = factions.find(fid);
		if(!IsReady(fid, it))
			return ERR_FC_OUTOFSERVICE;
		FactionDetailInfo* pf = it->second;

		FactionListRelation_Re re;
		re.last_op_time = pf->info.last_op_time;
		re.alliance = pf->info.alliance;
		re.hostile = pf->info.hostile;
		re.apply = pf->info.apply;
		
		GFactionServer::Player player;
		GFactionServer* gfs = GFactionServer::GetInstance();
		if (gfs->GetPlayer(rid,player))
		{
			re.localsid = player.localsid;
			gfs->DispatchProtocol(player.linkid, re);
		}
		return 0;
	}

	void Factiondb::ListOnlineFaction(std::vector<unsigned int>& list)
	{
		Thread::RWLock::RDScoped l(locker);
		for(Map::iterator it = factions.begin(); it!=factions.end(); ++it)
		{
			if(it->second->info.level >= 2) list.push_back(it->first);
		}
	}

	void Factiondb::OnLeave(unsigned int fid, unsigned int rid)
	{
		Thread::RWLock::RDScoped l(locker);
		Map::iterator it = factions.find(fid);
		if(!IsReady(fid, it))
			return;
		FactionDetailInfo* pf = it->second;

		for(FMemberInfoVector::iterator it=pf->info.member.begin(); it!=pf->info.member.end();it++)
		{
			if(it->roleid==rid)
			{
				pf->info.member.erase(it);
				pf->version++;
				break;
			}
		}
		NotifyMemberUpdate(GT_FACTION_MEMBER_DEL, fid, 0, rid);
		RmvDelayExpel(rid);
	}

	void Factiondb::OnJoin(unsigned int fid, FMemberInfo& user)
	{

		Thread::RWLock::RDScoped l(locker);
		Map::iterator it = factions.find(fid);
		if(!IsReady(fid, it))
			return;
		FactionDetailInfo* pf = it->second;
		if (GFactionServer::GetInstance()->IsPlayerOnline(user.roleid))
			pf->online.add(user.roleid);
		user.loginday = (unsigned short)(Timer::GetTime()/86400);
		pf->info.member.add(user);
		pf->version++;
		NotifyPlayerFactionRelation(fid, user.roleid);
		NotifyMemberUpdate(GT_FACTION_MEMBER_ADD, fid, user.froleid, user.roleid, user.name);
	}

	void Factiondb::UpdateUser(unsigned int fid, GUserFaction& user)
	{
		Thread::RWLock::RDScoped l(locker);
		Map::iterator it = factions.find(fid);
		if(!IsReady(fid, it))
			return;
		FactionDetailInfo* pf = it->second;
		for(FMemberInfoVector::iterator it=pf->info.member.begin(); it!=pf->info.member.end();it++)
		{
			if(it->roleid==user.rid)
			{
				it->froleid = user.role;
				it->nickname.swap(user.nickname);
				pf->version++;
				break;
			}
		}
	}

	void Factiondb::RemoveFactionInfo(unsigned int fid)
	{
	}

	void Factiondb::UpdateFactionCache(UpdateFactionArg* arg)
	{
		Thread::RWLock::RDScoped l(locker);
		Map::iterator it = factions.find(arg->fid);
		if(!IsReady(arg->fid, it))
			return;
		FactionDetailInfo* pf = it->second;
		if(arg->type==1)
			pf->info.level++;
		else
			pf->info.announce = arg->announce;
		pf->version++;
	}

	void Factiondb::OnLogin(unsigned int fid, unsigned int rid, char role, int level, int contrib, int reputation, unsigned char reincarn_times, unsigned char gender, Octets& delayexpel)
	{
		Thread::RWLock::WRScoped l(locker);
		Map::iterator it = factions.find(fid);
		if(!IsReady(fid, it))
			return;
		FactionDetailInfo* pf = it->second;
		for(FMemberInfoVector::iterator it=pf->info.member.begin(); it!=pf->info.member.end();it++)
		{
			if(it->roleid==rid)
			{
				it->level = level;
				it->froleid = role;
				it->loginday = (unsigned short)(Timer::GetTime()/86400);
				it->contrib = contrib;
				it->reputation = reputation;
				it->reincarn_times = reincarn_times;
				it->gender = gender;
				
				int oldexpeltime = it->expeltime;
				char olddelayexpel = it->delayexpel;

				if(delayexpel.size())
				{
					try{
						Marshal::OctetsStream oval(delayexpel);
						oval >> it->delayexpel >> it->expeltime;
					}
					catch (Marshal::Exception e) // 防止错误的数据格式
					{ 
				   		it->delayexpel = olddelayexpel; 
				    		it->expeltime = oldexpeltime;
					}
				}

				if(it->delayexpel) // 用最新的时间更新删除倒计时
				{
				    	AddDelayExpel(fid,rid,it->expeltime);
				}
				else if(oldexpeltime) // 避免timeout的时候db失败且一直还在delayexpel tick
				{
					RmvDelayExpel(rid);
				}

				IntVector::iterator ito;
				for(ito=pf->online.begin(); ito!=pf->online.end()&&(*ito)!=(int)rid;ito++);
				if(ito==pf->online.end())
					pf->online.add(rid);
				break;
			}
		}
	}

	void Factiondb::OnLogout(unsigned int fid, unsigned int rid)
	{
		Thread::RWLock::RDScoped l(locker);
		Map::iterator it = factions.find(fid);
		if(!IsReady(fid, it))
			return;
		FactionDetailInfo* pf = it->second;
		for(FMemberInfoVector::iterator it=pf->info.member.begin(); it!=pf->info.member.end();it++)
		{
			if(it->roleid==rid)
			{
				for(IntVector::iterator ito=pf->online.begin(); ito!=pf->online.end();ito++)
				{
					if(*ito==(int)rid)
					{
						pf->online.erase(ito);
						break;
					}
				}
				break;
			}
		}

	}

	int Factiondb::UpgradeFaction(unsigned int fid, int roleid, unsigned int money, OperWrapper::wref_t oper)
	{
		char level = 0;

		{
			Thread::RWLock::RDScoped l(locker);
			Map::iterator it = factions.find(fid);
			if(!IsReady(fid, it))
				return ERR_FC_OUTOFSERVICE;
			else
				level = it->second->info.level;
		}
		int cost = FactionConfig::UpgradeCost(++level);
		if(cost<0 || cost>(int)money)
			return ERR_FC_OUTOFSERVICE;
		DBFactionUpgradeArg arg(fid, roleid, money-cost, level);
		DBFactionUpgrade* rpc = (DBFactionUpgrade*) Rpc::Call(RPC_DBFACTIONUPGRADE,&arg);
		rpc->oper = oper;
		GFactionDBClient::GetInstance()->SendProtocol(rpc);
		return 0;
	}

	void Factiondb::OnPromote(unsigned int fid, int superior, int roleid, char suprole, char newrole)
	{
		Thread::RWLock::RDScoped l(locker);
		Map::iterator it = factions.find(fid);
		if(!IsReady(fid, it))
			return;
		FactionDetailInfo* pf = it->second;
		if(newrole==_R_MASTER)
			pf->info.master = roleid;
		for(FMemberInfoVector::iterator it=pf->info.member.begin(); it!=pf->info.member.end();it++)
		{
			if((int)it->roleid==roleid) {
				it->froleid = newrole;
				NotifyMemberUpdate(GT_FACTION_MEMBER_MOVE, fid, newrole, roleid);
			}
			if(newrole==_R_MASTER && (int)it->roleid==superior) {
				it->froleid = _R_MEMBER;
				NotifyMemberUpdate(GT_FACTION_MEMBER_MOVE, fid, _R_MEMBER, superior);
			}
		}
	}

	void Factiondb::OnDeleteRole(unsigned int fid, unsigned int rid)
	{
		int newmaster = 0;
		int role = 0;
		Thread::RWLock::RDScoped l(locker);
		Map::iterator it = factions.find(fid);
		if(!IsReady(fid, it))
			return;
		FactionDetailInfo* pf = it->second;

		for(FMemberInfoVector::iterator it=pf->info.member.begin(); it!=pf->info.member.end();it++)
		{
			if(it->roleid==rid)
			{
				role = it->froleid;
				
				if(it->froleid==_R_MASTER && pf->info.member.size()>1)
				{
					FMemberInfoVector::iterator itm = pf->info.member.begin();
					int minrole = _R_MEMBER+1;
					for(FMemberInfoVector::iterator its=pf->info.member.begin();its!=pf->info.member.end();its++)
					{
						if(its->froleid<minrole && its!=it)
						{
							minrole = its->froleid;
							itm = its;
						}
					}
					newmaster = itm->roleid;
					itm->froleid = _R_MASTER;

					if(AlreadyDelayExpel(itm->roleid)) // db已经先行删除 不需要同步db
					{							
						itm->delayexpel = 0;
						itm->expeltime = 0;
	
						RmvDelayExpel(itm->roleid);
						
						gfs_broadcast_factiondelayexpelannounce(fid,FC_DELAYEXPEL_CANCEL,rid,itm->roleid,0);
					}

					FactionId arg(fid);
					GetFactionDetail* rpc = (GetFactionDetail*) Rpc::Call(RPC_GETFACTIONDETAIL,&arg);
					GFactionDBClient::GetInstance()->SendProtocol(rpc);
				}
				
				for(IntVector::iterator ito=pf->online.begin(); ito!=pf->online.end();ito++)
				{
					if(*ito==(int)rid)
					{
						pf->online.erase(ito);
						break;
					}
				}
				
				pf->info.member.erase(it);
				break;				
			}
		}
		if(newmaster)
		{
			GFactionServer::GetInstance()->UpdatePlayer(newmaster, fid, _R_MASTER, false );
			gfs_broadcast_masterresign_re(fid,newmaster);
		}
		gfs_broadcast_leave_re(fid,rid,role);
		
		RmvDelayExpel(rid);
	}

	void Factiondb::OnUpgrade(unsigned int fid, char level)
	{
		Thread::RWLock::RDScoped l(locker);
		Map::iterator it = factions.find(fid);
		if(!IsReady(fid, it))
			return;
		FactionDetailInfo* pf = it->second;
		pf->info.level = level;
		pf->version++;
	}

	void Factiondb::SyncFactionInfo(GFactionInfo& info)
	{
		int oldlevel;
		{
			Thread::RWLock::RDScoped l(locker);
			Map::iterator it = factions.find(info.fid);
			if(!IsReady(info.fid, it))
				return;
			FactionDetailInfo* pf = it->second;
			oldlevel = pf->info.level;
			pf->info.name = info.name;
			pf->info.level = info.level;
			pf->info.master = info.master.rid;
			pf->info.announce = info.announce;
		}

		if(oldlevel<info.level)
			gfs_broadcast_upgradefaction_re(info.fid);
	}

	int Factiondb::GetMaxbonus(unsigned int fid, int rid)
	{
		Thread::RWLock::RDScoped l(locker);
		Map::iterator it = factions.find(fid);
		if(!IsReady(fid, it))
			return 0;
		FactionDetailInfo* pf = it->second;
		if(rid>0 && (int)pf->info.master!=rid)
			return 0;
		time_t now = Timer::GetTime();
		unsigned short day = now/86400;

		if(now-pf->updatetime>3600) 
		{
			pf->updatetime = now;
			int suma=0,sumb=0;
			for(FMemberInfoVector::iterator it=pf->info.member.begin(); it!=pf->info.member.end();++it)
			{
				if(day-it->loginday<=8 && it->level>=50)
				{
					sumb++;
					suma += it->level;
				}
			}
			suma -= sumb*50;
			if(suma>1600)
				suma = 1600;
			pf->maxbonus = (int)(0.0533*suma*suma*suma);
			if(pf->maxbonus>MAX_PACKAGE_MONEY*0.9)
				pf->maxbonus = (int)(MAX_PACKAGE_MONEY*0.9);
			if(pf->maxbonus<10000)
				pf->maxbonus = 10000;
			DEBUG_PRINT("GetMaxbonus, fid=%d,population=%d,maxbonus=%d,time=%d\n",fid,pf->info.member.size(),
					pf->maxbonus, now);
		}
		return pf->maxbonus;
	}

	bool Factiondb::IsReady(unsigned int fid, Map::iterator& it,bool autoload)
	{
		time_t now = Timer::GetTime();
		if(!fid)
			return false;
		
		if(it==factions.end())
		{
			if(autoload)
			{
				FactionDetailInfo* detail = new FactionDetailInfo;
				detail->updatetime = now;
				detail->status = 0;
				detail->waitload = false;
				it = factions.insert(std::make_pair(fid,detail)).first;
			}
		}
		else if(it->second->status)
			return true;
		else if(it->second->waitload) // for db loading
			return false;
		else if(now-it->second->updatetime<300)
			return false;		
		else if(autoload)
			it->second->updatetime = now;

		if(autoload)
		{
			FactionId arg(fid);
			GetFactionDetail* rpc = (GetFactionDetail*) Rpc::Call(RPC_GETFACTIONDETAIL,&arg);
			GFactionDBClient::GetInstance()->SendProtocol(rpc);
			it->second->waitload = true;
		}
		return false;
	}

	void Factiondb::NotifyPlayerFactionRelation(int fid, int roleid)
	{
		typedef std::map<int/*gsid*/, std::vector<int/*roleid*/> > gs_role_map;

		Thread::RWLock::RDScoped l(locker);
		Map::iterator it = factions.find(fid);
		if(!IsReady(fid, it))
			return;
		FactionDetailInfo* pf = it->second;

		GetPlayerFactionRelation_Re re;
		re.factionid = fid;
		for(size_t i=0; i<pf->info.alliance.size(); i++)
			re.alliance.push_back(pf->info.alliance[i].fid);
		for(size_t i=0; i<pf->info.hostile.size(); i++)
			re.hostile.push_back(pf->info.hostile[i].fid);

		GFactionServer* gfs=GFactionServer::GetInstance();
		if(roleid)
		{
			re.roleid_list.push_back(roleid);
			GFactionServer::Player player;
			if(gfs->GetPlayer(roleid,player))	
				GProviderServer::GetInstance()->DispatchProtocol(player.gsid,re);
			return;
		}
		
		gs_role_map role_map;
		for(GFactionServer::FactionMemberMap::iterator
				itb=gfs->factionmembermap.lower_bound(fid),
				ite=gfs->factionmembermap.upper_bound(fid);
				itb!=ite; ++itb)
		{
			if ((*itb).second)
				role_map[(*itb).second->gsid].push_back((*itb).second->roleid);
		}
		for(gs_role_map::iterator it=role_map.begin(); it!=role_map.end(); ++it)
		{
			re.roleid_list.swap(it->second);
			gfs->DispatchProtocol(it->first,re);
		}
	}

	void Factiondb::NotifyMemberPlayerRename(unsigned int fid, int rid, Octets& name)
	{
		DEBUG_PRINT("Factiondb::NotifyMemberPlayerRename fid=%d rid=%d namesize=%d\n", fid,rid,name.size());
		Thread::RWLock::RDScoped l(locker);
		Map::iterator it = factions.find(fid);
		if(!IsReady(fid, it))
			return;
		typedef std::map<int/*linkid*/, std::vector<int/*localsid*/> > link_localsid_map;
		GFactionServer* gfs = GFactionServer::GetInstance();
		PlayerNameUpdate notice;
		notice.roleid = rid;
		notice.newname = name;
		link_localsid_map role_map;
		for(GFactionServer::FactionMemberMap::iterator
				itb=gfs->factionmembermap.lower_bound(fid),
				ite=gfs->factionmembermap.upper_bound(fid);
				itb!=ite; ++itb)
		{
			if ((*itb).second)
			{
				role_map[(*itb).second->linkid].push_back((*itb).second->localsid);
			}
		}
		for(link_localsid_map::iterator it=role_map.begin(), et=role_map.end(); it!=et; ++it)
		{
			notice.link_localsid_list.swap(it->second);
			gfs->DispatchProtocol(it->first,notice);
		}
		DEBUG_PRINT("Factiondb::NotifyMemberPlayerRename OK! fid=%d rid=%d namesize=%d\n", fid,rid,name.size());
	}
	
	int Factiondb::AllianceApply(int fid, int dst_fid, OperWrapper::wref_t oper)
	{
		{
			Thread::RWLock::RDScoped l(locker);

			Map::iterator it = factions.find(fid);
			if(!IsReady(fid, it))
				return ERR_FC_OUTOFSERVICE;

			FactionDetailInfo* pf = it->second;
			if(pf->info.level < 2) return ERR_FR_LEVEL_NOT_MATCH;		//三级帮派
			if(Timer::GetTime()-pf->info.last_op_time < OP_COOLDOWN_TIME) return ERR_FR_OP_IN_COOLDOWN;
			if(GetRelationType(fid,dst_fid) != NONE_RELATION) return ERR_FR_OP_DENY_IN_CUR_RELATION; 
		}
		DBFactionAllianceApply * rpc = (DBFactionAllianceApply *)Rpc::Call(
					RPC_DBFACTIONALLIANCEAPPLY,
					DBFactionAllianceApplyArg(fid,dst_fid,Timer::GetTime()+APPLY_TIMEOUT,Timer::GetTime())				
				);
		rpc->oper = oper;
		GFactionDBClient::GetInstance()->SendProtocol(rpc);
		return ERR_SUCCESS;
	}
	
	int Factiondb::AllianceReply(int fid, int dst_fid, char agree, OperWrapper::wref_t oper)
	{
		{
			Thread::RWLock::RDScoped l(locker);

			Map::iterator it = factions.find(fid);
			if(!IsReady(fid, it))
				return ERR_FC_OUTOFSERVICE;

			if(!Find(it->second->info.apply, ALLIANCE_FROM_OTHER, dst_fid))
				return ERR_FR_NO_MATCH_APPLY;
		}
		DBFactionAllianceReply * rpc = (DBFactionAllianceReply *)Rpc::Call(
					RPC_DBFACTIONALLIANCEREPLY,
					DBFactionAllianceReplyArg(fid,dst_fid,agree,agree?Timer::GetTime()+RELATION_DURATION:0)				
				);
		rpc->oper = oper;
		GFactionDBClient::GetInstance()->SendProtocol(rpc);
		return ERR_SUCCESS;
	}
	
	int Factiondb::HostileApply(int fid, int dst_fid, OperWrapper::wref_t oper)
	{
		{
			Thread::RWLock::RDScoped l(locker);

			Map::iterator it = factions.find(fid);
			if(!IsReady(fid, it))
				return ERR_FC_OUTOFSERVICE;

			FactionDetailInfo* pf = it->second;
			if(pf->info.level < 2) return ERR_FR_LEVEL_NOT_MATCH;		//三级帮派
			if(Timer::GetTime()-pf->info.last_op_time < OP_COOLDOWN_TIME) return ERR_FR_OP_IN_COOLDOWN;
			if(GetRelationType(fid,dst_fid) != NONE_RELATION) return ERR_FR_OP_DENY_IN_CUR_RELATION; 
		}
		DBFactionHostileApply * rpc = (DBFactionHostileApply *)Rpc::Call(
					RPC_DBFACTIONHOSTILEAPPLY,
					DBFactionHostileApplyArg(fid,dst_fid,Timer::GetTime()+APPLY_TIMEOUT,Timer::GetTime())				
				);
		rpc->oper = oper;
		GFactionDBClient::GetInstance()->SendProtocol(rpc);
		return ERR_SUCCESS;
	}
	
	int Factiondb::HostileReply(int fid, int dst_fid, char agree, OperWrapper::wref_t oper)
	{
		{
			Thread::RWLock::RDScoped l(locker);

			Map::iterator it = factions.find(fid);
			if(!IsReady(fid, it))
				return ERR_FC_OUTOFSERVICE;

			if(!Find(it->second->info.apply, HOSTILE_FROM_OTHER, dst_fid))
				return ERR_FR_NO_MATCH_APPLY;
		}
		DBFactionHostileReply * rpc = (DBFactionHostileReply *)Rpc::Call(
					RPC_DBFACTIONHOSTILEREPLY,
					DBFactionHostileReplyArg(fid,dst_fid,agree,agree?Timer::GetTime()+RELATION_DURATION:0)				
				);
		rpc->oper = oper;
		GFactionDBClient::GetInstance()->SendProtocol(rpc);
		return ERR_SUCCESS;
	}
	int Factiondb::RemoveRelationApply(int fid, int dst_fid, char force, OperWrapper::wref_t oper)
	{
		{
			Thread::RWLock::RDScoped l(locker);

			Map::iterator it = factions.find(fid);
			if(!IsReady(fid, it))
				return ERR_FC_OUTOFSERVICE;

			FactionDetailInfo* pf = it->second;
			if(Timer::GetTime()-pf->info.last_op_time < OP_COOLDOWN_TIME) return ERR_FR_OP_IN_COOLDOWN;
			int cur_relation = GetRelationType(fid,dst_fid);
			if(cur_relation != ALLIANCE && cur_relation != HOSTILE) return ERR_FR_OP_DENY_IN_CUR_RELATION; 
		}
		DBFactionRemoveRelationApply * rpc = (DBFactionRemoveRelationApply *)Rpc::Call(
					RPC_DBFACTIONREMOVERELATIONAPPLY,
					DBFactionRemoveRelationApplyArg(fid,dst_fid,force,force?0:Timer::GetTime()+APPLY_TIMEOUT,Timer::GetTime())				
				);
		rpc->oper = oper;
		GFactionDBClient::GetInstance()->SendProtocol(rpc);
		return ERR_SUCCESS;
	}
	int Factiondb::RemoveRelationReply(int fid, int dst_fid, char agree, OperWrapper::wref_t oper)
	{
		{
			Thread::RWLock::RDScoped l(locker);

			Map::iterator it = factions.find(fid);
			if(!IsReady(fid, it))
				return ERR_FC_OUTOFSERVICE;

			if(!Find(it->second->info.apply, REMOVE_RELATION_FROM_OTHER, dst_fid))
				return ERR_FR_NO_MATCH_APPLY;
		}
		DBFactionRemoveRelationReply * rpc = (DBFactionRemoveRelationReply *)Rpc::Call(
					RPC_DBFACTIONREMOVERELATIONREPLY,
					DBFactionRemoveRelationReplyArg(fid,dst_fid,agree)				
				);
		rpc->oper = oper;
		GFactionDBClient::GetInstance()->SendProtocol(rpc);
		return ERR_SUCCESS;
	}
		
	void Factiondb::OnAllianceApply(int fid, int dst_fid, int end_time, int op_time)
	{
		Thread::RWLock::WRScoped l(locker);
		relation_map[faction_pair(fid,dst_fid)] = faction_relation(ALLIANCE_HALF, end_time, fid, dst_fid);
		{
			Map::iterator it = factions.find(fid);
			if(IsReady(fid, it, false))
			{
				Add(it->second->info.apply,ALLIANCE_TO_OTHER,dst_fid,end_time);
				it->second->info.last_op_time = op_time;
			}
		}
		{
			Map::iterator it = factions.find(dst_fid);
			if(IsReady(dst_fid, it, false))
				Add(it->second->info.apply,ALLIANCE_FROM_OTHER,fid,end_time);
		}
	}
	void Factiondb::OnAllianceReply(int fid, int dst_fid, char agree, int end_time, Octets& fname1, Octets& fname2)
	{
		Thread::RWLock::WRScoped l(locker);
		if(agree)
			relation_map[faction_pair(fid,dst_fid)] = faction_relation(ALLIANCE, end_time, fid, dst_fid);
		else
			relation_map.erase(faction_pair(fid,dst_fid));
		{
			Map::iterator it = factions.find(fid);
			if(IsReady(fid, it, false))
			{
				Remove(it->second->info.apply,ALLIANCE_FROM_OTHER,dst_fid);
				if(agree) Add(it->second->info.alliance,dst_fid,end_time);
			}
		}
		{
			Map::iterator it = factions.find(dst_fid);
			if(IsReady(dst_fid, it, false))
			{
				Remove(it->second->info.apply,ALLIANCE_TO_OTHER,fid);
				if(agree) Add(it->second->info.alliance,fid,end_time);
			}
		}
		if(agree)
		{
			WorldChat chat;
			chat.channel = GN_CHAT_CHANNEL_SYSTEM;
			chat.roleid = CMSG_FACTION_BECAME_ALLIANCE;
			chat.msg = Marshal::OctetsStream() << fname1 << fname2;
			GFactionServer::GetInstance()->BroadcastLink(chat);
			//通知gs
			NotifyPlayerFactionRelation(fid);
			NotifyPlayerFactionRelation(dst_fid);
		}
	}
	void Factiondb::OnHostileApply(int fid, int dst_fid, int end_time, int op_time)
	{
		Thread::RWLock::WRScoped l(locker);
		relation_map[faction_pair(fid,dst_fid)] = faction_relation(HOSTILE_HALF, end_time, fid, dst_fid);
		{
			Map::iterator it = factions.find(fid);
			if(IsReady(fid, it, false))
			{
				Add(it->second->info.apply,HOSTILE_TO_OTHER,dst_fid,end_time);
				it->second->info.last_op_time = op_time;
			}
		}
		{
			Map::iterator it = factions.find(dst_fid);
			if(IsReady(dst_fid, it, false))
				Add(it->second->info.apply,HOSTILE_FROM_OTHER,fid,end_time);
		}
	}
	void Factiondb::OnHostileReply(int fid, int dst_fid, char agree, int end_time, Octets& fname1, Octets& fname2)
	{
		Thread::RWLock::WRScoped l(locker);
		if(agree)
			relation_map[faction_pair(fid,dst_fid)] = faction_relation(HOSTILE, end_time, fid, dst_fid);
		else
			relation_map.erase(faction_pair(fid,dst_fid));
		{
			Map::iterator it = factions.find(fid);
			if(IsReady(fid, it, false))
			{
				Remove(it->second->info.apply,HOSTILE_FROM_OTHER,dst_fid);
				if(agree) Add(it->second->info.hostile,dst_fid,end_time);
			}
		}
		{
			Map::iterator it = factions.find(dst_fid);
			if(IsReady(dst_fid, it, false))
			{
				Remove(it->second->info.apply,HOSTILE_TO_OTHER,fid);
				if(agree) Add(it->second->info.hostile,fid,end_time);
			}
		}
		if(agree)
		{
			WorldChat chat;
			chat.channel = GN_CHAT_CHANNEL_SYSTEM;
			chat.roleid = CMSG_FACTION_BECAME_HOSTILE;
			chat.msg = Marshal::OctetsStream() << fname1 << fname2;
			GFactionServer::GetInstance()->BroadcastLink(chat);
			//通知gs
			NotifyPlayerFactionRelation(fid);
			NotifyPlayerFactionRelation(dst_fid);
		}
	}
	void Factiondb::OnRemoveRelationApply(int fid, int dst_fid, char force, int end_time, int op_time, Octets& fname1, Octets& fname2)
	{
		Thread::RWLock::WRScoped l(locker);
		int old_relation = NONE_RELATION;
		if(force)
			relation_map.erase(faction_pair(fid,dst_fid));
		else
			relation_map[faction_pair(fid,dst_fid)] = faction_relation(REMOVE_RELATION_HALF, end_time, fid, dst_fid);
		{
			Map::iterator it = factions.find(fid);
			if(IsReady(fid, it, false))
			{
				if(force)
				{
					if(Remove(it->second->info.alliance,dst_fid))
						old_relation = ALLIANCE;
					else if(Remove(it->second->info.hostile,dst_fid))
						old_relation = HOSTILE;
				}
				else
					Add(it->second->info.apply,REMOVE_RELATION_TO_OTHER,dst_fid,end_time);
				it->second->info.last_op_time = op_time;
			}
		}
		{
			Map::iterator it = factions.find(dst_fid);
			if(IsReady(dst_fid, it, false))
			{
				if(force)
				{
					if(Remove(it->second->info.alliance,fid))
						old_relation = ALLIANCE;
					else if(Remove(it->second->info.hostile,fid))
						old_relation = HOSTILE;
				}
				else
					Add(it->second->info.apply,REMOVE_RELATION_FROM_OTHER,fid,end_time);
			}
		}
		if(force && old_relation != NONE_RELATION)
		{
			WorldChat chat;
			chat.channel = GN_CHAT_CHANNEL_SYSTEM;
			chat.roleid = (old_relation==ALLIANCE ? CMSG_FACTION_REMOVE_ALLIANCE :CMSG_FACTION_REMOVE_HOSTILE);
			chat.msg = Marshal::OctetsStream() << fname1 << fname2 << (int)force;
			GFactionServer::GetInstance()->BroadcastLink(chat);
			//通知gs
			NotifyPlayerFactionRelation(fid);
			NotifyPlayerFactionRelation(dst_fid);
		}
	}
	void Factiondb::OnRemoveRelationReply(int fid, int dst_fid, char agree, Octets& fname1, Octets& fname2)
	{
		Thread::RWLock::WRScoped l(locker);
		int old_relation = NONE_RELATION;
		{
			Map::iterator it = factions.find(fid);
			if(IsReady(fid, it, false))
			{
				Remove(it->second->info.apply,REMOVE_RELATION_FROM_OTHER,dst_fid);
				if(agree)
				{
					if(Remove(it->second->info.alliance,dst_fid))
						old_relation = ALLIANCE;
					else if(Remove(it->second->info.hostile,dst_fid))
						old_relation = HOSTILE;
					relation_map.erase(faction_pair(fid,dst_fid));
				}
				else
				{
					int end_time = -1;
					if( (end_time = Get(it->second->info.alliance,dst_fid)) > 0)
						relation_map[faction_pair(fid,dst_fid)] = faction_relation(ALLIANCE, end_time, fid, dst_fid);	
					else if( (end_time = Get(it->second->info.hostile,dst_fid)) > 0)
						relation_map[faction_pair(fid,dst_fid)] = faction_relation(HOSTILE, end_time, fid, dst_fid);	
				}
			}
		}
		{
			Map::iterator it = factions.find(dst_fid);
			if(IsReady(dst_fid, it, false))
			{
				Remove(it->second->info.apply,REMOVE_RELATION_TO_OTHER,fid);
				if(agree)
				{
					if(Remove(it->second->info.alliance,fid))
						old_relation = ALLIANCE;
					else if(Remove(it->second->info.hostile,fid))
						old_relation = HOSTILE;
					relation_map.erase(faction_pair(fid,dst_fid));
				}
				else
				{
					int end_time = -1;
					if( (end_time = Get(it->second->info.alliance,fid)) > 0)
						relation_map[faction_pair(fid,dst_fid)] = faction_relation(ALLIANCE, end_time, fid, dst_fid);	
					else if( (end_time = Get(it->second->info.hostile,fid)) > 0)
						relation_map[faction_pair(fid,dst_fid)] = faction_relation(HOSTILE, end_time, fid, dst_fid);	
				}
			}
		}
		if(agree && old_relation != NONE_RELATION)
		{
			WorldChat chat;
			chat.channel = GN_CHAT_CHANNEL_SYSTEM;
			chat.roleid = (old_relation==ALLIANCE ? CMSG_FACTION_REMOVE_ALLIANCE :CMSG_FACTION_REMOVE_HOSTILE);
			chat.msg = Marshal::OctetsStream() << fname1 << fname2 << (int)0;
			GFactionServer::GetInstance()->BroadcastLink(chat);
			//通知gs
			NotifyPlayerFactionRelation(fid);
			NotifyPlayerFactionRelation(dst_fid);
		}
	}
	void Factiondb::OnRelationTimeout(int type, int fid1, int fid2, Octets& fname1, Octets& fname2)
	{
		Thread::RWLock::WRScoped l(locker);
		relation_map.erase(faction_pair(fid1,fid2));
		{
			Map::iterator it = factions.find(fid1);
			if(IsReady(fid1, it, false))
			{
				if(type == ALLIANCE)
					Remove(it->second->info.alliance,fid2);
				else if(type == HOSTILE)
					Remove(it->second->info.hostile,fid2);
			}
		}
		{
			Map::iterator it = factions.find(fid2);
			if(IsReady(fid2, it, false))
			{
				if(type == ALLIANCE)
					Remove(it->second->info.alliance,fid1);
				else if(type == HOSTILE)
					Remove(it->second->info.hostile,fid1);
			}
		}
		{
			WorldChat chat;
			chat.channel = GN_CHAT_CHANNEL_SYSTEM;
			chat.roleid = (type==ALLIANCE ? CMSG_FACTION_REMOVE_ALLIANCE :CMSG_FACTION_REMOVE_HOSTILE);
			chat.msg = Marshal::OctetsStream() << fname1 << fname2 << (int)0;
			GFactionServer::GetInstance()->BroadcastLink(chat);
			//通知gs
			NotifyPlayerFactionRelation(fid1);
			NotifyPlayerFactionRelation(fid2);
		}
	}
	void Factiondb::OnRelationApplyTimeout(int type, int fid1, int fid2, int end_time, Octets& fname1, Octets& fname2)
	{
		Thread::RWLock::WRScoped l(locker);
		{
			Map::iterator it = factions.find(fid1);
			if(IsReady(fid1, it, false))
			{
				Remove(it->second->info.apply,fid2);
				if(type == ALLIANCE_HALF)
				{
					relation_map.erase(faction_pair(fid1,fid2));
				}
				else if(type == HOSTILE_HALF)
				{
					Add(it->second->info.hostile,fid2,end_time);
					relation_map[faction_pair(fid1,fid2)] = faction_relation(HOSTILE, end_time, fid1, fid2);	
				}
				else if(type == REMOVE_RELATION_HALF)
				{
					int etime = -1;
					if( (etime = Get(it->second->info.alliance,fid2)) > 0)
						relation_map[faction_pair(fid1,fid2)] = faction_relation(ALLIANCE, etime, fid1, fid2);	
					else if( (etime = Get(it->second->info.hostile,fid2)) > 0)
						relation_map[faction_pair(fid1,fid2)] = faction_relation(HOSTILE, etime, fid1, fid2);	
				}
			}
		}
		{
			Map::iterator it = factions.find(fid2);
			if(IsReady(fid2, it, false))
			{
				Remove(it->second->info.apply,fid1);
				if(type == ALLIANCE_HALF)
				{
					relation_map.erase(faction_pair(fid1,fid2));
				}
				else if(type == HOSTILE_HALF)
				{
					Add(it->second->info.hostile,fid1,end_time);
					relation_map[faction_pair(fid1,fid2)] = faction_relation(HOSTILE, end_time, fid1, fid2);	
				}
				else if(type == REMOVE_RELATION_HALF)
				{
					int etime = -1;
					if( (etime = Get(it->second->info.alliance,fid1)) > 0)
						relation_map[faction_pair(fid1,fid2)] = faction_relation(ALLIANCE, etime, fid1, fid2);	
					else if( (etime = Get(it->second->info.hostile,fid1)) > 0)
						relation_map[faction_pair(fid1,fid2)] = faction_relation(HOSTILE, etime, fid1, fid2);	
				}
			}
		}
		if(type == HOSTILE_HALF)
		{
			WorldChat chat;
			chat.channel = GN_CHAT_CHANNEL_SYSTEM;
			chat.roleid = CMSG_FACTION_BECAME_HOSTILE;
			chat.msg = Marshal::OctetsStream() << fname1 << fname2;
			GFactionServer::GetInstance()->BroadcastLink(chat);
			//通知gs
			NotifyPlayerFactionRelation(fid1);
			NotifyPlayerFactionRelation(fid2);
		}
	}

	void Factiondb::OnPlayerRename(int fid, int roleid, Octets& name)
	{
		DEBUG_PRINT("Factiondb::OnPlayerRename fid=%d rid=%d\n", fid,roleid);
		{
			Thread::RWLock::WRScoped l(locker);
			Map::iterator it = factions.find(fid);
			if(!IsReady(fid, it))
				return;
			for(FMemberInfoVector::iterator tit=it->second->info.member.begin(), etit=it->second->info.member.end(); tit != etit; tit++)
			{
				if (tit->roleid == (unsigned int)roleid)
				{
					tit->name = name;
					DEBUG_PRINT("Factiondb::OnPlayerRename rename ok! namesize=%d\n",name.size());
					break;
				}
			}
		}
		NotifyMemberPlayerRename(fid, roleid, name);
	}

	void Factiondb::OnDelayExpel(int fid, int roleid, int time, int operatorid)
	{
		Thread::RWLock::WRScoped l(locker);
		Map::iterator it = factions.find(fid);
		if(!IsReady(fid, it))
			return;
		for(FMemberInfoVector::iterator tit=it->second->info.member.begin(), etit=it->second->info.member.end(); tit != etit; tit++)
		{
			if (tit->roleid == (unsigned int)roleid)
			{
				tit->delayexpel = 1;
				tit->expeltime = time;

				AddDelayExpel(fid,roleid,time);
				
				DEBUG_PRINT("Factiondb::OnDelayExpel ok! roleid=%d factionid=%d\n",roleid,fid);

				if(operatorid)// 更新给操作者一条最新的被操纵者信息
				{
					gfs_send_singlemember(operatorid,*tit);
				}

				break;
			}
		}
	
	}

	void Factiondb::OnCancelExpel(int fid, int roleid)
	{
		Thread::RWLock::WRScoped l(locker);
		Map::iterator it = factions.find(fid);
		if(!IsReady(fid, it))
			return;
		for(FMemberInfoVector::iterator tit=it->second->info.member.begin(), etit=it->second->info.member.end(); tit != etit; tit++)
		{
			if (tit->roleid == (unsigned int)roleid)
			{
				tit->delayexpel = 0;
				tit->expeltime = 0;

				RmvDelayExpel(roleid);
				
				DEBUG_PRINT("Factiondb::OnCancelExpel ok! roleid=%d factionid=%d\n",roleid,fid);
				break;
			}
		}
	}

	void Factiondb::OnTimeoutExpel(int fid, int roleid)
	{
		DEBUG_PRINT("Factiondb::OnTimeoutExpel ok! roleid=%d factionid=%d\n",roleid,fid);
	   	if(IsSpecialMember(fid, roleid))
		{
		   	// anounce first
		   	gfs_broadcast_factiondelayexpelannounce(fid,FC_DELAYEXPEL_TIMEOUT,0,roleid,0);
			GFactionServer::GetInstance()->UpdatePlayer(roleid,_FACTION_ID_INVALID,_R_UNMEMBER);
			// cache update
		   	OnLeave(fid,roleid);
		}
		else
		{
			RmvDelayExpel(roleid);
		}
	}

	void Factiondb::OnUpdateExpel(unsigned int fid, unsigned int rid, char role, int level, int contrib, 
			int reputation, unsigned char reincarn_times, unsigned char gender, Octets& delayexpel)
	{
		Thread::RWLock::WRScoped l(locker);
		Map::iterator it = factions.find(fid);
		if(!IsReady(fid, it))
			return;
		FactionDetailInfo* pf = it->second;
		for(FMemberInfoVector::iterator it=pf->info.member.begin(); it!=pf->info.member.end();it++)
		{
			if(it->roleid==rid)
			{
				it->level = level;
				it->froleid = role;
				it->loginday = (unsigned short)(Timer::GetTime()/86400);
				it->contrib = contrib;
				it->reputation = reputation;
				it->reincarn_times = reincarn_times;
				it->gender = gender;

				int oldexpeltime = it->expeltime;
				char olddelayexpel = it->delayexpel;

				if(delayexpel.size())
				{
					try{
						Marshal::OctetsStream oval(delayexpel);
						oval >> it->delayexpel >> it->expeltime;
					}
					catch (Marshal::Exception e) // 防止错误的数据格式
					{ 
				   		it->delayexpel = olddelayexpel; 
				    		it->expeltime = oldexpeltime;
					}
				}

				if(it->delayexpel) // 用最新的时间更新删除倒计时
				{
				   	AddDelayExpel(fid,rid,it->expeltime);
				}
				else if(oldexpeltime) // 避免timeout的时候db失败且一直还在delayexpel tick
				{
					RmvDelayExpel(rid);
				}
				break;
			}
		}
	}

	void Factiondb::OnFactionRename(int fid,const Octets& new_name)
	{
		Thread::RWLock::RDScoped l(locker);
		Map::iterator it = factions.find(fid);
		if(!IsReady(fid, it))
			return;
		FactionDetailInfo* pf = it->second;
		pf->info.name = new_name;
	}

	int64_t Factiondb::GetUnifid(unsigned int fid)
	{
		Thread::RWLock::RDScoped l(locker);
		Map::iterator it = factions.find(fid);
		if(!IsReady(fid, it))
			return 0;
		FactionDetailInfo* pf = it->second;

		return pf->info.unifid;
	}

	void Factiondb::SetUnifid(unsigned int fid,int64_t unifid)
	{
		Thread::RWLock::RDScoped l(locker);
		Map::iterator it = factions.find(fid);
		if(!IsReady(fid, it))
			return;
		FactionDetailInfo* pf = it->second;

		pf->info.unifid = unifid;

		gfs_update_factionunifidannounce(fid,GetPvpMask(fid),unifid);
	}

	int Factiondb::GetDelayExpelTime(unsigned int rid)
	{
	    Thread::RWLock::RDScoped l(locker);
	    DelayExpelMap::iterator ibeg = delayexpel_map.find(rid);
	    return ibeg != delayexpel_map.end() ? ibeg->second.first : 0;
	}

	void Factiondb::DelayExpelTick(int now)
	{
		DelayExpelMap::iterator ibeg = delayexpel_map.begin();
		DelayExpelMap::iterator iend = delayexpel_map.end();

		int wastetime = now - last_delayupdate_time;
		last_delayupdate_time = now;

		for(;ibeg != iend; ++ibeg)
		{
			ibeg->second.first -= wastetime;
	    	DEBUG_PRINT("DelayExpelTick roleid%d delaytime %ds",ibeg->first,ibeg->second.first);
			if(ibeg->second.first <= 0)
			{
				ibeg->second.first = 0;
		    	DelayExpelTimeout(ibeg->second.second,ibeg->first);
			}
		}
	}

	void Factiondb::SettleDelayExpel(int wastetime)
	{
		DEBUG_PRINT("SettleDelayExpel wastetime %ds",wastetime);
		Thread::RWLock::WRScoped l(locker);	    
    	DelayExpelMap::iterator ibeg = delayexpel_map.begin();
		DelayExpelMap::iterator iend = delayexpel_map.end();

		for(;ibeg != iend; ++ibeg)
		{
			ibeg->second.first -= wastetime;
	    	if(ibeg->second.first <= 0)
			{
				ibeg->second.first = 0;
		    	DelayExpelTimeout(ibeg->second.second,ibeg->first);
			}
		}

	}
		
	unsigned char Factiondb::GetPvpMask(unsigned int fid)
	{
		Thread::RWLock::WRScoped l(locker);
		PvPMaskMap::iterator iter = pvpmask_map.find(fid);	
		
		return iter == pvpmask_map.end() ? 0 : iter->second;	
	}
	
	void Factiondb::SetPvpMask(unsigned int fid, unsigned char pmask)
	{
		Thread::RWLock::WRScoped l(locker);		
		pvpmask_map[fid] = pmask;

		DEBUG_PRINT("SetPvpMask faction %d mask%u",fid,pmask);
		Map::iterator it = factions.find(fid);
		if(!IsReady(fid, it))
			return ;
		gfs_update_factionpvpmask(fid,pmask,it->second->info.unifid);
	}

	void Factiondb::SetPvpStatus(char status) 
	{ 
		Thread::RWLock::WRScoped l(locker);
		pvp_status = status; 

		DEBUG_PRINT("SetPvpStatus status change to %u",status);
		if(!status)
		{
			PvPMaskMap::iterator iter = pvpmask_map.begin();
			PvPMaskMap::iterator iend = pvpmask_map.end();
			for(;iter != iend; ++iter)
			{
				Map::iterator it = factions.find(iter->first);
				int64_t unifid = 0;
				if(it != factions.end()) 
					unifid = it->second->info.unifid;
				gfs_update_factionpvpmask(iter->first,0,unifid);
			}
			
			pvpmask_map.clear();
		}
	}
};


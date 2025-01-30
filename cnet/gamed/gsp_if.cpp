
#include "types.h"
#include "matcher.h"
#include "obj_interface.h"
#include "libcommon.h"
#include "gsp_if.h"
#include "gproviderclient.hpp"
#include "s2cgamedatasend.hpp"
#include "playerlogin_re.hpp"
#include "playerlogout.hpp"
#include "playeroffline_re.hpp"
#include "playerkickout_re.hpp"
#include "s2cmulticast.hpp"
#include "chatmulticast.hpp"
#include "gtradediscard.hpp"
#include "gtradestart_re.hpp"
#include "chatsinglecast.hpp"
#include "playerheartbeat.hpp"
#include "switchservercancel.hpp"
#include "switchserverstart.hpp"
#include "switchserversuccess.hpp"
#include "switchservertimeout.hpp"
#include "disconnectplayer.hpp"
#include "gettaskdata.hpp"
#include "settaskdata.hpp"
#include "facemodifycancel.hpp"
#include "facemodify.hpp"
#include "chatbroadcast.hpp"
#include "debugcommand.hpp"
#include "setchatemotion.hpp"
#include "acreportcheater.hpp"
#include "actriggerquestion.hpp"
#include "putspouse.hrp"
#include "publicchat.hpp"
#include "sendrefcashused.hpp"
#include "sendtaskreward.hpp"
#include "factionchat.hpp"
#include "playerinfoupdate.hpp"
#include "snsrolebriefupdate.hpp"
#include "factioncongregaterequest.hpp"
#include "notifyplayerjoinorleaveforce.hpp"
#include "increaseforceactivity.hpp"
#include "synmutadata.hpp"
#include "countrybattleapply.hpp"
#include "countrybattlejoinnotice.hpp"
#include "countrybattleleavenotice.hpp"
#include "countrybattleofflinenotice.hpp"
#include "countrybattleonlinenotice.hpp"
#include "countrybattleentermapnotice.hpp"
#include "countrybattleserverregister.hpp"
#include "countrybattlestart_re.hpp"
#include "countrybattleend.hpp"
#include "playerteamop.hpp"
#include "playerteammemberop.hpp"
#include "trychangeds.hpp"
#include "playerchangeds_re.hpp"
#include "playerrename.hpp"
#include "playergivepresent.hpp"
#include "playeraskforpresent.hpp"
#include "touchpointquery.hpp"
#include "touchpointcost.hpp"
#include "giftcoderedeem.hpp"
#include "mobileserverregister.hpp"
#include "uniquedatamodifyrequire.hpp"
#include "autoteamconfigdata"
#include "autoteamconfigregister.hpp"
#include "autoteamsetgoal.hpp"
#include "autoteamplayerready_re.hpp"
#include "tankbattleserverregister.hpp"
#include "tankbattlestart_re.hpp"
#include "tankbattleend.hpp"
#include "tankbattleplayerscoreupdate.hpp"
#include "tankbattleplayerapply.hpp"
#include "tankbattleplayerenter.hpp"
#include "tankbattleplayerleave.hpp"
#include "factionresourcebattleserverregister.hpp"
#include "factionresourcebattleeventnotice.hpp"
#include "factionresourcebattleplayerquery.hpp"
#include "factionresourcebattlerequestconfig_re.hpp"
#include "dbplayerchangegender.hrp"
#include "updatesolochallengerank.hpp"
#include "getsolochallengerank.hpp"
#include "updateenemylist.hpp"
#include "mndomainbattlestart_re.hpp"
#include "mndomainbattleentersuccessnotice.hpp"
#include "mndomainbattleend.hpp"
#include "mnbattleserverregister.hpp"
#include "mndomainbattleenter.hpp"
#include "mndomainbattleleavenotice.hpp"
#include "mnfactionbattleapply.hpp"
#include "mngettoplist.hpp"
#include "mngetdomaindata.hpp"

#include <pthread.h>
#include <conf.h>
#include <queue>
#include <glog.h>

#include <assert.h>
#define _FACTIONSERVER_ID 101
void handle_user_cmd(int cs_index, int sid,int uid, const void * buf, unsigned int size);
void thread_usage_stat(const char * ident);
using namespace GNET;
namespace GMSV
{
enum DOMAIN_TYPE
{
	DOMAIN_TYPE_NULL = 0,
	DOMAIN_TYPE_3RD_CLASS,
	DOMAIN_TYPE_2ND_CLASS,
	DOMAIN_TYPE_1ST_CLASS,
};

static Thread::Mutex  g_city_lock;
static Thread::Mutex  g_factio_bonus_lock;
static int __global_gid = -1;
static city_entry city_list[256] = {{0,0}};
struct faction_bonus
{
	int faction;
	int bonus;
	faction_bonus(int faction, int bonus):faction(faction),bonus(bonus)
	{}
};

static std::vector<faction_bonus> __faction_bonus;

void SetCityOwner(int id, int level,int owner)
{
	g_city_lock.Lock();
	id = id & 0xFF;
	city_list[id].level = level;
	city_list[id].owner = owner;
	g_city_lock.UNLock();
}

int GetCityOwner(unsigned int id)
{
	if(id >= 256) return -1;
	return city_list[id].owner;
}

int GetCityLevel(unsigned int id)
{
	if(id >= 256) return -1;
	return city_list[id].level;
}

int GetMafiaCityCount(int mafiaid)
{
	int count = 0;
	if(mafiaid)
	{
		for(unsigned int i = 0; i < 256; i ++)
		{
			if(city_list[i].owner == mafiaid)	++count;
		}
	}
	return count;
}

struct __TBONUS
{
	int level1;
	int level2;
	int level3;
	__TBONUS():level1(0),level2(0),level3(0){}
};
void RefreshCityBonus()
{
	
	typedef std::map<int, __TBONUS> BONUSMAP;
	BONUSMAP __map;
	g_city_lock.Lock();
	for(unsigned int i = 0; i < 256; i ++)
	{
		if(city_list[i].owner <=0) continue;
		int owner = city_list[i].owner;
		switch(city_list[i].level)
		{
			case DOMAIN_TYPE_3RD_CLASS:
				__map[owner].level3 ++;
				break;
			case DOMAIN_TYPE_2ND_CLASS:
				__map[owner].level2 ++;
				break;
			case DOMAIN_TYPE_1ST_CLASS:
				__map[owner].level1 ++;
			break;
			default:
			break;
		}
	}
	g_city_lock.UNLock();

	
	std::vector<faction_bonus> tmp;
	BONUSMAP::iterator it = __map.begin();
	for(; it != __map.end(); ++it)
	{
		int bonus = 0;
		int level1 = it->second.level1;
		int level2 = it->second.level2;
		int level3 = it->second.level3;
		if(level3 > 0)
		{
			if(level3 == 1)
			{
				bonus = 1;
			}
			else
			{
				bonus = (int) ((sqrt(1 + 8*(level3 - 2)) - 1)/2 + 2);
			}
		}
		
		bonus += level2 * 2 + level1 * 3;
		tmp.push_back(faction_bonus(it->first,bonus));
		printf("faction:%d, bonus:%d , leve1:%d level2:%d level3:%d\n",it->first,bonus,level1,level2,level3);
	}

	g_factio_bonus_lock.Lock();
	__faction_bonus.swap(tmp);
	g_factio_bonus_lock.UNLock();
}

int GetFactionBonus(int faction)
{
	int bonus = 0;
	g_factio_bonus_lock.Lock();
	std::vector<faction_bonus>::iterator it = __faction_bonus.begin();
	for(; it !=  __faction_bonus.end(); ++it)
	{
		if(it->faction == faction)
		{
			bonus = it->bonus;
			break;
		}
	}
	g_factio_bonus_lock.UNLock();
	return bonus;
}

static int _count = 0;

unsigned long long s2c_cmd_number_counter1[1024];
unsigned long long s2c_cmd_number_counter2[1024];

static __inline__ void RecordS2CCmd(unsigned short cmd, unsigned int l)
{
	if(cmd >= 1024) return;
	s2c_cmd_number_counter1[cmd] ++;
	s2c_cmd_number_counter2[cmd] += l;
}

struct user_cmd_t
{
	int cs_index;
	int sid;
	int uid;
	Octets data;
};

#define MAX_USER_CMD_QUEUE_NUM 2
static std::deque<user_cmd_t> g_xlist[MAX_USER_CMD_QUEUE_NUM];
static Thread::Mutex  g_xlock[MAX_USER_CMD_QUEUE_NUM];
static int user_cmd_queue_num = 1;

void 
queue_user_cmd(int cs_index, int sid, int uid,Octets & data)
{
	user_cmd_t tmp;
	tmp.cs_index = cs_index;
	tmp.sid = sid;
	tmp.uid = uid;
	tmp.data.swap(data);
	int index = uid % user_cmd_queue_num;
	Thread::Mutex::Scoped keeper(g_xlock[index]);
	g_xlist[index].push_back(tmp);
}

bool SendToLS(int lid, GNET::Protocol * p)
{
	if(lid <=0) return false;
	return GProviderClient::DispatchProtocol(lid,p);
}

bool SendClientData(int lid, int userid/* actually is roleid*/,int sid,const void * buf, unsigned int size)
{
	if(lid <=0) return false;
	_count ++;
//	printf("send c2s(%d/%d) data size %d\n", userid,sid,size);
	RecordS2CCmd(*(unsigned short *)buf, 1);
	return GProviderClient::DispatchProtocol(lid,S2CGamedataSend(userid,sid,Octets(buf,size)));
}

bool MultiSendClientData(int lid,const puser * first, const puser * last,const void * buf, unsigned int size,int except_id)
{
	if(lid <=0) return false;
	_count ++;
	S2CMulticast packet; 
	packet.data.insert(packet.data.end(),buf,size);
	for(;first != last; ++first)
	{
		const std::pair<int,int> & val = *first;
		if(except_id !=  val.first)
		packet.playerlist.add(GNET::Player(val.first,val.second));
	}
	RecordS2CCmd(*(unsigned short *)buf, packet.playerlist.size());
	return GProviderClient::DispatchProtocol(lid,&packet);
}

bool MultiSendClientData(int lid, const puser * first, const puser * last,const void * buf, unsigned int size)
{
	if(lid <=0) return false;
	_count ++;
	S2CMulticast packet; 
	packet.data.insert(packet.data.end(),buf,size);
	for(;first != last; ++first)
	{
		const std::pair<int,int> & val = *first;
		packet.playerlist.add(GNET::Player(val.first,val.second));
	}
	RecordS2CCmd(*(unsigned short *)buf, packet.playerlist.size());
	return GProviderClient::DispatchProtocol(lid,&packet);
}


bool MultiChatMsg(int lid,const puser* first, const puser* last,const chat_msg & chat,int except_id)
{
	if(lid <=0) return false;
	_count ++;
	ChatMultiCast packet; 
	packet.msg.insert(packet.msg.end(),chat.msg,chat.size);
	packet.data.insert(packet.data.end(),chat.data,chat.dsize);
	packet.srcroleid = chat.speaker;
	packet.channel = chat.channel;
	packet.emotion = chat.emote_id;
	packet.srclevel = chat.speaker_level;
	for(;first != last; ++first)
	{
		const std::pair<int,int> & val = *first;
		if(except_id !=  val.first)
		packet.playerlist.add(GNET::Player(val.first,val.second));
	}
	return GProviderClient::DispatchProtocol(lid,&packet);
}

bool SendChatMsg(int lid, int userid, int sid,const chat_msg & chat)
{
	ChatSingleCast packet;
	packet.msg.insert(packet.msg.end(),chat.msg,chat.size);
	packet.data.insert(packet.data.end(),chat.data,chat.dsize);
	packet.srcroleid = chat.speaker;
	packet.channel = chat.channel;
	packet.dstroleid = userid;
	packet.dstlocalsid = sid;
	packet.emotion = chat.emote_id;
	return GProviderClient::DispatchProtocol(lid,&packet);
}

bool SetChatEmote(int userid, char emote_id)
{
	SetChatEmotion protocol;
	protocol.roleid = userid;
	protocol.emotion = emote_id;
	return GProviderClient::DispatchProtocol(0,&protocol);
}

bool BroadChatMsg(const chat_msg &chat)
{
	ChatBroadCast packet;
	packet.msg.insert(packet.msg.end(),chat.msg,chat.size);
	packet.data.insert(packet.data.end(),chat.data,chat.dsize);
	packet.srcroleid = chat.speaker;
	packet.channel = chat.channel;
	packet.emotion = chat.emote_id;
	return GProviderClient::DispatchProtocol(0,&packet);
}

bool SystemChatMsg(const void * msg, unsigned int size, char channel, const void * data, unsigned int dsize)
{
	PublicChat packet;
	packet.msg.insert(packet.msg.end(),msg,size);
	if(data) packet.data.insert(packet.data.end(),data,dsize);
	packet.roleid = 0;
	packet.channel = channel;
	packet.localsid = 0;
	packet.emotion = 0;
	return GProviderClient::DispatchProtocol(0,&packet);
}

bool CountryChatMsg(const chat_msg &chat)
{
	PublicChat packet;
	packet.channel = chat.channel;
	packet.emotion = chat.emote_id;
	packet.roleid = chat.speaker;
	packet.localsid = 0;
	packet.msg.insert(packet.msg.end(),chat.msg,chat.size);
	packet.data.insert(packet.data.end(),chat.data,chat.dsize);
	return GProviderClient::DispatchProtocol(0,&packet);
}

bool FactionBroadcastMsg(int fid, int type, const void * msg, unsigned int size)
{
	FactionChat chat;
	chat.channel = CHAT_CHANNEL_SYSTEM; 
	chat.src_roleid = type;
	chat.msg.insert(chat.msg.end(),msg,size);
	chat.dst_localsid = fid;
	return GProviderClient::GetInstance()->DispatchProtocol(_FACTIONSERVER_ID,chat);
}

bool SendPlayerHeartbeat(int lid, int userid, int localsid)
{
	PlayerHeartBeat packet(userid,lid,localsid);
	return GProviderClient::DispatchProtocol(lid,&packet);
}

bool SendLoginRe(int lid, int userid/*actually is roleid*/, int sid,int result, char flag)
{
	if(lid <=0) return false;
	return GProviderClient::DispatchProtocol(0,PlayerLogin_Re(result,userid,lid,sid,flag));
}

bool SendSwitchServerSuccess(int lid, int userid, int localsid,int gs_id)
{
	if(lid <=0) return false;
	SwitchServerSuccess p(userid,lid,localsid,gs_id);
	GProviderClient::DispatchProtocol(lid,p);
	return GProviderClient::DispatchProtocol(0,p);
}

bool SendSwitchServerTimeout(int lid, int userid, int localsid)
{
	if(lid <=0) return false;
	SwitchServerTimeout p(userid,lid,localsid);
	GProviderClient::DispatchProtocol(lid,p);
	return GProviderClient::DispatchProtocol(0,p);
}

bool SendSwitchServerCancel(int lid, int userid, int localsid)
{
	if(lid <=0) return false;
	SwitchServerCancel p(userid,lid,localsid);
	GProviderClient::DispatchProtocol(lid,p);
	return GProviderClient::DispatchProtocol(0,p);
}

bool SendSwitchServerStart(int lid, int userid, int localsid,int src_gsid, int dst_gsid, const void * key_buf, unsigned int key_size)
{
	//发送流程修改为gs->gdeliveryd->glinkd
	if(lid <=0) return false;
	SwitchServerStart  p(userid,lid,localsid,src_gsid,dst_gsid,Octets(key_buf,key_size));
	return GProviderClient::DispatchProtocol(0,p);
}

bool SendKickoutRe(int userid/* actually is roleid */, int sid , int result)
{
	return GProviderClient::DispatchProtocol(0,PlayerKickout_Re(result?-1:ERR_SUCCESS,userid));
}

bool SendOfflineRe(int lid,int userid/*actually is roleid*/,int sid, int result)
{
	if(lid <=0) return false;
	return GProviderClient::DispatchProtocol(0,PlayerOffline_Re(result?result:ERR_SUCCESS,userid));
}

bool SendLogout(int lid, int userid/*actually is roleid*/, int sid,int reason)
{
	if(lid <=0) return false;
	return GProviderClient::DispatchProtocol(0,PlayerLogout(reason,userid,lid,sid));
}

bool SendPlayerInfoUpdate(int roleid, int level) 
{
	return GProviderClient::DispatchProtocol(0, PlayerInfoUpdate(roleid, level));
}

bool SendPlayerTeamOp(char operation, int64_t team_uid,int captain,std::vector<int> members)
{
	return GProviderClient::DispatchProtocol(0, PlayerTeamOp(operation,team_uid,captain,members));
}

bool SendPlayerTeamMemberOp(int64_t team_uid, char operation, int member)
{
	return GProviderClient::DispatchProtocol(0, PlayerTeamMemberOp(team_uid,operation,member));
}

bool SendSNSRoleBrief(int roleid, object_interface obj_if)
{
	SNSRoleBriefUpdate update;
	update.roleid = roleid;
	const basic_prop & basic = obj_if.GetBasicProp();
	const extend_prop & extend = obj_if.GetExtendProp();
	update.brief.level 		= basic.level;
	update.brief.exp 		= basic.exp;
	update.brief.sp 		= basic.skill_point;
	update.brief.level2 	= basic.sec_level;
	update.brief.reputation = obj_if.GetReputation();
	update.brief.pp			= basic.status_point;
	update.brief.vitality	= extend.vitality;
	update.brief.energy		= extend.energy;
	update.brief.strength	= extend.strength;
	update.brief.agility	= extend.agility;
	update.brief.max_hp		= extend.max_hp;
	update.brief.max_mp		= extend.max_mp;
	update.brief.run_speed	= extend.run_speed;
	update.brief.attack		= extend.attack;
	update.brief.damage_low			= extend.damage_low;
	update.brief.damage_high		= extend.damage_high;
	update.brief.attack_speed		= extend.attack_speed;
	update.brief.attack_range		= extend.attack_range;
	update.brief.damage_magic_low	= extend.damage_magic_low;
	update.brief.damage_magic_high	= extend.damage_magic_high;
	update.brief.resistance0		= extend.resistance[0];
	update.brief.resistance1		= extend.resistance[1];
	update.brief.resistance2		= extend.resistance[2];
	update.brief.resistance3		= extend.resistance[3];
	update.brief.resistance4		= extend.resistance[4];
	update.brief.defense			= extend.defense;
	update.brief.armor				= extend.armor;
	update.brief.crit_rate			= obj_if.GetCrit();
	update.brief.crit_damage		= obj_if.GetCritDamage();
	update.brief.attack_degree		= obj_if.GetAttackDegree();
	update.brief.defend_degree		= obj_if.GetDefendDegree();
	update.brief.invisible_degree	= obj_if.GetInvisibleDegree();
	update.brief.anti_invisible_degree	= obj_if.GetAntiInvisibleDegree();
	update.brief.soul_power		= obj_if.GetSoulPower();
	update.brief.spouse			= obj_if.GetSpouseID();
	update.brief.factionid 		= obj_if.GetMafiaID();
	//
	if(!GetSkills(update.skills.skills, update.skills.crc, obj_if))
		return false;
	//
	if(!GetEquipment(update.equipment.equipment.inv, update.equipment.crc, obj_if))
		return false;
	//
	if(!GetPetCorral(update.petcorral.petcorral, update.petcorral.crc, obj_if))
		return false;
	update.brief.skills_crc = update.skills.crc;
	update.brief.equipment_crc = update.equipment.crc;
	update.brief.petcorral_crc = update.petcorral.crc;

	return GProviderClient::DispatchProtocol(0, update);
}

bool SendDisconnect(int lid, int userid/*actually is roleid*/, int sid,int reason)
{
	if(lid <=0) return false;
	return GProviderClient::DispatchProtocol(0,DisconnectPlayer(userid,lid,sid,__global_gid));
}

void DiscardTrade(int trade_id, int userid)
{
	GProviderClient::DispatchProtocol(0,GTradeDiscard(trade_id,0));
}

void SendCosmeticRequest(int userid, int ticket_index, int ticket_id)
{
	GProviderClient::DispatchProtocol(0,FaceModify(userid, ticket_id,ticket_index));
}

void CancelCosmeticRequest(int userid)
{
	GProviderClient::DispatchProtocol(0,FaceModifyCancel(userid));
}

void ReplyTradeRequest(int trade_id, int userid,int localsid,bool isSuccess)
{
	GProviderClient::DispatchProtocol(0,GTradeStart_Re(isSuccess?0:1,trade_id,userid,localsid));
}

void SendDebugCommand(int roleid, int type, const char * buf, unsigned int size)
{
	DebugCommand data;
	data.roleid = (unsigned int)roleid;
	data.tag = type;
	data.data.replace(buf,size);
	GProviderClient::DispatchProtocol(0,data);
}

void SetTaskData(int taskid, const void * buf, unsigned int size)
{
	GNET::SetTaskData data;
	data.taskid = taskid;
	data.taskdata.insert(data.taskdata.end(),buf,size);
	GProviderClient::DispatchProtocol(0,data);
}

void GetTaskData(int taskid, int uid, const void * env_data, unsigned int env_size)
{
	GNET::GetTaskData data;
	data.taskid = taskid;
	data.playerid = uid;
	data.env.insert(data.env.end(),env_data,env_size);
	GProviderClient::DispatchProtocol(0,data);
}

void ReportCheater2Gacd(int roleid, int cheattype, const void *buf, unsigned int size)
{
    GNET::ACReportCheater acrc;
    acrc.roleid = roleid;
    acrc.cheattype = cheattype;
    if( buf ) acrc.cheatinfo.replace(buf, size);
    GProviderClient::DispatchProtocol(0, acrc);
}

void TriggerQuestion2Gacd(int roleid)
{
    GNET::ACTriggerQuestion actq;
    actq.roleid = roleid;
    GProviderClient::DispatchProtocol(0, actq);
}

void SetCouple(int id1, int id2, int op)
{
	Rpc *rpc = Rpc::Call(RPC_PUTSPOUSE, PutSpouseArg(op,id1,id2));
	GProviderClient::DispatchProtocol(0,rpc);
}

void SendRefCashUsed(int roleid, int cash_used, int level)
{
	GNET::SendRefCashUsed srcu;
	srcu.roleid = roleid;
	srcu.cash_used = cash_used;
	srcu.level = level;
	GProviderClient::DispatchProtocol(0, srcu);	
}

void SendTaskReward(int roleid, int bonus_add)
{
	GNET::SendTaskReward str;
	str.roleid = roleid;
	str.bonus_add = bonus_add;
	GProviderClient::DispatchProtocol(0, str);
}

bool SendFactionCongregateRequest(int fid, int sponsor, void* data, unsigned int size)
{
	GNET::FactionCongregateRequest req;
	req.factionid = fid;
	req.sponsor = sponsor;
	req.data.insert(req.data.end(),data,size);
	return GProviderClient::DispatchProtocol(_FACTIONSERVER_ID, req);
}

void SendPlayerJoinOrLeaveForce(int force_id, bool is_join)
{
	GNET::NotifyPlayerJoinOrLeaveForce notice;
	notice.force_id = force_id;
	notice.is_join = is_join;
	GProviderClient::DispatchProtocol(0, notice);
}

void SendIncreaseForceActivity(int force_id, int activity)
{
	GNET::IncreaseForceActivity ifa;
	ifa.force_id = force_id;
	ifa.activity = activity;
	GProviderClient::DispatchProtocol(0, ifa);
}

void SendSynMutaData(int roleid,int level,int reincarnation_times)
{
	GNET::SynMutaData smd;
	smd.synmask = SYNMUTADATAMASK_LEVEL;
	smd.roleid = roleid;
	smd.level = level;
	smd.reincarnation_times = reincarnation_times;
	GProviderClient::DispatchProtocol(0,smd);
}

void CountryBattleApply(CBApplyEntry * list, unsigned int count)
{
	GNET::CountryBattleApply data;
	for(unsigned int i=0; i<count; i++)
	{
		data.list.push_back(CountryBattleApplyEntry(list[i].roleid, list[i].major_strength, list[i].minor_strength));
	}
	GProviderClient::DispatchProtocol(0,data);
}

void CountryBattleJoin(int roleid, int country_id, int worldtag, int major_strength, int minor_strength, char is_king)
{
	GProviderClient::DispatchProtocol(0,CountryBattleJoinNotice(roleid,country_id,worldtag,major_strength,minor_strength,is_king));
}

void CountryBattleLeave(int roleid, int country_id, int major_strength, int minor_strength)
{
	GProviderClient::DispatchProtocol(0,CountryBattleLeaveNotice(roleid,country_id,major_strength,minor_strength));
}

void CountryBattleOnline(int roleid, int country_id, int worldtag, int minor_strength, char is_king)
{
	GProviderClient::DispatchProtocol(0,CountryBattleOnlineNotice(roleid, country_id, worldtag, minor_strength, is_king));
}

void CountryBattleOffline(int roleid, int country_id)
{
	GProviderClient::DispatchProtocol(0,CountryBattleOfflineNotice(roleid,country_id));
}

void CountryBattleEnterMap(int roleid, int worldtag)
{
	GProviderClient::DispatchProtocol(0,CountryBattleEnterMapNotice(roleid,worldtag));
}

void SendCountryBattleServerRegister(int type, int world_index, int world_tag, int battle_type)
{
	GProviderClient::DispatchProtocol(0,CountryBattleServerRegister(type,battle_type,world_index,world_tag));	
}

void ResponseCountryBattleStart(int battle_id, int world_tag, int retcode, int defender, int attacker)
{
	GProviderClient::DispatchProtocol(0,CountryBattleStart_Re(retcode,battle_id,world_tag, defender, attacker));
}
	
void SendCountryBattleEnd(int battle_id, int result, int defender, int attacker, CBPersonalScore* dscore, unsigned int dcount, CBPersonalScore* ascore, unsigned int acount) 
{
	CountryBattleEnd package;
	package.battle_id = battle_id;
	package.battle_result = result;
	package.attacker = attacker;
	package.defender = defender;
	for(unsigned int i=0; i<acount; i++)
	{
		package.attacker_score.push_back(GCountryBattlePersonalScore(ascore[i].roleid, 
					ascore[i].cls, 
					ascore[i].minor_strength,
					ascore[i].combat_time, 
					ascore[i].attend_time, 
					ascore[i].kill_count, 
					ascore[i].death_count, 
					ascore[i].score,
					ascore[i].dmg_output,
					ascore[i].dmg_endure,
					ascore[i].dmg_output_weighted,
					ascore[i].kill_count_weighted));
	}
	for(unsigned int i=0; i<dcount; i++)
	{
		package.defender_score.push_back(GCountryBattlePersonalScore(dscore[i].roleid, 
					dscore[i].cls, 
					dscore[i].minor_strength,
					dscore[i].combat_time, 
					dscore[i].attend_time, 
					dscore[i].kill_count, 
					dscore[i].death_count, 
					dscore[i].score,
					dscore[i].dmg_output,
					dscore[i].dmg_endure,
					dscore[i].dmg_output_weighted,
					dscore[i].kill_count_weighted));
	}
	GProviderClient::DispatchProtocol(0,package);
}

bool SendPlayerRename(int roleid, int item_pos, int item_id, int item_num, const void * new_name, unsigned int new_name_len, object_interface & obj_if)
{
	PlayerRename proto;
	proto.roleid = roleid;
	proto.attach_obj_pos = item_pos;
	proto.attach_obj_id = item_id;
	proto.attach_obj_num = item_num;
	proto.newname = Octets(new_name, new_name_len);
	
	GMailSyncData data;
	if ( !GetSyncData(data, obj_if))
		return false;
	
	Marshal::OctetsStream os;
	os << data;
	proto.syncdata.swap(os);

	if(obj_if.TradeLockPlayer(0,DBMASK_PUT_SYNC_TIMEOUT)==0)
	{
		if (GProviderClient::GetInstance()->DispatchProtocol( 0, proto ))
			return true;
		obj_if.TradeUnLockPlayer();
	}
	return false;
}

bool SendPlayerChangeGender(int roleid, int item_pos, int item_id, int item_num, unsigned char new_gender, const void* custom_data, unsigned int custom_data_len, object_interface& obj_if)
{
    GMailSyncData syncdata;
    if (!GetSyncData(syncdata, obj_if))
        return false;

    if (obj_if.TradeLockPlayer(0, DBMASK_PUT_SYNC_TIMEOUT) == 0)
    {
        Rpc* rpc = Rpc::Call(RPC_DBPLAYERCHANGEGENDER, DBPlayerChangeGenderArg(roleid, item_id, item_num, item_pos, new_gender, Octets(custom_data, custom_data_len), syncdata));

        if (GProviderClient::GetInstance()->DispatchProtocol(0, rpc))
            return true;

        obj_if.TradeUnLockPlayer();
    }

    return false;
}


bool SendUpdateSoloChallengeRank(int roleid, int total_time)
{
    UpdateSoloChallengeRank proto;
    proto.roleid = roleid;
    proto.total_time = total_time;

    if (GProviderClient::GetInstance()->DispatchProtocol(0, proto))
        return true;

    return false;
}

bool SendGetSoloChallengeRank(int roleid, char ranktype, char cls)
{
    GetSoloChallengeRank proto;
    proto.roleid = roleid;
    proto.ranktype = ranktype;
    proto.cls = cls;

    if (GProviderClient::GetInstance()->DispatchProtocol(0, proto))
        return true;

    return false;
}

bool SendUpdateEnemyList(char optype, int srcroleid, int dstroleid)
{
    UpdateEnemyList proto;
    proto.optype = optype;
    proto.srcroleid = srcroleid;
    proto.dstroleid = dstroleid;

    if (GProviderClient::GetInstance()->DispatchProtocol(0, proto))
        return true;

    return false;
}


bool SendPlayerAskForPresent(int roleid, int target_roleid, int goods_id, int goods_index, int goods_slot)
{
	PlayerAskForPresent proto;
	proto.roleid = roleid;
	proto.target_roleid = target_roleid;
	proto.goods_id = goods_id;
	proto.goods_index = goods_index;
	proto.goods_slot = goods_slot;
	
	if (GProviderClient::GetInstance()->DispatchProtocol( 0, proto ))
		return true;

	return false;
}

bool SendPlayerGivePresent(int roleid, int target_roleid, unsigned int cash_cost, char has_gift, int log_price1,
	   	int log_price2, int mail_id, GDB::itemlist & ilist, object_interface & obj_if)
{
	PlayerGivePresent proto;
	proto.roleid = roleid;
	proto.target_roleid = target_roleid;
	proto.cash_cost = cash_cost;
	proto.has_gift = has_gift;
	proto.log_price1 = log_price1;
	proto.log_price2 = log_price2;
	proto.mail_id = mail_id;
	
	GMailSyncData data;
	if ( !GetSyncData(data, obj_if))
		return false;
	
	Marshal::OctetsStream os;
	os << data;
	proto.syncdata.swap(os);

	GDB::itemlist_to_inventory(proto.goods, ilist);
	
	if(obj_if.TradeLockPlayer(0,DBMASK_PUT_SYNC_TIMEOUT)==0)
	{
		if (GProviderClient::GetInstance()->DispatchProtocol( 0, proto ))
			return true;
		obj_if.TradeUnLockPlayer();
	}
	return false;
}

bool SendPlayerQueryTouchPoint(int roleid)
{
    GNET::TouchPointQuery req;
    req.roleid = roleid;
    return GProviderClient::DispatchProtocol(0, req);
}
 
bool SendPlayerCostTouchPoint(int roleid,int64_t orderid,unsigned int cost)
{
    GNET::TouchPointCost req;
    req.roleid = roleid;
    req.orderid = orderid;
	req.cost = cost;
	return GProviderClient::DispatchProtocol(0, req);
}
	
bool SendPlayerGiftCodeRedeem(int roleid,const char (&cardnumber)[20])
{
	GNET::GiftCodeRedeem req;
	req.roleid = roleid;
	Octets temp(cardnumber,sizeof(cardnumber));
	req.cardnumber.swap(temp);

	return GProviderClient::DispatchProtocol(0, req);
}

bool InitMatcher(const char * file, const char * in_code, const char * check_code, const char * table_code)
{
	if (GNET::Matcher::GetInstance()->Load(const_cast<char*>(file), in_code, check_code, table_code))
		return false;
	return true;
}

bool CheckMatcher(char * str, unsigned int size)
{
	return GNET::Matcher::GetInstance()->Match(str,size)!=0;
}

bool SendTryChangeDS(int roleid, int flag, short type, int64_t mnfid)
{
	return GProviderClient::DispatchProtocol(0,TryChangeDS(roleid, flag, type, mnfid));
}

bool SendPlayerChangeDSRe(int retcode, int roleid, int flag)
{
	PlayerChangeDS_Re re;
	re.retcode = retcode;
	re.roleid = roleid;
	re.flag = flag;
	return GProviderClient::DispatchProtocol(0,re);
}

void SendMobileServerRegister(int world_index, int world_tag)
{
	GProviderClient::DispatchProtocol(0,MobileServerRegister(world_index,world_tag));
}

void InitUniqueData(int world_tag,int version)
{
	UniqueDataModifyRequire req;
	req.worldtag = world_tag;
	req.vtype = 0;
	req.version = version;

	GProviderClient::DispatchProtocol(0,req);
}
/////////////////////////////////////////////////////////////////////////////////////////////
// 在此添加新的unique类型接口

void ModifyUniqueData(int key,int type,const void* odata, unsigned int osz,const void* ndata,unsigned int nsz,int world_tag, bool exclusive, bool broadcast, int version,bool timeout)
{
	UniqueDataModifyRequire req;
	req.worldtag = world_tag;
	req.vtype = type;
	req.key = key;
	Octets newval(ndata,nsz);
	req.value.swap( newval );
	Octets oldval(odata,osz);
	req.oldvalue.swap( oldval );
	req.exclusive = exclusive;
	req.broadcast = broadcast;
	req.version = version;
	req.timeout = timeout;

	GProviderClient::DispatchProtocol(0,req);
}
/////////////////////////////////////////////////////////////////////////////////////////////
void SendTrickBattleServerRegister(int world_index, int world_tag)
{
	GProviderClient::DispatchProtocol(0,TankBattleServerRegister(world_index,world_tag));
}

void ResponseTrickBattleStart(int battle_id, int world_tag, int retcode)
{
	GProviderClient::DispatchProtocol(0,TankBattleStart_Re(retcode,battle_id,world_tag));
}

void SendTrickBattleEnd(int battle_id, int world_tag)
{
	GProviderClient::DispatchProtocol(0,TankBattleEnd(battle_id,world_tag));
}

void SendTrickBattlePersonalScore(int battle_id,  int world_tag, TBPersonalScore * list, unsigned int size)
{
	TankBattlePlayerScoreUpdate proto;
	proto.battle_id = battle_id;
	proto.world_tag = world_tag;
	for(unsigned int i=0; i<size; i++)
	{
		proto.player_scores.push_back(TankBattlePlayerScoreInfo(list[i].roleid, list[i].kill_count, list[i].death_count, list[i].score));
	}
	GProviderClient::DispatchProtocol(0,proto);
}

void SendTrickBattleApply(int roleid, int chariot)
{
	GProviderClient::DispatchProtocol(0,TankBattlePlayerApply(roleid,chariot));
}

void SendTrickBattleEnter(int roleid, int battle_id, int world_tag)
{
	GProviderClient::DispatchProtocol(0,TankBattlePlayerEnter(roleid, battle_id, world_tag));
}

void SendTrickBattleLeave(int roleid, int battle_id, int world_tag)
{
	GProviderClient::DispatchProtocol(0,TankBattlePlayerLeave(roleid, battle_id, world_tag));
}


void SendAutoTeamData(const AutoTeamConfig* pConfig)
{
	std::vector<AutoTeamConfigData> config_data;
	for(unsigned int i = 0; i < pConfig->goal_cnt; ++i)
	{
		const AutoTeamConfig::Goal& info = pConfig->goal_list[i];

		AutoTeamConfigData data;
		data.goal_id = info.id;
		data.need_player_cnt = info.need_player_cnt;
		
		for(unsigned char j = 0; j < info.occupation_list_size; ++j)
		{
			std::pair<char, char> occupation_pair;
			occupation_pair.first = info.occupation_list[j].occupation;
			occupation_pair.second = info.occupation_list[j].need_cnt;
			data.occupation_info.push_back(occupation_pair);
		}

		config_data.push_back(data);
	}
	
	GProviderClient::DispatchProtocol(0, AutoTeamConfigRegister(config_data));
}

void SendPlayerSetAutoTeamGoal(int roleid, char goal_type, char op, int goal_id)
{
	GProviderClient::DispatchProtocol(0, AutoTeamSetGoal(roleid, goal_type, op, goal_id));
}

void SendAutoTeamPlayerReady_Re(int roleid, int leader_id, char retcode)
{
	GProviderClient::DispatchProtocol(0, AutoTeamPlayerReady_Re(roleid, leader_id, retcode));
}

void SendMafiaPvPEvent(int type, int mafia_src, int mafia_dest , int roleid, unsigned char rank, int domainid,std::vector<MafiaMemberInfo>* memberlist)
{
	FactionResourceBattleEventNotice event;
	event.event_type = type;
	event.src_faction = mafia_src;
	event.dest_faction = mafia_dest;
	event.domain_id = domainid;
	event.leader_role.roleid = roleid;
	event.leader_role.rank = rank;
	if(memberlist) 
	{		
		for(unsigned int i = 0; i < memberlist->size(); ++i)
		{
			GFactionResourceBattleRole mr;
			mr.roleid = (*memberlist)[i].roleid;
			mr.rank = (*memberlist)[i].rank;
			event.members.push_back(mr);
		}
	}
	GLog::log(GLOG_INFO,"帮派pvp事件发生 type%d mafia_src%d mafia_dest%d roleid%d domainid%d",type, mafia_src, mafia_dest, roleid, domainid);
	GProviderClient::DispatchProtocol(0, event);
}

void SendMafiaPvPRegister(int serverid,int worldtag)
{
	GProviderClient::DispatchProtocol(0, FactionResourceBattleServerRegister(serverid,worldtag));
}

void SendMafiaPvPQuery(int roleid, int mafiaid)
{
	GProviderClient::DispatchProtocol(0, FactionResourceBattlePlayerQuery(roleid,mafiaid));
}

void SendMafiaDomainConfig(MPDomainConfig* dlist, unsigned int dsize, int* clist, unsigned int csize)
{
	FactionResourceBattleRequestConfig_Re re;
	for(unsigned int n = 0; n < dsize; ++n)
		re.config_list.push_back(GFactionResourceBattleConfig(
					dlist[n].domain_count, dlist[n].minebase_count,
					dlist[n].bonus_base, dlist[n].bonus_minecar,
					dlist[n].rob_minebase_limit, dlist[n].rob_minecar_limit));

	for(unsigned int n = 0; n < csize; ++n)
		re.controller_list.push_back(clist[n]);

	GProviderClient::DispatchProtocol(0,re);
}

void * autoWakeUp(void *)
{
	do
	{
		usleep(50000);
		PollIO::WakeUp();
	}while(1);
	return NULL;
}       

void * cmdDispatcher(void * tmp)
{
	int index = (long)tmp;
	std::deque<user_cmd_t> tmpList;
	int tustat_time = 0;
	while(1)
	{
		while(g_xlist[index].empty()) {usleep(2000);}
		g_xlock[index].Lock();
		tmpList.swap(g_xlist[index]);
		g_xlock[index].UNLock();
		while(!tmpList.empty())
		{
			user_cmd_t & cmd = tmpList.front();
			handle_user_cmd(cmd.cs_index, cmd.sid,cmd.uid, cmd.data.begin(), cmd.data.size());
			tmpList.pop_front();
		}

		if(GNET::Timer::GetTime() > tustat_time)
		{
			tustat_time = GNET::Timer::GetTime();
			thread_usage_stat("cmddisp");
		}
	}
	return NULL;
}

bool InitGSP(const char * gmconf_file,int gid, int worldtag ,float x_min, float x_max, float y_min, float y_max, const char * version)
{
	// GNET::PollControl::ReInit();

	Conf * conf = Conf::GetInstance(gmconf_file);
	pthread_t ph;
	pthread_create(&ph,NULL,autoWakeUp,NULL);
	assert(x_min < x_max && y_min < y_max);
#ifdef __USE_SPEC_GAMEDATASEND__
	std::string str = conf->find("ThreadPool","two_usercmd_threads");
	if(strcmp(str.c_str(), "true") == 0)	user_cmd_queue_num = 2;
	for(long i = 0; i < user_cmd_queue_num; i ++)
	{
		pthread_create(&ph,NULL,cmdDispatcher,(void*)i);
	}
#endif
	__global_gid = gid;
	return GProviderClient::Connect(gid,version, worldtag,x_min,x_max,y_max,y_min);
}

void StartPollIO()
{
	Thread::Pool::AddTask(PollIO::Task::GetInstance());
	Thread::Pool::Run();
}

void ThreadUsageStat()
{
	class ProbeTask : public GNET::Thread::Runnable
	{
	public:
		ProbeTask(int priority):Runnable(priority){}
		virtual void Run()
		{
			char buf[16]={0};
			snprintf(buf,16,"IOPool%d",m_priority);
			thread_usage_stat(buf);
			delete this;
		}
	};
	static const int pri[4]={0,1,100,101};
	GNET::Thread::Pool::AddTask(new ProbeTask(pri[rand()%4]));
}

void SendMNFactionServerRegister(int server_id, int world_tag)
{
	GProviderClient::DispatchProtocol(0,MNBattleServerRegister(server_id, world_tag));
}

void ResponseMnfactionBattleStart(int ret_code, int domain_id, int world_tag)
{
	GProviderClient::DispatchProtocol(0,MNDomainBattleStart_Re(ret_code, domain_id, world_tag));
}

void MnfactionEnter(MnfactionEnterEntry *ent)
{
	GNET::MNDomainBattleEnter data;
	data.roleid     = ent->roleid;
	data.unifid     = ent->faction_id;
	data.domain_id   = ent->domain_id;
	GProviderClient::DispatchProtocol(0,data);
}

void MNDomainBattleLeaveNotice(int roleid, int64_t unifid, int domain_id)
{
	GProviderClient::DispatchProtocol(0, GNET::MNDomainBattleLeaveNotice(roleid, unifid, domain_id));
}

int MNDomainBattleEnterSuccessNotice(int roleid, int64_t unifid, int domain_id)
{
	GProviderClient::DispatchProtocol(0, GNET::MNDomainBattleEnterSuccessNotice(roleid, unifid, domain_id));
	return 0;
}

void SendMnfactionBattleEnd(int domain_id, int64_t win_unifid)
{
	GProviderClient::DispatchProtocol(0, GNET::MNDomainBattleEnd(domain_id, win_unifid));
}

static mnfaction_domain_entry mn_domain_list[56] = {{0,0,0,0,0,0}};
static Thread::Mutex  g_domain_lock;

void SetMnDomain(int domain_id, unsigned char domain_type, int64_t owner_unifid, int64_t attacker_unifid, int64_t defender_unifid)
{
	Thread::Mutex::Scoped lock(g_domain_lock);
	mn_domain_list[domain_id]._domain_id    = domain_id;
	mn_domain_list[domain_id]._domain_type	= domain_type;
	mn_domain_list[domain_id]._owner_unifid = owner_unifid;
	mn_domain_list[domain_id]._attacker_unifid = attacker_unifid;
	mn_domain_list[domain_id]._defender_unifid = defender_unifid;
	GLog::log(GLOG_INFO, "MNFACTION_LOG DOMAIN_OWNER_INFO domain_id : %d , _owner_unifid : %lld , _attacker_unifid : %lld , _defender_unifid : %lld ",domain_id, owner_unifid, attacker_unifid, defender_unifid);
	//mn_domain_list[domain_id]._expiretime   = expiretime;
}

int GetMnDomainCount(int64_t unifid, unsigned char domain_type)
{
	int num = 0;
	for(unsigned int i = 0; i<56; i++)
	{
		if(mn_domain_list[i]._owner_unifid == unifid && mn_domain_list[i]._domain_type == domain_type)
			++num;
	}
	return num;
}

int64_t GetMnDomainOwner(int domain_id)
{
	return mn_domain_list[domain_id]._owner_unifid;	
}

int MnFactionSignUp(unsigned char domain_type, int id_mafia, int64_t unifid, object_interface& obj_if, int roleid, unsigned int cost)
{
/*	if(domain_type == 0)//C级城
	{
		if(GetMafiaCityCount(id_mafia) < 5)
			return 1;
	}
	else//A,B级城
	{
		if(!unifid)
			return 2;
		else
		{
			if(!GetMnDomainCount(unifid, domain_type - 1))//打B级城必须有C级城
				return 3;
		}
	}*/
	
	GMailSyncData data;
	if (!GetSyncData(data, obj_if))
		return 4;
	
	if (obj_if.TradeLockPlayer(0, DBMASK_PUT_SYNC_TIMEOUT) == 0)
	{
		GNET::MNFactionBattleApply proto;
		proto.fid         = id_mafia;
		proto.unifid      = unifid;
		proto.roleid      = roleid;
		proto.target      = domain_type;
		proto.cost        = cost;
		proto.syncdata    = data;

		if(!GProviderClient::DispatchProtocol(0, proto))
		{
			obj_if.TradeUnLockPlayer();
			return 5;
		}
	}
	return 0;
}

void MnFactionRank(int roleid)
{
	GProviderClient::DispatchProtocol(0, MNGetTopList(roleid));
}

void MnFactionGetDomainData(int roleid)
{
	GProviderClient::DispatchProtocol(0, MNGetDomainData(roleid));
}

}

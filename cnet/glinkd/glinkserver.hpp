#ifndef __GNET_GLINKSERVER_HPP
#define __GNET_GLINKSERVER_HPP

#include "protocol.h"
#include "timermanager.h"
#include "macros.h"
#include <set>
#include <unordered_map>
#include "groleforbid"
#include "cache.h"
#include "matrixchecker.h"
#include "localmacro.h"
#include "requestlimiter.h"

#define __USERCOUNT_MIN		1000	
#define	__USERCOUNT_THRESHOLD	50
#define TIMER_INTERVAL		5
#define ACREPORT_TIMER          180

#define __HALFLOGIN_DEFAULT	30
#define __HALFLOGIN_THRESHOLD	15
namespace GNET
{
struct ProtoStat
{
	int keepalive;
	int gamedatasend;
	int publicchat;
	int remain_time;

	ProtoStat() : keepalive(0),gamedatasend(0),publicchat(0),remain_time(ACREPORT_TIMER) 
	{
       	}
	void Reset() {
		keepalive=0;
		gamedatasend=0;
		publicchat=0;
		remain_time=ACREPORT_TIMER;
	}
};	

struct RoleData
{
	unsigned int sid;
	int	userid;
	int	roleid;
	int	gs_id;	
	int	status;
	time_t	login_time;
	void*	switch_flag;

	RoleData() : sid(_SID_INVALID),userid(0),roleid(0),gs_id(_GAMESERVER_ID_INVALID),status(_STATUS_ONLINE),login_time(0) { }

	RoleData(unsigned int _sid,int _userid,int _roleid,int _gs_id,int _status=_STATUS_ONLINE): 
		sid(_sid),userid(_userid),roleid(_roleid),gs_id(_gs_id),status(_status),login_time(0) 
	{ 
	}
};

struct SessionInfo
{
	Octets challenge;
	Octets identity;
	Octets response;
	int    userid;
	int    roleid;
	int    gsid;
	time_t login_time;
	bool   busy;
	SockAddr local;
	SockAddr peer;
	MatrixChecker * checker;
	ProtoStat protostat;
	RequestLimiter policy;
	char	passwd_flag;	//bit0:密码太久未更换 bit1:密保卡太久未更换
	char	accountinfo_flag;//bit0:需要补填身份证和姓名 bit1:需要补填账号密码邮箱
	Octets used_elec_number; //本次登陆使用的二代神盾电子码
	Octets cli_fingerprint; //记录了包括客户端版本在内的部分信息
	
	//cross server related
	Octets iseckey;
	Octets oseckey;
	
	SessionInfo() : userid(0), roleid(0), gsid(0), login_time(0), busy(0), local(0), peer(0), checker(NULL), passwd_flag(0), accountinfo_flag(0)
	{
	}

	void SetLocal(SockAddr addr) { local = addr;}
	void SetPeer(SockAddr addr) { peer = addr;}
	SockAddr GetLocal() const { return local;}
	SockAddr GetPeer() const { return peer;}
	const struct in_addr& GetPeerAddr() const {  return ((const struct sockaddr_in*)peer)->sin_addr; }
};

class GLinkServer : public TimerManager, public Timer::Observer
{
	static GLinkServer instance;
	size_t	accumulate_limit;
	size_t	user_count_limit;
	size_t	halflogin_limit;
	const Session::State *GetInitState() const;
	bool OnCheckAccumulate(size_t size) const { return accumulate_limit == 0 || size < accumulate_limit; }
	void OnAddSession(Session::ID sid);
	void OnDelSession(Session::ID sid) { }
	void OnDelSession(Session::ID sid, int status);
	void OnSetTransport(Session::ID sid, const SockAddr& local, const SockAddr& peer);

	PassiveIO* passiveio;
public:

	/*switch user map*/
	typedef std::unordered_map<Session::ID, RoleData>		SwitchUserMap;
	typedef std::deque<Protocol*> ProtocolQueue;
	typedef std::unordered_map<Session::ID, ProtocolQueue>	AccumulateProtocolMap;
	typedef std::unordered_map<Session::ID, SessionInfo>	SessionInfoMap;
	typedef SessionInfoMap::iterator		Iterator;
	SwitchUserMap	switchusermap;
	AccumulateProtocolMap accumproto_map;

	string section_num;

	unsigned int server_attr;
	int freecreatime;
	unsigned char ExpRate;
	unsigned int version;/*define gamecontent version number*/
	char challenge_algo;/* challenge algorithm*/
	Octets edition;
	int au_version;

	static GLinkServer *GetInstance() { return &instance; }
	void SetSectionNum(const char* sn) { section_num=sn; }
	std::string Identification() const { return string("GLinkServer")+section_num; }
	void SetAccumulate(size_t size) { accumulate_limit = size; }
	void ActiveCloseSession( Session::ID sid );

	GLinkServer() : accumulate_limit(0), user_count_limit(__USERCOUNT_MIN), halflogin_limit(__HALFLOGIN_DEFAULT),
			version(100), challenge_algo(ALGO_MD5), au_version(0)
	{ 
		passiveio=NULL; 
		forbidcomplaincache.init(102400,-1,3600); 
		server_attr=0; 
		freecreatime=0; 
		ExpRate = 0;
	}

	void StartListen() { if (NULL==passiveio) passiveio=Protocol::Server(this); }
	void StopListen() { if (NULL!=passiveio) passiveio->Close(); passiveio=NULL; }
	bool IsListening() { return passiveio!=NULL; }
	void TriggerListen();
	void SetEdition(Octets& _edition) { edition = _edition;}
	void SetUserCountLimit(size_t size) { user_count_limit = size > __USERCOUNT_MIN ? size:__USERCOUNT_MIN; }
	void SetHalfLoginLimit(size_t size) { halflogin_limit = size > __HALFLOGIN_DEFAULT ? size: __HALFLOGIN_DEFAULT; }
	bool ExceedHalfloginLimit(size_t size) { return size >= halflogin_limit+__HALFLOGIN_THRESHOLD; }
	bool UnderHalfloginLimit(size_t size) { return size <= halflogin_limit-__HALFLOGIN_THRESHOLD; }
	bool ExceedUserLimit(size_t size) { return size >= user_count_limit+__USERCOUNT_THRESHOLD; }
	bool UnderUserLimit(size_t size) { return size <= user_count_limit-__USERCOUNT_THRESHOLD; }

	void SessionError(Session::ID sid, int errcode, const char *info);

	//server attribute
	void SetServerAttr( unsigned int attr ) {
		server_attr=attr;
	}
	unsigned int GetServerAttr() {
		return server_attr;
	}
	void SetFreeCreatime( int fct ) {
		freecreatime = fct;
	}
	int GetFreeCreatime() {
		return freecreatime;
	}
	void SetExpRate(unsigned char exp) { ExpRate = exp; }
	unsigned char GetExpRate() { return ExpRate; }

	void SetChallengeAlgo(char algo) { challenge_algo = algo;}

	void SetAUVersion(int _au_version) { au_version = _au_version; }
	int GetAUVersion() { return au_version; }

	//login maps
	typedef std::set<Session::ID>	HalfLoginSet;
	typedef std::unordered_map<int, RoleData>	RoleMap;

	HalfLoginSet	halfloginset;
	RoleMap	roleinfomap;
	SessionInfoMap	sessions;

	//alive keeper timer map
	typedef std::unordered_map<Session::ID, int> AlivetimeMap;
	typedef std::unordered_map<Session::ID, int> ReadyCloseMap;
	AlivetimeMap alivetimemap;
	ReadyCloseMap readyclosemap;

	//forbid chat timer map	
	typedef std::unordered_map<int/*userid*/,GRoleForbid> MuteMap;
	MuteMap shutupusermap;
	MuteMap shutuprolemap;

	//forbid Complain2GM cache
	Cache<int/*userid*/,int/*userid*/> forbidcomplaincache;

	//user privilege map
	typedef std::unordered_map<int /*id*/,ByteVector>  PrivilegeMap;
	PrivilegeMap privilegemap;
	bool IsForbidChat(int role, int userid, GRoleForbid & forbid) const
	{       
		MuteMap::const_iterator it;
		it = shutupusermap.find(userid);
		if (it != shutupusermap.end())
		{
			forbid = it->second;
			return true;
		}       
		it = shutuprolemap.find(role);
		if (it != shutuprolemap.end())
		{
			forbid = it->second;
			return true;
		}
		return false;
	}       

	void SetUserPrivilege(int userid, ByteVector& priv)
	{
		privilegemap[userid].swap(priv);
	}
	void GetUserPrivilege(int userid, ByteVector& priv) const
	{
		PrivilegeMap::const_iterator it = privilegemap.find(userid);
		if ( it != privilegemap.end())
		{
			priv = it->second;
		}
	}
	bool PrivilegeCheck(int userid,unsigned char privilege)
	{
		PrivilegeMap::const_iterator it = privilegemap.find(userid);
		if(it==privilegemap.end())
			return false;
		const ByteVector& v = it->second;
		for(ByteVector::const_iterator i=v.begin();i!=v.end();++i)
			if(*i==privilege)
				return true;
		return false;
	}

	static bool IsRoleOnGame( Session::ID sid );
	static bool IsRoleOnGame( int roleid );
	static bool ValidSid(Session::ID sid)
	{
		return instance.sessions.find(sid) != instance.sessions.end();
	}
	static bool ValidUser(Session::ID sid, int userid)
	{
		Iterator it = instance.sessions.find(sid);
		return it != instance.sessions.end() && it->second.userid == userid;
	}
	static bool ValidRole(Session::ID sid,int roleid)
	{
		RoleMap::const_iterator it = instance.roleinfomap.find(roleid);
		return it != instance.roleinfomap.end() && it->second.roleid == roleid && it->second.sid==sid;
	}
	bool PrivilegeCheck(Session::ID sid,int roleid,unsigned char privilege)
	{
		Iterator it = instance.sessions.find(sid);
		if(it==instance.sessions.end())
			return false;
		if(it->second.roleid!=roleid)
			return false;
		return PrivilegeCheck(it->second.userid, privilege);
	}

	//Update
	void Update()
	{
		static int checktimer=TIMER_INTERVAL;
		if (checktimer--) return;
		CheckSessionTimePolicy();
		checktimer=TIMER_INTERVAL;
	}
	void AddChecker(Session::ID sid, MatrixChecker *checker);
	/*switch user map*/
	bool IsInSwitch(const RoleData& ui)
	{
		return IsInSwitch(ui.sid, ui.roleid);
	}
	bool IsInSwitch(Session::ID sid, int roleid)
	{
		SwitchUserMap::iterator it=switchusermap.find(sid);
		if (it == switchusermap.end()) return false;
		if ((*it).second.roleid == roleid) return true;
		else return false;
	}
	bool GetSwitchUser(Session::ID sid,RoleData& ui)
	{
		SwitchUserMap::iterator it=switchusermap.find(sid);
		if (it == switchusermap.end()) 
			return false;
		ui = (*it).second;
		SessionInfo * sinfo = GetSessionInfo(ui.sid);
		if (sinfo) 
			sinfo->gsid = ui.gs_id;
		return true;
	}
	void PushSwitchUser(const RoleData& ui)
	{	
		if (IsInSwitch(ui)) return;
		switchusermap[ui.sid]=ui;
		accumproto_map[ui.sid]=	ProtocolQueue();
	}	
	void PopSwitchUser(const RoleData& ui)
	{
		if (!IsInSwitch(ui)) return;
		switchusermap.erase(ui.sid);
		ProtocolQueue& proto_queue = accumproto_map[ui.sid];
		for (size_t i=0; i<proto_queue.size();i++)
			proto_queue[i]->Destroy();
		proto_queue.clear();
		accumproto_map.erase(ui.sid);
	}
	void AccumProto4Switch(Session::ID sid,const Protocol& p)
	{
		accumproto_map[sid].push_back(p.Clone());
	}
	void SendAccumProto4Switch(const RoleData& ui,int gs_id);
	SessionInfo * GetSessionInfo(Session::ID sid)
	{
		Iterator it = sessions.find(sid);
		if (it != sessions.end())
			return &(it->second);
		return NULL;
	}
	RoleData * GetRoleInfo(int roleid)
	{
		RoleMap::iterator it = roleinfomap.find(roleid);
		if (it != roleinfomap.end())
			return &(it->second);
		return NULL;
	}

	void SetReadyCloseTime(Session::ID sid, int time)
	{
		readyclosemap[sid] = time;
	}
	void SetAliveTime(Session::ID sid, int time)
	{
		alivetimemap[sid] = time;
	}
	void RoleLogin(Session::ID localsid, int roleid, int gsid, ByteVector& auth);
	void RoleLogout(int roleid);

	bool SetSaveISecurity(Session::ID id, Security::Type type, const Octets& key)
	{	
		if(Manager::SetISecurity(id, type, key)) {
			Iterator it = sessions.find(id);
			
			if(it != sessions.end()) {
				it->second.iseckey = key;
			}
			
			return it != sessions.end();
		} else {
			return false;
		}
	}
	bool SetSaveOSecurity(Session::ID id, Security::Type type, const Octets& key)
	{	
		if(Manager::SetOSecurity(id, type, key)) {
			Iterator it = sessions.find(id);
			
			if(it != sessions.end()) {
				it->second.oseckey = key;
			}
				
			return it != sessions.end();
		} else {
			return false;
		}
	}
	
	bool GetSecurityKeys(Session::ID id, Octets& ikey, Octets& okey)
	{
		ikey = okey = Octets();
		
		Iterator it = sessions.find(id);
		if (it != sessions.end()) {
			ikey = it->second.iseckey;
			okey = it->second.oseckey;
		}
		
		return it != sessions.end();
	}
};

/* Check keepalive map and shutup user map periodically, and remove timeout items
 */  
class CheckTimer : public Thread::Runnable
{
	int update_time;
public:
	CheckTimer(int _time,int _proir=1) : Runnable(_proir),update_time(_time) { }
	void Run();
private:
	void CheckConnection();
	void CheckForbid();
	void CheckProtoStat();	
};

};
#endif

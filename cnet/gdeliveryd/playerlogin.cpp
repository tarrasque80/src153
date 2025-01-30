#include "playerlogin.hpp"

#include "gdeliveryserver.hpp"
#include "gproviderserver.hpp"
#include "playerlogin_re.hpp"
#include "announceforbidinfo.hpp"
#include "mapforbid.h"
#include "mapuser.h"
#include "dbtranspointdeal.hrp"
#include "gamedbclient.hpp"
#include "dbclearconsumable.hrp"
#include "mappasswd.h"
#include "centraldeliveryclient.hpp"
#include "senddataandidentity.hpp"
#include "referencemanager.h"
#include "waitqueue.h"

namespace GNET
{
	void PlayerLogin::SendFailResult(GDeliveryServer* dsm,Manager::Session::ID sid,int retcode) {
		dsm->Send(sid,PlayerLogin_Re(retcode,roleid,_PROVIDER_ID_INVALID,localsid, flag));
	}
	void PlayerLogin::SendForbidInfo(GDeliveryServer* dsm,Manager::Session::ID sid,const GRoleForbid& forbid) {
		dsm->Send(sid,AnnounceForbidInfo(roleid,localsid,forbid));
	}
	bool PlayerLogin::PermitLogin(GDeliveryServer* dsm,Manager::Session::ID sid)
	{
		GRoleForbid	forbid;
		if( ForbidRoleLogin::GetInstance().GetForbidRoleLogin( roleid, forbid ) )
		{
			SendForbidInfo(dsm,sid,forbid);
			SendFailResult(dsm,sid,ERR_ROLEFORBID);
			return false;
		}
		return true;
	}
	void PlayerLogin::DoLogin(Manager::Session::ID sid, UserInfo* pinfo, bool is_central)
	{
		bool blSuccess=false;
		int retcode = ERR_LOGINFAIL;
		int userid = pinfo->userid;
		usbbind = Passwd::GetInstance().IsUsbUser(userid);
		
		bool can_role_login = pinfo->CheckRoleLogin(roleid);
		if(is_central && flag == DS_TO_CENTRALDS && pinfo->IsRoleCrossLocked(roleid)) {
			//跨服服务器上，如果flag是从原服到跨服，此时角色应该处于锁定状态，不在做是否可以登录的检查，强制设为true
			can_role_login = true;
		}

		if (can_role_login && !is_central && !flag)
		{
			switch(WaitQueueManager::GetInstance()->OnPlayerLogin(userid,roleid,provider_link_id))
			{
				case WQ_NOWAIT:
					break;

				case WQ_BEGWAIT:
				case WQ_INQUEUE:
					{
						can_role_login = false;
						blSuccess = true;
						LOG_TRACE("PlayerLogin userid %d roleid %d usbbind %d begin wait", userid, roleid, usbbind);
					}
					break;

				case WQ_MAXUSER:
					{
						can_role_login = false;
						retcode = 6;
						Log::log(LOG_ERR,"PlayerLogin send wrong! userid %d roleid %d usbbind %d maxnum limit!", userid, roleid, usbbind);
					}
					break;

				case WQ_FAIL:
				default:
					{
						can_role_login = false;
						Log::log(LOG_ERR,"PlayerLogin send wrong! userid %d roleid %d usbbind %d waitqueue fail!", userid, roleid, usbbind);
					}
					break;
			}
		}

		if (can_role_login)
		{ 	
			auth = pinfo->privileges;
			
			int gs_id = 0;
			if (!pinfo->is_phone)	//如果不是使用手机登录，走正常登陆流程
			{
				gs_id = GProviderServer::FindGameServer( *(GProviderServer::point_t*)(&pinfo->role_pos[roleid % MAX_ROLE_COUNT]),
						pinfo->worldtag[roleid % MAX_ROLE_COUNT] );
				if (GProviderServer::GetInstance()->IsPhoneGS(gs_id))	//电脑登陆不允许进入手机gs
					gs_id = _GAMESERVER_ID_INVALID;
				LOG_TRACE("PlayerLogin userid %d roleid %d usbbind %d gs_id %d", userid, roleid, usbbind, gs_id);
			}
			else	//使用手机登录服务器，分配到手机gs
			{
				if (!GProviderServer::GetInstance()->GetLessPhoneGS(gs_id))
					gs_id = _GAMESERVER_ID_INVALID;
				LOG_TRACE("PlayerLogin on phone. userid %d roleid %d usbbind %d gs_id=%d", userid, roleid, usbbind, gs_id);
			}

			if (gs_id!=_GAMESERVER_ID_INVALID)
			{
				if(GProviderServer::GetInstance()->DispatchProtocol(gs_id,this))
				{
					DBClearConsumable::Check360User(pinfo);
					pinfo->gameid = gs_id;
					pinfo->status = _STATUS_SELECTROLE;
					pinfo->linkid = provider_link_id;
					ForbiddenUsers::GetInstance().Push(userid,roleid,pinfo->status);
					UserContainer::GetInstance().RoleLogin(pinfo, roleid);
					GProviderServer::GetInstance()->AddPhoneGSPlayerNum(gs_id);	//会在函数里边检测是否手机gs
					blSuccess=true;
				}
				else
					Log::log(LOG_ERR,"PlayerLogin send wrong! userid %d roleid %d usbbind %d gs_id=%d", userid, roleid, usbbind, gs_id);
			}
			else
				Log::log(LOG_ERR,"PlayerLogin wrong! userid %d roleid %d usbbind %d gs_id=%d world_id=%d", userid, roleid, usbbind, gs_id, pinfo->worldtag[roleid % MAX_ROLE_COUNT]);
		}
		if (!blSuccess)
			SendFailResult( GDeliveryServer::GetInstance(),sid,retcode);
	}

	void PlayerLogin::TryRemoteLogin(Manager::Session::ID sid, UserInfo* pinfo)
	{
		//与PlayerChangeDS_Re保持一致
		int userid = pinfo->userid;
		CrossInfoData* pCrsRole = pinfo->GetCrossInfo(roleid);
		if(pCrsRole == NULL) {
			SendFailResult(GDeliveryServer::GetInstance(), sid, ERR_LOGINFAIL);
			return;
		}

		CentralDeliveryClient* cdc = CentralDeliveryClient::GetInstance();
		if(!cdc->IsConnect()) {
			SendFailResult(GDeliveryServer::GetInstance(), sid, ERR_CDS_COMMUNICATION);
			return;
		}
		
		int ret = cdc->CheckLimit();
		if(ret != ERR_SUCCESS) {
			SendFailResult(GDeliveryServer::GetInstance(), sid, ret);
			return;
		}

		int version = pCrsRole->data_timestamp;
		RemoteLoggingUsers::GetInstance().Push(userid, roleid, pinfo->status);
		
		UserContainer::GetInstance().RoleLogin(pinfo, roleid);//会将pinfo置为_STATUS_READYGAME状态
		pinfo->status = _STATUS_REMOTE_HALFLOGIN;//放在RoleLogin之后
		
		//在原服->跨服时?ReferenceManager依然要OnLogi，是认为玩家在跨服依然算作在线，还应该给推荐人相应奖励
		{
			ReferenceManager::GetInstance()->OnLogin(pinfo);
		}

		Octets random_key;
		Security *rand = Security::Create(RANDOM);
		rand->Update(random_key.resize(32));
		rand->Destroy();
		pinfo->rand_key = random_key;
		
		int src_zoneid = GDeliveryServer::GetInstance()->GetZoneid();
		
		bool usbbind = Passwd::GetInstance().IsUsbUser(userid);
		int rewardmask = Passwd::GetInstance().GetUserReward(userid);

		int type = (pinfo->rewardtype2 << 16) | pinfo->rewardtype; 
		SendDataAndIdentity pro(roleid, pCrsRole->remote_roleid, userid, src_zoneid, pinfo->ip, pinfo->iseckey, pinfo->oseckey, pinfo->account, pinfo->rand_key, DIRECT_TO_CENTRALDS, 
			CrossPlayerData(), version, pinfo->logintime, (char)(pinfo->gmstatus & GMSTATE_ACTIVE != 0), type, pinfo->rewarddata, pinfo->privileges, usbbind, rewardmask);
		
		ForbidUserTalk::GetInstance().GetForbidUserTalk(userid, pro.forbid_talk);

		LOG_TRACE("CrossRelated SendDataAndIdentity(no data) to Central DS, roleid %d remote_roleid %d userid %d ip %d isec.size %d osec.size %d account.size %d random_key.size %d version %d logintime %d forbid_time %d",
			roleid, pCrsRole->remote_roleid, userid, pinfo->ip, pinfo->iseckey.size(), pinfo->oseckey.size(), pinfo->account.size(), pinfo->rand_key.size(), version, pinfo->logintime, pro.forbid_talk.time);
		
		cdc->SendProtocol(pro);
	}

}

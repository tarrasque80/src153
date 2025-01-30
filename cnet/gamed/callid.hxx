
#ifndef __GNET_GAMED_CALLID
#define __GNET_GAMED_CALLID

namespace GNET
{

enum CallID
{
	RPC_PUTSPOUSE	=	4047,
	RPC_DBPLAYERCHANGEGENDER	=	3071,
	RPC_DBSELLPOINT	=	611,
	RPC_DBBUYPOINT	=	615,
	RPC_BATTLEEND	=	859,
	RPC_SYSAUCTIONPREPAREITEM	=	4363,
	RPC_GETFACTIONFORTRESS	=	4404,
	RPC_PUTFACTIONFORTRESS	=	4405,
};

enum ProtocolType
{
	PROTOCOL_PLAYERKICKOUT_RE	=	64,
	PROTOCOL_PLAYERLOGIN_RE	=	66,
	PROTOCOL_PLAYEROFFLINE_RE	=	68,
	PROTOCOL_PLAYERLOGOUT	=	69,
	PROTOCOL_PLAYERINFOUPDATE	=	4460,
	PROTOCOL_PLAYERTEAMOP	=	4461,
	PROTOCOL_PLAYERTEAMMEMBEROP	=	4462,
	PROTOCOL_QUERYPLAYERSTATUS	=	110,
	PROTOCOL_GETTASKDATA	=	111,
	PROTOCOL_SETTASKDATA	=	113,
	PROTOCOL_S2CGAMEDATASEND	=	74,
	PROTOCOL_S2CMULTICAST	=	77,
	PROTOCOL_S2CBROADCAST	=	78,
	PROTOCOL_CHATMULTICAST	=	81,
	PROTOCOL_CHATBROADCAST	=	120,
	PROTOCOL_CHATSINGLECAST	=	94,
	PROTOCOL_KEEPALIVE	=	90,
	PROTOCOL_PLAYERHEARTBEAT	=	93,
	PROTOCOL_DISCONNECTPLAYER	=	106,
	PROTOCOL_GTRADESTART_RE	=	4018,
	PROTOCOL_GTRADEDISCARD	=	4020,
	PROTOCOL_SETCHATEMOTION	=	134,
	PROTOCOL_FACEMODIFY	=	125,
	PROTOCOL_FACEMODIFYCANCEL	=	126,
	PROTOCOL_SYNMUTADATA	=	146,
	PROTOCOL_SWITCHSERVERSUCCESS	=	4103,
	PROTOCOL_SWITCHSERVERTIMEOUT	=	4104,
	PROTOCOL_FACTIONOPREQUEST	=	4804,
	PROTOCOL_FACTIONBEGINSYNC_RE	=	4809,
	PROTOCOL_PLAYERFACTIONINFO	=	4801,
	PROTOCOL_GETPLAYERFACTIONRELATION	=	4821,
	PROTOCOL_BATTLECHALLENGE	=	852,
	PROTOCOL_BATTLECHALLENGEMAP	=	854,
	PROTOCOL_BATTLEENTER	=	860,
	PROTOCOL_BATTLESERVERREGISTER	=	856,
	PROTOCOL_BATTLESTART_RE	=	858,
	PROTOCOL_DEBUGCOMMAND	=	873,
	PROTOCOL_GMRESTARTSERVER_RE	=	359,
	PROTOCOL_GMCONTROLGAME_RE	=	381,
	PROTOCOL_GETMAILLIST	=	4202,
	PROTOCOL_GETMAIL	=	4204,
	PROTOCOL_GETMAILATTACHOBJ	=	4206,
	PROTOCOL_DELETEMAIL	=	4208,
	PROTOCOL_PRESERVEMAIL	=	4210,
	PROTOCOL_PLAYERSENDMAIL	=	4212,
	PROTOCOL_PLAYERSENDMASSMAIL	=	4217,
	PROTOCOL_AUCTIONOPEN	=	800,
	PROTOCOL_AUCTIONBID	=	802,
	PROTOCOL_SENDAUCTIONBID	=	822,
	PROTOCOL_SENDBATTLECHALLENGE	=	869,
	PROTOCOL_AUCTIONLIST	=	804,
	PROTOCOL_AUCTIONCLOSE	=	806,
	PROTOCOL_AUCTIONGET	=	808,
	PROTOCOL_AUCTIONGETITEM	=	820,
	PROTOCOL_AUCTIONATTENDLIST	=	816,
	PROTOCOL_AUCTIONEXITBID	=	818,
	PROTOCOL_AUCTIONLISTUPDATE	=	823,
	PROTOCOL_QUERYREWARDTYPE	=	510,
	PROTOCOL_QUERYREWARDTYPE_RE	=	511,
	PROTOCOL_ACTRIGGERQUESTION	=	5030,
	PROTOCOL_QUERYGAMESERVERATTR	=	512,
	PROTOCOL_SELLPOINT	=	601,
	PROTOCOL_BUYPOINT	=	607,
	PROTOCOL_GETSELLLIST	=	603,
	PROTOCOL_SELLCANCEL	=	605,
	PROTOCOL_STOCKCOMMISSION	=	401,
	PROTOCOL_STOCKACCOUNT	=	407,
	PROTOCOL_STOCKTRANSACTION	=	402,
	PROTOCOL_STOCKBILL	=	405,
	PROTOCOL_STOCKCANCEL	=	411,
	PROTOCOL_SENDREFCASHUSED	=	4902,
	PROTOCOL_SENDTASKREWARD	=	4962,
	PROTOCOL_REFGETREFERENCECODE_RE	=	4909,
	PROTOCOL_BILLINGBALANCESA	=	8048,
	PROTOCOL_BILLINGCONFIRM	=	8052,
	PROTOCOL_BILLINGCANCEL	=	8054,
	PROTOCOL_WEBTRADEPREPOST	=	4302,
	PROTOCOL_WEBTRADEPRECANCELPOST	=	4305,
	PROTOCOL_WEBTRADELIST	=	4308,
	PROTOCOL_WEBTRADEGETITEM	=	4310,
	PROTOCOL_WEBTRADEATTENDLIST	=	4312,
	PROTOCOL_WEBTRADEGETDETAIL	=	4314,
	PROTOCOL_WEBTRADEUPDATE	=	4324,
	PROTOCOL_SYSAUCTIONLIST	=	4351,
	PROTOCOL_SYSAUCTIONGETITEM	=	4353,
	PROTOCOL_SYSAUCTIONACCOUNT	=	4355,
	PROTOCOL_SYSAUCTIONBID	=	4357,
	PROTOCOL_SYSAUCTIONCASHTRANSFER	=	4359,
	PROTOCOL_CREATEFACTIONFORTRESS	=	4406,
	PROTOCOL_FACTIONSERVERREGISTER	=	4409,
	PROTOCOL_NOTIFYFACTIONFORTRESSSTATE	=	4410,
	PROTOCOL_FACTIONFORTRESSENTER	=	4412,
	PROTOCOL_FACTIONFORTRESSLIST	=	4414,
	PROTOCOL_FACTIONFORTRESSCHALLENGE	=	4416,
	PROTOCOL_FACTIONFORTRESSBATTLELIST	=	4419,
	PROTOCOL_FACTIONFORTRESSGET	=	4421,
	PROTOCOL_SNSROLEBRIEFUPDATE	=	4471,
	PROTOCOL_NOTIFYPLAYERJOINORLEAVEFORCE	=	4704,
	PROTOCOL_INCREASEFORCEACTIVITY	=	4705,
	PROTOCOL_COUNTRYBATTLEAPPLY	=	4751,
	PROTOCOL_COUNTRYBATTLEJOINNOTICE	=	4753,
	PROTOCOL_COUNTRYBATTLELEAVENOTICE	=	4754,
	PROTOCOL_COUNTRYBATTLEONLINENOTICE	=	4755,
	PROTOCOL_COUNTRYBATTLEOFFLINENOTICE	=	4756,
	PROTOCOL_COUNTRYBATTLEENTERMAPNOTICE	=	4757,
	PROTOCOL_COUNTRYBATTLESERVERREGISTER	=	4758,
	PROTOCOL_COUNTRYBATTLESTART_RE	=	4764,
	PROTOCOL_COUNTRYBATTLEEND	=	4766,
	PROTOCOL_PLAYERRENAME	=	3059,
	PROTOCOL_UPDATESOLOCHALLENGERANK	=	3074,
	PROTOCOL_GETSOLOCHALLENGERANK	=	3075,
	PROTOCOL_UPDATEENEMYLIST	=	238,
	PROTOCOL_TRYCHANGEDS	=	1101,
	PROTOCOL_PLAYERCHANGEDS_RE	=	1103,
	PROTOCOL_KEGETSTATUS	=	4852,
	PROTOCOL_KECANDIDATEAPPLY	=	4854,
	PROTOCOL_KEVOTING	=	4858,
	PROTOCOL_PSHOPCREATE	=	900,
	PROTOCOL_PSHOPBUY	=	902,
	PROTOCOL_PSHOPSELL	=	904,
	PROTOCOL_PSHOPCANCELGOODS	=	906,
	PROTOCOL_PSHOPPLAYERBUY	=	908,
	PROTOCOL_PSHOPPLAYERSELL	=	910,
	PROTOCOL_PSHOPSETTYPE	=	912,
	PROTOCOL_PSHOPACTIVE	=	914,
	PROTOCOL_PSHOPMANAGEFUND	=	916,
	PROTOCOL_PSHOPDRAWITEM	=	918,
	PROTOCOL_PSHOPCLEARGOODS	=	920,
	PROTOCOL_PSHOPSELFGET	=	922,
	PROTOCOL_PLAYERGIVEPRESENT	=	3065,
	PROTOCOL_PLAYERASKFORPRESENT	=	3068,
	PROTOCOL_TOUCHPOINTQUERY	=	4050,
	PROTOCOL_TOUCHPOINTCOST	=	4052,
	PROTOCOL_GIFTCODEREDEEM	=	4056,
	PROTOCOL_MOBILESERVERREGISTER	=	159,
	PROTOCOL_UNIQUEDATAMODIFYREQUIRE	=	3115,
	PROTOCOL_TANKBATTLESERVERREGISTER	=	4871,
	PROTOCOL_TANKBATTLEPLAYERAPPLY	=	4872,
	PROTOCOL_TANKBATTLEPLAYERENTER	=	4875,
	PROTOCOL_TANKBATTLEPLAYERLEAVE	=	4876,
	PROTOCOL_TANKBATTLESTART_RE	=	4878,
	PROTOCOL_TANKBATTLEEND	=	4879,
	PROTOCOL_TANKBATTLEPLAYERSCOREUPDATE	=	4880,
	PROTOCOL_AUTOTEAMCONFIGREGISTER	=	961,
	PROTOCOL_AUTOTEAMSETGOAL	=	962,
	PROTOCOL_AUTOTEAMPLAYERREADY_RE	=	965,
	PROTOCOL_FACTIONRESOURCEBATTLESERVERREGISTER	=	4427,
	PROTOCOL_FACTIONRESOURCEBATTLEEVENTNOTICE	=	4429,
	PROTOCOL_FACTIONRESOURCEBATTLEPLAYERQUERY	=	4430,
	PROTOCOL_FACTIONRESOURCEBATTLEREQUESTCONFIG_RE	=	4426,
	PROTOCOL_FACTIONRENAMEGSVERIFY_RE	=	4528,
	PROTOCOL_MNFACTIONBATTLEAPPLY	=	5211,
	PROTOCOL_MNBATTLESERVERREGISTER	=	5221,
	PROTOCOL_MNDOMAINBATTLESTART_RE	=	5223,
	PROTOCOL_MNDOMAINBATTLEENTER	=	5224,
	PROTOCOL_MNDOMAINBATTLEENTERSUCCESSNOTICE	=	5226,
	PROTOCOL_MNDOMAINBATTLELEAVENOTICE	=	5227,
	PROTOCOL_MNDOMAINBATTLEEND	=	5228,
	PROTOCOL_MNGETTOPLIST	=	5246,
	PROTOCOL_MNGETDOMAINDATA	=	5240,
	PROTOCOL_ANNOUNCEPROVIDERID	=	73,
	PROTOCOL_PLAYERKICKOUT	=	63,
	PROTOCOL_PLAYERLOGIN	=	65,
	PROTOCOL_PLAYEROFFLINE	=	67,
	PROTOCOL_PLAYERSTATUSSYNC	=	95,
	PROTOCOL_PLAYERSTATUSANNOUNCE	=	109,
	PROTOCOL_GETTASKDATA_RE	=	112,
	PROTOCOL_SETTASKDATA_RE	=	114,
	PROTOCOL_ENTERWORLD	=	72,
	PROTOCOL_C2SGAMEDATASEND	=	75,
	PROTOCOL_PUBLICCHAT	=	79,
	PROTOCOL_GTRADESTART	=	4017,
	PROTOCOL_GTRADEEND	=	4019,
	PROTOCOL_SWITCHSERVERSTART	=	4101,
	PROTOCOL_SWITCHSERVERCANCEL	=	4102,
	PROTOCOL_ANNOUNCEGM	=	121,
	PROTOCOL_GMRESTARTSERVER	=	358,
	PROTOCOL_FACEMODIFY_RE	=	127,
	PROTOCOL_FACTIONBEGINSYNC	=	4808,
	PROTOCOL_FACTIONENDSYNC	=	4810,
	PROTOCOL_PLAYERFACTIONINFO_RE	=	4802,
	PROTOCOL_GETPLAYERFACTIONRELATION_RE	=	4822,
	PROTOCOL_FACTIONCONGREGATEREQUEST	=	4823,
	PROTOCOL_GMAILENDSYNC	=	4216,
	PROTOCOL_QUERYGAMESERVERATTR_RE	=	513,
	PROTOCOL_ACREPORTCHEATER	=	5029,
	PROTOCOL_GMCONTROLGAME	=	380,
	PROTOCOL_BATTLESTART	=	857,
	PROTOCOL_BATTLEENTERNOTICE	=	862,
	PROTOCOL_BATTLEMAPNOTICE	=	870,
	PROTOCOL_ADDICTIONCONTROL	=	556,
	PROTOCOL_ONDIVORCE	=	4048,
	PROTOCOL_BILLINGREQUEST	=	9001,
	PROTOCOL_BILLINGBALANCE	=	9000,
	PROTOCOL_BILLINGBALANCESA_RE	=	8049,
	PROTOCOL_BILLINGCONFIRM_RE	=	8053,
	PROTOCOL_SENDREFADDBONUS	=	4901,
	PROTOCOL_SENDREWARDADDBONUS	=	4951,
	PROTOCOL_REFGETREFERENCECODE	=	4908,
	PROTOCOL_PRIVATECHAT	=	96,
	PROTOCOL_FACTIONCHAT	=	4803,
	PROTOCOL_NOTIFYFACTIONFORTRESSINFO2	=	4411,
	PROTOCOL_FACTIONFORTRESSENTERNOTICE	=	4413,
	PROTOCOL_ADDCASHNOTIFY	=	142,
	PROTOCOL_SYNCFORCEGLOBALDATA	=	4703,
	PROTOCOL_AUMAILSENDED	=	235,
	PROTOCOL_COUNTRYBATTLEAPPLY_RE	=	4752,
	PROTOCOL_COUNTRYBATTLECONFIGNOTIFY	=	4759,
	PROTOCOL_COUNTRYBATTLESTART	=	4763,
	PROTOCOL_COUNTRYBATTLEENTER	=	4765,
	PROTOCOL_COUNTRYBATTLESYNCPLAYERPOS	=	4769,
	PROTOCOL_COUNTRYBATTLEDESTROYINSTANCE	=	4792,
	PROTOCOL_SSOGETTICKET	=	147,
	PROTOCOL_PLAYERENTERLEAVEGT	=	4463,
	PROTOCOL_AUTOLOCKCHANGED	=	785,
	PROTOCOL_PLAYERCHANGEDS	=	1102,
	PROTOCOL_CASHMONEYEXCHANGENOTIFY	=	157,
	PROTOCOL_SERVERFORBIDNOTIFY	=	160,
	PROTOCOL_SERVERTRIGGERNOTIFY	=	163,
	PROTOCOL_PLAYERRENAME_RE	=	3060,
	PROTOCOL_FACTIONRENAMEGSVERIFY	=	4527,
	PROTOCOL_KEKINGNOTIFY	=	4864,
	PROTOCOL_TOUCHPOINTQUERY_RE	=	4051,
	PROTOCOL_TOUCHPOINTCOST_RE	=	4053,
	PROTOCOL_AUADDUPMONEYQUERY_RE	=	4055,
	PROTOCOL_GIFTCODEREDEEM_RE	=	4057,
	PROTOCOL_UNIQUEDATAMODIFYNOTICE	=	3116,
	PROTOCOL_UNIQUEDATASYNCH	=	3117,
	PROTOCOL_TANKBATTLEENTER	=	4874,
	PROTOCOL_TANKBATTLESTART	=	4877,
	PROTOCOL_AUTOTEAMPLAYERREADY	=	964,
	PROTOCOL_AUTOTEAMCOMPOSESTART	=	966,
	PROTOCOL_AUTOTEAMCOMPOSEFAILED	=	967,
	PROTOCOL_FACTIONRESOURCEBATTLEREQUESTCONFIG	=	4425,
	PROTOCOL_FACTIONRESOURCEBATTLESTATUSNOTICE	=	4428,
	PROTOCOL_MNDOMAINBATTLESTART	=	5222,
	PROTOCOL_MNDOMAINBATTLEENTER_RE	=	5225,
	PROTOCOL_MNDOMAININFOGSNOTICE	=	5209,
};

};
#endif

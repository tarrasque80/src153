
#ifndef __GNET_GDELIVERYD_CALLID
#define __GNET_GDELIVERYD_CALLID

namespace GNET
{

enum CallID
{
	RPC_DBPLAYERREQUITEFRIEND	=	3073,
	RPC_GQUERYPASSWD	=	502,
	RPC_USERLOGIN	=	15,
	RPC_USERLOGIN2	=	8067,
	RPC_CASHSERIAL	=	8009,
	RPC_GETADDCASHSN	=	514,
	RPC_MATRIXPASSWD	=	550,
	RPC_MATRIXPASSWD2	=	8066,
	RPC_MATRIXTOKEN	=	8070,
	RPC_PUTSPOUSE	=	4047,
	RPC_QUERYUSERID	=	8001,
	RPC_FORBIDUSER	=	8004,
	RPC_PLAYERPOSITIONRESETRQST	=	155,
	RPC_DBPLAYERCHANGEGENDER	=	3071,
	RPC_DBFACTIONRENAME	=	4530,
	RPC_DBSELLPOINT	=	611,
	RPC_DBBUYPOINT	=	615,
	RPC_DBGAMETALKROLELIST	=	4451,
	RPC_DBGAMETALKROLERELATION	=	4452,
	RPC_DBGAMETALKFACTIONINFO	=	4453,
	RPC_DBGAMETALKROLESTATUS	=	4454,
	RPC_DBGAMETALKROLEINFO	=	4455,
	RPC_PLAYERIDENTITYMATCH	=	1113,
	RPC_DBMNPUTBATTLEBONUS	=	5230,
	RPC_ADDFRIENDRQST	=	204,
	RPC_TRADESTARTRQST	=	4003,
	RPC_GMQUERYROLEINFO	=	124,
	RPC_GETMAXONLINENUM	=	375,
	RPC_GMGETGAMEATTRI	=	376,
	RPC_GMSETGAMEATTRI	=	377,
	RPC_CASHMONEYEXCHANGECONTROL	=	8200,
	RPC_SERVERFORBIDCONTROL	=	8201,
	RPC_USERLOGOUT	=	33,
	RPC_GETUSERCOUPON	=	8062,
	RPC_COUPONEXCHANGE	=	8063,
	RPC_INSTANTADDCASH	=	8015,
	RPC_BATTLEEND	=	859,
	RPC_SYSAUCTIONPREPAREITEM	=	4363,
	RPC_GETFACTIONFORTRESS	=	4404,
	RPC_PUTFACTIONFORTRESS	=	4405,
	RPC_DBCREATEROLE	=	3007,
	RPC_DBDELETEROLE	=	3008,
	RPC_DBUNDODELETEROLE	=	3009,
	RPC_PUTUSER	=	3001,
	RPC_GETUSER	=	3002,
	RPC_DELUSER	=	3003,
	RPC_GETROLE	=	3005,
	RPC_GETROLEINFO	=	3051,
	RPC_DELROLE	=	3006,
	RPC_PUTROLEBASE	=	3012,
	RPC_GETROLEBASE	=	3013,
	RPC_GETROLEPOCKET	=	3053,
	RPC_PUTROLEPOCKET	=	3052,
	RPC_PUTROLESTATUS	=	3014,
	RPC_GETROLESTATUS	=	3015,
	RPC_PUTROLEEQUIPMENT	=	3016,
	RPC_GETROLEEQUIPMENT	=	3017,
	RPC_PUTROLETASK	=	3018,
	RPC_GETROLETASK	=	3019,
	RPC_PUTROLEDATA	=	8002,
	RPC_GETROLEDATA	=	8003,
	RPC_TRADEINVENTORY	=	3020,
	RPC_TRADESAVE	=	3021,
	RPC_PUTROLE	=	3024,
	RPC_GETMONEYINVENTORY	=	3022,
	RPC_PUTMONEYINVENTORY	=	3023,
	RPC_GETROLEBASESTATUS	=	3025,
	RPC_PUTROLESTOREHOUSE	=	3026,
	RPC_GETROLESTOREHOUSE	=	3027,
	RPC_PUTROLEFORBID	=	3030,
	RPC_GETROLEFORBID	=	3031,
	RPC_GETROLEID	=	3033,
	RPC_GETFRIENDLIST	=	201,
	RPC_PUTFRIENDLIST	=	200,
	RPC_PUTMESSAGE	=	215,
	RPC_GETMESSAGE	=	216,
	RPC_GETTASKDATARPC	=	115,
	RPC_PUTTASKDATARPC	=	135,
	RPC_DBVERIFYMASTER	=	4609,
	RPC_DBGETCONSUMEINFOS	=	384,
	RPC_DBMAPPASSWORDLOAD	=	3043,
	RPC_DBMAPPASSWORDSAVE	=	3044,
	RPC_DBSOLOCHALLENGERANKLOAD	=	3047,
	RPC_DBSOLOCHALLENGERANKSAVE	=	3048,
	RPC_GETUSERROLES	=	3401,
	RPC_CLEARSTOREHOUSEPASSWD	=	3402,
	RPC_CANCHANGEROLENAME	=	3403,
	RPC_RENAMEROLE	=	3404,
	RPC_UID2LOGICUID	=	3411,
	RPC_ROLEID2UID	=	3412,
	RPC_TRANSACTIONACQUIRE	=	3034,
	RPC_TRANSACTIONABORT	=	3035,
	RPC_TRANSACTIONCOMMIT	=	3036,
	RPC_DBGETMAILLIST	=	4251,
	RPC_DBGETMAIL	=	4252,
	RPC_DBGETMAILATTACH	=	4253,
	RPC_DBSETMAILATTR	=	4254,
	RPC_DBSENDMAIL	=	4255,
	RPC_DBDELETEMAIL	=	4256,
	RPC_DBSENDMASSMAIL	=	4257,
	RPC_DBAUCTIONOPEN	=	810,
	RPC_DBAUCTIONBID	=	811,
	RPC_DBAUCTIONGET	=	813,
	RPC_DBAUCTIONCLOSE	=	815,
	RPC_DBAUCTIONLIST	=	812,
	RPC_DBAUCTIONTIMEOUT	=	814,
	RPC_DBSYNCSELLINFO	=	612,
	RPC_DBSELLTIMEOUT	=	613,
	RPC_DBSELLCANCEL	=	614,
	RPC_DBTRANSPOINTDEAL	=	618,
	RPC_DBBATTLECHALLENGE	=	865,
	RPC_DBBATTLELOAD	=	863,
	RPC_DBBATTLESET	=	864,
	RPC_DBBATTLEEND	=	868,
	RPC_DBBATTLEMAIL	=	871,
	RPC_DBBATTLEBONUS	=	872,
	RPC_DBSTOCKLOAD	=	415,
	RPC_DBSTOCKTRANSACTION	=	414,
	RPC_DBSTOCKBALANCE	=	413,
	RPC_DBSTOCKCOMMISSION	=	416,
	RPC_DBSTOCKCANCEL	=	417,
	RPC_DBSETCASHPASSWORD	=	3100,
	RPC_DBAUTOLOCKSET	=	784,
	RPC_DBAUTOLOCKGET	=	392,
	RPC_DBAUTOLOCKSETOFFLINE	=	391,
	RPC_DBFORBIDUSER	=	5037,
	RPC_DBCLEARCONSUMABLE	=	7000,
	RPC_DBREFWITHDRAWTRANS	=	4903,
	RPC_DBREFGETREFERRAL	=	4910,
	RPC_DBREFGETREFERRER	=	4911,
	RPC_DBREFUPDATEREFERRAL	=	4912,
	RPC_DBREFUPDATEREFERRER	=	4913,
	RPC_DBGETREWARD	=	4957,
	RPC_DBPUTCONSUMEPOINTS	=	4958,
	RPC_DBPUTREWARDBONUS	=	4961,
	RPC_DBREWARDMATURE	=	4959,
	RPC_DBEXCHANGECONSUMEPOINTS	=	4960,
	RPC_DBPLAYERPOSITIONRESET	=	3110,
	RPC_DBWEBTRADELOAD	=	4301,
	RPC_DBWEBTRADELOADSOLD	=	4323,
	RPC_DBWEBTRADEPREPOST	=	4304,
	RPC_DBWEBTRADEPRECANCELPOST	=	4307,
	RPC_DBWEBTRADEPOST	=	4316,
	RPC_DBWEBTRADECANCELPOST	=	4317,
	RPC_DBWEBTRADESHELF	=	4318,
	RPC_DBWEBTRADECANCELSHELF	=	4319,
	RPC_DBWEBTRADESOLD	=	4320,
	RPC_DBWEBTRADEPOSTEXPIRE	=	4321,
	RPC_DBWEBTRADEGETROLESIMPLEINFO	=	4322,
	RPC_DBSYSAUCTIONCASHTRANSFER	=	4361,
	RPC_DBSYSAUCTIONCASHSPEND	=	4362,
	RPC_PUTSERVERDATA	=	3056,
	RPC_GETSERVERDATA	=	3057,
	RPC_GETCASHTOTAL	=	3058,
	RPC_DBFACTIONFORTRESSLOAD	=	4401,
	RPC_DBPUTFACTIONFORTRESS	=	4402,
	RPC_DBDELFACTIONFORTRESS	=	4403,
	RPC_DBCREATEFACTIONFORTRESS	=	4408,
	RPC_DBFACTIONFORTRESSCHALLENGE	=	4418,
	RPC_DBFORCELOAD	=	4701,
	RPC_DBPUTFORCE	=	4702,
	RPC_DBCOUNTRYBATTLEBONUS	=	4771,
	RPC_FETCHPLAYERDATA	=	1106,
	RPC_ACTIVATEPLAYERDATA	=	1107,
	RPC_DELPLAYERDATA	=	1124,
	RPC_SAVEPLAYERDATA	=	1114,
	RPC_FREEZEPLAYERDATA	=	1112,
	RPC_TOUCHPLAYERDATA	=	1108,
	RPC_DBUPDATEPLAYERCROSSINFO	=	1125,
	RPC_DBLOADGLOBALCONTROL	=	3111,
	RPC_DBPUTGLOBALCONTROL	=	3112,
	RPC_DBUNIQUEDATALOAD	=	3113,
	RPC_DBUNIQUEDATASAVE	=	3114,
	RPC_DBPLAYERRENAME	=	3062,
	RPC_DBROLENAMELIST	=	3064,
	RPC_DBKELOAD	=	4851,
	RPC_DBKECANDIDATEAPPLY	=	4856,
	RPC_DBKECANDIDATECONFIRM	=	4857,
	RPC_DBKEVOTING	=	4860,
	RPC_DBKEKINGCONFIRM	=	4861,
	RPC_DBKEDELETEKING	=	4862,
	RPC_DBKEDELETECANDIDATE	=	4863,
	RPC_DBPSHOPCREATE	=	930,
	RPC_DBPSHOPBUY	=	931,
	RPC_DBPSHOPSELL	=	932,
	RPC_DBPSHOPCANCELGOODS	=	933,
	RPC_DBPSHOPPLAYERBUY	=	934,
	RPC_DBPSHOPPLAYERSELL	=	935,
	RPC_DBPSHOPSETTYPE	=	936,
	RPC_DBPSHOPACTIVE	=	937,
	RPC_DBPSHOPMANAGEFUND	=	938,
	RPC_DBPSHOPDRAWITEM	=	939,
	RPC_DBPSHOPLOAD	=	940,
	RPC_DBPSHOPGET	=	941,
	RPC_DBPSHOPCLEARGOODS	=	942,
	RPC_DBPSHOPTIMEOUT	=	943,
	RPC_DBPLAYERGIVEPRESENT	=	3067,
	RPC_DBPLAYERASKFORPRESENT	=	3070,
	RPC_DBSYSMAIL3	=	4221,
	RPC_DBGETPLAYERPROFILEDATA	=	956,
	RPC_DBPUTPLAYERPROFILEDATA	=	957,
	RPC_DBTANKBATTLEBONUS	=	4883,
	RPC_DBFACTIONRESOURCEBATTLEBONUS	=	4432,
	RPC_DBCOPYROLE	=	5101,
	RPC_DBMNFACTIONAPPLYINFOGET	=	5208,
	RPC_DBMNFACTIONBATTLEAPPLY	=	5213,
	RPC_DBMNFACTIONAPPLYRESNOTIFY	=	5216,
	RPC_DBMNSENDBATTLEBONUS	=	5233,
	RPC_DBMNFACTIONINFOGET	=	5201,
	RPC_DBMNFACTIONSTATEUPDATE	=	5210,
	RPC_DBMNFACTIONINFOUPDATE	=	5206,
	RPC_DBMNDOMAININFOUPDATE	=	5207,
	RPC_DBMNFACTIONAPPLYINFOPUT	=	5217,
	RPC_DBMNSENDBONUSNOTIFY	=	5234,
	RPC_PRECREATEROLE	=	3037,
	RPC_POSTCREATEROLE	=	3038,
	RPC_POSTDELETEROLE	=	3039,
	RPC_PRECREATEFACTION	=	3040,
	RPC_POSTCREATEFACTION	=	3041,
	RPC_POSTDELETEFACTION	=	3042,
	RPC_PRECREATEFAMILY	=	3046,
	RPC_POSTCREATEFAMILY	=	3049,
	RPC_POSTDELETEFAMILY	=	3050,
	RPC_PREPLAYERRENAME	=	3061,
	RPC_PREFACTIONRENAME	=	4529,
	RPC_ACCOUNTADDROLE	=	3010,
	RPC_ACCOUNTDELROLE	=	3011,
	RPC_ANNOUNCEFACTIONROLEDEL	=	4811,
};

enum ProtocolType
{
	PROTOCOL_QUERYUSERPRIVILEGE	=	506,
	PROTOCOL_QUERYUSERFORBID	=	508,
	PROTOCOL_PLAYERKICKOUT	=	63,
	PROTOCOL_PLAYEROFFLINE	=	67,
	PROTOCOL_GETTASKDATA_RE	=	112,
	PROTOCOL_SETTASKDATA_RE	=	114,
	PROTOCOL_PLAYERSTATUSANNOUNCE	=	109,
	PROTOCOL_ONLINEANNOUNCE	=	4,
	PROTOCOL_ROLELIST_RE	=	83,
	PROTOCOL_CREATEROLE_RE	=	85,
	PROTOCOL_DELETEROLE_RE	=	87,
	PROTOCOL_UNDODELETEROLE_RE	=	89,
	PROTOCOL_ACCOUNTLOGINRECORD	=	143,
	PROTOCOL_PLAYERBASEINFO_RE	=	92,
	PROTOCOL_PLAYERBASEINFOCRC_RE	=	99,
	PROTOCOL_SETCUSTOMDATA_RE	=	101,
	PROTOCOL_GETCUSTOMDATA_RE	=	117,
	PROTOCOL_GETPLAYERIDBYNAME_RE	=	119,
	PROTOCOL_SETUICONFIG_RE	=	103,
	PROTOCOL_GETUICONFIG_RE	=	105,
	PROTOCOL_SETHELPSTATES_RE	=	129,
	PROTOCOL_GETHELPSTATES_RE	=	131,
	PROTOCOL_GETPLAYERBRIEFINFO_RE	=	108,
	PROTOCOL_GMGETPLAYERCONSUMEINFO_RE	=	383,
	PROTOCOL_WAITQUEUESTATENOTIFY	=	164,
	PROTOCOL_CANCELWAITQUEUE_RE	=	166,
	PROTOCOL_GMCONTROLGAME	=	380,
	PROTOCOL_GMCONTROLGAME_RE	=	381,
	PROTOCOL_FACEMODIFY_RE	=	127,
	PROTOCOL_ACCOUNTINGREQUEST	=	503,
	PROTOCOL_CHATBROADCAST	=	120,
	PROTOCOL_WORLDCHAT	=	133,
	PROTOCOL_FACTIONCHAT	=	4803,
	PROTOCOL_CHATMULTICAST	=	81,
	PROTOCOL_CHATSINGLECAST	=	94,
	PROTOCOL_ROLESTATUSANNOUNCE	=	7,
	PROTOCOL_ADDFRIEND_RE	=	203,
	PROTOCOL_ADDFRIENDREMARKS_RE	=	237,
	PROTOCOL_GETFRIENDS_RE	=	207,
	PROTOCOL_UPDATEENEMYLIST_RE	=	239,
	PROTOCOL_GETENEMYLIST_RE	=	241,
	PROTOCOL_SETGROUPNAME_RE	=	209,
	PROTOCOL_SETFRIENDGROUP_RE	=	211,
	PROTOCOL_GETSAVEDMSG_RE	=	218,
	PROTOCOL_CHATROOMCREATE_RE	=	220,
	PROTOCOL_CHATROOMINVITE_RE	=	222,
	PROTOCOL_CHATROOMJOIN_RE	=	224,
	PROTOCOL_CHATROOMLIST_RE	=	229,
	PROTOCOL_FRIENDEXTLIST	=	230,
	PROTOCOL_SENDAUMAIL_RE	=	234,
	PROTOCOL_TRADESTART_RE	=	4002,
	PROTOCOL_TRADEADDGOODS_RE	=	4005,
	PROTOCOL_TRADEREMOVEGOODS_RE	=	4007,
	PROTOCOL_TRADESUBMIT_RE	=	4011,
	PROTOCOL_TRADEMOVEOBJ_RE	=	4009,
	PROTOCOL_TRADECONFIRM_RE	=	4013,
	PROTOCOL_TRADEDISCARD_RE	=	4015,
	PROTOCOL_TRADEEND	=	4016,
	PROTOCOL_GTRADESTART	=	4017,
	PROTOCOL_GTRADEEND	=	4019,
	PROTOCOL_PLAYERRENAME_RE	=	3060,
	PROTOCOL_NOTIFYFACTIONPLAYERRENAME	=	4424,
	PROTOCOL_POSTPLAYERRENAME	=	3063,
	PROTOCOL_PLAYERNAMEUPDATE	=	158,
	PROTOCOL_PLAYERGIVEPRESENT_RE	=	3066,
	PROTOCOL_PLAYERASKFORPRESENT_RE	=	3069,
	PROTOCOL_GETSOLOCHALLENGERANK_RE	=	3076,
	PROTOCOL_ANNOUNCEGM	=	121,
	PROTOCOL_GMONLINENUM_RE	=	351,
	PROTOCOL_GMLISTONLINEUSER_RE	=	353,
	PROTOCOL_GMKICKOUTUSER_RE	=	355,
	PROTOCOL_GMFORBIDSELLPOINT_RE	=	379,
	PROTOCOL_GMKICKOUTROLE_RE	=	361,
	PROTOCOL_GMSHUTUP_RE	=	357,
	PROTOCOL_GMSHUTUPROLE_RE	=	363,
	PROTOCOL_GMTOGGLECHAT_RE	=	365,
	PROTOCOL_GMFORBIDROLE_RE	=	367,
	PROTOCOL_REPORT2GM_RE	=	369,
	PROTOCOL_COMPLAIN2GM_RE	=	371,
	PROTOCOL_ANNOUNCEFORBIDINFO	=	123,
	PROTOCOL_SETMAXONLINENUM_RE	=	374,
	PROTOCOL_VERIFYMASTER	=	519,
	PROTOCOL_VERIFYMASTER_RE	=	520,
	PROTOCOL_GMSETTIMELESSAUTOLOCK_RE	=	386,
	PROTOCOL_IWEBAUTOLOCKGET_RE	=	388,
	PROTOCOL_IWEBAUTOLOCKSET_RE	=	390,
	PROTOCOL_ACWHOAMI	=	5002,
	PROTOCOL_ACSTATUSANNOUNCE	=	5026,
	PROTOCOL_ACSTATUSANNOUNCE2	=	5034,
	PROTOCOL_ACACCUSE	=	5055,
	PROTOCOL_PLAYERACCUSE_RE	=	162,
	PROTOCOL_ANNOUNCENEWMAIL	=	4201,
	PROTOCOL_GETMAILLIST_RE	=	4203,
	PROTOCOL_GETMAIL_RE	=	4205,
	PROTOCOL_GETMAILATTACHOBJ_RE	=	4207,
	PROTOCOL_DELETEMAIL_RE	=	4209,
	PROTOCOL_PRESERVEMAIL_RE	=	4211,
	PROTOCOL_PLAYERSENDMAIL_RE	=	4213,
	PROTOCOL_GMAILENDSYNC	=	4216,
	PROTOCOL_SYSSENDMAIL_RE	=	4215,
	PROTOCOL_SYSSENDMAIL3_RE	=	8069,
	PROTOCOL_SYSRECOVEREDOBJMAIL_RE	=	4220,
	PROTOCOL_AUCTIONOPEN_RE	=	801,
	PROTOCOL_AUCTIONBID_RE	=	803,
	PROTOCOL_AUCTIONCLOSE_RE	=	807,
	PROTOCOL_AUCTIONLIST_RE	=	805,
	PROTOCOL_AUCTIONGET_RE	=	809,
	PROTOCOL_AUCTIONGETITEM_RE	=	821,
	PROTOCOL_AUCTIONATTENDLIST_RE	=	817,
	PROTOCOL_AUCTIONEXITBID_RE	=	819,
	PROTOCOL_AUCTIONLISTUPDATE_RE	=	824,
	PROTOCOL_BATTLEGETMAP_RE	=	851,
	PROTOCOL_BATTLESTATUS_RE	=	867,
	PROTOCOL_BATTLECHALLENGE_RE	=	853,
	PROTOCOL_BATTLECHALLENGEMAP_RE	=	855,
	PROTOCOL_BATTLEENTER_RE	=	861,
	PROTOCOL_BATTLESTART_RE	=	858,
	PROTOCOL_BATTLEENTERNOTICE	=	862,
	PROTOCOL_BATTLEMAPNOTICE	=	870,
	PROTOCOL_BATTLESTART	=	857,
	PROTOCOL_BATTLEFACTIONNOTICE	=	874,
	PROTOCOL_QUERYREWARDTYPE	=	510,
	PROTOCOL_QUERYREWARDTYPE_RE	=	511,
	PROTOCOL_QUERYGAMESERVERATTR_RE	=	513,
	PROTOCOL_ANNOUNCESERVERATTRIBUTE	=	132,
	PROTOCOL_ANNOUNCECHALLENGEALGO	=	136,
	PROTOCOL_ANNOUNCEAUTHDVERSION	=	137,
	PROTOCOL_GETSELLLIST_RE	=	604,
	PROTOCOL_FINDSELLPOINTINFO_RE	=	620,
	PROTOCOL_ANNOUNCESELLRESULT	=	610,
	PROTOCOL_SELLCANCEL_RE	=	606,
	PROTOCOL_BUYPOINT_RE	=	608,
	PROTOCOL_SELLPOINT_RE	=	602,
	PROTOCOL_ANNOUNCEZONEID	=	505,
	PROTOCOL_ANNOUNCEZONEID2	=	523,
	PROTOCOL_ANNOUNCEZONEID3	=	527,
	PROTOCOL_STOCKCOMMISSION_RE	=	409,
	PROTOCOL_STOCKACCOUNT_RE	=	408,
	PROTOCOL_STOCKTRANSACTION_RE	=	410,
	PROTOCOL_STOCKBILL_RE	=	406,
	PROTOCOL_STOCKCANCEL_RE	=	412,
	PROTOCOL_CASHLOCK_RE	=	4261,
	PROTOCOL_CASHPASSWORDSET_RE	=	4264,
	PROTOCOL_ONDIVORCE	=	4048,
	PROTOCOL_AUTOLOCKSET_RE	=	783,
	PROTOCOL_FORWARDCHAT	=	8000,
	PROTOCOL_DISABLEAUTOLOCK	=	8007,
	PROTOCOL_ACFORBIDCHEATER	=	8008,
	PROTOCOL_AUTOLOCKCHANGED	=	785,
	PROTOCOL_SENDREFADDBONUS	=	4901,
	PROTOCOL_SENDREWARDADDBONUS	=	4951,
	PROTOCOL_REFWITHDRAWBONUS_RE	=	4907,
	PROTOCOL_REFLISTREFERRALS_RE	=	4905,
	PROTOCOL_REWARDMATURENOTICE	=	4956,
	PROTOCOL_EXCHANGECONSUMEPOINTS_RE	=	4955,
	PROTOCOL_GETREWARDLIST_RE	=	4953,
	PROTOCOL_WEBTRADEPREPOST_RE	=	4303,
	PROTOCOL_WEBTRADEPRECANCELPOST_RE	=	4306,
	PROTOCOL_WEBTRADELIST_RE	=	4309,
	PROTOCOL_WEBTRADEGETITEM_RE	=	4311,
	PROTOCOL_WEBTRADEATTENDLIST_RE	=	4313,
	PROTOCOL_WEBTRADEGETDETAIL_RE	=	4315,
	PROTOCOL_WEBTRADEUPDATE_RE	=	4325,
	PROTOCOL_POST	=	8020,
	PROTOCOL_GAMEPOSTCANCEL	=	8022,
	PROTOCOL_WEBPOSTCANCEL_RE	=	8025,
	PROTOCOL_SHELF_RE	=	8027,
	PROTOCOL_SHELFCANCEL_RE	=	8029,
	PROTOCOL_SOLD_RE	=	8031,
	PROTOCOL_POSTEXPIRE_RE	=	8033,
	PROTOCOL_WEBGETROLELIST_RE	=	8035,
	PROTOCOL_NEWKEEPALIVE	=	8036,
	PROTOCOL_SYSAUCTIONLIST_RE	=	4352,
	PROTOCOL_SYSAUCTIONGETITEM_RE	=	4354,
	PROTOCOL_SYSAUCTIONACCOUNT_RE	=	4356,
	PROTOCOL_SYSAUCTIONBID_RE	=	4358,
	PROTOCOL_SYSAUCTIONCASHTRANSFER_RE	=	4360,
	PROTOCOL_CREATEFACTIONFORTRESS_RE	=	4407,
	PROTOCOL_NOTIFYFACTIONFORTRESSINFO2	=	4411,
	PROTOCOL_FACTIONFORTRESSENTERNOTICE	=	4413,
	PROTOCOL_FACTIONFORTRESSLIST_RE	=	4415,
	PROTOCOL_FACTIONFORTRESSCHALLENGE_RE	=	4417,
	PROTOCOL_FACTIONFORTRESSBATTLELIST_RE	=	4420,
	PROTOCOL_FACTIONFORTRESSGET_RE	=	4422,
	PROTOCOL_NOTIFYFACTIONFORTRESSID	=	4423,
	PROTOCOL_ANNOUNCEZONEIDTOIM	=	8101,
	PROTOCOL_GAMESYSMSG	=	8102,
	PROTOCOL_GAMEDATAREQ	=	8103,
	PROTOCOL_GAMEDATARESP	=	8104,
	PROTOCOL_IMKEEPALIVE	=	8105,
	PROTOCOL_ANNOUNCERESP	=	8106,
	PROTOCOL_ROLELISTREQ	=	8121,
	PROTOCOL_ROLELISTRESP	=	8122,
	PROTOCOL_ROLERELATIONREQ	=	8123,
	PROTOCOL_ROLERELATIONRESP	=	8124,
	PROTOCOL_ROLESTATUSREQ	=	8125,
	PROTOCOL_ROLESTATUSRESP	=	8126,
	PROTOCOL_ROLEINFOREQ	=	8137,
	PROTOCOL_ROLEINFORESP	=	8138,
	PROTOCOL_ROLESTATUSUPDATE	=	8127,
	PROTOCOL_ROLEGROUPUPDATE	=	8128,
	PROTOCOL_ROLEFRIENDUPDATE	=	8129,
	PROTOCOL_ROLEBLACKLISTUPDATE	=	8130,
	PROTOCOL_ROLEMSG	=	8131,
	PROTOCOL_ROLEOFFLINEMESSAGES	=	8132,
	PROTOCOL_ROLEACTIVATION	=	8133,
	PROTOCOL_REMOVEROLE	=	8134,
	PROTOCOL_ROLEINFOUPDATE	=	8135,
	PROTOCOL_ROLEFORBIDUPDATE	=	8136,
	PROTOCOL_FACTIONINFOREQ	=	8161,
	PROTOCOL_FACTIONINFORESP	=	8162,
	PROTOCOL_FACTIONMEMBERUPDATE	=	8163,
	PROTOCOL_FACTIONINFOUPDATE	=	8164,
	PROTOCOL_FACTIONMSG	=	8165,
	PROTOCOL_REMOVEFACTION	=	8166,
	PROTOCOL_FACTIONFORBIDUPDATE	=	8167,
	PROTOCOL_SYNCTEAMS	=	8151,
	PROTOCOL_TEAMCREATE	=	8152,
	PROTOCOL_TEAMDISMISS	=	8153,
	PROTOCOL_TEAMMEMBERUPDATE	=	8154,
	PROTOCOL_ROLEENTERVOICECHANNEL	=	8177,
	PROTOCOL_ROLEENTERVOICECHANNELACK	=	8179,
	PROTOCOL_ROLELEAVEVOICECHANNEL	=	8178,
	PROTOCOL_ROLELEAVEVOICECHANNELACK	=	8280,
	PROTOCOL_USERCOUPON_RE	=	139,
	PROTOCOL_USERCOUPONEXCHANGE_RE	=	141,
	PROTOCOL_ADDCASHNOTIFY	=	142,
	PROTOCOL_USERADDCASH_RE	=	145,
	PROTOCOL_SSOGETTICKET_RE	=	148,
	PROTOCOL_SSOGETTICKETREQ	=	8016,
	PROTOCOL_SYNCFORCEGLOBALDATA	=	4703,
	PROTOCOL_AUMAILSENDED	=	235,
	PROTOCOL_GAME2AU	=	8039,
	PROTOCOL_DBFRIENDEXTLIST	=	231,
	PROTOCOL_COUNTRYBATTLEAPPLY_RE	=	4752,
	PROTOCOL_COUNTRYBATTLECONFIGNOTIFY	=	4759,
	PROTOCOL_COUNTRYBATTLEMOVE_RE	=	4761,
	PROTOCOL_COUNTRYBATTLESYNCPLAYERLOCATION	=	4762,
	PROTOCOL_COUNTRYBATTLESTART	=	4763,
	PROTOCOL_COUNTRYBATTLEENTER	=	4765,
	PROTOCOL_COUNTRYBATTLEGETMAP_RE	=	4768,
	PROTOCOL_COUNTRYBATTLESYNCPLAYERPOS	=	4769,
	PROTOCOL_COUNTRYBATTLEGETCONFIG_RE	=	4773,
	PROTOCOL_COUNTRYBATTLEGETSCORE_RE	=	4775,
	PROTOCOL_COUNTRYBATTLEPREENTERNOTIFY	=	4776,
	PROTOCOL_COUNTRYBATTLERESULT	=	4778,
	PROTOCOL_COUNTRYBATTLESINGLEBATTLERESULT	=	4780,
	PROTOCOL_COUNTRYBATTLEKINGASSIGNASSAULT_RE	=	4782,
	PROTOCOL_COUNTRYBATTLEGETBATTLELIMIT_RE	=	4785,
	PROTOCOL_COUNTRYBATTLEGETKINGCOMMANDPOINT_RE	=	4787,
	PROTOCOL_GETCNETSERVERCONFIG_RE	=	4789,
	PROTOCOL_COUNTRYBATTLEDESTROYINSTANCE	=	4792,
	PROTOCOL_QPANNOUNCEDISCOUNT	=	149,
	PROTOCOL_QPGETACTIVATEDSERVICES_RE	=	151,
	PROTOCOL_QPADDCASH_RE	=	153,
	PROTOCOL_PLAYERENTERLEAVEGT	=	4463,
	PROTOCOL_PLAYERCHANGEDS	=	1102,
	PROTOCOL_CHANGEDS_RE	=	1104,
	PROTOCOL_ANNOUNCECENTRALDELIVERY	=	1131,
	PROTOCOL_KICKOUTUSER2	=	1126,
	PROTOCOL_CASHMONEYEXCHANGENOTIFY	=	157,
	PROTOCOL_SERVERFORBIDNOTIFY	=	160,
	PROTOCOL_SERVERTRIGGERNOTIFY	=	163,
	PROTOCOL_KEGETSTATUS_RE	=	4853,
	PROTOCOL_KECANDIDATEAPPLY_RE	=	4855,
	PROTOCOL_KEVOTING_RE	=	4859,
	PROTOCOL_KEKINGNOTIFY	=	4864,
	PROTOCOL_PSHOPCREATE_RE	=	901,
	PROTOCOL_PSHOPBUY_RE	=	903,
	PROTOCOL_PSHOPSELL_RE	=	905,
	PROTOCOL_PSHOPCANCELGOODS_RE	=	907,
	PROTOCOL_PSHOPPLAYERBUY_RE	=	909,
	PROTOCOL_PSHOPPLAYERSELL_RE	=	911,
	PROTOCOL_PSHOPSETTYPE_RE	=	913,
	PROTOCOL_PSHOPACTIVE_RE	=	915,
	PROTOCOL_PSHOPMANAGEFUND_RE	=	917,
	PROTOCOL_PSHOPDRAWITEM_RE	=	919,
	PROTOCOL_PSHOPCLEARGOODS_RE	=	921,
	PROTOCOL_PSHOPSELFGET_RE	=	923,
	PROTOCOL_PSHOPPLAYERGET_RE	=	925,
	PROTOCOL_PSHOPLIST_RE	=	927,
	PROTOCOL_PSHOPLISTITEM_RE	=	929,
	PROTOCOL_TOUCHPOINTQUERY_RE	=	4051,
	PROTOCOL_TOUCHPOINTCOST_RE	=	4053,
	PROTOCOL_AUADDUPMONEYQUERY_RE	=	4055,
	PROTOCOL_GIFTCODEREDEEM_RE	=	4057,
	PROTOCOL_UNIQUEDATAMODIFYNOTICE	=	3116,
	PROTOCOL_UNIQUEDATASYNCH	=	3117,
	PROTOCOL_PLAYERPROFILEGETPROFILEDATA_RE	=	952,
	PROTOCOL_PLAYERPROFILEGETMATCHRESULT_RE	=	955,
	PROTOCOL_UNIQUEDATAMODIFYBROADCAST	=	3118,
	PROTOCOL_TANKBATTLEENTER	=	4874,
	PROTOCOL_TANKBATTLESTART	=	4877,
	PROTOCOL_TANKBATTLEPLAYERAPPLY_RE	=	4873,
	PROTOCOL_TANKBATTLEPLAYERGETRANK_RE	=	4882,
	PROTOCOL_AUTOTEAMSETGOAL_RE	=	963,
	PROTOCOL_AUTOTEAMPLAYERREADY	=	964,
	PROTOCOL_AUTOTEAMCOMPOSESTART	=	966,
	PROTOCOL_AUTOTEAMCOMPOSEFAILED	=	967,
	PROTOCOL_AUTOTEAMPLAYERLEAVE	=	968,
	PROTOCOL_FACTIONRESOURCEBATTLEREQUESTCONFIG	=	4425,
	PROTOCOL_FACTIONRESOURCEBATTLESTATUSNOTICE	=	4428,
	PROTOCOL_FACTIONRESOURCEBATTLEPLAYERQUERYRESULT	=	4431,
	PROTOCOL_FACTIONRESOURCEBATTLELIMITNOTICE	=	4433,
	PROTOCOL_FACTIONRESOURCEBATTLEGETMAP_RE	=	4435,
	PROTOCOL_FACTIONRESOURCEBATTLEGETRECORD_RE	=	4437,
	PROTOCOL_FACTIONRESOURCEBATTLENOTIFYPLAYEREVENT	=	4438,
	PROTOCOL_FACTIONUNIQUEIDANNOUNCE	=	4824,
	PROTOCOL_DBMNFACTIONCACHEUPDATE	=	5204,
	PROTOCOL_MNDOMAINBATTLESTART	=	5222,
	PROTOCOL_MNDOMAINBATTLEENTER_RE	=	5225,
	PROTOCOL_MNDOMAININFOGSNOTICE	=	5209,
	PROTOCOL_MNFACTIONBATTLEAPPLY_RE	=	5212,
	PROTOCOL_MNGETDOMAINDATA_RE	=	5241,
	PROTOCOL_MNGETPLAYERLASTENTERINFO_RE	=	5243,
	PROTOCOL_MNGETFACTIONBRIEFINFO_RE	=	5245,
	PROTOCOL_MNGETFACTIONINFO_RE	=	5249,
	PROTOCOL_MNGETTOPLIST_RE	=	5247,
	PROTOCOL_CROSSCHAT	=	5301,
	PROTOCOL_CROSSCHAT_RE	=	5302,
	PROTOCOL_CROSSSOLOCHALLENGERANK	=	3077,
	PROTOCOL_CROSSSOLOCHALLENGERANK_RE	=	3078,
	PROTOCOL_PLAYERLOGIN	=	65,
	PROTOCOL_PLAYERSTATUSSYNC	=	95,
	PROTOCOL_ENTERWORLD	=	72,
	PROTOCOL_STATUSANNOUNCE	=	6,
	PROTOCOL_ROLELIST	=	82,
	PROTOCOL_CREATEROLE	=	84,
	PROTOCOL_DELETEROLE	=	86,
	PROTOCOL_UNDODELETEROLE	=	88,
	PROTOCOL_PLAYERBASEINFO	=	91,
	PROTOCOL_PLAYERBASEINFOCRC	=	98,
	PROTOCOL_GETPLAYERIDBYNAME	=	118,
	PROTOCOL_SETCUSTOMDATA	=	100,
	PROTOCOL_GETCUSTOMDATA	=	116,
	PROTOCOL_SETUICONFIG	=	102,
	PROTOCOL_GETUICONFIG	=	104,
	PROTOCOL_SETHELPSTATES	=	128,
	PROTOCOL_GETHELPSTATES	=	130,
	PROTOCOL_GETPLAYERBRIEFINFO	=	107,
	PROTOCOL_GMGETPLAYERCONSUMEINFO	=	382,
	PROTOCOL_COLLECTCLIENTMACHINEINFO	=	37,
	PROTOCOL_CANCELWAITQUEUE	=	165,
	PROTOCOL_PUBLICCHAT	=	79,
	PROTOCOL_PRIVATECHAT	=	96,
	PROTOCOL_ADDFRIEND	=	202,
	PROTOCOL_ADDFRIENDREMARKS	=	236,
	PROTOCOL_GETFRIENDS	=	206,
	PROTOCOL_GETENEMYLIST	=	240,
	PROTOCOL_SETGROUPNAME	=	208,
	PROTOCOL_SETFRIENDGROUP	=	210,
	PROTOCOL_DELFRIEND	=	212,
	PROTOCOL_DELFRIEND_RE	=	213,
	PROTOCOL_FRIENDSTATUS	=	214,
	PROTOCOL_GETSAVEDMSG	=	217,
	PROTOCOL_CHATROOMCREATE	=	219,
	PROTOCOL_CHATROOMINVITE	=	221,
	PROTOCOL_CHATROOMJOIN	=	223,
	PROTOCOL_CHATROOMLEAVE	=	225,
	PROTOCOL_CHATROOMEXPEL	=	226,
	PROTOCOL_CHATROOMSPEAK	=	227,
	PROTOCOL_CHATROOMLIST	=	228,
	PROTOCOL_SENDAUMAIL	=	233,
	PROTOCOL_PLAYERREQUITEFRIEND	=	3072,
	PROTOCOL_TRADESTART	=	4001,
	PROTOCOL_TRADEADDGOODS	=	4004,
	PROTOCOL_TRADEREMOVEGOODS	=	4006,
	PROTOCOL_TRADESUBMIT	=	4010,
	PROTOCOL_TRADEMOVEOBJ	=	4008,
	PROTOCOL_TRADECONFIRM	=	4012,
	PROTOCOL_TRADEDISCARD	=	4014,
	PROTOCOL_SWITCHSERVERCANCEL	=	4102,
	PROTOCOL_GMRESTARTSERVER	=	358,
	PROTOCOL_GMONLINENUM	=	350,
	PROTOCOL_GMLISTONLINEUSER	=	352,
	PROTOCOL_GMKICKOUTUSER	=	354,
	PROTOCOL_ACKICKOUTUSER	=	5035,
	PROTOCOL_GMFORBIDSELLPOINT	=	378,
	PROTOCOL_GMKICKOUTROLE	=	360,
	PROTOCOL_GMSHUTUP	=	356,
	PROTOCOL_GMSHUTUPROLE	=	362,
	PROTOCOL_GMTOGGLECHAT	=	364,
	PROTOCOL_GMFORBIDROLE	=	366,
	PROTOCOL_GMPRIVILEGECHANGE	=	122,
	PROTOCOL_REPORT2GM	=	368,
	PROTOCOL_COMPLAIN2GM	=	370,
	PROTOCOL_ANNOUNCELINKTYPE	=	372,
	PROTOCOL_SETMAXONLINENUM	=	373,
	PROTOCOL_GMSETTIMELESSAUTOLOCK	=	385,
	PROTOCOL_IWEBAUTOLOCKGET	=	387,
	PROTOCOL_IWEBAUTOLOCKSET	=	389,
	PROTOCOL_ACREPORT	=	5001,
	PROTOCOL_ACANSWER	=	5032,
	PROTOCOL_ACPROTOSTAT	=	5024,
	PROTOCOL_REPORTIP	=	35,
	PROTOCOL_CHECKNEWMAIL	=	4200,
	PROTOCOL_SYSSENDMAIL	=	4214,
	PROTOCOL_SYSRECOVEREDOBJMAIL	=	4219,
	PROTOCOL_GETSELLLIST	=	603,
	PROTOCOL_FINDSELLPOINTINFO	=	619,
	PROTOCOL_SELLCANCEL	=	605,
	PROTOCOL_BATTLEGETMAP	=	850,
	PROTOCOL_BATTLESTATUS	=	866,
	PROTOCOL_COUNTRYBATTLEMOVE	=	4760,
	PROTOCOL_COUNTRYBATTLEGETMAP	=	4767,
	PROTOCOL_COUNTRYBATTLEGETPLAYERLOCATION	=	4770,
	PROTOCOL_COUNTRYBATTLEGETCONFIG	=	4772,
	PROTOCOL_COUNTRYBATTLEGETSCORE	=	4774,
	PROTOCOL_COUNTRYBATTLEPREENTER	=	4777,
	PROTOCOL_COUNTRYBATTLERETURNCAPITAL	=	4779,
	PROTOCOL_COUNTRYBATTLEKINGASSIGNASSAULT	=	4781,
	PROTOCOL_COUNTRYBATTLEKINGRESETBATTLELIMIT	=	4783,
	PROTOCOL_COUNTRYBATTLEGETBATTLELIMIT	=	4784,
	PROTOCOL_COUNTRYBATTLEGETKINGCOMMANDPOINT	=	4786,
	PROTOCOL_GETCNETSERVERCONFIG	=	4788,
	PROTOCOL_CASHLOCK	=	4260,
	PROTOCOL_CASHPASSWORDSET	=	4263,
	PROTOCOL_MATRIXFAILURE	=	553,
	PROTOCOL_AUTOLOCKSET	=	782,
	PROTOCOL_REFWITHDRAWBONUS	=	4906,
	PROTOCOL_REFLISTREFERRALS	=	4904,
	PROTOCOL_REFGETREFERENCECODE	=	4908,
	PROTOCOL_EXCHANGECONSUMEPOINTS	=	4954,
	PROTOCOL_GETREWARDLIST	=	4952,
	PROTOCOL_WEBTRADEROLEPREPOST	=	4326,
	PROTOCOL_WEBTRADEROLEPRECANCELPOST	=	4328,
	PROTOCOL_WEBTRADEROLEGETDETAIL	=	4329,
	PROTOCOL_USERCOUPON	=	138,
	PROTOCOL_USERCOUPONEXCHANGE	=	140,
	PROTOCOL_USERADDCASH	=	144,
	PROTOCOL_SSOGETTICKET	=	147,
	PROTOCOL_QPGETACTIVATEDSERVICES	=	150,
	PROTOCOL_QPADDCASH	=	152,
	PROTOCOL_REPORTCHAT	=	156,
	PROTOCOL_PLAYERACCUSE	=	161,
	PROTOCOL_ANNOUNCELINKVERSION	=	1130,
	PROTOCOL_PSHOPPLAYERGET	=	924,
	PROTOCOL_PSHOPLIST	=	926,
	PROTOCOL_PSHOPLISTITEM	=	928,
	PROTOCOL_PLAYERPROFILEGETPROFILEDATA	=	951,
	PROTOCOL_PLAYERPROFILESETPROFILEDATA	=	953,
	PROTOCOL_PLAYERPROFILEGETMATCHRESULT	=	954,
	PROTOCOL_TANKBATTLEPLAYERGETRANK	=	4881,
	PROTOCOL_FACTIONRESOURCEBATTLEGETMAP	=	4434,
	PROTOCOL_FACTIONRESOURCEBATTLEGETRECORD	=	4436,
	PROTOCOL_MNGETPLAYERLASTENTERINFO	=	5242,
	PROTOCOL_MNGETFACTIONBRIEFINFO	=	5244,
	PROTOCOL_MNGETFACTIONINFO	=	5248,
	PROTOCOL_KEYEXCHANGE	=	2,
	PROTOCOL_KICKOUTUSER	=	10,
	PROTOCOL_ACCOUNTINGRESPONSE	=	504,
	PROTOCOL_QUERYUSERPRIVILEGE_RE	=	507,
	PROTOCOL_QUERYUSERFORBID_RE	=	509,
	PROTOCOL_UPDATEREMAINTIME	=	36,
	PROTOCOL_TRANSBUYPOINT_RE	=	617,
	PROTOCOL_ADDCASH	=	515,
	PROTOCOL_ADDCASH_RE	=	516,
	PROTOCOL_SYSSENDMAIL3	=	8068,
	PROTOCOL_ADDICTIONCONTROL	=	556,
	PROTOCOL_BILLINGREQUEST	=	9001,
	PROTOCOL_BILLINGBALANCE	=	9000,
	PROTOCOL_BILLINGBALANCESA_RE	=	8049,
	PROTOCOL_BILLINGCONFIRM_RE	=	8053,
	PROTOCOL_NETBARANNOUNCE	=	529,
	PROTOCOL_AUTHDVERSION	=	8010,
	PROTOCOL_SSOGETTICKETREP	=	8017,
	PROTOCOL_AU2GAME	=	8038,
	PROTOCOL_DISCOUNTANNOUNCE	=	8064,
	PROTOCOL_ANNOUNCEPROVIDERID	=	73,
	PROTOCOL_PLAYERLOGIN_RE	=	66,
	PROTOCOL_PLAYERKICKOUT_RE	=	64,
	PROTOCOL_PLAYERLOGOUT	=	69,
	PROTOCOL_PLAYEROFFLINE_RE	=	68,
	PROTOCOL_QUERYPLAYERSTATUS	=	110,
	PROTOCOL_GETTASKDATA	=	111,
	PROTOCOL_SETTASKDATA	=	113,
	PROTOCOL_PLAYERINFOUPDATE	=	4460,
	PROTOCOL_PLAYERTEAMOP	=	4461,
	PROTOCOL_PLAYERTEAMMEMBEROP	=	4462,
	PROTOCOL_GTRADESTART_RE	=	4018,
	PROTOCOL_GTRADEDISCARD	=	4020,
	PROTOCOL_KEEPALIVE	=	90,
	PROTOCOL_DISCONNECTPLAYER	=	106,
	PROTOCOL_SWITCHSERVERSTART	=	4101,
	PROTOCOL_SWITCHSERVERSUCCESS	=	4103,
	PROTOCOL_SWITCHSERVERTIMEOUT	=	4104,
	PROTOCOL_SETCHATEMOTION	=	134,
	PROTOCOL_FACEMODIFY	=	125,
	PROTOCOL_FACEMODIFYCANCEL	=	126,
	PROTOCOL_GMRESTARTSERVER_RE	=	359,
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
	PROTOCOL_QUERYGAMESERVERATTR	=	512,
	PROTOCOL_ACREPORTCHEATER	=	5029,
	PROTOCOL_ACTRIGGERQUESTION	=	5030,
	PROTOCOL_BATTLECHALLENGE	=	852,
	PROTOCOL_BATTLECHALLENGEMAP	=	854,
	PROTOCOL_BATTLEENTER	=	860,
	PROTOCOL_BATTLESERVERREGISTER	=	856,
	PROTOCOL_DEBUGCOMMAND	=	873,
	PROTOCOL_STOCKCOMMISSION	=	401,
	PROTOCOL_STOCKACCOUNT	=	407,
	PROTOCOL_STOCKTRANSACTION	=	402,
	PROTOCOL_STOCKBILL	=	405,
	PROTOCOL_STOCKCANCEL	=	411,
	PROTOCOL_BILLINGBALANCESA	=	8048,
	PROTOCOL_BILLINGCONFIRM	=	8052,
	PROTOCOL_BILLINGCANCEL	=	8054,
	PROTOCOL_SENDREFCASHUSED	=	4902,
	PROTOCOL_SENDTASKREWARD	=	4962,
	PROTOCOL_REFGETREFERENCECODE_RE	=	4909,
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
	PROTOCOL_SYNMUTADATA	=	146,
	PROTOCOL_COUNTRYBATTLEAPPLY	=	4751,
	PROTOCOL_COUNTRYBATTLEJOINNOTICE	=	4753,
	PROTOCOL_COUNTRYBATTLELEAVENOTICE	=	4754,
	PROTOCOL_COUNTRYBATTLEONLINENOTICE	=	4755,
	PROTOCOL_COUNTRYBATTLEOFFLINENOTICE	=	4756,
	PROTOCOL_COUNTRYBATTLEENTERMAPNOTICE	=	4757,
	PROTOCOL_COUNTRYBATTLESERVERREGISTER	=	4758,
	PROTOCOL_COUNTRYBATTLESTART_RE	=	4764,
	PROTOCOL_COUNTRYBATTLEEND	=	4766,
	PROTOCOL_PLAYERCHANGEDS_RE	=	1103,
	PROTOCOL_TRYCHANGEDS	=	1101,
	PROTOCOL_PLAYERRENAME	=	3059,
	PROTOCOL_UPDATESOLOCHALLENGERANK	=	3074,
	PROTOCOL_GETSOLOCHALLENGERANK	=	3075,
	PROTOCOL_UPDATEENEMYLIST	=	238,
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
	PROTOCOL_FACTIONRESOURCEBATTLEREQUESTCONFIG_RE	=	4426,
	PROTOCOL_FACTIONRESOURCEBATTLESERVERREGISTER	=	4427,
	PROTOCOL_FACTIONRESOURCEBATTLEEVENTNOTICE	=	4429,
	PROTOCOL_FACTIONRESOURCEBATTLEPLAYERQUERY	=	4430,
	PROTOCOL_MNFACTIONBATTLEAPPLY	=	5211,
	PROTOCOL_MNBATTLESERVERREGISTER	=	5221,
	PROTOCOL_MNDOMAINBATTLESTART_RE	=	5223,
	PROTOCOL_MNDOMAINBATTLEENTER	=	5224,
	PROTOCOL_MNDOMAINBATTLEENTERSUCCESSNOTICE	=	5226,
	PROTOCOL_MNDOMAINBATTLELEAVENOTICE	=	5227,
	PROTOCOL_MNDOMAINBATTLEEND	=	5228,
	PROTOCOL_MNGETTOPLIST	=	5246,
	PROTOCOL_MNGETDOMAINDATA	=	5240,
	PROTOCOL_DELROLEANNOUNCE	=	3029,
	PROTOCOL_DBFRIENDEXTLIST_RE	=	232,
	PROTOCOL_TRANSBUYPOINT	=	616,
	PROTOCOL_SYNCSELLINFO	=	609,
	PROTOCOL_DEBUGADDCASH	=	521,
	PROTOCOL_ANNOUNCEZONEGROUP	=	1132,
	PROTOCOL_MNFACTIONINFOUPDATE	=	5205,
	PROTOCOL_DELFACTIONANNOUNCE	=	4818,
	PROTOCOL_CREATEFACTIONFORTRESS	=	4406,
	PROTOCOL_ACREMOTECODE	=	5003,
	PROTOCOL_ACQUESTION	=	5031,
	PROTOCOL_ACACCUSERE	=	5056,
	PROTOCOL_POST_RE	=	8021,
	PROTOCOL_GAMEPOSTCANCEL_RE	=	8023,
	PROTOCOL_WEBPOSTCANCEL	=	8024,
	PROTOCOL_SHELF	=	8026,
	PROTOCOL_SHELFCANCEL	=	8028,
	PROTOCOL_SOLD	=	8030,
	PROTOCOL_POSTEXPIRE	=	8032,
	PROTOCOL_WEBGETROLELIST	=	8034,
	PROTOCOL_LOADEXCHANGE	=	1109,
	PROTOCOL_DSANNOUNCEIDENTITY	=	1116,
	PROTOCOL_SENDDATAANDIDENTITY	=	1110,
	PROTOCOL_SENDDATAANDIDENTITY_RE	=	1115,
	PROTOCOL_REMOTELOGINQUERY	=	1117,
	PROTOCOL_REMOTELOGOUT	=	1119,
	PROTOCOL_KICKOUTREMOTEUSER_RE	=	1121,
	PROTOCOL_GETREMOTEROLEINFO_RE	=	1123,
	PROTOCOL_GETREMOTECNETSERVERCONFIG_RE	=	4791,
	PROTOCOL_CROSSGUARDNOTIFY	=	4793,
	PROTOCOL_MNFACTIONCACHEGET_RE	=	5203,
	PROTOCOL_MNFETCHFILTRATERESULT_RE	=	5219,
	PROTOCOL_MNFACTIONPROCLAIM_RE	=	5215,
	PROTOCOL_MNDOMAINSENDBONUSDATA	=	5231,
	PROTOCOL_MNFETCHTOPLIST_RE	=	5236,
	PROTOCOL_REMOTELOGINQUERY_RE	=	1118,
	PROTOCOL_KICKOUTREMOTEUSER	=	1120,
	PROTOCOL_GETREMOTEROLEINFO	=	1122,
	PROTOCOL_GETREMOTECNETSERVERCONFIG	=	4790,
	PROTOCOL_MNFACTIONCACHEGET	=	5202,
	PROTOCOL_MNFACTIONPROCLAIM	=	5214,
	PROTOCOL_MNFETCHFILTRATERESULT	=	5218,
	PROTOCOL_MNDOMAINSENDBONUSDATA_RE	=	5232,
	PROTOCOL_MNFETCHTOPLIST	=	5235,
};

};
#endif

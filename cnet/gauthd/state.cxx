#include "callid.hxx"

#ifdef WIN32
#include <winsock2.h>
#include "gnproto.h"
#include "gncompress.h"
#else
#include "protocol.h"
#include "binder.h"
#endif

namespace GNET
{

static GNET::Protocol::Type _state_GAuthServer[] = 
{
	PROTOCOL_KEYEXCHANGE,
	PROTOCOL_STATUSANNOUNCE,
	PROTOCOL_ACCOUNTINGREQUEST,
	PROTOCOL_ANNOUNCEZONEID,
	PROTOCOL_QUERYUSERPRIVILEGE,
	PROTOCOL_QUERYUSERFORBID,
	PROTOCOL_GMKICKOUTUSER,
	PROTOCOL_GMFORBIDSELLPOINT,
	PROTOCOL_GMSHUTUP,
	PROTOCOL_TRANSBUYPOINT,
	PROTOCOL_GETPLAYERIDBYNAME_RE,
	PROTOCOL_SYSSENDMAIL_RE,
	PROTOCOL_SYSSENDMAIL3_RE,
	PROTOCOL_VERIFYMASTER,
	PROTOCOL_VERIFYMASTER_RE,
	RPC_GQUERYPASSWD,
	RPC_USERLOGIN,
	RPC_USERLOGIN2,
	RPC_USERLOGOUT,
	RPC_CASHSERIAL,
	RPC_GETADDCASHSN,
	PROTOCOL_ADDCASH,
	PROTOCOL_ADDCASH_RE,
	RPC_MATRIXPASSWD,
	RPC_MATRIXPASSWD2,
	RPC_MATRIXTOKEN,
	PROTOCOL_MATRIXFAILURE,
	PROTOCOL_BILLINGREQUEST,
	PROTOCOL_BILLINGBALANCE,
	PROTOCOL_BILLINGBALANCESA,
	PROTOCOL_BILLINGCONFIRM,
	PROTOCOL_BILLINGCANCEL,
	PROTOCOL_ACFORBIDCHEATER,
	RPC_GETUSERCOUPON,
	RPC_COUPONEXCHANGE,
	PROTOCOL_GAME2AU,
	PROTOCOL_SSOGETTICKETREQ,
};

GNET::Protocol::Manager::Session::State state_GAuthServer(_state_GAuthServer,
						sizeof(_state_GAuthServer)/sizeof(GNET::Protocol::Type), 86400);


};


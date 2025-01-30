#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <dirent.h>

#include <set>
#include <map>
#include <vector>
#include <fstream>

#include "macros.h"
#include "accessdb.h"

#include "stocklog"
#include "user"
#include "gauctionitem"
#include "gauctiondetail"
#include "gterritorydetail"
#include "gterritorystore"
#include "gshoplog"
#include "gsyslog"
#include "gsysauctioncash"
#include "grolebase"
#include "groleequipment"
#include "gfactioninfo"
#include "gfriendlist"
#include "grolepocket"
#include "gmailbox"
#include "message"
#include "grolestatus"
#include "grolestorehouse"
#include "groletask"
#include "guserfaction"
#include "stockorder"
#include "gsyslog"
#include "guserstorehouse"
#include "gwebtradedetail"
#include "gserverdata"
#include "roleimportbean"
#include "groupbean"
#include "friendimportbean"
#include "titlebean"
#include "factionimportbean"
#include "factionid"
#include "gfactioninfo"
#include "gloginrecord"
#include "crossinfodata"
#include "gglobalcontroldata"
#include "greincarnationdata"
#include "localmacro.h"
#include "xmlcoder.h"
#include "gametalkdefs.h"
#include "utilfunction.h"
#include "storagetool.h"
#include "db.h"

#include "xmldecoder.h"
#include "grolestatusextraprop"
#include "mnfactioninfo"
#include "mnfactionapplyinfo"
#include "gfactionextenddata"
#include "gfactionextend"
#include "guniqueinfo"
#include "solochallengerankdataext"


namespace GNET
{

class PrintLogicuidQuery : public StorageEnv::IQuery
{
public:
	bool Update( StorageEnv::Transaction& txn, Octets& key, Octets& value)
	{
		Marshal::OctetsStream key_userid, value_user;
		key_userid = key;
		value_user = value;

		static int count = 0;
		count ++;

		unsigned int userid = 0;
		User	user;
		try
		{
			key_userid >> userid;
			value_user >> user;
			int logicuid = user.logicuid;
			if( user.logicuid && (user.logicuid!=userid || (user.rolelist&0xFFFF)))
			{
				// LOGICUID启用前没创建过角色的帐号logicuid==userid，但这种帐号在唯一名上不会有记录
				// 这个id有可能被唯一名分配给其他帐号
				printf( "%d,%d\n", userid, logicuid );
			}
		}
		catch( ... )
		{
			printf( "PrintLogicuidQuery, error marshal, userid=%d\n", userid );
		}

		return true;
	}
};

void PrintLogicuid( )
{
	printf("#userid");
	printf(",logicuid");
	printf("\n");

	PrintLogicuidQuery q;
	try
	{
		StorageEnv::Storage * pstorage = StorageEnv::GetStorage( "user" );
		StorageEnv::AtomTransaction	txn;
		try
		{
			StorageEnv::Storage::Cursor cursor = pstorage->cursor( txn );
			cursor.walk( q );
		}
		catch ( DbException e ) { throw; }
		catch ( ... )
		{
			DbException ee( DB_OLD_VERSION );
			txn.abort( ee );
			throw ee;
		}
	}
	catch ( DbException e )
	{
		Log::log( LOG_ERR, "PrintLogicuid, error when walk, what=%s\n", e.what() );
	}
}

class PrintUnameroleQuery : public StorageEnv::IQuery
{
public:
	int zoneid;
	bool Update( StorageEnv::Transaction& txn, Octets& key, Octets& value)
	{
		Marshal::OctetsStream key_roleid, value_base;
		key_roleid = key;
		value_base = value;

		static int count = 0;
		count ++;

		int roleid = -1;
		GRoleBase	base;
		try
		{
			key_roleid >> roleid;
			value_base >> base;
			Octets  namegbk, nameesc;
			CharsetConverter::conv_charset_u2l( base.name, namegbk );
			EscapeCSVString( namegbk, nameesc );
			printf( "%.*s,%d,%d,2,0\n", nameesc.size(), (char*)nameesc.begin(), zoneid, roleid );
		}
		catch( ... )
		{
			printf( "PrintUnameroleQuery, error marshal, roleid=%d\n", roleid );
		}

		return true;
	}
};

void PrintUnamerole( int zoneid )
{
	printf("#name");
	printf(",zoneid");
	printf(",roleid");
	printf(",status");
	printf(",time");
	printf("\n");

	PrintUnameroleQuery q;
	q.zoneid = zoneid;
	try
	{
		StorageEnv::Storage * pstorage = StorageEnv::GetStorage( "base" );
		StorageEnv::AtomTransaction	txn;
		try
		{
			StorageEnv::Storage::Cursor cursor = pstorage->cursor( txn );
			cursor.walk( q );
		}
		catch ( DbException e ) { throw; }
		catch ( ... )
		{
			DbException ee( DB_OLD_VERSION );
			txn.abort( ee );
			throw ee;
		}
	}
	catch ( DbException e )
	{
		Log::log( LOG_ERR, "PrintUnamerole, error when walk, what=%s\n", e.what() );
	}
}

class PrintUnamefactionQuery : public StorageEnv::IQuery
{
public:
	int zoneid;
	bool Update( StorageEnv::Transaction& txn, Octets& key, Octets& value)
	{
		Marshal::OctetsStream key_fname, value_fid;
		key_fname = key;
		value_fid = value;

		static int count = 0;
		count ++;

		int fid = -1;
		Octets	fname;
		try
		{
			key_fname >> fname;
			value_fid >> fid;
			Octets  namegbk, nameesc;
			CharsetConverter::conv_charset_u2l( fname, namegbk );
			EscapeCSVString( namegbk, nameesc );
			printf( "%.*s,%d,%d,2,0\n", nameesc.size(), (char*)nameesc.begin(), zoneid, fid );
		}
		catch( ... )
		{
			printf( "PrintUnamefactionQuery, error marshal, fid=%d\n", fid );
		}

		return true;
	}
};

void PrintUnamefaction( int zoneid )
{
	printf("#name");
	printf(",zoneid");
	printf(",factionid");
	printf(",status");
	printf(",time");
	printf("\n");

	PrintUnamefactionQuery q;
	q.zoneid = zoneid;
	try
	{
		StorageEnv::Storage * pstorage = StorageEnv::GetStorage( "factionname" );
		StorageEnv::AtomTransaction	txn;
		try
		{
			StorageEnv::Storage::Cursor cursor = pstorage->cursor( txn );
			cursor.walk( q );
		}
		catch ( DbException e ) { throw; }
		catch ( ... )
		{
			DbException ee( DB_OLD_VERSION );
			txn.abort( ee );
			throw ee;
		}
	}
	catch ( DbException e )
	{
		Log::log( LOG_ERR, "PrintUnamefaction, error when walk, what=%s\n", e.what() );
	}
}

class GenNameIdxQuery : public StorageEnv::IQuery
{
public:
	bool Update( StorageEnv::Transaction& txn, Octets& key, Octets& value)
	{
		Marshal::OctetsStream key_os, value_os;
		key_os = key;
		value_os = value;

		static int count = 0;
		if( ! (count++ % 100000) ) printf( "generating rolename records counter %d...\n", count );

		RoleId		id;
		try
		{
			StorageEnv::Storage * prolename = StorageEnv::GetStorage("rolename");
			try
			{
				GRoleBase	base;

				key_os >> id;
				value_os >> base;

				prolename->insert( base.name, key_os, txn );
			}
			catch ( DbException e ) { throw; }
			catch ( ... )
			{
				DbException ee( DB_OLD_VERSION );
				txn.abort( ee );
				throw ee;
			}
		}
		catch ( DbException e )
		{
			Log::log( LOG_ERR, "GenNameIdxQuery, roleid=%d, what=%s\n", id.id, e.what() );
		}
		return true;
	}
};

void GenNameIdx()
{
	GenNameIdxQuery q;

	try
	{
		StorageEnv::Storage * pstorage = StorageEnv::GetStorage( "base" );
		StorageEnv::AtomTransaction	txn;
		try
		{
			StorageEnv::Storage::Cursor cursor = pstorage->cursor( txn );
			cursor.walk( q );
		}
		catch ( DbException e ) { throw; }
		catch ( ... )
		{
			DbException ee( DB_OLD_VERSION );
			txn.abort( ee );
			throw ee;
		}
	}
	catch ( DbException e )
	{
		Log::log( LOG_ERR, "GenNameIdx, error when walk, what=%s\n", e.what() );
	}

	StorageEnv::checkpoint();
}

class ExportUniqueNamefactionQuery : public StorageEnv::IQuery
{
public:
	int zoneid;
	DBStandalone * pstandalone;
	int max_factionid;

	bool Update( StorageEnv::Transaction& txn, Octets& key, Octets& value)
	{
		Marshal::OctetsStream key_os, value_os, value_unamefaction, key_nextid;
		key_os = key;
		value_os = value;

		int		id;
		Octets	name;
		try
		{
			key_os >> name;
			int nextid = 0;
			key_nextid << nextid;
			if( key_nextid.odata() == name )
				return true;

			value_os >> id;
			if (id > max_factionid)
				max_factionid = id;

			int status = 2;	// UNIQUENAME_USED;
			value_unamefaction << zoneid << id << status << (int)Timer::GetTime();
			StorageEnv::Compressor	* compressor = new StorageEnv::Compressor();
			Octets com_val = compressor->Update(value_unamefaction);
			pstandalone->put( name.begin(), name.size(), com_val.begin(), com_val.size() );
		} catch ( Marshal::Exception & ) {
			Log::log( LOG_ERR, "ExportUniqueNamefactionQuery, error unmarshal, fid=%d.", id );
			return true;
		}
		return true;
	}
};

class ExportUniqueIdroleQuery : public StorageEnv::IQuery
{
public:
	int zoneid;
	DBStandalone * pstandalone;

	bool Update( StorageEnv::Transaction& txn, Octets& key, Octets& value)
	{
		Marshal::OctetsStream key_os, value_os, value_uidrole;
		key_os = key;
		value_os = value;

		unsigned int id;
		User			user;
		try
		{
			key_os >> id;
			value_os >> user;
			if( 0 != user.logicuid && (user.logicuid!=id || (user.rolelist&0xFFFF)))
			{
				value_uidrole << user.rolelist << user.logicuid;
				StorageEnv::Compressor	* compressor = new StorageEnv::Compressor();
				Octets com_val = compressor->Update(value_uidrole);
				pstandalone->put( key.begin(), key.size(), com_val.begin(), com_val.size() );
			}
		} catch ( Marshal::Exception & ) {
			Log::log( LOG_ERR, "ExportUniqueUidroleQuery, error unmarshal, uid=%d.", id );
			return true;
		}
		return true;
	}
};

class ExportUniqueLogicuidQuery : public StorageEnv::IQuery
{
public:
	int zoneid;
	DBStandalone * pstandalone;
	int max_logicuid;

	bool Update( StorageEnv::Transaction& txn, Octets& key, Octets& value)
	{
		Marshal::OctetsStream key_os, value_os, key_logicuid, value_userid;
		key_os = key;
		value_os = value;

		unsigned int userid;
		int	logicuid;
		User	user;
		try
		{
			key_os >> userid;
			value_os >> user;
			logicuid = user.logicuid;
			if( 0 != logicuid && (user.logicuid!=userid || (user.rolelist&0xFFFF)))
			{
				key_logicuid << logicuid;
				value_userid << userid;

				StorageEnv::Compressor	* compressor = new StorageEnv::Compressor();
				Octets com_val = compressor->Update(value_userid);
				pstandalone->put( key_logicuid.begin(), key_logicuid.size(), com_val.begin(), com_val.size() );
			}
			if (logicuid == 0)
				logicuid = userid;
			if (logicuid > max_logicuid)
				max_logicuid = logicuid;
		} catch ( Marshal::Exception & ) {
			Log::log( LOG_ERR, "ExportUniqueLogicuidQuery, error unmarshal, uid=%d.", userid );
			return true;
		}
		return true;
	}
};

class ExportUniqueNameroleQuery : public StorageEnv::IQuery
{
public:
	int zoneid;
	DBStandalone * pstandalone;

	bool Update( StorageEnv::Transaction& txn, Octets& key, Octets& value)
	{
		Marshal::OctetsStream key_os, value_os, value_unamerole;
		value_os = value;

		int		id;
		Octets	name;
		try
		{
			name = key;
			value_os >> id;

			int status = 2;	// UNIQUENAME_USED;
			value_unamerole << zoneid << id << status << (int)Timer::GetTime();
			StorageEnv::Compressor	* compressor = new StorageEnv::Compressor();
			Octets com_val = compressor->Update(value_unamerole);
			pstandalone->put( name.begin(), name.size(), com_val.begin(), com_val.size() );
		} catch ( Marshal::Exception & ) {
			Log::log( LOG_ERR, "ExportUniqueNameroleQuery, error unmarshal, fid=%d.", id );
			return true;
		}
		return true;
	}
};

bool SetLogicuidNextid(DBStandalone * pstandalone, int nextid )
{                       
	StorageEnv::Uncompressor * uncompressor = new StorageEnv::Uncompressor();
	StorageEnv::Compressor * compressor = new StorageEnv::Compressor();
	try
	{
		Marshal::OctetsStream value_logicuid, value_nextid;
		int temp = 0;
		Marshal::OctetsStream t;
		t << temp;
		unsigned int val_len;
		if ( void *val = pstandalone->find( t.begin(), (unsigned int)t.size(), &val_len ) )
		{       
			GNET::Octets dbval = uncompressor->Update(GNET::Octets(val, val_len));
			free(val);
			int logicuid;
			Marshal::OctetsStream(dbval) >> logicuid;
			printf( "old nextid record: logicuid = %d\n", logicuid );
		}
		else
		{
			printf( "old nextid not found.\n" );
		}
		value_nextid << nextid;
		Octets com_val = compressor->Update(value_nextid);
		pstandalone->put( t.begin(), t.size(), com_val.begin(), com_val.size() );
	}
	catch ( Marshal::Exception &)
	{
		Log::log( LOG_ERR, "SetLogicuidNextid, unmarshall error");
		delete uncompressor;
		delete compressor;
		return false;
	}
	delete uncompressor;
	delete compressor;
	return true;
}

bool SetFactionNextid(DBStandalone * pstandalone, int nextid )
{
	StorageEnv::Uncompressor * uncompressor = new StorageEnv::Uncompressor();
	StorageEnv::Compressor * compressor = new StorageEnv::Compressor();
	try
	{
		Marshal::OctetsStream  value_nextid;
		int temp = 0;
		Marshal::OctetsStream t;
		t << temp;

		unsigned int val_len;
		if ( void *val = pstandalone->find( t.begin(), (unsigned int)t.size(), &val_len ) )
		{       
			GNET::Octets dbval = uncompressor->Update(GNET::Octets(val, val_len));
			free(val);
			int zoneid, factionid, status, time;
			Marshal::OctetsStream(dbval) >> zoneid >> factionid >> status >> time;
			printf( "old nextid record: zoneid = %d, factionid = %d, status=%d, time=%d\n",	zoneid, factionid, status, time );
		}
		else
		{
			printf( "old nextid not found.\n" );
		}
		int status = 2;//UNIQUENAME_USED
		value_nextid << temp << nextid << status << temp;
		Octets com_val = compressor->Update(value_nextid);
		pstandalone->put( t.begin(), t.size(), com_val.begin(), com_val.size() );
		//			pstandalone->insert( t, value_nextid, txn );
	}
	catch (Marshal::Exception &)
	{
		Log::log(LOG_ERR, "Set nextid, error unmarshal");
		delete uncompressor;
		delete compressor;
		return false;
	}
	delete uncompressor;
	delete compressor;
	return true;
}

void ExportUnique( int zoneid )
{
	{
		printf( "export unique unamefaction.\n" );
		ExportUniqueNamefactionQuery q;
		q.zoneid = zoneid;
		q.max_factionid = 0;
		q.pstandalone = new DBStandalone( "unamefaction" );
		q.pstandalone->init();
		try
		{
			StorageEnv::Storage * pstorage = StorageEnv::GetStorage( "factionname" );
			StorageEnv::AtomTransaction	txn;
			try
			{
				StorageEnv::Storage::Cursor cursor = pstorage->cursor( txn );
				cursor.walk( q );
			}
			catch ( DbException e ) { throw; }
			catch ( ... )
			{
				DbException ee( DB_OLD_VERSION );
				txn.abort( ee );
				throw ee;
			}
		}
		catch ( DbException e )
		{
			Log::log( LOG_ERR, "ExportUnique unamefaction, error when walk, what=%s\n", e.what() );
		}
		int next_factionid = q.max_factionid + 1;
		printf( "Set next factionid to be allocated %d\n",next_factionid );
		SetFactionNextid(q.pstandalone, next_factionid );
		q.pstandalone->checkpoint();
		delete q.pstandalone;
		q.pstandalone = NULL;
	}

	{
		printf( "export unique uidrole.\n" );
		ExportUniqueIdroleQuery q;
		q.zoneid = zoneid;
		q.pstandalone = new DBStandalone( "uidrole" );
		q.pstandalone->init();
		try
		{
			StorageEnv::Storage * pstorage = StorageEnv::GetStorage( "user" );
			StorageEnv::AtomTransaction	txn;
			try
			{
				StorageEnv::Storage::Cursor cursor = pstorage->cursor( txn );
				cursor.walk( q );
			}
			catch ( DbException e ) { throw; }
			catch ( ... )
			{
				DbException ee( DB_OLD_VERSION );
				txn.abort( ee );
				throw ee;
			}
		}
		catch ( DbException e )
		{
			Log::log( LOG_ERR, "ExportUnique uidrole, error when walk, what=%s\n", e.what() );
		}
		q.pstandalone->checkpoint();
		delete q.pstandalone;
		q.pstandalone = NULL;
	}

	{
		printf( "export unique logicuid.\n" );
		ExportUniqueLogicuidQuery q;
		q.zoneid = zoneid;
		q.max_logicuid = 0;
		q.pstandalone = new DBStandalone( "logicuid" );
		q.pstandalone->init();
		try
		{
			StorageEnv::Storage * pstorage = StorageEnv::GetStorage( "user" );
			StorageEnv::AtomTransaction	txn;
			try
			{
				StorageEnv::Storage::Cursor cursor = pstorage->cursor( txn );
				cursor.walk( q );
			}
			catch ( DbException e ) { throw; }
			catch ( ... )
			{
				DbException ee( DB_OLD_VERSION );
				txn.abort( ee );
				throw ee;
			}
		}
		catch ( DbException e )
		{
			Log::log( LOG_ERR, "ExportUnique logicuid, error when walk, what=%s\n", e.what() );
		}
		int next_logicuid = q.max_logicuid + 16;
		printf( "Set next logicuid to be allocated %d\n",next_logicuid);
		SetLogicuidNextid(q.pstandalone, next_logicuid);
		q.pstandalone->checkpoint();
		delete q.pstandalone;
		q.pstandalone = NULL;
	}

	{
		printf( "export unique unamerole.\n" );
		ExportUniqueNameroleQuery q;
		q.zoneid = zoneid;
		q.pstandalone = new DBStandalone( "unamerole" );
		q.pstandalone->init();
		try
		{
			StorageEnv::Storage * pstorage = StorageEnv::GetStorage( "rolename" );
			StorageEnv::AtomTransaction	txn;
			try
			{
				StorageEnv::Storage::Cursor cursor = pstorage->cursor( txn );
				cursor.walk( q );
			}
			catch ( DbException e ) { throw; }
			catch ( ... )
			{
				DbException ee( DB_OLD_VERSION );
				txn.abort( ee );
				throw ee;
			}
		}
		catch ( DbException e )
		{
			Log::log( LOG_ERR, "ExportUnique unamerole, error when walk, what=%s\n", e.what() );
		}
		q.pstandalone->checkpoint();
		delete q.pstandalone;
		q.pstandalone = NULL;
	}
}

void DisplayRoleInfo(GRoleBase& base, GRoleDetail& role)
{
	Octets name;
	CharsetConverter::conv_charset_u2l( base.name, name );

	time_t dt = base.delete_time, ct = base.create_time, lt = base.lastlogin_time;

	printf("\nBase:\n");
	printf("\tID:%d\n",role.id);
	printf("\tuserid:%d\n",0==role.userid ? (role.id & ~0x0000000F) : role.userid);
	printf("\tname:%.*s\n",name.size(),(char*)name.begin());
	printf("\trace:%d\n",base.race);
	printf("\toccupation:%d\n",base.cls);
	printf("\tgender:%d\n",base.gender);
	printf("\tcustom_data.size:%d\n",role.custom_data.size());
	printf("\tcustom_stamp:%d\n",role.custom_stamp);
	printf("\tstatus:%d\n",base.status);
	printf("\tdelete_time:%s",dt>0 ? ctime(&dt) : "-\n");
	printf("\tcreate_time:%s",ct>0 ? ctime(&ct) : "-\n");
	printf("\tlastlogin_time:%s",lt>0 ? ctime(&lt) : "-\n");
	printf("\tforbid.size:%d",base.forbid.size());

	printf("\nStatus:\n");
	printf("\tlevel:%d\n",role.status.level);
	printf("\tlevel2:%d\n",role.status.level2);
	printf("\texp:%d\n",role.status.exp);
	printf("\tsp:%d\n",role.status.sp);
	printf("\tpp:%d\n",role.status.pp);
	printf("\thp:%d\n",role.status.hp);
	printf("\tmp:%d\n",role.status.mp);
	printf("\tposx:%4.1f\n",role.status.posx);
	printf("\tposy:%4.1f\n",role.status.posy);
	printf("\tposz:%4.1f\n",role.status.posz);
	printf("\tworldtag:%d\n",role.status.worldtag);
	printf("\tmoney:%d\n",role.inventory.money);
	printf("\tinvader_state:%d\n",role.status.invader_state);
	printf("\tinvader_time:%d\n",role.status.invader_time);
	printf("\tpariah_time:%d\n",role.status.pariah_time);
	printf("\tfactionid:%d\n",role.factionid);
	printf("\tfactionrole:%d\n",role.factionrole);
	printf("\treputation:%d\n",role.status.reputation);
	printf("\tcustom_status.size:%d\n",role.status.custom_status.size());
	printf("\tfilter_data.size:%d\n",role.status.filter_data.size());
	printf("\tcharactermode.size:%d\n",role.status.charactermode.size());
	printf("\tinstancekeylist.size:%d\n",role.status.instancekeylist.size());
	printf("\tdbltime_expire:%d\n",role.status.dbltime_expire);
	printf("\tdbltime_mode:%d\n",role.status.dbltime_mode);
	printf("\tdbltime_begin:%d\n",role.status.dbltime_begin);
	printf("\tdbltime_used:%d\n",role.status.dbltime_used);
	printf("\tdbltime_max:%d\n",role.status.dbltime_max);
	printf("\ttime_used:%d\n",role.status.time_used);
	printf("\ttimestamp:%d\n",role.inventory.timestamp);
	printf("\tstoresize:%d\n",role.storehouse.capacity);
	printf("\tpetcorral.size:%d\n",role.status.petcorral.size());
	printf("\n");
	struct extend_prop property;
	memset( &property, 0, sizeof(property) );
	memcpy(&property,role.status.property.begin(),std::min(sizeof(property),role.status.property.size()));
	printf("\tvitality:%d\n",property.vitality);
	printf("\tenergy:%d\n",property.energy);
	printf("\tstrength:%d\n",property.strength);
	printf("\tagility:%d\n",property.agility);
	printf("\tmax_hp:%d\n",property.max_hp);
	printf("\tmax_mp:%d\n",property.max_mp);
	printf("\thp_gen:%d\n",property.hp_gen);
	printf("\tmp_gen:%d\n",property.mp_gen);
	printf("\twalk_speed:%4.1f\n",property.walk_speed);
	printf("\trun_speed:%4.1f\n",property.run_speed);
	printf("\tswim_speed:%4.1f\n",property.swim_speed);
	printf("\tflight_speed:%4.1f\n\n",property.flight_speed);
	printf("\tattack:%d\n",property.attack);
	printf("\tdamage_low:%d\n",property.damage_low);
	printf("\tdamage_high:%d\n",property.damage_high);
	printf("\tattack_speed:%d\n",property.attack_speed);
	printf("\tattack_range:%4.1f\n",property.attack_range);
	printf("\tdamage_low:%d %d %d %d %d\n", property.addon_damage[0].damage_low,
			property.addon_damage[1].damage_low, property.addon_damage[2].damage_low,
			property.addon_damage[3].damage_low,property.addon_damage[4].damage_low );
	printf("\tdamage_high:%d %d %d %d %d\n", property.addon_damage[0].damage_high,
			property.addon_damage[1].damage_high, property.addon_damage[2].damage_high,
			property.addon_damage[3].damage_high, property.addon_damage[4].damage_high );
	printf("\tdamage_magic_low:%d\n", property.damage_magic_low );
	printf("\tdamage_magic_high:%d\n", property.damage_magic_high );
	printf("\tresistance:%d %d %d %d %d\n", property.resistance[0], property.resistance[1],
			property.resistance[2], property.resistance[3], property.resistance[4] );
	printf("\tdefense:%d\n",property.defense);
	printf("\tarmor:%d\n",property.armor);
	printf("\tmax_ap:%d\n",property.max_ap);
	printf("\n");
	printf("\tvar_data.size:%d\n", role.status.var_data.size() );
	printf("\tskills.size:%d\n", role.status.skills.size() );
	printf("\tstorehousepasswd.size:%d\n", role.status.storehousepasswd.size() );
	printf("\twaypointlist.size:%d\n", role.status.waypointlist.size() );
	printf("\tcoolingtime.size:%d\n", role.status.coolingtime.size() );

	printf("\bStorehouse:\n");
	printf("\tstorehouse.money:%d\n", role.storehouse.money );
	printf("\tstorehouse.size:%d\n\n", role.storehouse.items.size() );

	printf("\nInventory:\n");
	printf("\tinventory.size:%d\n", role.inventory.items.size() );

	printf("\nEquipment:\n");
	printf("\tequipment.size:%d\n", role.equipment.size() );

	printf("\nTaskinventory:\n");
	printf("\ttaskinventory.size:%d\n", role.task.task_inventory.size() );

	printf("\nTaskdata:\n");
	printf("\ttask_data.size:%d\n", role.task.task_data.size() );

	printf("\nTaskcomplete:\n");
	printf("\ttask_complete.size:%d\n", role.task.task_complete.size() );

	printf("\n");
}

bool QueryRole( int roleid )
{
	printf( "QueryRole Begin\n" );
	GRoleDetail	detail;

	Marshal::OctetsStream key_userid, key, value_user, value_base, value_status;
	Marshal::OctetsStream value_inventory, value_equipment, value_task, value_storehouse;

	try
	{
		StorageEnv::Storage * puser = StorageEnv::GetStorage("user");
		StorageEnv::Storage * pbase = StorageEnv::GetStorage("base");
		StorageEnv::Storage * pstatus = StorageEnv::GetStorage("status");
		StorageEnv::Storage * pinventory = StorageEnv::GetStorage("inventory");
		StorageEnv::Storage * pequipment = StorageEnv::GetStorage("equipment");
		StorageEnv::Storage * pstorehouse = StorageEnv::GetStorage("storehouse");
		StorageEnv::Storage * ptask = StorageEnv::GetStorage("task");
		StorageEnv::AtomTransaction txn;
		try
		{

			GRoleBase	base;
			GRoleStatus	status;
			key << roleid;
			value_base = pbase->find( key, txn );
			value_base >> base;

			key_userid << (0==base.userid ? (base.id & ~0x0000000F) : base.userid);
			if( puser->find( key_userid, value_user, txn ) )
			{
				User	user;
				value_user >> user;
				printf( "userid = %d, user rolelist = %x.\n",
									0==base.userid ? (base.id & ~0x0000000F) : base.userid, user.rolelist );
			}
			else
			{
				printf( "no user found for userid %d.\n", 0==base.userid ? (base.id & ~0x0000000F) : base.userid );
			}

			if (base.status == _ROLE_STATUS_READYDEL)
			{
				if( Timer::GetTime()-base.delete_time > GameDBManager::GetInstance()->GetDeleteRole_Timeout() )
					base.status =  _ROLE_STATUS_MUSTDEL;
			}
			GRoleBaseToDetail( base, detail );
			printf( "QueryRole base, size=%d\n", value_base.size() );

			if( pstatus->find( key, value_status, txn ) )
				value_status >> detail.status;
			printf( "QueryRole status, size=%d\n", value_status.size() );

			if( pinventory->find( key, value_inventory, txn ) )
				value_inventory >> detail.inventory;
			printf( "QueryRole inventory, size=%d\n", value_inventory.size() );

			if( pequipment->find( key, value_equipment, txn ) )
				value_equipment >> detail.equipment;
			printf( "QueryRole equipment, size=%d\n", value_equipment.size() );

			if( ptask->find( key, value_task, txn ) )
				value_task>> detail.task;
			printf( "QueryRole taskinventory, size=%d\n", detail.task.task_inventory.size() );

			if( pstorehouse->find( key, value_storehouse, txn ) )
				value_storehouse >> detail.storehouse;
			printf( "QueryRole storehouse, size=%d\n", value_storehouse.size() );

			printf( "QueryRole task, size=%d\n", detail.task.task_data.size() );

			printf( "QueryRole taskcomplete, size=%d\n", detail.task.task_complete.size() );

			printf( "QueryRole display\n" );
			DisplayRoleInfo( base, detail );
			return true;
		}
		catch ( DbException e ) { throw; }
		catch ( ... )
		{
			DbException ee( DB_OLD_VERSION );
			txn.abort( ee );
			throw ee;
		}
	}
	catch ( DbException e )
	{
		Log::log( LOG_ERR, "QueryRole, roleid=%d, what=%s\n", roleid, e.what() );
	}
	return false;
}

void DisplayRoleInfoCsv(GRoleBase& base, GRoleDetail& role)
{
	Octets name, name2;
	CharsetConverter::conv_charset_u2l( base.name, name );
	EscapeCSVString( name, name2 );

	time_t dt = base.delete_time, ct = base.create_time, lt = base.lastlogin_time;

	printf("%d",role.id);
	printf(",%d", 0==base.userid ? (base.id & ~0x0000000F) : base.userid );
	printf(",%.*s",name2.size(),(char*)name2.begin());
	printf(",%d",base.race);
	printf(",%d",base.cls);
	printf(",%d",base.gender);
	printf(",%d",base.custom_data.size());
	printf(",%d",base.custom_stamp);
	printf(",%d",base.status);
	if( dt > 0 )
	{
		struct tm * l = localtime(&dt);
		printf(",%.4d-%.2d-%.2d %.2d:%.2d:%.2d",
				1900+l->tm_year,1+l->tm_mon,l->tm_mday,l->tm_hour,l->tm_min,l->tm_sec);
	}
	else
		printf(",");
	if( ct > 0 )
	{
		struct tm * l = localtime(&ct);
		printf(",%.4d-%.2d-%.2d %.2d:%.2d:%.2d",
				1900+l->tm_year,1+l->tm_mon,l->tm_mday,l->tm_hour,l->tm_min,l->tm_sec);
	}
	else
		printf(",");
	if( lt > 0 )
	{
		struct tm * l = localtime(&lt);
		printf(",%.4d-%.2d-%.2d %.2d:%.2d:%.2d",
				1900+l->tm_year,1+l->tm_mon,l->tm_mday,l->tm_hour,l->tm_min,l->tm_sec);
	}
	else
		printf(",");
	printf(",%d",base.forbid.size());

	printf(",%d",role.status.level);
	printf(",%d",role.status.level2);
	printf(",%d",role.status.exp);
	printf(",%d",role.status.sp);
	printf(",%d",role.status.pp);
	printf(",%d",role.status.hp);
	printf(",%d",role.status.mp);
	printf(",%4.1f",role.status.posx);
	printf(",%4.1f",role.status.posy);
	printf(",%4.1f",role.status.posz);
	printf(",%d",role.status.worldtag);
	printf(",%d",role.inventory.money);
	printf(",%d",role.status.invader_state);
	printf(",%d",role.status.invader_time);
	printf(",%d",role.status.pariah_time);
	printf(",%d",role.factionid);
	printf(",%d",role.factionrole);
	printf(",%d",role.status.reputation);
	printf(",%d",role.status.custom_status.size());
	printf(",%d",role.status.filter_data.size());
	printf(",%d",role.status.charactermode.size());
	printf(",%d",role.status.instancekeylist.size());
	printf(",%d",role.status.dbltime_expire);
	printf(",%d",role.status.dbltime_mode);
	printf(",%d",role.status.dbltime_begin);
	printf(",%d",role.status.dbltime_used);
	printf(",%d",role.status.dbltime_max);
	printf(",%d",role.status.time_used);
	printf(",%d",role.inventory.timestamp);
	printf(",%d",role.storehouse.capacity);
	printf(",%d",role.status.petcorral.size());
	struct extend_prop property;
	memset( &property, 0, sizeof(property) );
	memcpy(&property,role.status.property.begin(),std::min(sizeof(property),role.status.property.size()));
	printf(",%d",property.vitality);
	printf(",%d",property.energy);
	printf(",%d",property.strength);
	printf(",%d",property.agility);
	printf(",%d",property.max_hp);
	printf(",%d",property.max_mp);
	printf(",%d",property.hp_gen);
	printf(",%d",property.mp_gen);
	printf(",%4.1f",property.walk_speed);
	printf(",%4.1f",property.run_speed);
	printf(",%4.1f",property.swim_speed);
	printf(",%4.1f",property.flight_speed);
	printf(",%d",property.attack);
	printf(",%d",property.damage_low);
	printf(",%d",property.damage_high);
	printf(",%d",property.attack_speed);
	printf(",%4.1f",property.attack_range);
	printf(",%d,%d,%d,%d,%d", property.addon_damage[0].damage_low,
			property.addon_damage[1].damage_low, property.addon_damage[2].damage_low,
			property.addon_damage[3].damage_low,property.addon_damage[4].damage_low );
	printf(",%d,%d,%d,%d,%d", property.addon_damage[0].damage_high,
			property.addon_damage[1].damage_high, property.addon_damage[2].damage_high,
			property.addon_damage[3].damage_high, property.addon_damage[4].damage_high );
	printf(",%d", property.damage_magic_low );
	printf(",%d", property.damage_magic_high );
	printf(",%d,%d,%d,%d,%d", property.resistance[0], property.resistance[1],
			property.resistance[2], property.resistance[3], property.resistance[4] );
	printf(",%d",property.defense);
	printf(",%d",property.armor);
	printf(",%d",property.max_ap);
	printf(",%d", role.status.var_data.size() );
	printf(",%d", role.status.skills.size() );
	printf(",%d", role.status.storehousepasswd.size() );
	printf(",%d", role.status.waypointlist.size() );
	printf(",%d", role.status.coolingtime.size() );

	printf(",%d", role.storehouse.money );
	printf(",%d", role.storehouse.items.size() );

	printf(",%d", role.inventory.items.size() );

	printf(",%d", role.equipment.size() );

	printf(",%d", role.task.task_inventory.size() );

	printf(",%d", role.task.task_data.size() );

	printf(",%d", role.task.task_complete.size() );

	int reincarn_times = 0;
	int reincarn_maxlevel = role.status.level;

    GReincarnationData data; 
	GetRoleReincarnationDetail(role.status.reincarnation_data,reincarn_times,reincarn_maxlevel,data);
	printf(",%d", reincarn_times);
	printf(",%d", reincarn_maxlevel);
	int realm_level = 0;
	GetRoleRealmDetail(role.status.realm_data,realm_level);
	printf(",%d", realm_level);

	printf("\n");
}

bool QueryRoleCsv( int roleid, StorageEnv::Transaction& txn )
{
	GRoleDetail	detail;

	Marshal::OctetsStream key, value_base, value_status;
	Marshal::OctetsStream value_inventory, value_equipment, value_task, value_storehouse;

	try
	{
		StorageEnv::Storage * pbase = StorageEnv::GetStorage("base");
		StorageEnv::Storage * pstatus = StorageEnv::GetStorage("status");
		StorageEnv::Storage * pinventory = StorageEnv::GetStorage("inventory");
		StorageEnv::Storage * pequipment = StorageEnv::GetStorage("equipment");
		StorageEnv::Storage * pstorehouse = StorageEnv::GetStorage("storehouse");
		StorageEnv::Storage * ptask = StorageEnv::GetStorage("task");
		try
		{
			key << roleid;

			GRoleBase	base;
			GRoleStatus	status;

			value_base = pbase->find( key, txn );
			value_base >> base;
			if (base.status == _ROLE_STATUS_READYDEL)
			{
				if(Timer::GetTime()-base.delete_time>GameDBManager::GetInstance()->GetDeleteRole_Timeout())
					base.status =  _ROLE_STATUS_MUSTDEL;
			}
			GRoleBaseToDetail( base, detail );
    
			if( pstatus->find( key, value_status, txn ) )
			{
				value_status >> detail.status;
			}
    
			if( pinventory->find( key, value_inventory, txn ) )
				value_inventory >> detail.inventory;
    
			if( pequipment->find( key, value_equipment, txn ) )
				value_equipment >> detail.equipment;
    
			if( ptask->find( key, value_task, txn ) )
				value_task>> detail.task;

			if( pstorehouse->find( key, value_storehouse, txn ) )
				value_storehouse >> detail.storehouse;
    
			DisplayRoleInfoCsv(base, detail );
			return true;
		}
		catch ( DbException e ) { throw; }
		catch ( ... )
		{
			DbException ee( DB_OLD_VERSION );
			txn.abort( ee );
			throw ee;
		}
	}
	catch ( DbException e )
	{
		Log::log( LOG_ERR, "QueryRoleCsv, roleid=%d, what=%s\n", roleid, e.what() );
	}
	return false;
}

class ListRoleCsvQuery : public StorageEnv::IQuery
{
public:
	int count;
	bool hasmore;
	Octets handle;
	ListRoleCsvQuery():count(0),hasmore(false){}
	bool Update( StorageEnv::Transaction& txn, Octets& key, Octets& value)
	{
		if(++count % 10000 == 0)
		{
			handle = key;
			hasmore = true;
			return false;
		}
		Marshal::OctetsStream key_os;
		key_os = key;

		RoleId		id;
		try
		{
			key_os >> id;
			QueryRoleCsv( id.id, txn );
		} catch ( Marshal::Exception & ) {
			Log::log( LOG_ERR, "ListRoleCsvQuery, error unmarshal, roleid=%d.", id.id );
			return true;
		}
		return true;
	}
};

void ListRole( )
{
	printf("roleid");
	printf(",userid");
	printf(",name");
	printf(",race");
	printf(",occupation");
	printf(",gender");
	printf(",custom_data_size");
	printf(",custom_stamp");
	printf(",status");
	printf(",delete_time");
	printf(",create_time");
	printf(",lastlogin_time");
	printf(",forbid_size");

	printf(",level");
	printf(",level2");
	printf(",exp");
	printf(",sp");
	printf(",pp");
	printf(",hp");
	printf(",mp");
	printf(",posx");
	printf(",posy");
	printf(",posz");
	printf(",worldtag");
	printf(",money");
	printf(",invader_state");
	printf(",invader_time");
	printf(",pariah_time");
	printf(",factionid");
	printf(",factionrole");
	printf(",reputation");
	printf(",custom_status_size");
	printf(",filter_data_size");
	printf(",charactermode_size");
	printf(",instancekeylist_size");
	printf(",dbltime_expire");
	printf(",dbltime_mode");
	printf(",dbltime_begin");
	printf(",dbltime_used");
	printf(",dbltime_max");
	printf(",time_used");
	printf(",timestamp");
	printf(",storesize");
	printf(",petcorral_size");
	printf(",vitality");
	printf(",energy");
	printf(",strength");
	printf(",agility");
	printf(",max_hp");
	printf(",max_mp");
	printf(",hp_gen");
	printf(",mp_gen");
	printf(",walk_speed");
	printf(",run_speed");
	printf(",swim_speed");
	printf(",flight_speed");
	printf(",attack");
	printf(",damage_low");
	printf(",damage_high");
	printf(",attack_speed");
	printf(",attack_range");
	printf(",damage_low0");
	printf(",damage_low1");
	printf(",damage_low2");
	printf(",damage_low3");
	printf(",damage_low4");
	printf(",damage_high0");
	printf(",damage_high1");
	printf(",damage_high2");
	printf(",damage_high3");
	printf(",damage_high4");
	printf(",damage_magic_low");
	printf(",damage_magic_high");
	printf(",resistance0");
	printf(",resistance1");
	printf(",resistance2");
	printf(",resistance3");
	printf(",resistance4");
	printf(",defense");
	printf(",armor");
	printf(",max_ap");
	printf(",var_data_size");
	printf(",skills_size");
	printf(",storehousepasswd_size");
	printf(",waypointlist_size");
	printf(",coolingtime_size");

	printf(",storehouse_money");
	printf(",storehouse_size");

	printf(",inventory_size");

	printf(",equipment_size");

	printf(",taskinventory_size");

	printf(",task_data_size");

	printf(",task_complete_size");
	printf(",reincarn_times");
	printf(",reincarn_maxlevel");
	printf(",realm_level");

	printf("\n");

	ListRoleCsvQuery q;
	do
	{
		q.hasmore = false;	
		try
		{
			StorageEnv::Storage * pstorage = StorageEnv::GetStorage( "base" );
			StorageEnv::AtomTransaction	txn;
			try
			{
				StorageEnv::Storage::Cursor cursor = pstorage->cursor( txn );
				cursor.walk( q.handle, q );
			}
			catch ( DbException e ) { throw; }
			catch ( ... )
			{
				DbException ee( DB_OLD_VERSION );
				txn.abort( ee );
				throw ee;
			}
		}
		catch ( DbException e )
		{
			Log::log( LOG_ERR, "ListRole, error when walk, what=%s\n", e.what() );
		}
	
		StorageEnv::checkpoint();
	}while(q.hasmore);
}

void DisplayRoleInfoBrief(GRoleDetail& role, GRoleBase& base)
{
	Octets name, name2;
	CharsetConverter::conv_charset_u2l( role.name, name );
	EscapeCSVString( name, name2 );

	printf("%d",role.id);
	printf(",%d",0==role.userid ? (role.id & ~0x0000000F) : role.userid);
	printf(",%.*s",name2.size(),(char*)name2.begin());
	printf(",%d",role.cls);
	printf(",%d",role.status.level);
	printf(",%d",role.status.exp);
	printf(",%d",role.inventory.money+role.storehouse.money);
	printf(",%d",role.status.reputation);
	//计算包裹中的银票数量，计算仓库中的银票数量 id=21652
	int c=0;
	GRoleInventoryVector & v1 = role.inventory.items;
	for(size_t i=0; i<v1.size(); i++)
	{
		if(v1[i].id == 21652)
			c += v1[i].count;
	}
	GRoleInventoryVector & v2 = role.storehouse.items;
	for(size_t i=0; i<v2.size(); i++)
	{
		if(v2[i].id == 21652)
			c += v2[i].count;
	}
	printf(",%d",c);
	//计算账号仓库中的银票数量
	int c3=0;
	GRoleInventoryVector & v3 = role.userstorehouse.items;
	for(size_t i=0; i<v3.size(); i++)
	{
		if(v3[i].id == 21652)
			c3 += v3[i].count;
	}

	CrossInfoData cross_info;
	if (base.cross_data.size() > 0)
		Marshal::OctetsStream(base.cross_data) >> cross_info;

	printf(",%d",c3);
	printf(",%d",role.lastlogin_time);
	printf(",%d",role.gender);
	printf(",%d",role.spouse);
	printf(",%d",cross_info.remote_roleid);
	printf(",%d",cross_info.src_zoneid);
	printf(",%d",(base.status==_ROLE_STATUS_CROSS_LOCKED||base.status==_ROLE_STATUS_CROSS_RECYCLE)?1:0);
	printf(",%d",role.status.level2);
	
	int reincarn_times = 0;
	int reincarn_maxlevel = role.status.level;

    GReincarnationData data;
	GetRoleReincarnationDetail(role.status.reincarnation_data,reincarn_times,reincarn_maxlevel,data);
	printf(",%d", reincarn_times);
	printf(",%d", reincarn_maxlevel);
	int realm_level = 0;
	GetRoleRealmDetail(role.status.realm_data,realm_level);
	printf(",%d", realm_level);

    for (int i = 0; i < 10; ++i)
    {
        if (i < reincarn_times)
            printf(",%d", data.records[i].level);
        else
            printf(",0");
    }

    printf(",%d", data.tome_exp);

	printf("\n");
}

bool QueryRoleBrief( int roleid, GRoleBase & base, int & money, int login_time, StorageEnv::Transaction& txn )
{
	GRoleDetail	detail;

	Marshal::OctetsStream key, key_userid;
	Marshal::OctetsStream value_status, value_inventory, value_storehouse, value_user, value_userstore;

	try
	{
		StorageEnv::Storage * pstatus = StorageEnv::GetStorage("status");
		StorageEnv::Storage * pstorehouse = StorageEnv::GetStorage("storehouse");
		StorageEnv::Storage * pinventory  = StorageEnv::GetStorage("inventory");
		StorageEnv::Storage * puser = StorageEnv::GetStorage("user");
		StorageEnv::Storage * puserstore = StorageEnv::GetStorage("userstore");
		try
		{
			key << roleid;

			GRoleBaseToDetail( base, detail );

			int lastlogin_time = detail.lastlogin_time;

			key_userid << (0==base.userid ? (base.id & ~0x0000000F) : base.userid);
			if( puser->find( key_userid, value_user, txn) )
			{
				User user;
				value_user >> user;
				int mask = (1 << (roleid % MAX_ROLE_COUNT)) - 1;
				if(!(user.rolelist & mask))
				{
					//统计时认为账号仓库属于第一个角色	
					if( puserstore->find( key_userid, value_userstore, txn) )
						value_userstore >> detail.userstorehouse;	
					//如果是第一个角色，那么角色活动时间按照账号活动时间来处理，防止账号仓库不被统计
					if (login_time != 0)
					{
						if (user.login_record.size() == 8)
						{
							GLoginRecord login_record;
							Marshal::OctetsStream(user.login_record) >> login_record;
							lastlogin_time = login_record.login_time;
						}
					}
				}
			}

			if (lastlogin_time < login_time)
				return true;
			
			if( pstatus->find( key, value_status, txn ) )
				value_status >> detail.status;

			if( pinventory->find( key, value_inventory, txn ) )
				value_inventory >> detail.inventory;
 
			if( pstorehouse->find( key, value_storehouse, txn ) )
				value_storehouse >> detail.storehouse;

			money = detail.inventory.money + detail.storehouse.money;
			DisplayRoleInfoBrief( detail, base );
			return true;
		}
		catch ( DbException e ) { throw; }
		catch ( ... )
		{
			DbException ee( DB_OLD_VERSION );
			txn.abort( ee );
			throw ee;
		}
	}
	catch ( DbException e )
	{
		Log::log( LOG_ERR, "QueryRoleBrief, roleid=%d, what=%s\n", roleid, e.what() );
	}
	return false;
}

class ListRoleBriefQuery : public StorageEnv::IQuery
{
public:
	int64_t	money_total;
	int count;
	bool hasmore;
	Octets handle;
	int login_time;
	ListRoleBriefQuery(int t_login_time):money_total(0),count(0),hasmore(false),login_time(t_login_time){}
	bool Update( StorageEnv::Transaction& txn, Octets& key, Octets& value)
	{
		if(++count % 10000 == 0)
		{
			handle = key;
			hasmore = true;
			return false;
		}
		Marshal::OctetsStream key_os, value_os;
		key_os = key;
		value_os = value;

		RoleId		id;
		GRoleBase	base;
		try
		{
			key_os >> id;
			value_os >> base;

			int money = 0;
			QueryRoleBrief( id.id, base, money, login_time, txn );
			money_total += money;
		} catch ( Marshal::Exception & ) {
			Log::log( LOG_ERR, "ListRoleBriefQuery, error unmarshal, roleid=%d.", id.id );
			return true;
		}
		return true;
	}
};

void ListRoleBrief(std::string& login_time_str)
{
	printf("roleid");
	printf(",userid");
	printf(",name");
	printf(",occupation");
	printf(",level");
	printf(",exp");
	printf(",moneyall");
	printf(",reputation");
	printf(",role_yinpiao");
	printf(",user_yinpiao");
	printf(",updatetime");
	printf(",gender");
	printf(",spouse");
	printf(",remote_roleid");
	printf(",remote_zoneid");
	printf(",cross_locked");
	printf(",level2");
	printf(",reincarn_times");
	printf(",reincarn_maxlevel");
	printf(",realm_level");

    for (int i = 1; i <= 10; ++i)
        printf(",reincarn_level_%d", i);

    printf(",tome_exp");

	printf("\n");
	
	int login_time = 0;
	if (login_time_str.length() != 0)
	{
		int year = -1, month = -1, day = -1;
		sscanf(login_time_str.c_str(), "%d-%d-%d", &year, &month, &day);
		time_t ntime = time(NULL);
		struct tm ntimetm;
		localtime_r(&ntime, &ntimetm);
		ntimetm.tm_year = year-1900;
		if (ntimetm.tm_year < 0 || ntimetm.tm_year > 137)
		{
			printf("ListRoleBrief error, login_time wrong!\n");
			return;
		}
		ntimetm.tm_mon = month-1;
		if (ntimetm.tm_mon < 0 || ntimetm.tm_mon > 11)
		{
			printf("ListRoleBrief error, login_time wrong!\n");
			return;
		}
		ntimetm.tm_mday = day;
		if (ntimetm.tm_mday < 1 || ntimetm.tm_mday > 31)
		{
			printf("ListRoleBrief error, login_time wrong!\n");
			return;
		}
		ntimetm.tm_hour = 0;
		ntimetm.tm_min = 0;
		ntimetm.tm_sec = 0;
		login_time = mktime(&ntimetm);
		if (login_time < 0)
		{
			printf("ListRoleBrief error, login_time wrong!\n");
			return;
		}
	}
	ListRoleBriefQuery q(login_time);
	do
	{
		q.hasmore = false;
		try
		{
			StorageEnv::Storage * pstorage = StorageEnv::GetStorage( "base" );
			StorageEnv::AtomTransaction	txn;
			try
			{
				StorageEnv::Storage::Cursor cursor = pstorage->cursor( txn );
				cursor.walk( q.handle, q );
			}
			catch ( DbException e ) { throw; }
			catch ( ... )
			{
				DbException ee( DB_OLD_VERSION );
				txn.abort( ee );
				throw ee;
			}
		}
		catch ( DbException e )
		{
			Log::log( LOG_ERR, "ListRoleBrief, error when walk, what=%s\n", e.what() );
		}
	
		StorageEnv::checkpoint();
	}while(q.hasmore);
	
	printf("0,0,\"\",0,0,0,%lld\n",q.money_total);
}

void DisplayUserInfoBrief(User& user)
{
	GSysAuctionCash sa_cash;
	if(user.cash_sysauction.size())
		Marshal::OctetsStream(user.cash_sysauction)>>sa_cash;
	int cash_cur;
	//include cash stored in stockexchage
	cash_cur = user.cash_add + user.cash_buy - user.cash_sell - user.cash_used - sa_cash.cash_used_2;
	printf("%d",user.logicuid);
	printf(",%x",user.rolelist);
	printf(",%d",user.cash);
	printf(",%d",user.money);
	printf(",%d",sa_cash.cash_2);
	printf(",%d",user.cash_add);
	printf(",%d",user.cash_buy);
	printf(",%d",user.cash_sell);
	printf(",%d",user.cash_used);
	printf(",%d",sa_cash.cash_used_2);
	printf(",%d",user.add_serial);
	printf(",%d",user.use_serial);
	printf(",%d",cash_cur);
	printf(",%d",user.exg_log.size());
	printf("\n");
}

class ListUserBriefQuery : public StorageEnv::IQuery
{
public:
	int64_t curcash_total;
	int64_t cash_add_total;
	int64_t cash_used_total;
	int64_t cash_total;
	int64_t cash_sell_total;
	int64_t money_total;
	int64_t cash_buy_total;
	int64_t numberofuser;
	int64_t cash_2_total;
	int64_t cash_used_2_total;
	ListUserBriefQuery():curcash_total(0),cash_add_total(0),cash_used_total(0),cash_total(0),cash_sell_total(0),money_total(0),
						 cash_buy_total(0),numberofuser(0),cash_2_total(0),cash_used_2_total(0)
	{
	}
	bool Update( StorageEnv::Transaction& txn, Octets& key, Octets& value)
	{
		Marshal::OctetsStream key_os, value_os;
		key_os = key;
		value_os = value;

		UserID     id;
		User       user;
		GSysAuctionCash sa_cash;
		try
		{
			key_os >> id;
			value_os >> user;
			if(user.cash_sysauction.size())
				Marshal::OctetsStream(user.cash_sysauction)>>sa_cash;
			if(!user.cash_add && !user.cash_buy && !user.cash_sell && !user.cash_used && !sa_cash.cash_used_2)
				return true;
			DisplayUserInfoBrief(user);
			cash_add_total += (int)user.cash_add;
			int cash_cur = user.cash_add + user.cash_buy - user.cash_sell - user.cash_used - sa_cash.cash_used_2;
			curcash_total += cash_cur;
			cash_total += (int)user.cash;
			cash_used_total += (int)user.cash_used;
			cash_sell_total += (int)user.cash_sell;
			cash_buy_total += (int)user.cash_buy;
			money_total += (int)user.money;
			numberofuser++;
			cash_2_total += (int)sa_cash.cash_2;
			cash_used_2_total += (int)sa_cash.cash_used_2;

		} catch ( Marshal::Exception & ) {
			Log::log( LOG_ERR, "ListUserBriefQuery, error unmarshal, userid=%d.", id.id );
			return true;
		}
		return true;
	}
};

void ListUserBrief( )
{

	//printf("cash:         cash count that stored in stock exchange. \n");
	//printf("cash_add:     cash count which user has bought by RMB. \n");
	//printf("cash_buy:     cash count which role has bought by money in game. \n");
	//printf("cash_sell:    cash count which role has sold  by money in game. \n");
	//printf("cash_used:    cash count which role has spent in game. \n");
	//printf("cash_current: cash count which user now owns in game. \n");
	//printf("money:        game money which role has stored in stock exchange. \n");
	//printf(" All roles of the user share money and cash in stock exchange.\n");


	/* All roles of the user share money and cash in stock exchange */

	printf("userid");
	printf(",rolelist");
	//cash count that stored in stock exchange
	printf(",cash");
	//game money which role has stored in stock exchange
	printf(",money");
	//cash count that stored in sysauction
	printf(",cash_2");
	//cash count which user has bought by RMB
	printf(",cash_add");
	//cash count which role has bought by money in game
	printf(",cash_buy");
	//cash count which role has sold  by money in game
	printf(",cash_sell");
	//cash count which role has spent in game
	printf(",cash_used");
	//cash count which role has spent in sysauction
	printf(",cash_used_2");
	printf(",add_serial");
	printf(",use_serial");
	//cash count which user now owns in game. cash_current = cash_add + cash_buy - cash_sell - cash_used
	printf(",cash_current");
	printf(",exg_logcount");

	printf("\n");


	ListUserBriefQuery q;

	try
	{
		StorageEnv::Storage * pstorage = StorageEnv::GetStorage( "user" );
		StorageEnv::AtomTransaction     txn;
		try
		{
			StorageEnv::Storage::Cursor cursor = pstorage->cursor( txn );
			cursor.walk( q );
		}
		catch ( DbException e ) { throw; }
		catch ( ... )
		{
			DbException ee( DB_OLD_VERSION );
			txn.abort( ee );
			throw ee;
		}
	}
	catch ( DbException e )
	{
		Log::log( LOG_ERR, "ListUserBrief, error when walk, what=%s\n", e.what() );
	}

	printf("\ncash_add_total  : %lld\n",q.cash_add_total);
	printf("cash_now_total  : %lld\n",q.curcash_total);
	q.cash_used_total += q.cash_sell_total - q.cash_buy_total + q.cash_used_2_total; 
	printf("cash_used_total : %lld\n",q.cash_used_total);
	if(q.curcash_total > (q.cash_add_total - q.cash_used_total)){
		printf("WARNING: cash_now_total > cash_add_total - cash_used_total\n");
	}

	printf("\ncash_buy_total : %lld\n",q.cash_buy_total);
	printf("cash_sell_total  : %lld\n",q.cash_sell_total);	
	if(q.cash_sell_total - q.cash_buy_total < q.cash_buy_total*0.02 ){
		printf("WARNING: cash_buy_total*1.02 > cash_sell_total\n");	
	}	

	printf("\ncash_total in stock exchange  : %lld\n",q.cash_total);
	printf("money_total in stock exchange : %lld\n",q.money_total);
	
	printf("\ncash_used_2_total in sysauction: %lld\n",q.cash_used_2_total);
	printf("cash_2_total in sysauction: %lld\n",q.cash_2_total);
	
	printf("number of user : %lld\n",q.numberofuser);	
}

class ListFactionQuery : public StorageEnv::IQuery
{
public:
	bool Update( StorageEnv::Transaction& txn, Octets& key, Octets& value)
	{
		Marshal::OctetsStream key_os, value_os;
		key_os = key;
		value_os = value;

		RoleId			id;
		GFactionInfo	faction;
		try
		{
			key_os >> id;
			if( 0 == id.id )
				return true;

			value_os >> faction;

			Octets name, name2;
			CharsetConverter::conv_charset_u2l( faction.name, name );
			EscapeCSVString( name, name2 );

			printf("%d",faction.fid);
			printf(",%.*s",name2.size(),(char*)name2.begin() );
			printf(",%d",faction.level);
			printf(",%d",faction.master.rid);
			printf(",%d",faction.master.role);
			printf(",%d",faction.member.size());
			printf("\n");
		} catch ( Marshal::Exception & ) {
			Log::log( LOG_ERR, "ListFactionQuery, error unmarshal, roleid=%d.", id.id );
			return true;
		}
		return true;
	}
};

void ListFaction( )
{
	printf("fid");
	printf(",name");
	printf(",level");
	printf(",masterid");
	printf(",masterrole");
	printf(",member_size");
	printf("\n");

	ListFactionQuery q;
	try
	{
		StorageEnv::Storage * pstorage = StorageEnv::GetStorage( "factioninfo" );
		StorageEnv::AtomTransaction	txn;
		try
		{
			StorageEnv::Storage::Cursor cursor = pstorage->cursor( txn );
			cursor.walk( q );
		}
		catch ( DbException e ) { throw; }
		catch ( ... )
		{
			DbException ee( DB_OLD_VERSION );
			txn.abort( ee );
			throw ee;
		}
	}
	catch ( DbException e )
	{
		Log::log( LOG_ERR, "ListFaction, error when walk, what=%s\n", e.what() );
	}
}

class ListFactionUserQuery : public StorageEnv::IQuery
{
public:
	bool Update( StorageEnv::Transaction& txn, Octets& key, Octets& value)
	{
		Marshal::OctetsStream key_os, value_os;
		key_os = key;
		value_os = value;

		RoleId			id;
		GUserFaction	uf;
		try
		{
			key_os >> id;

			value_os >> uf;

			Octets name, name2, nickname, nickname2;
			CharsetConverter::conv_charset_u2l( uf.name, name );
			CharsetConverter::conv_charset_u2l( uf.nickname, nickname );
			EscapeCSVString( name, name2 );
			EscapeCSVString( nickname, nickname2 );

			printf("%d",uf.rid);
			printf(",%.*s",name2.size(),(char*)name2.begin() );
			printf(",%d",uf.fid);
			printf(",%d",uf.cls);
			printf(",%d",uf.role);
			printf(",%d",0/*uf.loyalty*/);
			printf(",%.*s",nickname2.size(),(char*)nickname2.begin() );
			printf("\n");
		} catch ( Marshal::Exception & ) {
			Log::log( LOG_ERR, "ListFactionUserQuery, error unmarshal, roleid=%d.", id.id );
			return true;
		}
		return true;
	}
};

void ListFactionUser( )
{
	printf("rid");
	printf(",name");
	printf(",fid");
	printf(",cls");
	printf(",role");
	printf(",loyalty");
	printf(",nickname");
	printf("\n");

	ListFactionUserQuery q;

	try
	{
		StorageEnv::Storage * pstorage = StorageEnv::GetStorage( "userfaction" );
		StorageEnv::AtomTransaction	txn;
		try
		{
			StorageEnv::Storage::Cursor cursor = pstorage->cursor( txn );
			cursor.walk( q );
		}
		catch ( DbException e ) { throw; }
		catch ( ... )
		{
			DbException ee( DB_OLD_VERSION );
			txn.abort( ee );
			throw ee;
		}
	}
	catch ( DbException e )
	{
		Log::log( LOG_ERR, "ListFactionUser, error when walk, what=%s\n", e.what() );
	}
}

class ListFactionRelationQuery : public StorageEnv::IQuery
{
public:
	bool Update( StorageEnv::Transaction& txn, Octets& key, Octets& value)
	{
		Marshal::OctetsStream key_os, value_os;
		key_os = key;
		value_os = value;

		int	fid;
		GFactionRelation relation;
		
		try {
			key_os >> fid;
			value_os >> relation;

			printf("%d:", relation.fid);
			for(unsigned int i = 0; i < relation.alliance.size(); ++i) {
				if(i == 0) {
					printf("%d", relation.alliance[i].fid);
				} else  {
					printf(",%d", relation.alliance[i].fid);
				}
			}
			printf(":");
			for(unsigned int i = 0; i < relation.hostile.size(); ++i) {
				if(i == 0) {
					printf("%d", relation.hostile[i].fid);
				} else {
					printf(",%d", relation.hostile[i].fid);
				}
			}
			printf("\n");
		} catch ( Marshal::Exception & ) {
			Log::log( LOG_ERR, "ListFactionRelationQuery, error unmarshal, fid=%d.", fid );
			return true;
		}

		return true;
	}
};

void ListFactionRelation()
{
	ListFactionRelationQuery q;

	try
	{
		StorageEnv::Storage * pstorage = StorageEnv::GetStorage( "factionrelation" );
		StorageEnv::AtomTransaction	txn;
		try
		{
			StorageEnv::Storage::Cursor cursor = pstorage->cursor( txn );
			cursor.walk( q );
		}
		catch ( DbException e ) { throw; }
		catch ( ... )
		{
			DbException ee( DB_OLD_VERSION );
			txn.abort( ee );
			throw ee;
		}
	}
	catch ( DbException e )
	{
		Log::log( LOG_ERR, "ListFactionUser, error when walk, what=%s\n", e.what() );
	}
}

void ListCity( )
{
	printf("id");
	printf(",level");
	printf(",owner");
	printf(",occupy_time");
	printf(",challenger");
	printf(",deposit");
	printf(",cutoff_time");
	printf(",battle_time");
	printf(",bonus_time");
	printf(",color");
	printf(",status");
	printf(",timeout");
	printf(",maxbonus");
	printf("\n");

	try
	{
		StorageEnv::Storage * pstorage = StorageEnv::GetStorage( "city" );
		StorageEnv::AtomTransaction	txn;
		try
		{
			Marshal::OctetsStream	key;
			GTerritoryStore	store;
			key << 0;
			Marshal::OctetsStream(pstorage->find(key,txn)) >> store;
			std::vector<GTerritoryDetail>::iterator it=store.cities.begin(),ie=store.cities.end();
			for(;it!=ie;++it)
			{
				printf("%d",it->id);
				printf(",%d",it->level);
				printf(",%d",it->owner);
				printf(",%d",it->occupy_time);
				printf(",%d",it->challenger);
				printf(",%d",it->deposit);
				printf(",%d",it->cutoff_time);
				printf(",%d",it->battle_time);
				printf(",%d",it->bonus_time);
				printf(",%d",it->color);
				printf(",%d",it->status);
				printf(",%d",it->timeout);
				printf(",%d",it->maxbonus);
				printf("\n");
			}
		}
		catch ( DbException e ) { throw; }
		catch ( ... )
		{
			DbException ee( DB_OLD_VERSION );
			txn.abort( ee );
			throw ee;
		}
	}
	catch ( DbException e )
	{
		Log::log( LOG_ERR, "ListCity, error when walk, what=%s\n", e.what() );
	}
}

class ListShopLogQuery : public StorageEnv::IQuery
{
public:
	bool Update( StorageEnv::Transaction& txn, Octets& key, Octets& value)
	{
		Marshal::OctetsStream value_os = value;

		std::vector<GShopLog> logs;
		try
		{
			value_os >> logs;

			for(std::vector<GShopLog>::iterator it=logs.begin(),ie=logs.end();it!=ie;++it)
			{
				printf("%d", it->roleid);
				printf(",%d", it->order_id);
				printf(",%d", it->item_id);
				printf(",%d", it->expire);
				printf(",%d", it->item_count);
				printf(",%d", it->order_count);
				printf(",%d", it->cash_need);
				printf(",%d", it->time);
				printf(",%d", it->guid1);
				printf(",%d", it->guid2);
				printf("\n");
			}
		} catch ( Marshal::Exception & ) {
			Log::log( LOG_ERR, "ListShopLogQuery, error unmarshal.");
			return true;
		}
		return true;
	}
};


void ListShopLog( )
{
	printf("roldid");
	printf(",order_id");
	printf(",item_id");
	printf(",expire");
	printf(",item_count");
	printf(",order_count");
	printf(",cash_need");
	printf(",time");
	printf(",guid1");
	printf(",guid2");
	printf("\n");

	ListShopLogQuery q;
	try
	{
		StorageEnv::Storage * pstorage = StorageEnv::GetStorage( "shoplog" );
		StorageEnv::AtomTransaction     txn;
		try
		{
			StorageEnv::Storage::Cursor cursor = pstorage->cursor( txn );
			cursor.walk( q );
		}
		catch ( DbException e ) { throw; }
		catch ( ... )
		{
			DbException ee( DB_OLD_VERSION );
			txn.abort( ee );
			throw ee;
		}
	}
	catch ( DbException e )
	{
		Log::log( LOG_ERR, "ListShopLog, error when walk, what=%s\n", e.what() );
	}
}

void DisplayInventory( GRoleBase & base, GRoleInventoryVector & inventory, GRoleStorehouse & storehouse )
{
	struct inv_t {
		unsigned int	id;
		char			name[64];
		int				count;
	};

	struct inv_t invs[] = {
		{ 795, "原木", 0 },
		{ 796, "粗木料", 0 },
		{ 800, "生铁", 0 },
		{ 801, "高炭钢", 0 },
		{ 805, "砂石", 0 },
		{ 806, "碎石", 0 },
		{ 815, "煤粉", 0 },
		{ 816, "烟煤", 0 },
		{ 0, "", 0 }
	};

	int n = 0;
	for( size_t i=0; i<inventory.size(); i++ )
	{
		for( n=0; invs[n].id != 0; n++ )
		{
			if( inventory[i].id == invs[n].id )
				invs[n].count += inventory[i].count;
		}
	}

	/*for( size_t j=0; j<storehouse.storehouse.size(); j++ )
	{
		for( n=0; invs[n].id != 0; n++ )
		{
			if( storehouse.storehouse[j].id == invs[n].id )
				invs[n].count += storehouse.storehouse[j].count;
		}
	}
	*/

	Octets name, name2;
	CharsetConverter::conv_charset_u2l( base.name, name );
	EscapeCSVString( name, name2 );

	printf("%d",base.id);
	printf(",%d",0==base.userid ? (base.id & ~0x0000000F) : base.userid);
	//printf(",%.*s",name2.size(),(char*)name2.begin());

	for( n=0; invs[n].id != 0; n++ )
	{
		printf(",%d", invs[n].count);
	}
	printf("\n");
}

class UpdateRolesQuery : public StorageEnv::IQuery
{
	//value_os >> base;
	//char buffer[64];
	//sprintf( buffer, "./custom/%u.custom", id.id );
	//std::ofstream   ofs(buffer, std::ios_base::app);
	//ofs.write( (char*)base.custom_data.begin(), base.custom_data.size() );
	//ofs.close();
public:
	bool Update( StorageEnv::Transaction& txn, Octets& key, Octets& value)
	{
		Marshal::OctetsStream key_os, value_base, value_inventory, value_storehouse;
		key_os = key;
		value_base = value;

		static int count = 0;
		count ++;
		if( ! (count % 1000) )
			fprintf( stderr, "updating roles %d...\n", count );

		RoleId		id;
		try
		{
			StorageEnv::Storage * pinventory  = StorageEnv::GetStorage("inventory");
			StorageEnv::Storage * pstorehouse = StorageEnv::GetStorage("storehouse");
			try
			{
				GRoleBase	base;
				GRoleInventoryVector	inventory;
				GRoleStorehouse			storehouse;

				key_os >> id;
				value_base >> base;
				if( pinventory->find( key_os, value_inventory, txn ) )
					value_inventory >> inventory;
				if( pstorehouse->find( key_os, value_storehouse, txn ) )
					value_storehouse >> storehouse;
				DisplayInventory( base, inventory, storehouse );
			}
			catch ( DbException e ) { throw; }
			catch ( ... )
			{
				DbException ee( DB_OLD_VERSION );
				txn.abort( ee );
				throw ee;
			}
		}
		catch ( DbException e )
		{
			Log::log( LOG_ERR, "UpdateRolesQuery, roleid=%d, what=%s\n", id.id, e.what() );
		}
		return true;
	}
};

void UpdateRoles( )
{
	return;

/*
	int count = 0, errcount = 0;

	if( 0 != access("reproles.txt",R_OK) )
	{
		fprintf( stderr, "no reproles.txt find.\n" );
		return;
	}

	std::ifstream	ifs( "reproles.txt" );
	while( !ifs.eof() )
	{
		char	line[256];
		memset( line, 0, sizeof(line) );
		ifs.getline( line, sizeof(line) );
		line[sizeof(line)-1] = 0;
		if( !ifs.eof() && strlen(line) > 0 )
		{
			long roleid = atol( line );

			Marshal::OctetsStream key_os, value_status, value_status_new;
			RoleId		id;
			GRoleStatus	status;
			id.id = roleid;
			key_os << id;
	
			try
			{
				StorageEnv::Storage * pstatus = StorageEnv::GetStorage("status");
				StorageEnv::AtomTransaction	txn;
				try
				{
					RoleId		id;
					GRoleBase	base;
					GRoleStatus	status;
    
					value_status = pstatus->find( key_os, txn );
					value_status >> status;
					if( status.reputation < 50 )
					{
						Log::log( LOG_ERR, "UpdateRoles, logical error, roleid=%d, reputation=%d",
								id.id, status.reputation );
						errcount ++;
					}

					status.money+= 10000;
					value_status_new << status;
					pstatus->insert( key_os, value_status_new, txn );
					printf( "update role %d successfully.\n", roleid );
					count ++;
				}
				catch ( DbException e ) { throw; }
				catch ( ... )
				{
					DbException ee( DB_OLD_VERSION );
					txn.abort( ee );
					throw ee;
				}
			}
			catch ( DbException e )
			{
				printf( "update role %d error, what=%s\n", id.id, e.what() );
				errcount ++;
			}
		}
	}
	ifs.close();

	printf( "update count = %d. errcount = %d\n", count, errcount );
*/
	printf("roleid");
	printf(",userid");
	//printf(",name");
	printf(",原木(795)");
	printf(",粗木料(796)");
	printf(",生铁(800)");
	printf(",高炭钢(801)");
	printf(",砂石(805)");
	printf(",碎石(806)");
	printf(",煤粉(815)");
	printf(",烟煤(816)");
	printf("\n");

	UpdateRolesQuery q;
	try
	{
		StorageEnv::Storage * pstorage = StorageEnv::GetStorage( "base" );
		StorageEnv::AtomTransaction	txn;
		try
		{
			StorageEnv::Storage::Cursor cursor = pstorage->cursor( txn );
			cursor.walk( q );
		}
		catch ( DbException e ) { throw; }
		catch ( ... )
		{
			DbException ee( DB_OLD_VERSION );
			txn.abort( ee );
			throw ee;
		}
	}
	catch ( DbException e )
	{
		Log::log( LOG_ERR, "UpdateRoles, error when walk, what=%s\n", e.what() );
	}
}

class ConvertStatusDBQuery : public StorageEnv::IQuery
{
public:
	bool Update( StorageEnv::Transaction& txn, Octets& key, Octets& value)
	{
		Marshal::OctetsStream os_key, os_old, os_new;
		os_key = key;
		os_old = value;

		static int count = 0;
		if( ! (count++ % 100000) ) printf( "\tconverting database records counter %d...\n", count );

		int roleid;

		try
		{
			StorageEnv::Storage * pconvtemp = StorageEnv::GetStorage("conv_temp");
			try
			{
				GRoleStatus	status;
				os_key >> roleid;
				os_old >> status;
				os_new << status;
				pconvtemp->insert( key, os_new, txn );
			}
			catch ( DbException e ) { throw; }
			catch ( ... )
			{
				DbException ee( DB_OLD_VERSION );
				txn.abort( ee );
				throw ee;
			}
		}
		catch ( DbException e )
		{
			Log::log( LOG_ERR, "ConvertStatusDBQuery, roleid=%d, what=%s\n", roleid, e.what() );
		}
		return true;
	}
};

void ConvertDB( )
{
	return;

	printf( "\nconvert database\n" );

	std::string data_dir = StorageEnv::get_datadir();

	ConvertStatusDBQuery qstatus;
	try
	{
		StorageEnv::Storage * pstorage = StorageEnv::GetStorage( "status" );
		StorageEnv::AtomTransaction	txn;
		try
		{
			StorageEnv::Storage::Cursor cursor = pstorage->cursor( txn );
			cursor.walk( qstatus );
		}
		catch ( DbException e ) { throw; }
		catch ( ... )
		{
			DbException ee( DB_OLD_VERSION );
			txn.abort( ee );
			throw ee;
		}
	}
	catch ( DbException e )
	{
		Log::log( LOG_ERR, "ConvertDB-status, error when walk, what=%s\n", e.what() );
	}
	StorageEnv::checkpoint();
	StorageEnv::Close();
	if( 0 == access( (data_dir + "/conv_temp").c_str(), R_OK ) )	
		system( ("/bin/mv -f " + data_dir + "/conv_temp " + data_dir + "/status").c_str() );
	StorageEnv::Open();
}

class RepairDBQuery : public StorageEnv::IQuery
{
public:
	bool Update( StorageEnv::Transaction& txn, Octets& key, Octets& value)
	{
		Marshal::OctetsStream key_roleid, key_userid, value_base, value_user;
		key_roleid = key;
		value_base = value;

		int roleid;

		try
		{
			StorageEnv::Storage * puser = StorageEnv::GetStorage("user");
			try
			{
				GRoleBase	base;
				User		user;
				key_roleid >> roleid;
				value_base >> base;
				if( 0 == base.userid )	
					base.userid = LOGICUID(roleid);
				key_userid << base.userid;

				if( roleid < 128 )
					return true;

				if( puser->find( key_userid, value_user, txn ) )
				{
					value_user >> user;
					if( 0 == user.logicuid )
					{
						user.logicuid = LOGICUID(roleid);
						puser->insert( key_userid, Marshal::OctetsStream()<<user, txn );
						printf( "\tRepair mod userid=%d, logicuid=%d, roleliest=%x\n", base.userid, user.logicuid, user.rolelist );
					}
					/*
					RoleList rolelist(user.rolelist);
					if( !rolelist.IsRoleExist( roleid ) )
					{
						if( -1 == rolelist.AddRole( roleid % MAX_ROLE_COUNT ) )
							printf( "ERROR:Repair roleid=%d, rolelist full=%x", roleid, user.rolelist );
						user.logicuid = LOGICUID(roleid);
						user.rolelist = rolelist.GetRoleList();
						value_user.clear();
						value_user << user;
						puser->insert( key_userid, value_user, txn );
						printf( "\tRepair roleid=%d, new user roleliest=%x\n", roleid, user.rolelist );
					}
					*/
				}
				else
				{
					RoleList rolelist;
					rolelist.InitialRoleList();

					rolelist.AddRole( roleid % MAX_ROLE_COUNT );
					user.logicuid = LOGICUID(roleid);
					user.rolelist = rolelist.GetRoleList();
					puser->insert( key_userid, Marshal::OctetsStream()<<user, txn );
					printf( "\tRepair add userid=%d, logicuid=%d, roleliest=%x\n", base.userid, user.logicuid, user.rolelist );
				}
			}
			catch ( DbException e ) { throw; }
			catch ( ... )
			{
				DbException ee( DB_OLD_VERSION );
				txn.abort( ee );
				throw ee;
			}
		}
		catch ( DbException e )
		{
			Log::log( LOG_ERR, "RepairDBQuery, roleid=%d, what=%s\n", roleid, e.what() );
		}
		return true;
	}
};

void RepairDB( )
{
	printf( "\nrepair database\n" );

	RepairDBQuery q;
	try
	{
		StorageEnv::Storage * pstorage = StorageEnv::GetStorage( "base" );
		StorageEnv::AtomTransaction	txn;
		try
		{
			StorageEnv::Storage::Cursor cursor = pstorage->cursor( txn );
			cursor.walk( q );
		}
		catch ( DbException e ) { throw; }
		catch ( ... )
		{
			DbException ee( DB_OLD_VERSION );
			txn.abort( ee );
			throw ee;
		}
	}
	catch ( DbException e )
	{
		Log::log( LOG_ERR, "RepairDB, error when walk, what=%s\n", e.what() );
	}
	StorageEnv::checkpoint();
	printf( "repair finished.\n" );
}

class TimeoutRoleQuery : public StorageEnv::IQuery
{
	int		  ncount;	
	IntVector rolelist;
public:
	TimeoutRoleQuery() { ncount=0; }
	IntVector& GetRoleList() { return rolelist; }
	bool Update( StorageEnv::Transaction& txn, Octets& key, Octets& value)
	{
		Marshal::OctetsStream key_os, value_os;
		key_os = key;
		value_os = value;

		int		roleid;
		int		deltime;
		try
		{
			key_os >> roleid;
			value_os >> deltime;
			if( Timer::GetTime()-deltime > GameDBManager::GetInstance()->GetDeleteRole_Timeout() )
			{
				//announce delivery server
				rolelist.add(roleid);
				LOG_TRACE( "TimeoutRoleQuery, roleid=%d.\n",roleid );
				ncount++;
				if (ncount>=100/*maxium number of roles in one request*/) return false;
			}

		} catch ( Marshal::Exception & ) {
			Log::log( LOG_ERR, "TimeoutRoleQuery, error unmarshal, roleid=%d.", roleid );
		}
		return true;
	}
};

void GetTimeoutRole( IntVector& rolelist )
{
	TimeoutRoleQuery q;
	try
	{
		StorageEnv::Storage * pstorage = StorageEnv::GetStorage( "waitdel" );
		StorageEnv::AtomTransaction	txn;
		try
		{
			StorageEnv::Storage::Cursor cursor = pstorage->cursor( txn );
			cursor.walk( q );
		}
		catch ( DbException e ) { throw; }
		catch ( ... )
		{
			DbException ee( DB_OLD_VERSION );
			txn.abort( ee );
			throw ee;
		}
	}
	catch ( DbException e )
	{
		Log::log( LOG_ERR, "GetTimeoutRole, error when walk, what=%s\n", e.what() );
	}
	rolelist.GetVector().swap(q.GetRoleList().GetVector());
}
bool ExportRole( int roleid )
{
	try
	{
		StorageEnv::Storage * pbase = StorageEnv::GetStorage("base");
		StorageEnv::Storage * pstatus = StorageEnv::GetStorage("status");
		StorageEnv::Storage * pinventory = StorageEnv::GetStorage("inventory");
		StorageEnv::Storage * pequipment = StorageEnv::GetStorage("equipment");
		StorageEnv::Storage * pstorehouse = StorageEnv::GetStorage("storehouse");
		StorageEnv::Storage * ptask = StorageEnv::GetStorage("task");
		StorageEnv::AtomTransaction txn;

		Marshal::OctetsStream key;
		GRoleData data;

		try
		{
			key << roleid;
			Marshal::OctetsStream(pbase->find(key,txn)) >> data.base;
			Marshal::OctetsStream(pstatus->find(key,txn)) >> data.status;
			Marshal::OctetsStream(pinventory->find(key,txn)) >> data.pocket;
			Marshal::OctetsStream(pequipment->find(key,txn)) >> data.equipment;
			Marshal::OctetsStream(pstorehouse->find(key,txn)) >> data.storehouse;
			Marshal::OctetsStream(ptask->find(key,txn)) >> data.task;

			XmlCoder coder;
			coder.append_header();
			coder.append("role", data);
			puts(coder.c_str());
			return true;
		}
		catch ( DbException e ) { throw; }
		catch ( ... )
		{
			DbException ee( DB_OLD_VERSION );
			txn.abort( ee );
			throw ee;
		}
	}
	catch ( DbException e )
	{
		Log::log( LOG_ERR, "ExportRole, roleid=%d, what=%s\n", roleid, e.what() );
	}
	return false;
}

bool ClearRole( int roleid, char * tablename )
{
	if(roleid < 32)	return false;
	try
	{
		StorageEnv::Storage * pbase = StorageEnv::GetStorage("base");
		StorageEnv::Storage * pstatus = StorageEnv::GetStorage("status");
		StorageEnv::Storage * pinventory = StorageEnv::GetStorage("inventory");
		StorageEnv::Storage * pequipment = StorageEnv::GetStorage("equipment");
		StorageEnv::Storage * pstorehouse = StorageEnv::GetStorage("storehouse");
		StorageEnv::Storage * ptask = StorageEnv::GetStorage("task");
		StorageEnv::AtomTransaction txn;

		Marshal::OctetsStream key, value;
		try
		{
			key << roleid;
			if(pbase->find(key,value,txn))
			{
				if(tablename == NULL || 0 == strcmp(tablename,"status"))
				{
					pstatus->del(key,txn);
					printf("clear role data:roleid=%d,table=\"status\"\n", roleid);				
				}
				if(tablename == NULL || 0 == strcmp(tablename,"inventory"))
				{
					pinventory->del(key,txn);
					printf("clear role data:roleid=%d,table=\"inventory\"\n", roleid);				
				}
				if(tablename == NULL || 0 == strcmp(tablename,"equipment"))
				{
					pequipment->del(key,txn);
					printf("clear role data:roleid=%d,table=\"equipment\"\n", roleid);				
				}
				if(tablename == NULL || 0 == strcmp(tablename,"storehouse"))
				{
					pstorehouse->del(key,txn);
					printf("clear role data:roleid=%d,table=\"storehouse\"\n", roleid);				
				}
				if(tablename == NULL || 0 == strcmp(tablename,"task"))
				{
					ptask->del(key,txn);
					printf("clear role data:roleid=%d,table=\"task\"\n", roleid);				
				}
			}
			else
			{
				printf("ERROR in ClearRole. ROLE NOT FOUND!\n");	
			}
			return true;
		}
		catch ( DbException e ) { throw; }
		catch ( ... )
		{
			DbException ee( DB_OLD_VERSION );
			txn.abort( ee );
			throw ee;
		}
	}
	catch ( DbException e )
	{
		Log::log( LOG_ERR, "ExportRole, roleid=%d, what=%s\n", roleid, e.what() );
	}
	return false;
}

bool ExportUser( int userid )
{
	try
	{
		StorageEnv::Storage * puser = StorageEnv::GetStorage( "user" );
		StorageEnv::AtomTransaction txn;
		try
		{
			User user;
			Marshal::OctetsStream(puser->find(Marshal::OctetsStream()<<userid, txn)) >> user;

			XmlCoder encoder;
			encoder.append_header();
			encoder.append("user", user);
			puts(encoder.c_str());
			return true;
		}
		catch ( DbException e ) { throw; }
		catch ( ... )
		{
			DbException ee( DB_OLD_VERSION );
			txn.abort( ee );
			throw ee;
		}
	}
	catch ( DbException e )
	{
		Log::log( LOG_ERR, "ExportUser failed, userid=%d, what=%s\n", userid, e.what());
	}
	return false;
}

bool ModifyCash( int userid, int cash_add, int cash_used )
{
	try
	{
		StorageEnv::Storage * puser = StorageEnv::GetStorage( "user" );
		StorageEnv::CommonTransaction txn;
		try
		{
			Marshal::OctetsStream key;
			key << userid;
			User user;
			Marshal::OctetsStream(puser->find(key, txn)) >> user;

			user.cash_add = cash_add;
			user.cash_used = cash_used;

			puser->insert(key, Marshal::OctetsStream()<<user, txn);
			return true;
		}
		catch ( DbException e ) { throw; }
		catch ( ... )
		{
			DbException ee( DB_OLD_VERSION );
			txn.abort( ee );
			throw ee;
		}
	}
	catch ( DbException e )
	{
		Log::log( LOG_ERR, "ExportUser failed, userid=%d, what=%s\n", userid, e.what());
	}
	return false;
}

void SetCashInvisible(const char* file)
{
	std::ifstream ifs(file);
	std::string line;
	try
	{
		StorageEnv::Storage * puser = StorageEnv::GetStorage( "user" );
		StorageEnv::AtomTransaction txn;
		try
		{
			User user;
			while (std::getline(ifs, line))
			{
				Marshal::OctetsStream key, value;
				int userid = atoi(line.c_str());
				key << userid;
				if(puser->find(key,value,txn))
				{
					value >> user;
					if((user.status&STATUS_CASHINVISIBLE)!=0)
					{
						user.status &= ~STATUS_CASHINVISIBLE;
						puser->insert(key,Marshal::OctetsStream()<<user,txn);
					}
				}
			}
		}
		catch ( DbException e ) { throw; }
		catch ( ... )
		{
			DbException ee( DB_OLD_VERSION );
			txn.abort( ee );
			throw ee;
		}
	}
	catch ( DbException e )
	{
		Log::log( LOG_ERR, "SetCashInvisible, error updating, what=%s\n", e.what() );
	}
}
class ListSysLogQuery : public StorageEnv::IQuery
{
public:
	bool Update( StorageEnv::Transaction& txn, Octets& key, Octets& value)
	{
		Marshal::OctetsStream value_os = value;
		Marshal::OctetsStream key_os = key;

		GSysLog log;
		try
		{
			int64_t id;
			key_os >> id;
			value_os >> log;
			printf("%lld", id);
			printf(",%d", log.roleid);
			printf(",%d", log.time);
			printf(",%d", log.ip);
			printf(",%d", log.source);
			printf(",%d", log.money);
			for(GRoleInventoryVector::iterator it=log.items.begin(),ie=log.items.end();it!=ie;++it)
			{
				printf(",%d:%d:%d", it->id, it->pos, it->count);
			}
			printf("\n");
		} catch ( Marshal::Exception & ) {
			Log::log( LOG_ERR, "ListSysLogQuery, error unmarshal.");
			return true;
		}
		return true;
	}
};
void ListSysLog( )
{
	printf("guid");
	printf(",roleid");
	printf(",time");
	printf(",ip");
	printf(",source");
	printf(",money");
	printf(",items");
	printf("\n");

	ListSysLogQuery q;
	try
	{
		StorageEnv::Storage * pstorage = StorageEnv::GetStorage( "syslog" );
		StorageEnv::AtomTransaction	txn;
		try
		{
			StorageEnv::Storage::Cursor cursor = pstorage->cursor( txn );
			cursor.walk( q );
		}
		catch ( DbException e ) { throw; }
		catch ( ... )
		{
			DbException ee( DB_OLD_VERSION );
			txn.abort( ee );
			throw ee;
		}
	}
	catch ( DbException e )
	{
		Log::log( LOG_ERR, "ListSysLog, error when walk, what=%s\n", e.what() );
	}
}

void ReadRoleIDFile(const char *roleidfile, std::set<unsigned int> &roleidset)
{
	roleidset.clear();
	unsigned int roleid;

	std::ifstream ifs(roleidfile);
	if (ifs.fail())
		return;

	while (!ifs.eof())
	{
		ifs >> roleid;
		if (ifs.fail())
			break;
		roleidset.insert(roleid);
	}
}

void ExportRoleList(const char *roleidfile, const char *rolelistfile)
{
	if( 0 != access(roleidfile, R_OK) )
	{
		Log::log(LOG_ERR, "Fail to access roleid file: %s", roleidfile);
		return;
	}

	DBStandalone * pdb = new DBStandalone(rolelistfile);
	if(!pdb)
	{
		Log::log( LOG_ERR, "Cannot open table %s.", rolelistfile);
		return;
	}
	pdb->init();

	std::set<unsigned int> roleidset;
	ReadRoleIDFile(roleidfile, roleidset);

	int count = 0;
	StorageEnv::AtomTransaction txn;
	StorageEnv::Storage * pbase = StorageEnv::GetStorage("base");
	StorageEnv::Storage * pstatus = StorageEnv::GetStorage("status");
	StorageEnv::Storage * pinventory = StorageEnv::GetStorage("inventory");
	StorageEnv::Storage * pequipment = StorageEnv::GetStorage("equipment");
	StorageEnv::Storage * pstorehouse = StorageEnv::GetStorage("storehouse");
	StorageEnv::Storage * ptask = StorageEnv::GetStorage("task");

	for (std::set<unsigned int>::iterator itr=roleidset.begin(); itr!=roleidset.end(); itr++)
	{
		try
		{
			Marshal::OctetsStream oskey, osroledata;
			oskey << RoleId(*itr);

			GRoleData roledata;
			Octets ocvalue;
			try
			{
				if(!pbase->find(oskey, ocvalue, txn))
				{
					Log::log( LOG_ERR, "ExportRoleList, roleid=%d not found", *itr);
					continue;
				}
				Marshal::OctetsStream(ocvalue) >> roledata.base;
				if (!roledata.base.userid)
					roledata.base.userid = LOGICUID(roledata.base.id);
				roledata.base.spouse = 0;
				if(roledata.base.status == _ROLE_STATUS_CROSS_LOCKED || roledata.base.status == _ROLE_STATUS_CROSS_RECYCLE) roledata.base.status = _ROLE_STATUS_NORMAL;

				if (pstatus->find(oskey, ocvalue, txn))
					Marshal::OctetsStream(ocvalue) >> roledata.status;
				if (pinventory->find(oskey, ocvalue, txn))
					Marshal::OctetsStream(ocvalue) >> roledata.pocket;
				if (pequipment->find(oskey, ocvalue, txn))
					Marshal::OctetsStream(ocvalue) >> roledata.equipment;
				if (pstorehouse->find(oskey, ocvalue, txn))
					Marshal::OctetsStream(ocvalue) >> roledata.storehouse;
				if (ptask->find(oskey, ocvalue, txn))
					Marshal::OctetsStream(ocvalue) >> roledata.task;

				osroledata << roledata;
				pdb->put( oskey.begin(), oskey.size(), osroledata.begin(), osroledata.size() );

				count++;
			}
			catch ( DbException e ) { throw; }
			catch ( ... )
			{
				DbException ee( DB_OLD_VERSION );
				txn.abort( ee );
				throw ee;
			}
		}
		catch ( DbException e )
		{
			Log::log( LOG_ERR, "ExportRoleList, error when find roleid=%d, what=%s\n", *itr, e.what());
		}
	}

	pdb->checkpoint();
	delete pdb;

	Log::log( LOG_INFO, "Export %d roles to %s", count, rolelistfile );
}

static unsigned int g_nextlogicuid = 1024;

class UserLogicUIDQuery : public StorageEnv::IQuery
{
public:
	bool Update( StorageEnv::Transaction& txn, Octets& key, Octets& value)
	{
		User user;
		Marshal::OctetsStream(value) >> user;

		if (g_nextlogicuid <= user.logicuid)
			g_nextlogicuid = user.logicuid+16;

		return true;
	}
};

void InitNextLogicUID()
{
	UserLogicUIDQuery q;
	try
	{
		StorageEnv::Storage *puser = StorageEnv::GetStorage("user");
		StorageEnv::AtomTransaction txn;
		try
		{
			StorageEnv::Storage::Cursor cursor = puser->cursor(txn);
			cursor.walk(q);
		}
		catch ( DbException e ) { throw; }
		catch ( ... )
		{
			DbException ee( DB_OLD_VERSION );
			txn.abort( ee );
			throw ee;
		}
	}
	catch ( DbException e )
	{
		Log::log( LOG_ERR, "InitNextLogicUID, error when walk, what=%s\n", e.what() );
	}
}

class ImportRoleListQuery : public IQueryData
{
public:
	int count;

	bool update( const void *key, unsigned int key_len, const void *val, unsigned int val_len )
	{
		StorageEnv::Storage * puser = StorageEnv::GetStorage("user");
		StorageEnv::Storage * pbase = StorageEnv::GetStorage("base");
		StorageEnv::Storage * pstatus = StorageEnv::GetStorage("status");
		StorageEnv::Storage * pinventory = StorageEnv::GetStorage("inventory");
		StorageEnv::Storage * pequipment = StorageEnv::GetStorage("equipment");
		StorageEnv::Storage * pstorehouse = StorageEnv::GetStorage("storehouse");
		StorageEnv::Storage * ptask = StorageEnv::GetStorage("task");
		StorageEnv::Storage * prolename = StorageEnv::GetStorage("rolename");


		GRoleData roledata;
		Octets dummy(val, val_len);
		Marshal::OctetsStream(dummy) >> roledata;

		Marshal::OctetsStream osuserid, osroleid, osuser;
		osuserid << roledata.base.userid;

		try
		{
			User user;
			StorageEnv::AtomTransaction txn;

			try
			{
				if (puser->find(osuserid, osuser, txn))
				{
					osuser >> user;
					if(user.logicuid == 0)
					{
						Log::log(LOG_INFO, "ImportRoleListQuery, skip roleid=%d because user.logicuid=0", roledata.base.id);
						return true;
					}
				}
				else
				{
					user.logicuid = g_nextlogicuid;
					g_nextlogicuid += 16;
				}

				RoleList rolelist(user.rolelist);
				if (!rolelist.IsRoleListInitialed())
					rolelist.InitialRoleList();
				if (rolelist.GetRoleCount() >= 8)
				{
					Log::log(LOG_INFO, "ImportRoleListQuery, skip roleid=%d because role number exceeds 8", roledata.base.id);
					return true;
				}
				//int oldroleid = roledata.base.id;
				int roleid = rolelist.AddRole();
				roleid += user.logicuid;
				osroleid << roleid;
				user.rolelist = rolelist.GetRoleList();
				roledata.base.id = roleid;

				Octets dummyroleid, newrolename=roledata.base.name;
				int postfix = 1;
				while (prolename->find(newrolename, dummyroleid, txn))
				{
					char bufpostfix[32] = {0};
					snprintf(bufpostfix, 16, "%d", postfix);
					postfix++;
					int len = (int)strlen(bufpostfix);
					for (int i=len-1; i>=0; i--)
					{
						bufpostfix[i*2] = bufpostfix[i];
						bufpostfix[i*2+1] = 0;
					}
					newrolename = roledata.base.name;
					newrolename.insert(newrolename.end(), bufpostfix, 2*len);
				}
				roledata.base.name = newrolename;
				prolename->insert(roledata.base.name, osroleid, txn);

				puser->insert(osuserid, Marshal::OctetsStream()<<user, txn);
				pbase->insert( osroleid, Marshal::OctetsStream() << roledata.base, txn );
				pstatus->insert( osroleid, Marshal::OctetsStream() << roledata.status, txn );
				pinventory->insert( osroleid, Marshal::OctetsStream() << roledata.pocket, txn );
				pequipment->insert( osroleid, Marshal::OctetsStream() << roledata.equipment, txn );
				pstorehouse->insert( osroleid, Marshal::OctetsStream() << roledata.storehouse,txn);
				ptask->insert(osroleid, Marshal::OctetsStream() << roledata.task, txn);

				count++;
			}
			catch (DbException e) { throw; }
			catch ( ... )
			{
				DbException ee( DB_OLD_VERSION );
				txn.abort( ee );
				throw ee;
			}
		}
		catch (DbException e)
		{
			Log::log(LOG_ERR, "ImportRoleListQuery, error when find user, what=%s\n", e.what());
		}
		return true;
	}
};

void ImportRoleList(const char *rolelistfiles[], int filecount)
{
	for (int i = 0; i < filecount; i++)
	{
		if( 0 != access(rolelistfiles[i], R_OK) )
		{
			Log::log(LOG_ERR, "Fail to access rolelist file: %s", rolelistfiles[i]);
			return;
		}
	}

	InitNextLogicUID();
	
	int count = 0;
	for (int i = 0; i < filecount; i++)
	{
		DBStandalone * pdb = new DBStandalone( rolelistfiles[i] );
		if(!pdb)
		{
			Log::log( LOG_ERR, "Cannot open table %s.", rolelistfiles[i] );
			return;
		}
		pdb->init();

		ImportRoleListQuery q;
		q.count = 0;

		try
		{
			try
			{
				pdb->walk(&q);
			}
			catch ( DbException e ) { throw; }
			catch ( ... )
			{
				DbException ee( DB_OLD_VERSION );
				throw ee;
			}
		}
		catch ( DbException e )
		{
			Log::log( LOG_ERR, "ImportRoleList, error when walk, what=%s\n", e.what() );
		}

		delete pdb;
		count += q.count;

		Log::log( LOG_INFO, "Import %d roles from %s", q.count, rolelistfiles[i]);
	}

	StorageEnv::checkpoint();
	Log::log( LOG_INFO, "Totally import %d roles.", count);
}

template<typename KEY, typename VALUE>
class WalkTableQuery : public StorageEnv::IQuery
{
public:
	int count;
	int err_count;
	KEY k;
	VALUE v;
	WalkTableQuery():count(0),err_count(0){}
	
	bool Update( StorageEnv::Transaction& txn, Octets& key, Octets& value)
	{
		++count;
		try
		{
			Marshal::OctetsStream(key) >> k;
			Marshal::OctetsStream(value) >> v;
		}
		catch(...)
		{
			++err_count;
			return true;	
		}
		return true;
	}

	void OutputExtraInfo(){}
};

template<typename KEY>
class WalkTableQuery<KEY,void *> : public StorageEnv::IQuery
{
public:
	int count;
	int err_count;
	KEY k;
	WalkTableQuery():count(0),err_count(0){}
	
	bool Update( StorageEnv::Transaction& txn, Octets& key, Octets& value)
	{
		++count;
		try
		{
			Marshal::OctetsStream(key) >> k;
		}
		catch(...)
		{
			++err_count;
			return true;	
		}
		return true;
	}

	void OutputExtraInfo(){}
};

template<typename VALUE>
class WalkTableQuery<void *,VALUE> : public StorageEnv::IQuery
{
public:
	int count;
	int err_count;
	VALUE v;
	WalkTableQuery():count(0),err_count(0){}
	
	bool Update( StorageEnv::Transaction& txn, Octets& key, Octets& value)
	{
		++count;
		try
		{
			Marshal::OctetsStream(value) >> v;
		}
		catch(...)
		{
			++err_count;
			return true;	
		}
		return true;
	}

	void OutputExtraInfo(){}
};

template<>
class WalkTableQuery<void *,void *> : public StorageEnv::IQuery
{
public:
	int count;
	int err_count;
	WalkTableQuery():count(0),err_count(-1){}
	
	bool Update( StorageEnv::Transaction& txn, Octets& key, Octets& value)
	{
		++count;
		return true;
	}

	void OutputExtraInfo(){}
};

/*template<>
class WalkTableQuery<int, GFriendList> : public StorageEnv::IQuery
{
public:
	int count;
	int err_count;
	std::map<int, int> count_distribution;
	int k;
	GFriendList v;
	WalkTableQuery():count(0),err_count(0){}
	
	bool Update( StorageEnv::Transaction& txn, Octets& key, Octets& value)
	{
		++count;
		try
		{
			Marshal::OctetsStream(key) >> k;
			Marshal::OctetsStream(value) >> v;
		}
		catch(...)
		{
			++err_count;
			return true;	
		}
		count_distribution[v.friends.size()/10] += 1;	
		return true;
	}

	void OutputExtraInfo()
	{
		printf("friend count distribution:\n");
		for(size_t i=0; i<26; i++)
		{
			printf("%3d-%3d:\t%10d:\t%.2f%%\n",10*i,10*i+9,count_distribution[i],100.f*count_distribution[i]/count);	
		}
	}
};*/

template<typename KEY, typename VALUE>
void WalkTable(const char * tablename)
{
	printf("walk table %s\n", tablename);
	
	WalkTableQuery<KEY, VALUE> q;
	try
	{
		StorageEnv::Storage * pstorage = StorageEnv::GetStorage( tablename );
		StorageEnv::AtomTransaction     txn;
		try
		{
			StorageEnv::Storage::Cursor cursor = pstorage->cursor( txn );
			cursor.walk( q );
		}
		catch ( DbException e ) { throw; }
		catch ( ... )
		{
			DbException ee( DB_OLD_VERSION );
			txn.abort( ee );
			throw ee;
		}
	}
	catch ( DbException e )
	{
		Log::log( LOG_ERR, "WalkTable(%s), error when walk, what=%s\n", tablename, e.what() );
	}
	printf("\tcount=%d err_count=%d\n", q.count, q.err_count);
	q.OutputExtraInfo();
}

void WalkAllTable()
{
	typedef std::map<std::string, void(*)(const char *)> MAP;
	typedef std::map<std::string, void(*)(const char *)>::iterator ITERATOR;
	MAP map_table_func;
	map_table_func["auction"] 		= WalkTable<int,GAuctionDetail>;
	map_table_func["base"] 			= WalkTable<int,GRoleBase>;
	map_table_func["city"]			= WalkTable<int,GTerritoryStore>;
	map_table_func["equipment"] 	= WalkTable<int,GRoleEquipment>;
	map_table_func["factioninfo"] 	= WalkTable<int,GFactionInfo>;
	map_table_func["factionname"] 	= WalkTable<Octets,int>;
	map_table_func["friends"] 		= WalkTable<int,GFriendList>;
	map_table_func["gtask"] 		= WalkTable<int,void *>;
	map_table_func["inventory"] 	= WalkTable<int,GRolePocket>;
	map_table_func["mailbox"] 		= WalkTable<int,GMailBox>;
	map_table_func["messages"] 		= WalkTable<int,MessageVector>;
	map_table_func["rolename"] 		= WalkTable<void *,int>;
	map_table_func["status"] 		= WalkTable<int,GRoleStatus>;
	map_table_func["storehouse"] 	= WalkTable<int,GRoleStorehouse>;
	map_table_func["task"] 			= WalkTable<int,GRoleTask>;
	map_table_func["user"] 			= WalkTable<int,User>;
	map_table_func["userfaction"] 	= WalkTable<int,GUserFaction>;
	map_table_func["waitdel"] 		= WalkTable<int,int>;
	map_table_func["order"] 		= WalkTable<int,StockOrder>;
	map_table_func["syslog"] 		= WalkTable<int64_t,GSysLog>;
	map_table_func["userstore"] 	= WalkTable<int,GUserStorehouse>;
	map_table_func["webtrade"] 		= WalkTable<int64_t,GWebTradeDetail>;
	map_table_func["webtradesold"] 	= WalkTable<int64_t,GWebTradeDetail>;
	map_table_func["serverdata"] 	= WalkTable<int,GServerData>;

	std::string data_dir = StorageEnv::get_datadir();
	
	struct dirent **namelist;
	int n = scandir(data_dir.c_str(), &namelist, 0, alphasort);
	struct stat filestat;
	char indir[1024];

	if (n < 0)
	{
		perror("scandir");
		return;
	}
	else
	{
		while(n--)
		{
			sprintf(indir,"%s/%s",data_dir.c_str(),namelist[n]->d_name);
			stat(indir,&filestat);
			if(!S_ISDIR(filestat.st_mode) 
					&& 0 != strcmp(STORAGE_CONFIGDB,namelist[n]->d_name) 
					&& 0 != strcmp("clsconfig",namelist[n]->d_name) )
			{
				std::string dbname = namelist[n]->d_name;
				ITERATOR it = map_table_func.find(dbname);
				if(it != map_table_func.end())
				{
					(it->second)(dbname.c_str());
					printf("Size in bytes: %lld\n",filestat.st_size);
				}
			}
			free(namelist[n]);
		}
		free(namelist);
	}
}

namespace {

bool writeBuffer(int fd, char *buf, size_t size) {
	ssize_t cnt;
	while(size > 0) {
		if((cnt = write(fd, buf, size)) < 0) {
			return false;
		} else {
			buf += cnt;
			size -= cnt;
		}
	}
	return true;
}

bool writeOctetsStreamToFile(int fd, const Marshal::OctetsStream &os) {
	Marshal::OctetsStream marshal;
	marshal << Octets(os);
	return writeBuffer(fd, (char *)marshal.begin(), marshal.size());
}

}

class GTRoleDataExporter : public StorageEnv::IQuery
{
	int _fdrole;
	int _fdfriend;
	StorageEnv::Storage * _pbase;
	StorageEnv::Storage * _pstatus;
	StorageEnv::Storage * _pfriend;
	int _total;
	int _badUser;
	int _badRole;
	int _badFriend;
	int _oldRole;

	bool _ExportRole(StorageEnv::Transaction& txn, int roleid, unsigned int uid) {
		GRoleBase base;
		Marshal::OctetsStream sbase;
		RoleId rid((unsigned int)roleid);
		if(_pbase->find(Marshal::OctetsStream() << rid, sbase, txn)) {
			try {
				sbase >> base;
			} catch(...) {
				++_badRole;
				printf("Export role error: bad GRoleBase with id:%u", rid.id);
				return true;
			}
		} else
			return true;

		if(base.status == _ROLE_STATUS_MUSTDEL) return true;
		
		if(Timer::GetTime() - base.lastlogin_time > 15552000)
		{
			_oldRole++;
			return true;
		}
			
		GRoleStatus status;
		Marshal::OctetsStream sstatus;
		if(_pstatus->find(Marshal::OctetsStream() << rid, sstatus, txn)) {
			try {
				sstatus >> status;
			} catch(...) {
				++_badRole;
				printf("Export role error: bad GRoleStatus with id:%u", rid.id);
				return true;
			}
		} else 
			return true;

		RoleImportBean role;
		role.uid = (int64_t)uid;
		role.roleid = (int64_t)base.id;
		role.rolename.swap(base.name);
		role.gender = base.gender;
		role.race = 0;
		role.occupation = base.cls;
		role.level = status.level;

		Marshal::OctetsStream srole;
		srole << role;
		return writeOctetsStreamToFile(_fdrole, srole) && _ExportFriend(txn, roleid, base.spouse);
	}

	bool _ExportFriend(StorageEnv::Transaction& txn, int roleid, unsigned int spouse) {
		FriendImportBean friends;
		friends.roleid = (int64_t)roleid;

		GFriendList list;
		Marshal::OctetsStream slist;
		RoleId rid((unsigned int)roleid);

		if(_pfriend->find(Marshal::OctetsStream() << rid, slist, txn)) {
			try {
				slist >> list;
			} catch(...) {
				++_badFriend;
				printf("Export role error: bad GFriendList with id:%u", rid.id);
				return true;
			}

			std::map<char, int> gmap;	//map group id to index
			int index = 0;

			GroupBean group;
			group.gtype = GT_GROUP_NORMAL;
			group.groupid = 0;
			gmap[0] = index++;
			friends.friends.push_back(group);

			std::vector<GGroupInfo>::iterator git = list.groups.begin();
			std::vector<GGroupInfo>::iterator gend = list.groups.end();
			for(; git != gend; ++git) {
				GroupBean group;
				group.gtype = GT_GROUP_NORMAL;
				group.groupid = git->gid;
				group.groupname.swap(git->name);
				gmap[git->gid] = index++;
				friends.friends.push_back(group);
			}

			std::vector<GFriendInfo>::iterator fit = list.friends.begin();
			std::vector<GFriendInfo>::iterator fend = list.friends.end();
			for(; fit != fend; ++fit) {
				friends.friends[gmap[fit->gid]].friendlist.push_back((int64_t)fit->rid);
			}
		}

		if(spouse) {
			GroupBean group;
			group.gtype = GT_GROUP_SPOUSE;
			group.friendlist.push_back((int64_t)spouse);
			friends.friends.push_back(group);
		}

		Marshal::OctetsStream sfriend;
		sfriend << friends;
		return writeOctetsStreamToFile(_fdfriend, sfriend);
	}

public:
	GTRoleDataExporter(int fdrole, int fdfriend) : _fdrole(fdrole), _fdfriend(fdfriend) {
		_pbase = StorageEnv::GetStorage("base");
		_pstatus = StorageEnv::GetStorage("status");
		_pfriend = StorageEnv::GetStorage("friends");
		_total = _badUser = _badRole = _badFriend = _oldRole = 0;
	}

	bool Update( StorageEnv::Transaction& txn, Octets& key, Octets& value)
	{
		UserID uid;
		User user;
		try {
			Marshal::OctetsStream(key) >> uid;
		} catch(...) {
			++_badUser;
			printf("Export role error: bad UserID.\n");
			return true;
		}

		try {
			Marshal::OctetsStream(value) >> user;
		} catch(...) {
			++_badUser;
			printf("Export role error: bad User with id:%u.\n", uid.id);
			return true;
		}

		if(0 != user.logicuid) {
			RoleList rolelist(user.rolelist);
			int roleid;
			while((roleid = rolelist.GetNextRole()) >= 0) {
				int r = user.logicuid + roleid;
				if(!_ExportRole(txn, r, uid.id)) {
					printf("Write file error.\n");
					return false;
				} else {
					_total++;
				}
			}
		}
		return true;
	}

	void statistic() {
		printf("Export %d roles successful.\n", _total);
		printf("Found %d old roles.\n", _oldRole);
		printf("Found %d bad users.\n", _badUser);
		printf("Found %d bad roles.\n", _badRole);
		printf("Found %d bad friends.\n", _badFriend);
	}
};

class GTFactionDataExporter : public StorageEnv::IQuery
{
	int _fdfaction;
	int _total;
	int _badFaction;

	enum {
		TitleOffset = 2,
		TitleCount = 5
	};
public:
	GTFactionDataExporter(int fdfaction) : _fdfaction(fdfaction), _total(0), _badFaction(0) {}

	bool Update( StorageEnv::Transaction& txn, Octets& key, Octets& value)
	{
		FactionId fid;
		GFactionInfo info;

		try {
			Marshal::OctetsStream(key) >> fid;
		}  catch(...) {
			++_badFaction;
			printf("Export faction error: bad FactionId.\n");
			return true;
		}
		try {
			Marshal::OctetsStream(value) >> info;
		} catch(...) {
			++_badFaction;
			printf("Export faction error: bad GFactionInfo with id:%u.\n", fid.fid);
			return true;
		}
		FactionImportBean faction;
		faction.ftype = GT_FACTION_TYPE;
		faction.factionid = (int64_t)fid.fid;
		faction.factionname.swap(info.name);
		faction.announcement.swap(info.announce);

		for(int i = 0; i < TitleCount; ++i) {
			TitleBean title;
			title.titleid = TitleOffset + i;
			faction.members.push_back(title);
		}

		GMemberVector::iterator it = info.member.begin();
		GMemberVector::iterator end = info.member.end();
		for(; it != end ; ++it) {
			if(it->role < TitleOffset || it->role > (TitleOffset + TitleCount - 1)) continue;
			faction.members[it->role - TitleOffset].members.push_back((int64_t)it->rid);
		}

		Marshal::OctetsStream sfaction;
		sfaction << faction;
		if(!writeOctetsStreamToFile(_fdfaction, sfaction)) {
			printf("Write file error.\n");
			return false;
		} else  {
			++_total;
			return true;
		}
	}

	void statistic() {
		printf("Export %d factions successful.\n", _total);
		printf("Found %d bad factions.\n", _badFaction);
	}
};

void ExportGameTalkData(const char *outputdir)
{
	if(mkdir(outputdir, 0755) == -1) {
		printf("mkdir %s failed.\n", outputdir);
		return;
	}

	char buf[256];
	snprintf(buf, 256, "%s/gt_role.data", outputdir);
	int fdrole = open(buf, O_WRONLY | O_CREAT, 0644);
	snprintf(buf, 256, "%s/gt_friend.data", outputdir);
	int fdfriend = open(buf, O_WRONLY | O_CREAT, 0644);
	snprintf(buf, 256, "%s/gt_faction.data", outputdir);
	int fdfaction = open(buf, O_WRONLY | O_CREAT, 0644);

	GTRoleDataExporter rde(fdrole, fdfriend);
	GTFactionDataExporter fde(fdfaction);
	try
	{
		StorageEnv::Storage *puser = StorageEnv::GetStorage("user");
		StorageEnv::Storage *pfaction = StorageEnv::GetStorage("factioninfo");
		StorageEnv::AtomTransaction txn;
		try
		{
//			StorageEnv::Storage::Cursor cursor = pstorage->cursor( txn );
			puser->cursor(txn).walk(rde);
			pfaction->cursor(txn).walk(fde);
		}
		catch (DbException e) { throw; }
		catch (...)
		{
			DbException ee(DB_OLD_VERSION);
			txn.abort(ee);
			throw ee;
		}
	}
	catch (DbException e)
	{
		Log::log(LOG_ERR, "ExportGameTalkData error, what=%s\n", e.what());
	}
	rde.statistic();
	fde.statistic();
	close(fdrole);
	close(fdfriend);
	close(fdfaction);
}

void ExportUserStore(int roleid)
{
	try
	{
		StorageEnv::Storage * pbase = StorageEnv::GetStorage("base");
		StorageEnv::Storage * puserstore = StorageEnv::GetStorage("userstore"); 
		StorageEnv::AtomTransaction txn;
		Marshal::OctetsStream key1,key2;
		GUserStorehouse user_data;
		GRoleBase base_data;
		try
		{
			key1 << roleid;
			Marshal::OctetsStream(pbase->find(key1,txn)) >> base_data;
			printf("userid = %d\n",base_data.userid);
			if(base_data.userid)
			{
				key2 << base_data.userid;
			}
			else
			{
				key2 << LOGICUID(roleid);
			}
			Marshal::OctetsStream(puserstore->find(key2,txn)) >> user_data;
			
			XmlCoder coder;
			coder.append_header();
			coder.append("userstorehouse",user_data);
			puts(coder.c_str());
			return;
		}
		catch ( DbException e ) { throw; }
		catch ( ... )
		{
			DbException ee( DB_OLD_VERSION );
			txn.abort( ee );
			throw ee;
		}
	}
	catch ( DbException e )
	{
		Log::log( LOG_ERR, "ExportUserStore, roid=%d, what=%s\n", roleid, e.what() );
	}
	return;
}

class ProfitTimeQuery : public StorageEnv::IQuery
{
	int today_time;
	unsigned int count;
public:
	bool hasmore;
	Octets handle;
	StorageEnv::Storage * pstatus;
	explicit ProfitTimeQuery(int t_today_time) : today_time(t_today_time), count(0), hasmore(false)
	{
		pstatus = StorageEnv::GetStorage("status");
	}

	bool Update( StorageEnv::Transaction& txn, Octets& key, Octets& value)
	{
		if(++count % 10000 == 0)
		{
			handle = key;
			hasmore = true;
			return false;
		}
		RoleId id;
		GRoleBase base;
		GRoleStatus status;
		Marshal::OctetsStream key_os, value_os;
		key_os = key;
		value_os = value;
		try{
			key_os >> id;
			value_os >> base;
			Marshal::OctetsStream( pstatus->find( key, txn ) ) >> status;
		}
		catch ( ... )
		{
			Log::log( LOG_ERR, "ProfitTimeQuery error roleid=%d\n", id.id );
			return true;
		}
		if (base.lastlogin_time < today_time)
			return true;
		if (status.profit_time_data.size() == 2*sizeof(int))
		{
			int profit_time = *(int*)(status.profit_time_data.begin());
			printf("%d: %d: %d: %d\n",id.id, base.cls, status.level, profit_time);
		}
		return true;
	}
};

void ExportProfitTime()
{
	printf("roleid: ");
	printf("cls: ");
	printf("level: ");
	printf("profit_time_left");
	printf("\n");
	time_t ntime = time(NULL);
	struct tm ntimetm;
	localtime_r(&ntime, &ntimetm);
	ntimetm.tm_hour = 0;
	ntimetm.tm_min = 0;
	ntimetm.tm_sec = 0;
	ProfitTimeQuery q(mktime(&ntimetm));
	do{
		q.hasmore = false;
		try
		{
			StorageEnv::Storage * pbase = StorageEnv::GetStorage("base");
			StorageEnv::AtomTransaction     txn;
			try
			{
				StorageEnv::Storage::Cursor cursor = pbase->cursor( txn );
				cursor.walk( q.handle, q );
			}
			catch ( DbException e ) { throw; }
			catch ( ... )
			{
				DbException ee( DB_OLD_VERSION );
				txn.abort( ee );
				throw ee;
			}
		}
		catch ( DbException e )
		{
			Log::log( LOG_ERR, "ExportProfitTime error when walk, what=%s\n", e.what() );
		}
	}while(q.hasmore);
}

enum ITEM_POSITION
{
	PLAYER_POCKET = 0,
	PLAYER_EQUIPMENT,
	PLAYER_STOREHOUSE,
	PLAYER_MAILBOX,
	AUCTION,
	USER_STOREHOUSE,
	WEB_TRADE,
	MAX_ITEM_POSITION,
};

class RoleInvQuery;

class RoleInvQueryMgr
{
public:
	RoleInvQueryMgr()
    : sec_type_(NULLSECURITY) {}
	
	~RoleInvQueryMgr() {}
	
	void LoadValuableItems(const char *filename)
	{
		std::ifstream ifs(filename);
		string line;
		if(!valuable_items_.empty())
		{
			valuable_items_.clear();
		}
		
		while(std::getline(ifs, line))
		{
			int itemid = atoi(line.c_str());
			if(0 != itemid) {
				valuable_items_.insert(itemid);
			}
		}
	}

	bool IsValuableItem(int itemid)
	{
		return (valuable_items_.find(itemid) != valuable_items_.end());
	}

	void InitSecurity(Security::Type type, Octets &key)
	{
		sec_key_ = key;
		sec_type_ = type;
	}
	
	Octets &Encrypt(Octets &value)
	{
		Security *sec = Security::Create(sec_type_);
		sec->SetParameter(sec_key_);
		sec->Update(value);
		sec->Destroy();
		return value;
	}
	
	// 由 ActiveRoleQuery 调用
	void AddActiveRoleRecentWeek(int roleid) {
		active_roles_.insert(roleid);
	}
	
	bool LoadActiveRolesRecentWeek();
	
	bool IsLoginRecentWeek(int roleid)
	{
		return (active_roles_.find(roleid) != active_roles_.end());
	}

	void PrintInventorySnapshotField()
	{
		printf("roleid,");
		printf("item_position,");
		printf("item_id,");
		printf("item_data,"); // 记录的是加密后的数据
		printf("item_checksum\n");
	}

	void PrintInventoryInfo(int roleid, int position, GRoleInventory& inventory)
	{
		printf("%d,", roleid);
		printf("%d,", position);
		printf("%d,", inventory.id);
		Marshal::OctetsStream os;
		os << inventory;
		Octets checksum = MD5Hash::Digest(os);
	
		Encrypt(os);
		XmlCoder coder;
		printf("%s,", coder.toString(os).c_str());
		printf("%s\n", coder.toString(checksum).c_str());
	}

	void BuildSnapshot(const std::vector<string>& inv_tables);
	
private:
	RoleInvQuery* CreateQuery(const string &table_name);

	// 不许复制
	RoleInvQueryMgr(const RoleInvQueryMgr&);
	RoleInvQueryMgr& operator=(const RoleInvQueryMgr&);

	typedef std::set<int> ValuableItems;
	typedef std::set<int> ActiveRoles;

	Octets sec_key_;
	Security::Type sec_type_;
	ValuableItems valuable_items_;
	ActiveRoles active_roles_; // 最近一周登录过的角色
};

class RoleInvQuery : public StorageEnv::IQuery
{
public:
	friend class RoleInvQueryMgr;
	explicit RoleInvQuery(RoleInvQueryMgr& mgr):mgr_(mgr), hasmore_(false), count_(0) {}

	virtual bool Update( StorageEnv::Transaction& txn, Octets& key, Octets& value)
	{
		// 是否需要暂停以回收内存
		if (++count_ % 10000 == 0)
		{
			handle_ = key;
			hasmore_ = true;
			return false;
		}
		else
		{
			return DoUpdate(txn, key, value);
		}
	}

protected:
	virtual bool DoUpdate( StorageEnv::Transaction& txn, Octets& key, Octets& value) = 0;
	
	RoleInvQueryMgr& mgr_;
	
private:
	// 不许复制
	RoleInvQuery(const RoleInvQuery&);
	RoleInvQuery& operator=(const RoleInvQuery&);
	
	bool hasmore_;
	Octets handle_;
	int count_;
};

class InventoryQuery : public RoleInvQuery
{
public:
	explicit InventoryQuery(RoleInvQueryMgr& mgr): RoleInvQuery(mgr) {}

protected:
	virtual bool DoUpdate( StorageEnv::Transaction& txn, Octets& key, Octets& value)
	{
		Marshal::OctetsStream key_os, value_os;
		key_os = key;
		value_os = value;

		RoleId id;
		GRolePocket inventory;
		try
		{
			key_os >> id;
			if(!mgr_.IsLoginRecentWeek(id.id))
			{
				return true;
			}
			
			value_os >> inventory;
			for(unsigned int i = 0; i < inventory.items.size(); ++i)
			{
				if(mgr_.IsValuableItem(inventory.items[i].id))
				{
					mgr_.PrintInventoryInfo(id.id, PLAYER_POCKET, inventory.items[i]);
				}
			}
		}
		catch(Marshal::Exception &)
		{
			Log::log(LOG_ERR, "InventoryQuery, error unmarshal, roleid=%d.", id.id);
			return true;
		}
		
		return true;
	}
};

class EquipmentQuery : public RoleInvQuery
{
public:
	explicit EquipmentQuery(RoleInvQueryMgr& mgr): RoleInvQuery(mgr) {}

protected:
	virtual bool DoUpdate( StorageEnv::Transaction& txn, Octets& key, Octets& value)
	{
		Marshal::OctetsStream key_os, value_os;
		key_os = key;
		value_os = value;

		RoleId id;
		GRoleEquipment equipment;
		try
		{
			key_os >> id;
			if(!mgr_.IsLoginRecentWeek(id.id))
			{
				return true;
			}
			
			value_os >> equipment;
			for(unsigned int i = 0; i < equipment.inv.size(); ++i)
			{
				if(mgr_.IsValuableItem(equipment.inv[i].id))
				{
					mgr_.PrintInventoryInfo(id.id, PLAYER_EQUIPMENT, equipment.inv[i]);
				}
			}
		}
		catch(Marshal::Exception &)
		{
			Log::log(LOG_ERR, "EquipmentQuery, error unmarshal, roleid=%d.", id.id);
			return true;
		}
		
		return true;
	}
};

class StorehouseQuery : public RoleInvQuery
{
public:
	explicit StorehouseQuery(RoleInvQueryMgr& mgr): RoleInvQuery(mgr) {}

protected:
	void Walk(GRoleInventoryVector& vec, unsigned int roleid)
	{
		for(unsigned int i = 0; i < vec.size(); ++i)
		{
			if(mgr_.IsValuableItem(vec[i].id))
			{
				mgr_.PrintInventoryInfo(roleid, PLAYER_STOREHOUSE, vec[i]);
			}
		}
	}
	
	virtual bool DoUpdate( StorageEnv::Transaction& txn, Octets& key, Octets& value)
	{
		Marshal::OctetsStream key_os, value_os;
		key_os = key;
		value_os = value;

		RoleId id;
		GRoleStorehouse storehouse;
		try
		{
			key_os >> id;
			if(!mgr_.IsLoginRecentWeek(id.id))
			{
				return true;
			}
			value_os >> storehouse;
			
			Walk(storehouse.items, id.id);
			Walk(storehouse.dress, id.id); //材料
			Walk(storehouse.material, id.id); //时装
			Walk(storehouse.generalcard, id.id);
		}
		catch(Marshal::Exception &)
		{
			Log::log(LOG_ERR, "StorehouseQuery, error unmarshal, roleid=%d.", id.id);
			return true;
		}
		
		return true;
	}
};

class MailboxQuery: public RoleInvQuery
{
public:
	explicit MailboxQuery(RoleInvQueryMgr& mgr): RoleInvQuery(mgr) {}

protected:
	virtual bool DoUpdate( StorageEnv::Transaction& txn, Octets& key, Octets& value)
	{
		Marshal::OctetsStream key_os, value_os;
		key_os = key;
		value_os = value;

		RoleId id;
		GMailBox mailbox;
		try
		{
			key_os >> id;
			if(!mgr_.IsLoginRecentWeek(id.id))
			{
				return true;
			}
			value_os >> mailbox;

			for(unsigned int i = 0; i < mailbox.mails.size(); ++i)
			{
				if((mailbox.mails[i].header.attribute & _MA_ATTACH_OBJ) && mgr_.IsValuableItem(mailbox.mails[i].attach_obj.id))
				{
					mgr_.PrintInventoryInfo(id.id, PLAYER_MAILBOX, mailbox.mails[i].attach_obj);
				}
			}
		}
		catch(Marshal::Exception &)
		{
			Log::log(LOG_ERR, "MailboxQuery, error unmarshal, roleid=%d.", id.id);
			return true;
		}

		return true;
	}
};

class AuctionQuery: public RoleInvQuery
{
public:
	explicit AuctionQuery(RoleInvQueryMgr& mgr): RoleInvQuery(mgr) {}

protected:
	virtual bool DoUpdate( StorageEnv::Transaction& txn, Octets& key, Octets& value)
	{
		Marshal::OctetsStream key_os, value_os;
		key_os = key;
		value_os = value;

		GAuctionDetail auction;
		try
		{
			value_os >> auction;

			if(!mgr_.IsLoginRecentWeek(auction.info.seller))
			{
				return true;
			}

			if(mgr_.IsValuableItem(auction.item.id))
			{
				mgr_.PrintInventoryInfo(auction.info.seller, AUCTION, auction.item);
			}
		}
		catch(Marshal::Exception &)
		{
			Log::log(LOG_ERR, "AuctionQuery, error unmarshal, roleid=%d.", auction.info.seller);
			return true;
		}

		return true;
	}
};

class UserStoreQuery : public RoleInvQuery
{
public:
	explicit UserStoreQuery(RoleInvQueryMgr& mgr): RoleInvQuery(mgr) {}
	
protected:
	virtual bool DoUpdate( StorageEnv::Transaction& txn, Octets& key, Octets& value)
	{
		StorageEnv::Storage* puser = StorageEnv::GetStorage("user");

		Marshal::OctetsStream key_os, value_os;
		Marshal::OctetsStream value_user;
		key_os = key;
		value_os = value;

		User user;
		GUserStorehouse storehouse;

		unsigned int userid = 0;	
		unsigned int roleid = 0;
		try
		{
			key_os >> userid;
			if( puser->find( userid, value_user, txn) )
			{
				value_user >> user;
			}
			
			for(int i = 0; i < 16; ++i)
			{
				if(user.rolelist & (1 << i))
				{
					roleid = user.logicuid + i;
					break;
				}
			}
			
			if(!mgr_.IsLoginRecentWeek(roleid))
			{
				return true;
			}
			value_os >> storehouse;

			for(unsigned int i = 0; i < storehouse.items.size(); ++i)
			{
				if(mgr_.IsValuableItem(storehouse.items[i].id))
				{
					mgr_.PrintInventoryInfo(roleid, USER_STOREHOUSE, storehouse.items[i]);
				}
			}	
		}
		catch(Marshal::Exception &)
		{
			Log::log(LOG_ERR, "UserStorehouseQuery, error unmarshal, roleid=%d.", roleid);
			return true;
		}
		
		return true;
	}
};

class WebTradeQuery : public RoleInvQuery
{
public:
	explicit WebTradeQuery(RoleInvQueryMgr& mgr): RoleInvQuery(mgr) {}

protected:
	virtual bool DoUpdate( StorageEnv::Transaction& txn, Octets& key, Octets& value)
	{
		Marshal::OctetsStream key_os, value_os;
		key_os = key;
		value_os = value;

		GWebTradeDetail webtrade;
		try
		{
			int64_t sn;
			key_os >> sn;
			if(sn == 0) return true;

			value_os >> webtrade;
			if(!mgr_.IsLoginRecentWeek(webtrade.info.seller_roleid))
			{
				return true;
			}

			if((webtrade.info.posttype == 2) && mgr_.IsValuableItem(webtrade.item.id))
			{
				mgr_.PrintInventoryInfo(webtrade.info.seller_roleid, WEB_TRADE, webtrade.item);
			}
		}
		catch(Marshal::Exception &)
		{
			Log::log(LOG_ERR, "WebTradeQuery, error unmarshal, roleid=%d.", webtrade.info.seller_roleid);
		}
		
		return true;
	}
};

class ActiveRoleQuery : public StorageEnv::IQuery
{
public:
	explicit ActiveRoleQuery(RoleInvQueryMgr& mgr):hasmore_(false), count_(0), mgr_(mgr) {}

	bool Update(StorageEnv::Transaction& txn, Octets& key, Octets& value)
	{
		// 是否需要暂停以回收内存
		if (++count_ % 10000 == 0)
		{
			hasmore_ = true;
			handle_ = key;
			return false;
		}

		Marshal::OctetsStream key_os, value_os;
		key_os = key;
		value_os = value;

		RoleId id;
		GRoleBase base;
		try
		{
			key_os >> id;
			if (id.id == 0)
			{
				return true;
			}

			value_os >> base;

			static const int SECONDS_PER_WEEK = 7 * 86400;

			int now = time(NULL);
			if ((base.lastlogin_time != 0) && (now - base.lastlogin_time <= SECONDS_PER_WEEK))
			{
				mgr_.AddActiveRoleRecentWeek(id.id);
			}
		}
		catch (Marshal::Exception&)
		{
			Log::log( LOG_ERR, "ActiveRoleQuery, error unmarshal, roleid=%d.", id.id );
		}
		return true;
	}

public:
	bool hasmore_;
	Octets handle_;

private:
	int count_;
	RoleInvQueryMgr& mgr_;
};

bool RoleInvQueryMgr::LoadActiveRolesRecentWeek()
{
	ActiveRoleQuery query(*this);

	do
	{
		query.hasmore_ = false;

		try
		{
			StorageEnv::Storage* pstorage = StorageEnv::GetStorage("base");
			StorageEnv::Transaction txn;
			try
			{
				StorageEnv::Storage::Cursor cursor = pstorage->cursor(txn);
				cursor.walk(query.handle_, query);
			}
			catch(DbException) { throw; }
			catch(...)
			{
				DbException ee(DB_OLD_VERSION);
				txn.abort(ee);
				throw ee;
			}
		}
		catch(DbException e)
		{
			Log::log(LOG_ERR, "listroleinventory, error when walk, what=%s\n", e.what());
		}

		StorageEnv::checkpoint();
	} while (query.hasmore_);

	return true;
}
	
RoleInvQuery* RoleInvQueryMgr::CreateQuery(const string &table_name)
{
	if("inventory" == table_name) {
		return new InventoryQuery(*this);
	} else if("equipment" == table_name) {
		return new EquipmentQuery(*this);
	} else if("storehouse" == table_name) {
		return new StorehouseQuery(*this);
	} else if("mailbox" == table_name) {
		return new MailboxQuery(*this);
	} else if("auction" == table_name) {
		return new AuctionQuery(*this);
	} else if("userstore" == table_name) {
		return new UserStoreQuery(*this);
	} else if("webtrade" == table_name) {
		return new WebTradeQuery(*this);
	} else {
		return NULL;
	}
}

void RoleInvQueryMgr::BuildSnapshot(const std::vector<string>& inv_tables)
{
	PrintInventorySnapshotField();

	for(std::vector<string>::const_iterator it = inv_tables.begin(); it != inv_tables.end(); ++it)
	{
		RoleInvQuery* query = CreateQuery(*it);

		if (query == NULL)
		{
			Log::log(LOG_ERR, "listroleinventory, no query class written for %s\n", it->c_str());
			continue;
		}

		do
		{
			query->hasmore_ = false;

			try
			{
				StorageEnv::Storage* pinvstorage = StorageEnv::GetStorage(it->c_str());

				StorageEnv::Transaction txn;
				try
				{
					StorageEnv::Storage::Cursor cursor = pinvstorage->cursor(txn);
					cursor.walk(query->handle_, *query);
				}
				catch(DbException) { throw; }
				catch(...)
				{
					DbException ee(DB_OLD_VERSION);
					txn.abort(ee);
					throw ee;
				}
			}
			catch(DbException e)
			{
				Log::log(LOG_ERR, "listroleinventory, error when walk, what=%s\n", e.what());
			}

			StorageEnv::checkpoint();
		} while (query->hasmore_);

		delete query;
	}
}

void ListRoleInventory()
{
	RoleInvQueryMgr manager;

	Octets key(arc4_key_buf, 128);
	manager.InitSecurity(ARCFOURSECURITY, key);
	manager.LoadValuableItems("valuables_list.txt");
	manager.LoadActiveRolesRecentWeek();

	std::vector<string> table_names;
	table_names.push_back("inventory");
	table_names.push_back("equipment");
	table_names.push_back("storehouse");
	table_names.push_back("mailbox");
	table_names.push_back("auction");
	table_names.push_back("userstore");
	table_names.push_back("webtrade");

	manager.BuildSnapshot(table_names);
}

int GetDBCrossType()
{
	try {   
		StorageEnv::Storage* pstorage = StorageEnv::GetStorage("config");
		StorageEnv::CommonTransaction txn;
		
		try {
			DBConfig2 config;
			Marshal::OctetsStream(pstorage->find(Marshal::OctetsStream() << (int)DB_CONFIG2_KEY, txn)) >> config;
			LOG_TRACE("DB cross type %d", config.is_central_db);
			
			return config.is_central_db;
		} catch(DbException &e) { 
			throw;  
		} catch(...) { 
			DbException ee( DB_OLD_VERSION );
			txn.abort( ee );
			throw ee;
		}       
	} catch(DbException e) {       
		Log::log( LOG_ERR, "GetDBCrossType exception, what=%s\n", e.what() );
	}
	
	return -1;
}

void SetDBCrossType(bool is_central)
{
	try {   
		StorageEnv::Storage* pstorage = StorageEnv::GetStorage("config");
		StorageEnv::CommonTransaction txn;
		
		Marshal::OctetsStream key;
		key << (int)DB_CONFIG2_KEY;
		try {
			DBConfig2 config;
			Marshal::OctetsStream(pstorage->find(key, txn)) >> config;
			
			bool old_type = config.is_central_db;
			config.is_central_db = is_central;
			pstorage->insert(key, Marshal::OctetsStream() << config, txn);
			LOG_TRACE("DB change cross type %d->%d", old_type, config.is_central_db);
		} catch(DbException &e) { 
			throw;  
		} catch(...) { 
			DbException ee( DB_OLD_VERSION );
			txn.abort( ee );
			throw ee;
		}       
	} catch(DbException e) {       
		Log::log( LOG_ERR, "SetDBCrossType exception, what=%s\n", e.what() );
	}
}

int GetStandaloneDBCrossType(const char* srcpath)
{
	std::string src_dir = srcpath;
	
	DBStandalone* pstandalone = NULL;
	StorageEnv::Uncompressor* uncompressor = NULL;
	
	try
	{
		pstandalone = new DBStandalone((src_dir + "/config").c_str());
		pstandalone->init();
		uncompressor = new StorageEnv::Uncompressor();

		Marshal::OctetsStream t;
		t << (int)DB_CONFIG2_KEY;
		
		unsigned int val_len = 0; 
		if(void* val = pstandalone->find(t.begin(), (unsigned int)t.size(), &val_len))
		{  
			GNET::Octets dbval = uncompressor->Update(GNET::Octets(val, val_len));
			free(val);

			DBConfig2 config;
			Marshal::OctetsStream(dbval) >> config;
			LOG_TRACE("Standalone DB cross type %d", config.is_central_db);
			
			return config.is_central_db;
		}
		else
		{
			Log::log( LOG_ERR, "GetStandaloneDBCrossType exception2");
		}
	}
	catch( ... )
	{
		Log::log( LOG_ERR, "GetStandaloneDBCrossType exception");
	}

	if(pstandalone != NULL) 
	{
		delete pstandalone;
		pstandalone = NULL;
	}
	
	if(uncompressor != NULL)
	{
		delete uncompressor;
		uncompressor = NULL;
	}
	
	return -1;
}

void ListRoleCrossInfo(int roleid)
{
	printf("roleid");
	printf(",userid");
	printf(",remote_roleid");
	printf(",data_timestamp");
	printf(",cross_timestamp");
	printf(",src_zoneid");
	printf("\n");
	
	try {   
		StorageEnv::Storage* pbase = StorageEnv::GetStorage("base");
		StorageEnv::CommonTransaction txn;
		
		try {
			GRoleBase base;
			Marshal::OctetsStream(pbase->find(Marshal::OctetsStream() << roleid, txn)) >> base;
			
			CrossInfoData info;

			if(base.cross_data.size() > 0) {
				Marshal::OctetsStream(base.cross_data) >> info;
			}
			
			printf("%d", base.id);
			printf(",%d", 0 == base.userid ? (base.id & ~0x0000000F) : base.userid);
			printf(",%d", info.remote_roleid);
			printf(",%d", info.data_timestamp);
			printf(",%d", info.cross_timestamp);
			printf(",%d", info.src_zoneid);
		} catch(DbException &e) { 
			throw;  
		} catch(...) { 
			DbException ee( DB_OLD_VERSION );
			txn.abort( ee );
			throw ee;
		}       
	} catch(DbException e) {       
		Log::log( LOG_ERR, "ListRoleCrossInfo exception, what=%s\n", e.what() );
	}
}

void ResetRoleCrossInfo(int roleid, int remote_roleid, int data_timestamp, int cross_timestamp, int src_zoneid)
{
	try {   
		StorageEnv::Storage* pbase = StorageEnv::GetStorage("base");
		StorageEnv::CommonTransaction txn;
		Marshal::OctetsStream key;
		
		try {
			key << roleid;
			GRoleBase base;
			Marshal::OctetsStream(pbase->find(key, txn)) >> base;
			
			CrossInfoData info;
			info.remote_roleid = remote_roleid;
			info.data_timestamp = data_timestamp;
			info.cross_timestamp = cross_timestamp;
			info.src_zoneid = src_zoneid;
			
			base.cross_data = Marshal::OctetsStream() << info;
			pbase->insert(key, Marshal::OctetsStream() << base, txn);
		} catch(DbException &e) { 
			throw;  
		} catch(...) { 
			DbException ee( DB_OLD_VERSION );
			txn.abort( ee );
			throw ee;
		}       
	} catch(DbException e) {       
		Log::log( LOG_ERR, "ResetRoleCrossInfo exception, what=%s\n", e.what() );
	}
}

class SyncPlayerNameQuery : public StorageEnv::IQuery
{
	typedef  bool (SyncPlayerNameQuery::*UpdateFunc)(StorageEnv::Transaction &, Octets &, Octets &);
	typedef std::map<int, Octets> NameMap;
	UpdateFunc m_updatefunc;
	NameMap name_map;
	Octets handle;
	bool hasmore;
	int count;

	StorageEnv::Storage * prolenamehis;
	StorageEnv::Storage * pfriend;
	StorageEnv::Storage * pmessage;
	StorageEnv::Storage * pmailbox;
	StorageEnv::Storage * pwebtrade;
	StorageEnv::Storage * pwebtradesold;
	
	std::map<int, GFriendList> m_mapFriendList;
	std::map<int, MessageVector> m_mapMessage;
	std::map<int, GMailBox> m_mapMailbox;
	std::map<int64_t, GWebTradeDetail> m_mapWebTrade;
	std::map<int64_t, GWebTradeDetail> m_mapWebTradeSold;
	
	bool WalkTable(StorageEnv::Storage * pstorage, UpdateFunc func)
	{
		m_updatefunc = func;
		handle.clear();
		do{
			hasmore = false;
			count = 0;
			try
			{
				StorageEnv::AtomTransaction	txn;
				try
				{
					StorageEnv::Storage::Cursor cursor = pstorage->cursor(txn);
					cursor.walk(handle, *this);
				}
				catch ( DbException e ) { throw; }
				catch ( ... )
				{
					DbException ee( DB_OLD_VERSION );
					txn.abort( ee );
					throw ee;
				}
			}
			catch ( DbException e )
			{
				Log::log( LOG_ERR, "SyncNameChange walktable error , what=%s\n", e.what() );
				return false;
			}
			//存库并清空缓存
			StorageEnv::checkpoint();
		}
		while(hasmore);
		return true;
	}
	
	bool UpdateFriendList(StorageEnv::Transaction& txn, Octets& key, Octets& value)
	{
		try{
			int roleid;
			GFriendList friendlist;
			Marshal::OctetsStream(key) >> roleid;
			Marshal::OctetsStream(value) >> friendlist;
			bool need_repair = false;
			NameMap::iterator it , ite = name_map.end();
			GFriendInfoVector::iterator fit, fite = friendlist.friends.end();
			for (fit = friendlist.friends.begin(); fit != fite; ++fit)
			{
				it = name_map.find(fit->rid);
				if (it != ite && fit->name != it->second)
				{
					fit->name = it->second;
					need_repair = true;
				}
			}
			if (need_repair)
				m_mapFriendList[roleid] = friendlist;
		}
		catch( ... )
		{
			Log::log( LOG_ERR, "SyncNameChange UpdateFriendList error!");
		}
		return true;
	}

	bool WalkFriendList()
	{
		return WalkTable(pfriend, &SyncPlayerNameQuery::UpdateFriendList);
	}
	
	bool UpdateMessage(StorageEnv::Transaction& txn, Octets& key, Octets& value)
	{
		try{
			int roleid;
			MessageVector msgs;
			Marshal::OctetsStream(key) >> roleid;
			Marshal::OctetsStream(value) >> msgs;
			bool need_repair = false;
			NameMap::iterator it , ite = name_map.end();
			MessageVector::iterator mit , mite = msgs.end();
			for (mit = msgs.begin(); mit != mite; ++mit)
			{
				it = name_map.find(mit->srcroleid);
				if (it != ite && mit->src_name != it->second)
				{
					mit->src_name = it->second;
					need_repair = true;
				}
				it = name_map.find(mit->dstroleid);
				if (it != ite && mit->dst_name != it->second)
				{
					mit->dst_name = it->second;
					need_repair = true;
				}
			}
			if (need_repair)
				m_mapMessage[roleid] = msgs;
		}
		catch( ... )
		{
			Log::log( LOG_ERR, "SyncNameChange UpdateMessage error!");
		}
		return true;
	}

	bool WalkMessage()
	{
		return WalkTable(pmessage, &SyncPlayerNameQuery::UpdateMessage);
	}
	
	bool UpdateMailbox(StorageEnv::Transaction& txn, Octets& key, Octets& value)
	{
		try{
			int roleid;
			GMailBox mailbox;
			Marshal::OctetsStream(key) >> roleid;
			Marshal::OctetsStream(value) >> mailbox;
			bool need_repair = false;
			NameMap::iterator it , ite = name_map.end();
			GMailVector::iterator mit, mite = mailbox.mails.end();
			for (mit = mailbox.mails.begin(); mit != mite; ++mit)
			{
				it = name_map.find(mit->header.sender);
				if (it != ite && mit->header.sender_name != it->second)
				{
					mit->header.sender_name = it->second;
					need_repair = true;
				}
			}
			if (need_repair)
				m_mapMailbox[roleid] = mailbox;
		}
		catch( ... )
		{
			Log::log( LOG_ERR, "SyncNameChange UpdateMailbox error!");
		}
		return true;
	}

	bool WalkMailbox()
	{
		return WalkTable(pmailbox, &SyncPlayerNameQuery::UpdateMailbox);
	}
	
	bool UpdateWebTrade(StorageEnv::Transaction& txn, Octets& key, Octets& value)
	{
		try{
			int64_t sn;
			GWebTradeDetail webtradedetail;
			Marshal::OctetsStream(key) >> sn;
			Marshal::OctetsStream(value) >> webtradedetail;
			bool need_repair = false;
			NameMap::iterator it , ite = name_map.end();
			it = name_map.find(webtradedetail.info.seller_roleid);
			if (it != ite && webtradedetail.info.seller_name != it->second)
			{
				webtradedetail.info.seller_name = it->second;
				need_repair = true;
			}
			it = name_map.find(webtradedetail.info.buyer_roleid);
			if (it != ite && webtradedetail.info.buyer_name != it->second)
			{
				webtradedetail.info.buyer_name = it->second;
				need_repair = true;
			}
			if (need_repair)
				m_mapWebTrade[sn] = webtradedetail;
		}
		catch (...)
		{
			Log::log( LOG_ERR, "SyncNameChange UpdateWebTrade error!");
		}
		return true;
	}

	bool WalkWebTrade()
	{
		return WalkTable(pwebtrade, &SyncPlayerNameQuery::UpdateWebTrade);
	}

	bool UpdateWebTradeSold(StorageEnv::Transaction& txn, Octets& key, Octets& value)
	{
		try{
			int64_t sn;
			GWebTradeDetail webtradedetail;
			Marshal::OctetsStream(key) >> sn;
			Marshal::OctetsStream(value) >> webtradedetail;
			bool need_repair = false;
			NameMap::iterator it , ite = name_map.end();
			it = name_map.find(webtradedetail.info.seller_roleid);
			if (it != ite && webtradedetail.info.seller_name != it->second)
			{
				webtradedetail.info.seller_name = it->second;
				need_repair = true;
			}
			it = name_map.find(webtradedetail.info.buyer_roleid);
			if (it != ite && webtradedetail.info.buyer_name != it->second)
			{
				webtradedetail.info.buyer_name = it->second;
				need_repair = true;
			}
			if (need_repair)
				m_mapWebTradeSold[sn] = webtradedetail;
		}
		catch (...)
		{
			Log::log( LOG_ERR, "SyncNameChange UpdateWebTradeSold error!");
		}
		return true;
	}
	
	bool WalkWebTradeSold()
	{
		return WalkTable(pwebtradesold, &SyncPlayerNameQuery::UpdateWebTradeSold);
	}
	
public:

	bool Init()
	{
		prolenamehis = StorageEnv::GetStorage("rolenamehis");
		pfriend = StorageEnv::GetStorage("friends");
		pmessage = StorageEnv::GetStorage("messages");
		pmailbox = StorageEnv::GetStorage("mailbox");
		pwebtrade = StorageEnv::GetStorage("webtrade");
		pwebtradesold = StorageEnv::GetStorage("webtradesold");

		Marshal::OctetsStream key_all, value_all;
		key_all << (int)0;
		name_map.clear();
		try{
			StorageEnv::CommonTransaction txn;
			try{
				if (prolenamehis->find(key_all, value_all, txn))
					value_all >> name_map;
			}
			catch ( DbException e ) { throw; }
			catch ( ... )
			{
				DbException ee( DB_OLD_VERSION );
				txn.abort( ee );
				throw ee;
			}
		}
		catch( DbException e )
		{
			Log::log( LOG_ERR, "SyncPlayerName::Init error, what=%s\n", e.what());
			return false;
		}
		if (name_map.empty())
		{
			printf("name_map is empty, dont need syncplayername.\n");
			return false;
		}
		LOG_TRACE("name_map.size= %d\n",name_map.size());
		return true;
	}
	
	bool Update( StorageEnv::Transaction& txn, Octets& key, Octets& value)
	{
		if (++count % 10000 == 0)
		{
			hasmore = true;
			handle = key;
			return false;
		}
		return (this->*m_updatefunc)(txn, key, value);
	}

	bool RepairFriendList()
	{
		if (!WalkFriendList()) return false;
		LOG_TRACE("SyncPlayerName::RepairFriendList count=%d\n",m_mapFriendList.size());
		StorageEnv::AtomTransaction	txn;
		try{
			std::map<int, GFriendList>::iterator it, ie=m_mapFriendList.end();
			for (it=m_mapFriendList.begin(); it != ie; ++it)
			{
				pfriend->insert(Marshal::OctetsStream()<<it->first, Marshal::OctetsStream()<<it->second, txn);
			}
		}
		catch (DbException e)
		{
			Log::log( LOG_ERR, "SyncPlayerName::RepairFriendList error, what=%s\n", e.what());
			return false;
		}
		//存库并清空内存
		StorageEnv::checkpoint();
		m_mapFriendList.clear();
		return true;
	}

	bool RepairMessage()
	{
		if (!WalkMessage()) return false;
		LOG_TRACE("SyncPlayerName::RepairMessage count=%d\n",m_mapMessage.size());
		StorageEnv::AtomTransaction	txn;
		try
		{
			std::map<int, MessageVector>::iterator bit, bie=m_mapMessage.end();
			for (bit=m_mapMessage.begin(); bit != bie; ++bit)
			{
				pmessage->insert(Marshal::OctetsStream()<<bit->first, Marshal::OctetsStream()<<bit->second, txn);
			}
		}
		catch ( DbException e )
		{
			Log::log( LOG_ERR, "SyncPlayerName::RepairMessage error, what=%s\n", e.what() );
			return false;
		}
		//存库并清空内存
		StorageEnv::checkpoint();
		m_mapMessage.clear();
		return true;
	}

	bool RepairMailBox()
	{
		if (!WalkMailbox()) return false;
		LOG_TRACE("SyncPlayerName::RepairMailBox count=%d\n",m_mapMailbox.size());
		StorageEnv::AtomTransaction txn;
		try{
			std::map<int, GMailBox>::iterator bit, bie=m_mapMailbox.end();
			for (bit=m_mapMailbox.begin(); bit != bie; ++bit)
			{
				pmailbox->insert(Marshal::OctetsStream()<<bit->first, Marshal::OctetsStream()<<bit->second, txn);
			}
		}
		catch ( DbException e )
		{
			Log::log( LOG_ERR, "SyncPlayerName::RepairMailBox error, what=%s\n", e.what() );
			return false;
		}
		//存库并清空内存
		StorageEnv::checkpoint();
		m_mapMailbox.clear();
		return true;
	}

	bool RepairWebTrade()
	{
		if (!WalkWebTrade()) return false;
		LOG_TRACE("SyncPlayerName::RepairWebTrade count=%d\n",m_mapWebTrade.size());
		StorageEnv::AtomTransaction txn;
		try{
			std::map<int64_t, GWebTradeDetail>::iterator bit, bie=m_mapWebTrade.end();
			for (bit=m_mapWebTrade.begin(); bit != bie; ++bit)
			{
				pwebtrade->insert(Marshal::OctetsStream()<<bit->first, Marshal::OctetsStream()<<bit->second, txn);
			}
		}
		catch ( DbException e )
		{
			Log::log( LOG_ERR, "SyncPlayerName::RepairWebTrade error, what=%s\n", e.what() );
			return false;
		}
		//存库并清空内存
		StorageEnv::checkpoint();
		m_mapWebTrade.clear();
		return true;
	}
	
	bool RepairWebTradeSold()
	{
		if (!WalkWebTradeSold()) return false;
		LOG_TRACE("SyncPlayerName::RepairWebTradeSold count=%d\n",m_mapWebTradeSold.size());
		StorageEnv::AtomTransaction txn;
		try{
			std::map<int64_t, GWebTradeDetail>::iterator bit, bie=m_mapWebTradeSold.end();
			for (bit=m_mapWebTradeSold.begin(); bit != bie; ++bit)
			{
				pwebtradesold->insert(Marshal::OctetsStream()<<bit->first, Marshal::OctetsStream()<<bit->second, txn);
			}
		}
		catch ( DbException e )
		{
			Log::log( LOG_ERR, "SyncPlayerName::RepairWebTradeSold error, what=%s\n", e.what() );
			return false;
		}
		//存库并清空内存
		StorageEnv::checkpoint();
		m_mapWebTradeSold.clear();
		return true;
	}

	bool ClearNameMap()
	{
		Marshal::OctetsStream key_all;
		key_all << (int)0;
		name_map.clear();
		StorageEnv::AtomTransaction txn;
		try
		{
			prolenamehis->insert(key_all, Marshal::OctetsStream()<<name_map, txn);
		}
		catch(...)
		{
			Log::log(LOG_ERR, "clear name map exception");
			return false;
		}
		//存库并清空内存
		StorageEnv::checkpoint();
		return true;
	}
};

void SyncPlayerName()
{
	printf( "\nSyncPlayerName:\n");

	SyncPlayerNameQuery q;
	if (q.Init() && q.RepairFriendList() && q.RepairMessage() && q.RepairMailBox() && 
			q.RepairWebTrade() && q.RepairWebTradeSold() && q.ClearNameMap())
	{
		printf("SyncPlayerName ok.\n");
	}
}

typedef std::map<unsigned int /*tid*/, StockOrder> StockOrderSubMap;
typedef std::map<unsigned int /*userid*/, StockOrderSubMap> StockOrderMap;

class ReturnCashQuery : public StorageEnv::IQuery
{
	StockOrderMap & stock_order_map;
	int count_;
public:
	bool hasmore_;
	Octets handle_;
public:
	explicit ReturnCashQuery(StockOrderMap & som):stock_order_map(som), count_(0), hasmore_(false) {}
	bool Update(StorageEnv::Transaction& txn, Octets& key, Octets& value)
	{
		// 暂停以回收内存
		if (++count_ % 10000 == 0)
		{
			hasmore_ = true;
			handle_ = key;
			return false;
		}
		
		Marshal::OctetsStream key_os, value_os;
		key_os = key;
		value_os = value;

		unsigned int tid = 0;
		StockOrder order;
		try
		{
			key_os >> tid;
			value_os >> order;
			if (tid == STOCK_BALANCE_ID)	//这个是系统的，不需要处理
				return true;
			if (stock_order_map.find(order.userid) == stock_order_map.end())
			{
				StockOrderSubMap stock_order_sub_map;
				stock_order_sub_map[tid] = order;
				stock_order_map[order.userid] = stock_order_sub_map;
			}
			else
			{
				if (stock_order_map[order.userid].find(tid) != stock_order_map[order.userid].end())
				{
					Log::log( LOG_ERR, "ReturnCashQuery , duplicate orderinfo, tid=%d userid=%d", tid, order.userid );
					return true;
				}
				stock_order_map[order.userid][tid] = order;
			}
		}
		catch (Marshal::Exception&)
		{
			Log::log( LOG_ERR, "ReturnCashQuery , error unmarshal, tid=%d.", tid );
		}
		return true;
	}
};

unsigned int DoReturnCash(unsigned int userid, StorageEnv::Storage * puser, StorageEnv::Storage * porder, StockOrderMap & stock_order_map)
{
	Marshal::OctetsStream key, value;
	key << userid;
	User u;
	GSysAuctionCash sa_cash;
	unsigned int bag_cash = 0;
	try
	{
		StorageEnv::AtomTransaction	txn;
		try
		{
			if (!puser->find(key, value, txn))
				throw DbException(DB_VERIFY_BAD);
			value >> u;
			if (u.cash_sysauction.size())
			{
				Marshal::OctetsStream(u.cash_sysauction)>>sa_cash;
				//清空玩家放拍卖行里的元宝
				sa_cash.cash_2 = 0;
				u.cash_sysauction = Marshal::OctetsStream()<<sa_cash; 
			}
			//清空玩家放元宝寄售里的元宝
			u.cash = 0;
			//计算玩家背包里的元宝，需要用总的元宝数减去cash_used
			bag_cash = u.cash_add+u.cash_buy-u.cash_sell-sa_cash.cash_used_2-u.cash-sa_cash.cash_2-u.cash_used;

			//清空玩家在元宝寄售里下的单
			StockOrderMap::iterator it = stock_order_map.find(userid);
			if (it != stock_order_map.end())
			{
				StockOrderSubMap::iterator _it = (it->second).begin(), _eit = (it->second).end();
				for ( ; _it != _eit; ++_it)
				{
					if ((_it->second).price > 0)	//这个是玩家出售的单据
					{
						Marshal::OctetsStream orderkey;
						orderkey << _it->first;
						porder->del(orderkey, txn);
					}
				}
			}
			//保存玩家的user表里的元宝信息
			puser->insert(key, Marshal::OctetsStream()<<u, txn);
		}
		catch ( DbException e ) { throw; }
		catch ( ... )
		{
			DbException ee( DB_OLD_VERSION );
			txn.abort( ee );
			throw ee;
		}
	}
	catch ( DbException e )
	{
		Log::log( LOG_ERR, "DoReturnCash err, userid=%d, what=%s\n", userid, e.what() );
		return 0;
	}
	return bag_cash;
}

void ReturnCash(const char* useridfile)
{
	//初始化输入的userid列表
	if( 0 != access(useridfile, R_OK) )
	{
		Log::log(LOG_ERR, "Fail to access userid file: %s", useridfile);
		return;
	}
	
	std::set<unsigned int> useridset;
	ReadRoleIDFile(useridfile, useridset);

	if (useridset.empty())
	{
		Log::log(LOG_ERR, "Fail to read userid file: %s", useridfile);
		return;
	}
	Log::log(LOG_INFO,"ReadUserIDFile size=%d\n", useridset.size());

	//因为使用的交易流水号做的索引，找不到玩家userid对应的数据，所以需要把stockorder表里的数据全部取出来。
	StockOrderMap stock_order_map;

	StorageEnv::Storage * porder = StorageEnv::GetStorage("order");
	StorageEnv::Storage * puser = StorageEnv::GetStorage( "user" );
	ReturnCashQuery query(stock_order_map);
	do{
		query.hasmore_ = false;	
		try
		{
			StorageEnv::AtomTransaction	txn;
			try
			{
				StorageEnv::Storage::Cursor cursor = porder->cursor(txn);
				cursor.walk(query.handle_, query);
			}
			catch ( DbException e ) { throw; }
			catch ( ... )
			{
				DbException ee( DB_OLD_VERSION );
				txn.abort( ee );
				throw ee;
			}
		}
		catch ( DbException e )
		{
			Log::log( LOG_ERR, "ReturnCash walktable error , what=%s\n", e.what() );
			return;
		}
		//存库并清空缓存
		StorageEnv::checkpoint();
	}
	while(query.hasmore_);

	Log::log(LOG_INFO,"ReturnCashQuery end. stock_order_map size=%d\n", stock_order_map.size());
	
	//遍历玩家的userid列表，对输入的每一个玩家进行处理
	std::set<unsigned int>::iterator it = useridset.begin(), eit = useridset.end();
	for ( ; it != eit; it++)
	{
		unsigned int bag_cash = DoReturnCash(*it, puser, porder, stock_order_map);
		printf("%d,%d\n", *it, bag_cash);
	}
	Log::log(LOG_INFO,"ReturnCash end.\n");
}

void ImportRecallUser(const char* useridfile)
{
	//初始化输入的userid列表
	if( 0 != access(useridfile, R_OK) ) {
		Log::log(LOG_ERR, "Fail to access userid file: %s", useridfile);
		return;
	}

	int recall_user_count = 0;
	try {   
		StorageEnv::Storage* precalluser = StorageEnv::GetStorage("recalluser");
		StorageEnv::CommonTransaction txn;
		
		try {
			std::ifstream ifs(useridfile);
			if (ifs.fail()) return;
			
			Marshal::OctetsStream value;
			value << (char)1;

			while (!ifs.eof()) {
				Marshal::OctetsStream key;
				int userid = 0;
				ifs >> userid;
				if (ifs.fail()) break;
				
				if(userid == 0) continue;
				key << userid;
				precalluser->insert(key, value, txn);
				++recall_user_count;
			}
		} catch(DbException &e) { 
			throw;  
		} catch(...) { 
			DbException ee( DB_OLD_VERSION );
			txn.abort( ee );
			throw ee;
		}       
	} catch(DbException e) {       
		Log::log( LOG_ERR, "ImportRecallUser exception, what=%s\n", e.what() );
	}
	
	StorageEnv::checkpoint();
	Log::log(LOG_INFO,"ImportRecallUser end. recall user count=%d\n", recall_user_count);
}

void PrintWebTrade(const GWebTradeDetail & detail, char is_sold)
{
	printf("sn");
	printf(",seller_roleid");
	printf(",seller_userid");
	printf(",seller_name");
	printf(",posttype");
	printf(",money");
	printf(",item_id");
	printf(",item_count");
	printf(",state");
	printf(",post_endtime");
	printf(",show_endtime");
	printf(",price");
	printf(",sell_endtime");
	printf(",buyer_roleid");
	printf(",buyer_userid");
	printf(",buyer_name");
	printf(",commodity_id");
	printf(",is_sold");		//用于区分是否售出
	printf("\n");
	
	Octets seller_name,buyer_name;
	CharsetConverter::conv_charset_u2l(const_cast<Octets&>(detail.info.seller_name), seller_name);
	CharsetConverter::conv_charset_u2l(const_cast<Octets&>(detail.info.buyer_name), buyer_name);

	printf("%llu" , detail.info.sn);
	printf(",%d"  , detail.info.seller_roleid);
	printf(",%d"  , detail.info.seller_userid);
	printf(",%.*s", seller_name.size(),(char*)seller_name.begin());
	printf(",%d"  , detail.info.posttype);
	printf(",%d"  , detail.info.money);
	printf(",%d"  , detail.info.item_id);
	printf(",%d"  , detail.info.item_count);
	printf(",%d"  , detail.info.state);
	printf(",%d"  , detail.info.post_endtime);
	printf(",%d"  , detail.info.show_endtime);
	printf(",%d"  , detail.info.price);
	printf(",%d"  , detail.info.sell_endtime);
	printf(",%d"  , detail.info.buyer_roleid);
	printf(",%d"  , detail.info.buyer_userid);
	printf(",%.*s", buyer_name.size(),(char*)buyer_name.begin());
	printf(",%d"  , detail.info.commodity_id);
	printf(",%d"  , (int)is_sold);	//用于区分是否售出
	printf("\n");
}

void QueryWebTrade(int64_t sn)
{
	Marshal::OctetsStream key,value;
	try
	{
		StorageEnv::Storage * pwebtrade = StorageEnv::GetStorage("webtrade");
		StorageEnv::Storage * pwebtradesold = StorageEnv::GetStorage("webtradesold");
		StorageEnv::AtomTransaction txn;
		try
		{
			if (0 == sn)
				throw DbException(DB_VERIFY_BAD);

			GWebTradeDetail	detail;
			char is_sold = 0;
			key << sn;
			if( pwebtrade->find( key, value, txn ) )
				value >> detail;
			else if (pwebtradesold->find( key, value, txn ) )
			{
				is_sold = 1;
				value >> detail;
			}
			else
				throw DbException(DB_VERIFY_BAD);

			if (detail.info.sn != sn)
				throw DbException(DB_VERIFY_BAD); 
		
			PrintWebTrade(detail,is_sold);	
		}
		catch ( DbException e ) { throw; }
		catch ( ... )
		{
			DbException ee( DB_OLD_VERSION );
			txn.abort( ee );
			throw ee;
		}
	}
	catch ( DbException e )
	{
		Log::log( LOG_ERR, "QueryWebTrade, sn=%llu, what=%s\n", sn, e.what() );
	}
}

void QueryGlobalControl()
{
	Marshal::OctetsStream key,value;
	try
	{
		StorageEnv::Storage * pcontrol = StorageEnv::GetStorage("globalcontrol");
		StorageEnv::AtomTransaction txn;
		try
		{
			GGlobalControlData data;
			key << (int)0;		
			if(pcontrol->find(key,value,txn))
				value >> data;
			else
				throw DbException(DB_VERIFY_BAD);
			printf("cash_money_exchange_open:%d",data.cash_money_exchange_open);	
			printf("\ncash_money_exchange_rate:%d",data.cash_money_exchange_rate);
			printf("\nforbid_ctrl_list:");
			for(size_t i=0;i<data.forbid_ctrl_list.size();i++)
				printf("%d, ",data.forbid_ctrl_list[i]);
			printf("\nforbid_item_list:");
			for(size_t i=0;i<data.forbid_item_list.size();i++)
				printf("%d, ",data.forbid_item_list[i]);
			printf("\nforbid_service_list:");
			for(size_t i=0;i<data.forbid_service_list.size();i++)
				printf("%d, ",data.forbid_service_list[i]);
			printf("\nforbid_task_list:");
			for(size_t i=0;i<data.forbid_task_list.size();i++)
				printf("%d, ",data.forbid_task_list[i]);
			printf("\nforbid_skill_list:");
			for(size_t i=0;i<data.forbid_skill_list.size();i++)
				printf("%d, ",data.forbid_skill_list[i]);
			printf("\ntrigger_ctrl_list:");
			for(size_t i=0;i<data.trigger_ctrl_list.size();i++)
				printf("%d, ",data.trigger_ctrl_list[i]);
			
			printf("\n");
		}
		catch ( DbException e ) { throw; }
		catch ( ... )
		{
			DbException ee( DB_OLD_VERSION );
			txn.abort( ee );
			throw ee;
		}
	}
	catch ( DbException e )
	{
		Log::log( LOG_ERR, "QueryGlobalControl, what=%s\n", e.what() );
	}
}

void ImportRole(const char* roleidfile)
{
    if (roleidfile == NULL)
    {
        printf("read roleidfile error.\n");
        return;
    }

    GRoleData data;
    if (!GNET::XmlDecoder::Decode(roleidfile, data))
    {
        printf("decode roleidfile error.\n");
        return;
    }

    try
    {
        StorageEnv::Storage* pbase = StorageEnv::GetStorage("base");
        StorageEnv::Storage* pstatus = StorageEnv::GetStorage("status");
        StorageEnv::Storage* ppocket = StorageEnv::GetStorage("inventory");
        StorageEnv::Storage* pequipment = StorageEnv::GetStorage("equipment");
        StorageEnv::Storage* pstorehouse = StorageEnv::GetStorage("storehouse");
        StorageEnv::Storage* ptask = StorageEnv::GetStorage("task");
        StorageEnv::CommonTransaction txn;
        Marshal::OctetsStream key;

        try
        {
            key << data.base.id;
            pbase->insert(key, Marshal::OctetsStream() << data.base, txn);
            pstatus->insert(key, Marshal::OctetsStream() << data.status, txn);
            ppocket->insert(key, Marshal::OctetsStream() << data.pocket, txn);
            pequipment->insert(key, Marshal::OctetsStream() << data.equipment, txn);
            pstorehouse->insert(key, Marshal::OctetsStream() << data.storehouse, txn);
            ptask->insert(key, Marshal::OctetsStream() << data.task, txn);
        }
        catch (DbException e) {throw;}
        catch (...)
        {
            DbException ee(DB_OLD_VERSION);
            txn.abort(ee);
            throw ee;
        }
    }
    catch (DbException e)
    {
        Log::log(LOG_ERR, "ImportRole, roleid=%d, what=%s\n", data.base.id, e.what());
    }
}

class DeleteWaitdelQuery : public StorageEnv::IQuery
{
	typedef	std::vector<int> RIDVec;
	RIDVec del_role;
	bool need_continue;
public:
	DeleteWaitdelQuery() : need_continue(false) {}
	bool Update( StorageEnv::Transaction& txn, Octets& key, Octets& value)
	{
		Marshal::OctetsStream key_rid;
		key_rid = key;
		int roleid = -1;
		if(del_role.size() >= 30000)
		{
			need_continue = true;
			return false;
		}
		try
		{
			key_rid >> roleid;
			del_role.push_back(roleid);
		}
		catch(Marshal::Exception &)
		{
			printf( "[FATAL] DeleteWaitdelQuery, error unmarshal\n") ;
		}

		return true;
	}

	int DelRoleFaction(unsigned int rid,StorageEnv::Storage * puserfaction, StorageEnv::Storage * pfactioninfo, StorageEnv::AtomTransaction& txn)
	{
		Marshal::OctetsStream key,value;
		GUserFaction user;
		GFactionInfo info;
		unsigned int fid = 0;

		try
		{
			try
			{
				key << rid;

				if(!puserfaction->find(key, value, txn))
					return 0;
				value >> user;
				puserfaction->del( key, txn );

				if(user.fid==0) return 0;

//				printf("type=deleterole:roleid=%d:factionid=%d:role=%d\n", 
//					user.rid, user.fid, user.role);
				fid = user.fid;

				key.clear();
				key << fid;

				Marshal::OctetsStream(pfactioninfo->find( key, txn )) >> info;

				for(GMemberVector::iterator it=info.member.begin();it!=info.member.end();++it)
				{
					if(it->rid==rid)
					{
						info.member.erase(it);
						break;
					}
				}
				if(user.role==_R_MASTER)
				{
					GMemberVector::iterator itm = info.member.begin();
					int minrole = _R_MEMBER;
					for(GMemberVector::iterator it=info.member.begin();it!=info.member.end();++it)
					{
						if(it->role<minrole)
						{
							minrole = it->role;
							itm = it;
						}
					}

					if(info.member.size())
					{
						info.master.rid = itm->rid;
						itm->role = _R_MASTER;

						value.clear();
						key.clear();
						key << itm->rid;
						if(puserfaction->find(key, value, txn))
						{
							value >> user;
							user.role = _R_MASTER;
							user.delayexpel.clear(); // 清除延时删除标记
							puserfaction->insert( key, Marshal::OctetsStream() << user, txn );
						}
//						else
//							printf( "DelRoleFaction %d, userfaction %d not found\n", fid, itm->rid);
					}
					else
						info.master.rid = 0;
				}
				pfactioninfo->insert( Marshal::OctetsStream() << fid, Marshal::OctetsStream() << info, txn );
				return fid;
			}
			catch ( DbException e ) { throw; }
			catch ( ... )
			{
				DbException ee( DB_OLD_VERSION );
				txn.abort( ee );
				throw ee;
			}
		}
		catch ( DbException e )
		{
			printf( "[FATAL] CheckRoleFaction, roleid=%d, what=%s\n", rid, e.what() );
		}
		return 0;
	}
	bool Process(bool backup)
	{

		try
		{
			if(backup && !opendir("./delwait_backup"))
				mkdir("./delwait_backup",S_IRWXU|S_IRWXG|S_IRWXO); 

			RIDVec::iterator ibeg = del_role.begin();
			RIDVec::iterator iend = del_role.end();

			for(;ibeg != iend; ++ibeg)
			{
				StorageEnv::Storage * prolename = StorageEnv::GetStorage("rolename");
				StorageEnv::Storage * puser = StorageEnv::GetStorage("user");
				StorageEnv::Storage * pwaitdel = StorageEnv::GetStorage("waitdel");
				StorageEnv::Storage * pbase = StorageEnv::GetStorage("base");
				StorageEnv::Storage * pstatus = StorageEnv::GetStorage("status");
				StorageEnv::Storage * pinventory = StorageEnv::GetStorage("inventory");
				StorageEnv::Storage * pequipment = StorageEnv::GetStorage("equipment");
				StorageEnv::Storage * pstorehouse = StorageEnv::GetStorage("storehouse");
				StorageEnv::Storage * ptask = StorageEnv::GetStorage("task");
				StorageEnv::Storage * pfriends = StorageEnv::GetStorage("friends");
				StorageEnv::Storage * puserfaction = StorageEnv::GetStorage("userfaction");
				StorageEnv::Storage * pfactioninfo = StorageEnv::GetStorage("factioninfo");
				StorageEnv::AtomTransaction txn;
				int roleid = *ibeg;
				int userid = -1;
				Marshal::OctetsStream key_userid, key_roleid,value_base;
				key_roleid<<RoleId(roleid);
				GRoleBase rolebase;
				User user;

				try
				{
					if( pbase->find( key_roleid, value_base, txn ) )
					{
						try {
							value_base >> rolebase;
							userid = rolebase.userid;
						}
						catch ( Marshal::Exception & )
						{
							printf( "[FATAL] DeleteWaildel, base unmarshal error, roleid=%d.\n", roleid );
							throw;
						}
					}
					else
					{
						 pwaitdel->del( Marshal::OctetsStream() << roleid, txn );
						 continue;
					}

					if(userid==0)
						userid = LOGICUID(roleid);
		
					if(backup)
					{
						try
						{
							char dirsz[128];
							sprintf(dirsz,"./delwait_backup/%d",userid);
							if(!opendir(dirsz))
								mkdir(dirsz,S_IRWXU|S_IRWXG|S_IRWXO);

							GRoleData data;
							Marshal::OctetsStream base_value,status_value,inv_value,equ_value,store_value,task_value;
							if(pbase->find(key_roleid,base_value,txn) &&
   							   pstatus->find(key_roleid,status_value,txn) &&
							   pinventory->find(key_roleid,inv_value,txn) &&
							   pequipment->find(key_roleid,equ_value,txn) &&
							   pstorehouse->find(key_roleid,store_value,txn) &&
							   ptask->find(key_roleid,task_value,txn))
							{
								base_value >> data.base;
								status_value >> data.status;
								inv_value >> data.pocket;
								equ_value >> data.equipment;
								store_value >> data.storehouse;
								task_value >> data.task;
								
								XmlCoder coder;
								coder.append_header();
								coder.append("role", data);
							
								char filesz[128];
								sprintf(filesz,"delwait_backup/%d/%d.xml",userid,roleid);
								std::ofstream ofs(filesz,std::ios::out | std::ios::trunc);
								ofs << coder.c_str();
							}
						}
						catch(DbException e )
						{
							printf( "[FATAL] DeleteWaildel, backup error, roleid=%d.what=%s\n", roleid, e.what() );
							throw;
						}
					}	
				
					try
					{
					//update user rolelist
						if(userid > 0)
						{
							key_userid<<UserID(userid);
						
							Marshal::OctetsStream user_value;
							if(puser->find( key_userid,user_value, txn ))
							{
								user_value >> user; //backup old value; 
								RoleList rolelist(user.rolelist); //create rolelist object	
								rolelist.DelRole(roleid % MAX_ROLE_COUNT);
								user.rolelist = rolelist.GetRoleList();	
								puser->insert(key_userid, Marshal::OctetsStream() << user, txn);
							}
						}

						//delete role
						pbase->del( key_roleid, txn );

						pstatus->del( key_roleid, txn );
						pinventory->del( key_roleid, txn );
						pequipment->del( key_roleid, txn );
						pstorehouse->del( key_roleid, txn );
						ptask->del( key_roleid, txn );
						pfriends->del( key_roleid, txn );
						prolename->del( rolebase.name, txn );
						//delete roleid from "waitdel" table
						pwaitdel->del( Marshal::OctetsStream() << roleid, txn ); 
						DelRoleFaction(*ibeg ,puserfaction,pfactioninfo, txn);
						//printf( "DeleteWaildel, delete roleid=%d\n", roleid );
					}
					catch( DbException e )
					{
						printf("[FATAL] DelWaitdel:DelRole=%d what=%s\n",roleid, e.what() );
						throw;
					}
				}
				catch( DbException e ) { continue; }
				catch( ... )
				{
					DbException ee( DB_OLD_VERSION );
					txn.abort( ee );
					throw ee;
				}
			}
			printf("\n -------------------------------------------------------\n");
			printf("DelWaitdel:Total del role count: %d\n",del_role.size());			
			printf("\n -------------------------------------------------------\n");
		}
		catch ( ... )
		{
			printf("[FATAL] DelWaitdel:DelRole fail!\n" );
			return false;
		}

		return need_continue;
	}
	
};

bool DeleteWaitdel(bool backup)
{
	printf("\ndelete waitdel role\n");
	DeleteWaitdelQuery q;
	try
	{
		StorageEnv::Storage * pstorage = StorageEnv::GetStorage( "waitdel" );
		StorageEnv::AtomTransaction	txn;
		try
		{
			StorageEnv::Storage::Cursor cursor = pstorage->cursor( txn );
			cursor.walk( q );
		}
		catch ( DbException e ) { throw; }
		catch ( ... )
		{
			DbException ee( DB_OLD_VERSION );
			txn.abort( ee );
			throw ee;
		}
	}
	catch ( DbException e )
	{
		Log::log( LOG_ERR, "DeleteWaitdel, walk err , what=%s\n", e.what() );
	}

	if(q.Process(backup))
		return true;

	printf( "delete waitdel finished.\n" );
	return false;
}

typedef std::set<unsigned int> ILUID_SET;
class GetImmuneQuery : public StorageEnv::IQuery
{
public:
	ILUID_SET& iset;
	GetImmuneQuery(ILUID_SET& s): iset(s) {}

    bool Update(StorageEnv::Transaction& txn, Octets& key, Octets& value)
	{
		UserID uid;
		User user;
		try 
		{
			Marshal::OctetsStream(key) >> uid;
			Marshal::OctetsStream(value) >> user;
			if(user.cash_add + user.cash_buy)
			{
				iset.insert(user.logicuid);
				printf("GetImmunityQuery : get Immune User: %u LUID: %u.\n", uid.id, user.logicuid);
			}
		}
		catch(...)
		{
			printf("[FATAL] GetImmunityQuery error: bad User.\n");
		}
		return true;	
	}
};

class CalcWaitdelQuery_Step1 : public StorageEnv::IQuery
{
public:
	ILUID_SET& iset;
	ILUID_SET& dset;
	int linetime;
	CalcWaitdelQuery_Step1(ILUID_SET& is, ILUID_SET& ds, int lt): iset(is),dset(ds),linetime(lt) {}

    bool Update(StorageEnv::Transaction& txn, Octets& key, Octets& value)
	{
		RoleId		id;
		GRoleBase	base;
		try 
		{
			Marshal::OctetsStream(key) >> id;
			if(iset.find(LOGICUID(id.id)) != iset.end()) 
				return true;
			if(id.id < 1000)
				return true;

			Marshal::OctetsStream(value) >> base;

			if(base.lastlogin_time < linetime)
			{
				dset.insert(id.id);
			}
		}
		catch(...)
		{
			printf("[FATAL] CalcWaitdelQuery_Step1 error: bad RoleBase.\n");
		}
		return true;
	}
};

class CalcWaitdelQuery_Step2 : public StorageEnv::IQuery
{
public:
	ILUID_SET& dset;
	int levelline;
	CalcWaitdelQuery_Step2(ILUID_SET& ds,int ll): dset(ds),levelline(ll) {}

    bool Update(StorageEnv::Transaction& txn, Octets& key, Octets& value)
	{
		RoleId		id;
		GRoleStatus	status;
		try 
		{
			Marshal::OctetsStream(key) >> id;
			if(dset.find(id.id) == dset.end())
			{
				return true;
			}

			Marshal::OctetsStream(value) >> status;

			if(status.level >= levelline )
			{
				dset.erase(id.id);
				return true;
			}
		}
		catch(...)
		{
			printf("[FATAL] CalcWaitdelQuery_Step2 error: bad RoleBase.\n");
		}
		return true;
	}
};

void InsertWaitDel(ILUID_SET& waitdel_set,int deltimeline)
{
	try
	{
		StorageEnv::Storage * pwaitdel = StorageEnv::GetStorage( "waitdel" );
		StorageEnv::AtomTransaction	txn4;
		try
		{
			ILUID_SET::iterator ibeg = waitdel_set.begin();
			ILUID_SET::iterator iend = waitdel_set.end();
			for(;ibeg!=iend;++ibeg)
			{
				pwaitdel->insert( Marshal::OctetsStream() << RoleId(*ibeg), Marshal::OctetsStream() << deltimeline, txn4 );
				printf("CalcWaitdel Insert role: %d \n", *ibeg);
			}
		}
		catch ( DbException e ) { throw; }
		catch ( ... )
		{
			DbException ee( DB_OLD_VERSION );
			txn4.abort( ee );
			throw ee;
		}
	}
	catch ( DbException e )
	{
		printf( "[FATAL] CalcWaitdel Insert Waitdel  err , what=%s\n", e.what() );
	}
}

void CalcWaitdel(int year,int level)
{
	if(year < 2008) return;

	ILUID_SET immune_set;
	ILUID_SET waitdel_set;

	struct tm dt;
	dt.tm_year = year - 1900;
	dt.tm_mon = 0;
	dt.tm_mday = 1;
	dt.tm_hour = 0;
	dt.tm_min = 0;
	dt.tm_sec = 1;

	int deltimeline = mktime(&dt);

	GetImmuneQuery immune_q(immune_set);
	CalcWaitdelQuery_Step1 calc_step1_q(immune_set,waitdel_set,deltimeline);
	CalcWaitdelQuery_Step2 calc_step2_q(waitdel_set,level);

	try
	{
		StorageEnv::Storage * puser = StorageEnv::GetStorage( "user" );
		StorageEnv::AtomTransaction	txn1;
		try
		{
			StorageEnv::Storage::Cursor cursor = puser->cursor( txn1 );
			cursor.walk( immune_q );
		}
		catch ( DbException e ) { throw; }
		catch ( ... )
		{
			DbException ee( DB_OLD_VERSION );
			txn1.abort( ee );
			throw ee;
		}
	}
	catch ( DbException e )
	{
		printf( "[FATAL] GetImmuneQuery, walk err , what=%s\n", e.what() );
	}
	printf("\n\n\n IMMUNE count: %d \n\n\n",immune_set.size() );

	StorageEnv::checkpoint();

	try
	{
		StorageEnv::Storage * pbase = StorageEnv::GetStorage( "base" );
		StorageEnv::AtomTransaction	txn2;
		try
		{
			StorageEnv::Storage::Cursor cursor = pbase->cursor( txn2 );
			cursor.walk( calc_step1_q );
		}
		catch ( DbException e ) { throw; }
		catch ( ... )
		{
			DbException ee( DB_OLD_VERSION );
			txn2.abort( ee );
			throw ee;
		}
	}
	catch ( DbException e )
	{
		printf( "[FATAL] CalcWaitdelQuery_Step1, walk err , what=%s\n", e.what() );
	}
	printf("\n\n\n STEP1 count: %d \n\n\n",waitdel_set.size() );

	StorageEnv::checkpoint();

	try
	{
		StorageEnv::Storage * pstatus = StorageEnv::GetStorage( "status" );
		StorageEnv::AtomTransaction	txn3;
		try
		{
			StorageEnv::Storage::Cursor cursor = pstatus->cursor( txn3 );
			cursor.walk( calc_step2_q );
		}
		catch ( DbException e ) { throw; }
		catch ( ... )
		{
			DbException ee( DB_OLD_VERSION );
			txn3.abort( ee );
			throw ee;
		}
	}
	catch ( DbException e )
	{
		printf( "[FATAL] CalcWaitdelQuery_Step2, walk err , what=%s\n", e.what() );
	}

	StorageEnv::checkpoint();

	InsertWaitDel(waitdel_set,deltimeline);

	printf("\n\n\n TOTAL DEL count: %d \n",waitdel_set.size() );
}

class GetSigninDataQuery : public StorageEnv::IQuery
{
public:
    GetSigninDataQuery(int m) : month(m)
    {
        count = (month != 0 ? month : 12);
        for (int i = 0; i < MAX_AWARD; ++i)
            num[i] = 0;

        id_year.reserve(1024);
        id_perfect.reserve(128);
    }

    bool Update(StorageEnv::Transaction& txn, Octets& key, Octets& value)
    {
        Marshal::OctetsStream key_roleid, value_status;
        key_roleid = key;
        value_status = value;

        unsigned int roleid = 0;
        GRoleStatus status;
        GRoleStatusExtraProp extraprop;
        daily_signin info;

        int data = 0;
        int res = 0;

        try
        {
            key_roleid >> roleid;
            value_status >> status;

            if (status.extraprop.size() == 0)
                return true;

            Marshal::OctetsStream(status.extraprop) >> extraprop;
            std::map<int, Octets>::const_iterator iter = extraprop.data.find(1); // GROLE_STATUS_EXTRAPROP_DAILY_SIGN_IN

            if (iter == extraprop.data.end())
                return true;

            Marshal::OctetsStream(iter->second)
            >> info.update_time
            >> info.month_calendar
            >> info.curr_year_data
            >> info.last_year_data
            >> info.awarded_times
            >> info.late_signin_times
            >> info.reserved;

            if (month != 0)
                data = info.curr_year_data;
            else
                data = info.last_year_data;

            res = PERFECT_AWARD;
            for (int n = 0; n < count; ++n)
            {
                switch ((data >> (n << 1)) & 0x3)
                {
                    case OVER_3_TIMES_MISS: res = NO_AWARD; break;
                    case UNDER_3_TIMES_MISS: res = YEAR_AWARD; break;
                    default: break;
                }

                if (res == NO_AWARD) break;
            }

            ++num[res];

            if (res == YEAR_AWARD)
                id_year.push_back(roleid);
            else if (res == PERFECT_AWARD)
                id_perfect.push_back(roleid);
        }
        catch (...)
        {
            printf("GetSigninDataQuery, marshal error. roleid = %u\n", roleid);
        }

        return true;
    }

public:
    struct daily_signin
    {
        int update_time;
        int month_calendar;
        int curr_year_data;
        int last_year_data;
        char awarded_times;
        char late_signin_times;
        short reserved;
    };

    enum    
    {
        OVER_3_TIMES_MISS,
        UNDER_3_TIMES_MISS,
        NO_TIME_MISS,
        ALREADY_AWARDED,
    };

    enum    
    {
        NO_AWARD,
        YEAR_AWARD,
        PERFECT_AWARD,
        MAX_AWARD,
    };

    int month;
    int count;
    int num[MAX_AWARD];

    std::vector<unsigned int> id_year;
    std::vector<unsigned int> id_perfect;
};

void GetSigninData(int month)
{
    if ((month < 0) || (month > 12))
    {
        printf("input month (0 <= month <= 12) error. month = %d\n", month);
        return;
    }

    GetSigninDataQuery q(month);
    try
    {
        StorageEnv::Storage* pstatus = StorageEnv::GetStorage("status");
        StorageEnv::AtomTransaction txn;

        try
        {
            StorageEnv::Storage::Cursor cursor = pstatus->cursor(txn);
            cursor.walk(q);
        }
        catch (DbException e) {throw;}
        catch (...)
        {
            DbException ee(DB_OLD_VERSION);
            txn.abort(ee);
            throw ee;
        }
    }
    catch (DbException e)
    {
        Log::log(LOG_ERR, "GetSigninData, walk error. what = %s\n", e.what());
    }

    std::vector<unsigned int>::const_iterator iter, iter_end;
    printf("NO_AWARD: %d\n\n", q.num[GetSigninDataQuery::NO_AWARD]);

    printf("YEAR_AWARD: %d\n", q.num[GetSigninDataQuery::YEAR_AWARD]);
    iter = q.id_year.begin();
    iter_end = q.id_year.end();
    for (; iter != iter_end; ++iter)
        printf("%u\n", *iter);
    printf("\n");

    printf("PERFECT_AWARD: %d\n", q.num[GetSigninDataQuery::PERFECT_AWARD]);
    iter = q.id_perfect.begin();
    iter_end = q.id_perfect.end();
    for (; iter != iter_end; ++iter)
        printf("%u\n", *iter);
    printf("\n");
}

void SetClsPVPFlag(int flag)
{
    #pragma pack(1)
    struct player_var_data  //这个结构由gs定义
    {
        int version;
        int pk_count;
        int pvp_cooldown;
        bool pvp_flag;
    };
    #pragma pack()

    try
    {
        StorageEnv::Storage* pstatus = StorageEnv::GetStorage("status");
        StorageEnv::AtomTransaction txn;
        unsigned int roleid = 0;

        try
        {
            for (unsigned int cls = 0; cls < 12; ++cls)
            {
                Marshal::OctetsStream key_roleid, value_status;
                GRoleStatus status;

                roleid = GetDataRoleId(cls);
                key_roleid << RoleId(roleid);

                if (pstatus->find(key_roleid, value_status, txn))
                {
                    value_status >> status;
                    if (status.var_data.size() < sizeof(struct player_var_data))
                        continue;
                }

                struct player_var_data* p = (struct player_var_data*)status.var_data.begin();
                p->pvp_flag = (flag != 0);
                pstatus->insert(key_roleid, Marshal::OctetsStream() << status, txn);
            }
        }
        catch (Marshal::Exception&)
        {
            printf("SetClsPVPFlag, status marshal error, roleid = %u\n", roleid);
        }
        catch (DbException e) { throw; }
        catch (...)
        {
            DbException ee(DB_OLD_VERSION);
            txn.abort(ee);
            throw ee;
        }
    }
    catch (DbException e)
    {
        Log::log(LOG_ERR, "SetClsPVPFlag, db error. what = %s\n", e.what());
    }
}

class ListFinTaskCountQuery : public StorageEnv::IQuery
{
public:
	int count;
	int limit;
	bool hasmore;
	Octets handle;
	ListFinTaskCountQuery(int l): count(0),limit(l),hasmore(false){}

	bool Update( StorageEnv::Transaction& txn, Octets& key, Octets& value)
	{
		if(++count % 10000 == 0)
		{
			handle = key;
			hasmore = true;
			return false;
		}
		Marshal::OctetsStream key_os, value_os;
		key_os = key;
		value_os = value;

		RoleId		id;
	    GRoleTask task;
		try
		{
			key_os >> id;
			value_os >> task;
			int tc_size = task.task_complete.size();
			int tc_count = (tc_size - 4)/4;

			if(tc_count >= limit)
				printf("%d,\t%d\n",id.id,tc_count);

		} catch ( Marshal::Exception & ) {
			Log::log( LOG_ERR, "ListRoleCsvQuery, error unmarshal, roleid=%d.", id.id );
			return true;
		}
		return true;
	}

};

void ListFinTaskCount(int count)
{
	printf("roleid,\t");
	printf("fin-count\n");

	ListFinTaskCountQuery q(count);
	do
	{
		q.hasmore = false;
		try
		{
			StorageEnv::Storage * pstorage = StorageEnv::GetStorage( "task" );
			StorageEnv::AtomTransaction	txn;
			try
			{
				StorageEnv::Storage::Cursor cursor = pstorage->cursor( txn );
				cursor.walk( q.handle, q );
			}
			catch ( DbException e ) { throw; }
			catch ( ... )
			{
				DbException ee( DB_OLD_VERSION );
				txn.abort( ee );
				throw ee;
			}
		}
		catch ( DbException e )
		{
			Log::log( LOG_ERR, "ListFinTaskCount, error when walk, what=%s\n", e.what() );
		}
	
		StorageEnv::checkpoint();
	}while(q.hasmore);

}

class GetRoleLoginDataQuery : public StorageEnv::IQuery
{
public:
    GetRoleLoginDataQuery() : count(0), hasmore(false) {}
    bool Update(StorageEnv::Transaction& txn, Octets& key, Octets& value)
    {
        if (++count % 200000 == 0)
        {
            handle = key;
            hasmore = true;
            return false;
        }

        Marshal::OctetsStream key_roleid, value_base;
        key_roleid = key;

        GRoleBase base;
        unsigned int roleid = 0;
        unsigned int logicid = 0;
        int logintime = 0;

        try
        {
            StorageEnv::Storage* pbase = StorageEnv::GetStorage("base");
            value_base = pbase->find(key_roleid, txn);
            value_base >> base;

            roleid = base.id;
            logicid = (roleid & 0xfffffff0);
            logintime = base.lastlogin_time;
            roleid_login_data[roleid] = logintime;

            std::map<unsigned int, int>::iterator iter = logicid_login_data.find(logicid);
            if ((iter == logicid_login_data.end()) || (iter->second < logintime))
            {
                logicid_login_data[logicid] = logintime;
            }
        }
        catch (Marshal::Exception&)
        {
            Log::log(LOG_ERR, "GetRoleLoginDataQuery, base marshal error. roleid = %u", roleid);
        }
        catch (DbException e)
        {
            Log::log(LOG_ERR, "GetRoleLoginDataQuery, db error. roleid = %u, what = %s\n", roleid, e.what());
        }
        catch (...)
        {
            DbException ee(DB_OLD_VERSION);
            txn.abort(ee);
            Log::log(LOG_ERR, "GetRoleLoginDataQuery, other error. roleid = %u", roleid);
        }
        return true;
    }

public:
    int count;
    bool hasmore;
    Octets handle;

    std::map<unsigned int, int> roleid_login_data;
    std::map<unsigned int, int> logicid_login_data;
};

void GetRoleLoginData(int year, int logicid)
{
    if ((year < 2008) || (logicid < 0)) return;
    GetRoleLoginDataQuery q;

    do
    {
        q.hasmore = false;
        try
        {
            StorageEnv::Storage* pbase = StorageEnv::GetStorage("base");
            StorageEnv::AtomTransaction txn;
            try
            {
                StorageEnv::Storage::Cursor cursor = pbase->cursor(txn);
                cursor.walk(q.handle, q);
            }
            catch (DbException e) {throw;}
            catch (...)
            {
                DbException ee(DB_OLD_VERSION);
                txn.abort(ee);
                throw ee;
            }
        }
        catch (DbException e)
        {
            Log::log(LOG_ERR, "GetRoleLoginData, error when walk, what = %s\n", e.what());
        }
        StorageEnv::checkpoint();
    } while (q.hasmore);

    struct tm dt;
    dt.tm_year = year - 1900;
    dt.tm_mon = 0;
    dt.tm_mday = 1;
    dt.tm_hour = 0;
    dt.tm_min = 0;
    dt.tm_sec = 1;

    time_t time = mktime(&dt);
    std::map<unsigned int, int>::const_iterator iter, iter_end;

    int roleid_num = 0;
    iter = q.roleid_login_data.begin();
    iter_end = q.roleid_login_data.end();

    for (; iter != iter_end; ++iter)
    {
        if (iter->second < time) ++roleid_num;
    }

    printf("number of roleid (last_login_time < %d): %d\n", year, roleid_num);
    printf("number of roleid (last_login_time >= %d): %d\n\n", year, (q.roleid_login_data.size() - roleid_num));

    int logicid_num1 = 0;
    int logicid_num2 = 0;
    iter = q.logicid_login_data.begin();
    iter_end = q.logicid_login_data.end();

    for (; iter != iter_end; ++iter)
    {
        if (iter->second < time) ++logicid_num1;
        if (iter->first < (unsigned int)logicid) ++logicid_num2;
    }

    printf("number of logicid (last_login_time < %d): %d\n", year, logicid_num1);
    printf("number of logicid (last_login_time >= %d): %d\n\n", year, (q.logicid_login_data.size() - logicid_num1));

    printf("number of logicid (logic_id < %d): %d\n", logicid, logicid_num2);
    printf("number of logicid (logic_id >= %d): %d\n", logicid, (q.logicid_login_data.size() - logicid_num2));
}


class GetUserLoginDataQuery : public StorageEnv::IQuery
{
public:
    GetUserLoginDataQuery() : count(0), hasmore(false) {}
    bool Update(StorageEnv::Transaction& txn, Octets& key, Octets& value)
    {
        #pragma pack(1)
        struct login_record
        {
            int login_time;
        };
        #pragma pack()

        if (++count % 200000 == 0)
        {
            handle = key;
            hasmore = true;
            return false;
        }

        Marshal::OctetsStream key_userid, value_user;
        key_userid = key;

        RoleId id;
        User user;
        unsigned int userid = 0;
        unsigned int logicid = 0;
        int logintime = 0;

        try
        {
            StorageEnv::Storage* puser = StorageEnv::GetStorage("user");
            value_user = puser->find(key_userid, txn);
            value_user >> user;

            key_userid >> id;
            userid = id.id;
            logicid = user.logicuid;

            struct login_record* p = (struct login_record*)user.login_record.begin();
            logintime = p->login_time;
            userid_login_data[userid] = logintime;

            std::map<unsigned int, int>::iterator iter = logicid_login_data.find(logicid);
            if ((iter == logicid_login_data.end()) || (iter->second < logintime))
            {
                logicid_login_data[logicid] = logintime;
            }
        }
        catch (Marshal::Exception&)
        {
            Log::log(LOG_ERR, "GetUserLoginDataQuery, marshal error. userid = %u", userid);
        }
        catch (DbException e)
        {
            Log::log(LOG_ERR, "GetUserLoginDataQuery, db error. userid = %u, what = %s\n", userid, e.what());
        }
        catch (...)
        {
            DbException ee(DB_OLD_VERSION);
            txn.abort(ee);
            Log::log(LOG_ERR, "GetUserLoginDataQuery, other error. userid = %u", userid);
        }
        return true;
    }

public:
    int count;
    bool hasmore;
    Octets handle;

    std::map<unsigned int, int> userid_login_data;
    std::map<unsigned int, int> logicid_login_data;
};

void GetUserLoginData(int year, int logicid)
{
    if ((year < 2008) || (logicid < 0)) return;
    GetUserLoginDataQuery q;

    do
    {
        q.hasmore = false;
        try
        {
            StorageEnv::Storage* puser = StorageEnv::GetStorage("user");
            StorageEnv::AtomTransaction txn;
            try
            {
                StorageEnv::Storage::Cursor cursor = puser->cursor(txn);
                cursor.walk(q.handle, q);
            }
            catch (DbException e) {throw;}
            catch (...)
            {
                DbException ee(DB_OLD_VERSION);
                txn.abort(ee);
                throw ee;
            }
        }
        catch (DbException e)
        {
            Log::log(LOG_ERR, "GetUserLoginData, error when walk, what = %s\n", e.what());
        }
        StorageEnv::checkpoint();
    } while (q.hasmore);

    struct tm dt;
    dt.tm_year = year - 1900;
    dt.tm_mon = 0;
    dt.tm_mday = 1;
    dt.tm_hour = 0;
    dt.tm_min = 0;
    dt.tm_sec = 1;

    time_t time = mktime(&dt);
    std::map<unsigned int, int>::const_iterator iter, iter_end;

    int userid_num = 0;
    iter = q.userid_login_data.begin();
    iter_end = q.userid_login_data.end();

    for (; iter != iter_end; ++iter)
    {
        if (iter->second < time) ++userid_num;
    }

    printf("number of userid (last_login_time < %d): %d\n", year, userid_num);
    printf("number of userid (last_login_time >= %d): %d\n\n", year, (q.userid_login_data.size() - userid_num));

    int logicid_num1 = 0;
    int logicid_num2 = 0;
    iter = q.logicid_login_data.begin();
    iter_end = q.logicid_login_data.end();

    for (; iter != iter_end; ++iter)
    {
        if (iter->second < time) ++logicid_num1;
        if (iter->first < (unsigned int)logicid) ++logicid_num2;
    }

    printf("number of logicid (last_login_time < %d): %d\n", year, logicid_num1);
    printf("number of logicid (last_login_time >= %d): %d\n\n", year, (q.logicid_login_data.size() - logicid_num1));

    printf("number of logicid (logic_id < %d): %d\n", logicid, logicid_num2);
    printf("number of logicid (logic_id >= %d): %d\n", logicid, (q.logicid_login_data.size() - logicid_num2));
}


void OctetstoPrintString(Octets& src, Octets& dst)
{
    Octets tmp;
    CharsetConverter::conv_charset_u2l(src, tmp);
    EscapeCSVString(tmp, dst);
}

class QueryListMNFactionInfo : public StorageEnv::IQuery
{
public:
    QueryListMNFactionInfo(MNFactionInfoVector& data) : _data(data) {}

    bool Update(StorageEnv::Transaction& txn, Octets& key, Octets& value)
    {
        try
        {
            int64_t unifid = 0;
            Marshal::OctetsStream(key) >> unifid;

            if (unifid == 0)
            {
                unsigned int sn = 0;
                unsigned char state = 0;

                Marshal::OctetsStream(value) >> sn >> state;
                printf("\nQueryListMNFactionInfo: key=0, sn=%u, state=%d.\n", sn, state);
            }
            else
            {
                MNFactionInfo info;
                Marshal::OctetsStream(value) >> info;
                _data.push_back(info);
            }
        }
        catch (Marshal::Exception&)
        {
            Log::log(LOG_ERR, "QueryListMNFactionInfo, marshal error.");
        }

        return true;
    }

private:
    MNFactionInfoVector& _data;

};

void ListMNFactionInfo()
{
    MNFactionInfoVector info_list;
    QueryListMNFactionInfo q(info_list);

    try
    {
        StorageEnv::Storage* pmnfaction = StorageEnv::GetStorage("mnfactioninfo");
        StorageEnv::AtomTransaction txn;

        try
        {
            StorageEnv::Storage::Cursor cursor = pmnfaction->cursor(txn);
            cursor.walk(q);
        }
        catch (DbException e) {throw;}
        catch (...)
        {
            DbException e(DB_OLD_VERSION);
            txn.abort(e);
            throw e;
        }
    }
    catch (DbException e)
    {
        Log::log(LOG_ERR, "ListMNFactionInfo, what=%s.\n", e.what());
    }

    printf("\nListMNFactionInfo: num=%u.\n", info_list.size());
    MNFactionInfoVector::const_iterator iter = info_list.begin(), iter_end = info_list.end();

    for (; iter != iter_end; ++iter)
    {
        printf("\nunifid=%lld, fid=%u, faction_name_size=%u, master_name_size=%u, zoneid=%d, domain_num_size=%u, credit=%d, credit_this_week=%d, credit_get_time=%d, invite_count=%u, accept_sn=%u, bonus_sn=%u, version=%u, ",
            (iter->unifid), (iter->fid), (iter->faction_name.size()), (iter->master_name.size()), (iter->zoneid), (iter->domain_num.size()), (iter->credit), (iter->credit_this_week), (iter->credit_get_time), (iter->invite_count), (iter->accept_sn), (iter->bonus_sn), (iter->version));

        for (size_t i = 0; i < (iter->domain_num.size()); ++i)
        {
            printf("domain[%d]=%d, ", i, (iter->domain_num[i]));
        }

        Octets faction_name, fname = (iter->faction_name);
        OctetstoPrintString(fname, faction_name);
        printf("faction_name=(%.*s), ", faction_name.size(), (char*)faction_name.begin());

        Octets master_name, mname = (iter->master_name);
        OctetstoPrintString(mname, master_name);
        printf("master_name=(%.*s)", master_name.size(), (char*)master_name.begin());
    }
    printf("\n\n");
}


class QueryListMNFactionApplyInfo : public StorageEnv::IQuery
{
public:
    QueryListMNFactionApplyInfo(MNFactionApplyInfoVector& data) : _data(data) {}

    bool Update(StorageEnv::Transaction& txn, Octets& key, Octets& value)
    {
        try
        {
            MNFactionApplyInfo info;
            Marshal::OctetsStream(value) >> info;
            _data.push_back(info);
        }
        catch (Marshal::Exception&)
        {
            Log::log(LOG_ERR, "QueryListMNFactionApplyInfo, marshal error.");
        }

        return true;
    }

private:
    MNFactionApplyInfoVector& _data;

};

void ListMNFactionApplyInfo()
{
    MNFactionApplyInfoVector applyinfo_list;
    QueryListMNFactionApplyInfo q(applyinfo_list);

    try
    {
        StorageEnv::Storage* pappinfo = StorageEnv::GetStorage("mnfactionapplyinfo");
        StorageEnv::AtomTransaction txn;

        try
        {
            StorageEnv::Storage::Cursor cursor = pappinfo->cursor(txn);
            cursor.walk(q);
        }
        catch (DbException e) {throw;}
        catch (...)
        {
            DbException e(DB_OLD_VERSION);
            txn.abort(e);
            throw e;
        }
    }
    catch (DbException e)
    {
        Log::log(LOG_ERR, "ListMNFactionApplyInfo, what=%s.\n", e.what());
    }

    printf("\nListMNFactionApplyInfo: num=%u.\n", applyinfo_list.size());
    MNFactionApplyInfoVector::const_iterator iter = applyinfo_list.begin(), iter_end = applyinfo_list.end();

    for (; iter != iter_end; ++iter)
    {
        printf("unifid=%lld, applicant_id=%d, dest=%u, cost=%u.\n",
            (iter->unifid), (iter->applicant_id), (iter->dest), (iter->cost));
    }
}


class QueryListMNDomainInfo : public StorageEnv::IQuery
{
public:
    QueryListMNDomainInfo(MNDomainInfoVector& data) : _data(data) {}

    bool Update(StorageEnv::Transaction& txn, Octets& key, Octets& value)
    {
        try
        {
            MNDomainInfo info;
            Marshal::OctetsStream(value) >> info;
            _data.push_back(info);
        }
        catch (Marshal::Exception&)
        {
            Log::log(LOG_ERR, "QueryListMNDomainInfo, marshal error.");
        }

        return true;
    }

private:
    MNDomainInfoVector& _data;

};

void ListMNDomainInfo()
{
    MNDomainInfoVector domain_list;
    QueryListMNDomainInfo q(domain_list);

    try
    {
        StorageEnv::Storage* pdomain = StorageEnv::GetStorage("mndomaininfo");
        StorageEnv::AtomTransaction txn;

        try
        {
            StorageEnv::Storage::Cursor cursor = pdomain->cursor(txn);
            cursor.walk(q);
        }
        catch (DbException e) {throw;}
        catch (...)
        {
            DbException e(DB_OLD_VERSION);
            txn.abort(e);
            throw e;
        }
    }
    catch (DbException e)
    {
        Log::log(LOG_ERR, "ListMNDomainInfo, what=%s.\n", e.what());
    }

    printf("\nListMNDomainInfo: num=%u.\n", domain_list.size());
    MNDomainInfoVector::const_iterator iter = domain_list.begin(), iter_end = domain_list.end();

    for (; iter != iter_end; ++iter)
    {
        printf("domain_id=%d, domain_type=%d, owner_unifid=%lld, attacker_unifid=%lld, defender_unifid=%lld.\n",
            (iter->domain_id), (iter->domain_type), (iter->owner_unifid), (iter->attacker_unifid), (iter->defender_unifid));
    }
}


typedef std::vector<GFactionInfo> FactionInfoVector;

class QueryClearFactionInfoMNFid : public StorageEnv::IQuery
{
public:
    QueryClearFactionInfoMNFid(FactionInfoVector& data) : _data(data) {}

    bool Update(StorageEnv::Transaction& txn, Octets& key, Octets& value)
    {
        try
        {
            unsigned int fid = 0;
            Marshal::OctetsStream(key) >> fid;

            if (fid == 0)
            {
                unsigned int sn = 0;
                Marshal::OctetsStream(value) >> sn;
                printf("\nQueryClearFactionInfoMNFid: key=0, sn=%u.\n", sn);
            }
            else
            {
                GFactionInfo info;
                Marshal::OctetsStream(value) >> info;

                if (info.extenddata.size() > 0)
                {
                    GFactionExtendData ext;
                    Marshal::OctetsStream(info.extenddata) >> ext;

                    printf("QueryClearFactionInfoMNFid: fid=%u, unifid=%lld.\n", info.fid, ext.unifid);

                    if (ext.unifid != 0)
                    {
                        ext.unifid = 0;
                        info.extenddata = (Marshal::OctetsStream() << ext);
                        _data.push_back(info);
                    }
                }
            }
        }
        catch (Marshal::Exception&)
        {
            Log::log(LOG_ERR, "QueryClearFactionInfoMNFid, marshal error.");
        }

        return true;
    }

private:
    FactionInfoVector& _data;

};


typedef std::vector<GUserFaction> UserFactionVector;

class QueryClearUserFactionMNFid : public StorageEnv::IQuery
{
public:
    QueryClearUserFactionMNFid(UserFactionVector& data) : _data(data) {}

    bool Update(StorageEnv::Transaction& txn, Octets& key, Octets& value)
    {
        try
        {
            GUserFaction info;
            Marshal::OctetsStream(value) >> info;

            if (info.extend.size() > 0)
            {
                GFactionExtend ext;
                Marshal::OctetsStream(info.extend) >> ext;

                if (ext.uniqueinfo.size() > 0)
                {
                    GUniqueInfo uni;
                    Marshal::OctetsStream(ext.uniqueinfo) >> uni;

                    printf("QueryClearUserFactionMNFid: rid=%u, fid=%u, unifid=%lld.\n", info.rid, info.fid, uni.unifid);

                    if (uni.unifid != 0)
                    {
                        uni.unifid = 0;
                        ext.uniqueinfo = (Marshal::OctetsStream() << uni);
                        info.extend = (Marshal::OctetsStream() << ext);
                        _data.push_back(info);
                    }
                }
            }
        }
        catch (Marshal::Exception&)
        {
            Log::log(LOG_ERR, "QueryClearUserFactionMNFid, marshal error.");
        }

        return true;
    }

private:
    UserFactionVector& _data;

};


void ClearMNFactionId()
{
    FactionInfoVector vec1;
    QueryClearFactionInfoMNFid q1(vec1);

    UserFactionVector vec2;
    QueryClearUserFactionMNFid q2(vec2);

    try
    {
        StorageEnv::Storage* pfactioninfo = StorageEnv::GetStorage("factioninfo");
        StorageEnv::Storage* puserfaction = StorageEnv::GetStorage("userfaction");
        StorageEnv::AtomTransaction txn;

        try
        {
            StorageEnv::Storage::Cursor cursor1 = pfactioninfo->cursor(txn);
            cursor1.walk(q1);

            StorageEnv::Storage::Cursor cursor2 = puserfaction->cursor(txn);
            cursor2.walk(q2);

            pfactioninfo->del((Marshal::OctetsStream() << (unsigned int)0), txn);

            printf("\nClearMNFactionId: faction_info_num=%u.\n", vec1.size());
            FactionInfoVector::const_iterator it1 = vec1.begin(), it_end1 = vec1.end();
            for (; it1 != it_end1; ++it1)
            {
                printf("faction_id=%u\n", (it1->fid));
                pfactioninfo->insert((Marshal::OctetsStream() << (it1->fid)),
                                     (Marshal::OctetsStream() << (*it1)),
                                     txn);
            }

            printf("\nClearMNFactionId: user_faction_num=%u.\n", vec2.size());
            UserFactionVector::const_iterator it2 = vec2.begin(), it_end2 = vec2.end();
            for (; it2 != it_end2; ++it2)
            {
                printf("role_id=%u\n", (it2->rid));
                puserfaction->insert((Marshal::OctetsStream() << (it2->rid)),
                                     (Marshal::OctetsStream() << (*it2)),
                                     txn);
            }
        }
        catch (DbException e) {throw;}
        catch (...)
        {
            DbException e(DB_OLD_VERSION);
            txn.abort(e);
            throw e;
        }
    }
    catch (DbException e)
    {
        Log::log(LOG_ERR, "ClearMNFactionId, what=%s.\n", e.what());
    }
}


void ImportMNFactionInfo(const char* mnfactioninfofile)
{
    MNFactionInfoVector info_list;
    std::ifstream ifs(mnfactioninfofile);
    if (ifs.fail()) return;

    int domain_num[3] = {0};
    for (; !ifs.eof(); )
    {
        MNFactionInfo info;
        ifs >> info.unifid >> info.fid >> info.zoneid >>
            domain_num[0] >> domain_num[1] >> domain_num[2] >>
            info.credit >> info.credit_this_week >> info.credit_get_time >>
            info.invite_count >> info.accept_sn >> info.bonus_sn >> info.version;

        if (ifs.fail()) break;
        if (info.unifid != 0)
        {
            info.domain_num.resize(3);
            info.domain_num[0] = domain_num[0];
            info.domain_num[1] = domain_num[1];
            info.domain_num[2] = domain_num[2];
            info_list.push_back(info);
        }
    }

    printf("\nImportMNFactionInfo: num=%u.\n", info_list.size());

    try
    {
        StorageEnv::Storage* pmnfaction = StorageEnv::GetStorage("mnfactioninfo");
        StorageEnv::AtomTransaction txn;

        try
        {
            MNFactionInfoVector::iterator iter = info_list.begin(), iter_end = info_list.end();
            for (; iter != iter_end; ++iter)
            {
                Marshal::OctetsStream key, value;
                key << (iter->unifid);

                if (pmnfaction->find(key, value, txn))
                {
                    MNFactionInfo info;
                    value >> info;

                    iter->faction_name = info.faction_name;
                    iter->master_name = info.master_name;

                    pmnfaction->insert(key, (Marshal::OctetsStream() << (*iter)), txn);
                }
            }
        }
        catch (DbException e) {throw;}
        catch (...)
        {
            DbException e(DB_OLD_VERSION);
            txn.abort(e);
            throw e;
        }
    }
    catch (DbException e)
    {
        Log::log(LOG_ERR, "ImportMNFactionInfo, what=%s.\n", e.what());
    }
}


void ImportMNFactionApplyInfo(const char* mnfactionapplyinfofile)
{
    MNFactionApplyInfoVector info_list;
    std::ifstream ifs(mnfactionapplyinfofile);
    if (ifs.fail()) return;

    int dest = 0;
    for (; !ifs.eof(); )
    {
        MNFactionApplyInfo info;
        ifs >> info.unifid >> info.applicant_id >> dest >> info.cost;

        if (ifs.fail()) break;
        if (info.unifid != 0)
        {
            info.dest = dest;
            info_list.push_back(info);
        }
    }

    printf("\nImportMNFactionApplyInfo: num=%u.\n", info_list.size());

    try
    {
        StorageEnv::Storage* pmnfactionapp = StorageEnv::GetStorage("mnfactionapplyinfo");
        StorageEnv::AtomTransaction txn;

        try
        {
            MNFactionApplyInfoVector::const_iterator iter = info_list.begin(), iter_end = info_list.end();
            for (; iter != iter_end; ++iter)
            {
                Marshal::OctetsStream key, value;
                key << (iter->unifid);

                if (pmnfactionapp->find(key, value, txn))
                {
                    pmnfactionapp->insert(key, (Marshal::OctetsStream() << (*iter)), txn);
                }
            }
        }
        catch (DbException e) {throw;}
        catch (...)
        {
            DbException e(DB_OLD_VERSION);
            txn.abort(e);
            throw e;
        }
    }
    catch (DbException e)
    {
        Log::log(LOG_ERR, "ImportMNFactionApplyInfo, what=%s.\n", e.what());
    }
}


void ImportMNDomainInfo(const char* mndomaininfofile)
{
    MNDomainInfoVector info_list;
    std::ifstream ifs(mndomaininfofile);
    if (ifs.fail()) return;

    int domain_type = 0;
    for (; !ifs.eof(); )
    {
        MNDomainInfo info;
        ifs >> info.domain_id >> domain_type >> info.owner_unifid >>
            info.attacker_unifid >> info.defender_unifid;

        if (ifs.fail()) break;
        if (info.domain_id >= 0)
        {
            info.domain_type = domain_type;
            info_list.push_back(info);
        }
    }

    printf("\nImportMNDomainInfo: num=%u.\n", info_list.size());

    try
    {
        StorageEnv::Storage* pmndomain = StorageEnv::GetStorage("mndomaininfo");
        StorageEnv::AtomTransaction txn;

        try
        {
            MNDomainInfoVector::const_iterator iter = info_list.begin(), iter_end = info_list.end();
            for (; iter != iter_end; ++iter)
            {
                Marshal::OctetsStream key, value;
                key << (iter->domain_id);

                if (pmndomain->find(key, value, txn))
                {
                    pmndomain->insert(key, (Marshal::OctetsStream() << (*iter)), txn);
                }
            }
        }
        catch (DbException e) {throw;}
        catch (...)
        {
            DbException e(DB_OLD_VERSION);
            txn.abort(e);
            throw e;
        }
    }
    catch (DbException e)
    {
        Log::log(LOG_ERR, "ImportMNDomainInfo, what=%s.\n", e.what());
    }
}

void SetMNFactionState(int state)
{
	try
	{
		StorageEnv::Storage* pmnfaction = StorageEnv::GetStorage("mnfactioninfo");
		StorageEnv::AtomTransaction txn;
		unsigned int sn = 0;
		unsigned char cur_state = 0;
		Marshal::OctetsStream spec_key, spec_value;
		spec_key << (int64_t)0;

		if (pmnfaction->find(spec_key, spec_value, txn))
			spec_value >> sn >> cur_state;

		spec_value = (Marshal::OctetsStream() << sn << (unsigned char)state);
		pmnfaction->insert(spec_key, spec_value, txn);
		printf("SetMNFactionState sn=%d:oldstate=%d:newstate=%d\n", sn, cur_state, state);
	}
    catch (DbException e)
    {
        printf("SetMNFactionState, what=%s.\n", e.what());
    }
}


class QueryClearMNFactionInfoCredit: public StorageEnv::IQuery
{
public:
    QueryClearMNFactionInfoCredit(MNFactionInfoVector& data) : _data(data) {}

    bool Update(StorageEnv::Transaction& txn, Octets& key, Octets& value)
    {
        try
        {
            int64_t unifid = 0;
            Marshal::OctetsStream(key) >> unifid;
			if(unifid != 0)
			{
				MNFactionInfo info;
				Marshal::OctetsStream(value) >> info;
				printf("QueryClearMNFactionInfoCredit: unifid=%lld,credit=%d\n", info.unifid, info.credit);
				info.credit = 0;
				_data.push_back(info);
			}
		}
		catch (Marshal::Exception&)
		{
			Log::log(LOG_ERR, "QueryClearMNFactionInfoCredit, marshal error.");
		}

		return true;
	}

private:
	MNFactionInfoVector& _data;

};

void ClearMNFactionCredit()
{
	MNFactionInfoVector vec;
	QueryClearMNFactionInfoCredit q(vec);
	try
	{
		StorageEnv::Storage* pmnfaction = StorageEnv::GetStorage("mnfactioninfo");
		StorageEnv::AtomTransaction txn;
		try
		{
			StorageEnv::Storage::Cursor cursor = pmnfaction->cursor(txn);
			cursor.walk(q);
			MNFactionInfoVector::const_iterator it = vec.begin(), it_end = vec.end();
			for (; it != it_end; ++it)
			{
				printf("unifid=%lld,credit=%d\n", it->unifid,it->credit);
				pmnfaction->insert((Marshal::OctetsStream() << (it->unifid)),
						(Marshal::OctetsStream() << (*it)),
						txn);
			}
		}
		catch (DbException e) {throw;}
		catch (...)
		{
			DbException e(DB_OLD_VERSION);
			txn.abort(e);
			throw e;
		}
	}
	catch (DbException e)
	{
		Log::log(LOG_ERR, "ClearMNFactionCredit, what=%s.\n", e.what());
	}

}


void ListSoloChallengeRank()
{
    SoloChallengeRankDataExt rank_local, rank_global;
    Marshal::OctetsStream value;

    try
    {
        StorageEnv::Storage* prank = StorageEnv::GetStorage("solochallengerank");
        StorageEnv::AtomTransaction txn;

        try
        {
            if (prank->find((Marshal::OctetsStream() << 0), value, txn))
            {
                value >> rank_local;
            }

            if (prank->find((Marshal::OctetsStream() << 1), value, txn))
            {
                value >> rank_global;
            }
        }
        catch (DbException e) {throw;}
        catch (...)
        {
            DbException e(DB_OLD_VERSION);
            txn.abort(e);
            throw e;
        }
    }
    catch (DbException e)
    {
        Log::log(LOG_ERR, "ListSoloChallengeRank, what=%s.\n", e.what());
    }

    printf("\nListSoloChallengeRank: local rank, update_time=%d, data_size=%d, zone_id=%d.\n",
        rank_local.update_time, rank_local.data.size(), rank_local.zoneid);

    SoloChallengeRankDataVector::const_iterator it1 = rank_local.data.begin(), it1_end = rank_local.data.end();
    for (; it1 != it1_end; ++it1)
    {
        printf("roleid=%d, level=%d, cls=%d, total_time=%d, name_size=%d, type=%d, zoneid=%d, update_time=%d, ",
            it1->roleid, it1->level, it1->cls, it1->total_time, it1->name.size(), it1->type, it1->zoneid, it1->update_time);

        Octets name, rname = (it1->name);
        OctetstoPrintString(rname, name);
        printf("name=(%.*s).\n", name.size(), (char*)name.begin());
    }

    printf("\n\n");


    printf("\nListSoloChallengeRank: global rank, update_time=%d, data_size=%d, zone_id=%d.\n",
        rank_global.update_time, rank_global.data.size(), rank_global.zoneid);

    SoloChallengeRankDataVector::const_iterator it2 = rank_global.data.begin(), it2_end = rank_global.data.end();
    for (; it2 != it2_end; ++it2)
    {
        printf("roleid=%d, level=%d, cls=%d, total_time=%d, name_size=%d, type=%d, zoneid=%d, update_time=%d, ",
            it2->roleid, it2->level, it2->cls, it2->total_time, it2->name.size(), it2->type, it2->zoneid, it2->update_time);

        Octets name, rname = (it2->name);
        OctetstoPrintString(rname, name);
        printf("name=(%.*s).\n", name.size(), (char*)name.begin());
    }

    printf("\n\n");
}


void ClearSoloChallengeRank(int roleid, int zoneid)
{
    SoloChallengeRankDataExt rank_global;
    Marshal::OctetsStream value;

    try
    {
        StorageEnv::Storage* prank = StorageEnv::GetStorage("solochallengerank");
        StorageEnv::AtomTransaction txn;

        try
        {
            if (prank->find((Marshal::OctetsStream() << 1), value, txn))
            {
                value >> rank_global;
                SoloChallengeRankDataVector::iterator iter = rank_global.data.begin(), iter_end = rank_global.data.end();

                bool changed = false;
                for (; iter != iter_end; ++iter)
                {
                    if ((iter->roleid == roleid) && (iter->zoneid == zoneid))
                    {
                        rank_global.data.erase(iter);
                        changed = true;
                    }
                }

                if (changed)
                {
                    prank->insert((Marshal::OctetsStream() << 1),
                                  (Marshal::OctetsStream() << rank_global),
                                  txn);
                }
            }
        }
        catch (DbException e) {throw;}
        catch (...)
        {
            DbException e(DB_OLD_VERSION);
            txn.abort(e);
            throw e;
        }
    }
    catch (DbException e)
    {
        Log::log(LOG_ERR, "ClearSoloChallengeRank, what=%s.\n", e.what());
    }
}


};


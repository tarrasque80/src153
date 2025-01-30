#ifndef _ITEM_ELF_EXPPILL_H_
#define _ITEM_ELF_EXPPILL_H_

#include <stddef.h>
#include "../item.h" 
#include "../config.h"

struct elf_exppill_essence
{
	unsigned int exp;	
	int level;
};

class elf_exppill_item : public item_body
{
	struct elf_exppill_essence ess;
	
public:
	DECLARE_SUBSTANCE(elf_exppill_item);

	virtual bool Load(archive & ar); 
public:
	//item_body中纯虚函数
	ITEM_TYPE GetItemType()  { return ITEM_TYPE_ELF_EXPPILL;}
	bool ArmorDecDurability(int) { return false;}
	item_body * Clone() const { return new elf_exppill_item(*this);}


public:
	bool IsItemCanUseWithArg(item::LOCATION l, unsigned int arg_size)
	{
		return l == item::INVENTORY && arg_size == sizeof(unsigned int) && ess.exp > 0;
	}
	int OnUse(item::LOCATION l, int index, gactive_imp * imp, const char * arg, unsigned int arg_size);

public:
	unsigned short _crc;
	unsigned short GetDataCRC() { return _crc; }
	void CalcCRC()
	{   
		_crc = crc16( (unsigned char *)&ess, sizeof(ess));
	}

	void GetItemData(const void ** data, unsigned int &len)
	{
		*data = &ess;
		len = sizeof(ess);
 	}
	
public:
	elf_exppill_item():_crc(0){memset(&ess, 0, sizeof(ess));}
};






#endif

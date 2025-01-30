#ifndef _ITEM_INCSKILLABILITY_H_
#define _ITEM_INCSKILLABILITY_H_

#include <stddef.h>
#include "../item.h" 
#include "../config.h"

class incskillability_item : public item_body
{
public:
	DECLARE_SUBSTANCE(incskillability_item);

public:
	//item_body中纯虚函数
	ITEM_TYPE GetItemType()  { return ITEM_TYPE_INCSKILLABILITY;}
	bool ArmorDecDurability(int) { return false;}
	item_body * Clone() const { return new incskillability_item(*this);}

public:
	virtual bool IsItemCanUse(item::LOCATION l){return true;}
	virtual int OnUse(item::LOCATION l, gactive_imp * imp, unsigned int count);
};

#endif

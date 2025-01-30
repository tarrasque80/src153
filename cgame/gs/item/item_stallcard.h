#ifndef _ITEM_STALLCARD_H_
#define _ITEM_STALLCARD_H_

#include <stddef.h>
#include "../item.h" 
#include "../config.h"

class stallcard_item : public item_body
{
public:
	DECLARE_SUBSTANCE(stallcard_item);

public:
	//item_body中纯虚函数
	ITEM_TYPE GetItemType()  { return ITEM_TYPE_STALLCARD;}
	bool ArmorDecDurability(int) { return false;}
	item_body * Clone() const { return new stallcard_item(*this);}

public:	
	bool VerifyRequirement(item_list & list, gactive_imp* imp);
	void OnActivate(item::LOCATION, unsigned int pos, unsigned int count, gactive_imp* imp){}
	void OnDeactivate(item::LOCATION, unsigned int pos, unsigned int count,gactive_imp* imp){}
	virtual void OnPutIn(item::LOCATION l,item_list & list,unsigned int pos,unsigned int count,gactive_imp* obj); 
	virtual void OnTakeOut(item::LOCATION l,unsigned int pos, unsigned int count, gactive_imp* obj);
};

#endif

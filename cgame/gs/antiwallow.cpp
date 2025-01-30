#include "antiwallow.h"

namespace anti_wallow
{



static punitive_param list[MAX_WALLOW_LEVEL] = {{0}};



const punitive_param & GetParam(unsigned int level)
{
	if(level >= MAX_WALLOW_LEVEL)
	{
		level = MAX_WALLOW_LEVEL - 1;
	}
	return list[level];
}

void SetParam(unsigned int level, const punitive_param & param)
{
	if(level >= MAX_WALLOW_LEVEL)
	{
		level = MAX_WALLOW_LEVEL - 1;
	}
	list[level] = param;
	list[level].active = true;
}

void AdjustNormalExpSP(unsigned int level, int & exp, int & sp)
{
	const punitive_param & param = GetParam(level);
	exp = (int)(exp * param.exp + 0.5f);
	sp = (int)(sp * param.sp + 0.5f);
}

void AdjustTaskExpSP(unsigned int level, int & exp, int & sp)
{
	const punitive_param & param = GetParam(level);
	exp = (int)(exp * param.task_exp + 0.5f);
	sp = (int)(sp * param.task_sp + 0.5f);
}

void AdjustNormalMoneyItem(unsigned int level, float & money, float & item)
{
	const punitive_param & param = GetParam(level);
	money *= param.money;
	item *= param.item;
}

void AdjustTaskMoney(unsigned int level, int & money)
{
	const punitive_param & param = GetParam(level);
	money = (int)(money * param.task_money + 0.5f);
}
}


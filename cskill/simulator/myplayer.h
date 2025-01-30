#ifndef __SKILL_MYPLAYER
#define __SKILL_MYPLAYER

#include "skill.h"
#include "playerwrapper.h"

extern FILE* log_attack;

namespace GNET{

class MyPlayer : public PlayerWrapper
{
public:
	MyPlayer(object_interface o,TargetWrapper *t,SkillWrapper * w,Skill *s):PlayerWrapper(o,t,w,s) {}
	bool SetResurrect(bool)
	{
		fprintf(log_attack, "        ����    ��ʧ������� %.2f\n", ratio);
		return true;
	}
	bool SetSpeedup(bool)
	{
		fprintf(log_attack, "        ����    ����%.2f  ʱ��%.2f\n", ratio, time/1000);
		return true;
	}
	bool SetIncdefence(bool)
	{
		fprintf(log_attack, "        �����������    ����%.2f  ʱ��%.2f\n", ratio, time/1000);
		return true;
	}
	bool SetIncattack(bool)
	{
		fprintf(log_attack, "        ����������    ����%.2f  ʱ��%.2f\n", ratio, time/1000);
		return true;
	}
	bool SetIncmagic(bool)
	{
		fprintf(log_attack, "        ���ӷ�������    ����%.2f  ʱ��%.2f\n", ratio, time/1000);
		return true;
	}
	bool SetAbsorbhp(bool)
	{
		fprintf(log_attack, "        ��ȡHP          ����%.2f  ����%.2f\n", ratio, probability);
		return true;
	}
	bool SetInchp(bool)
	{
		fprintf(log_attack, "        HP��������%.2f ʱ��%.2f\n", ratio, time/1000);
		return true;
	}
	bool SetDechp(bool)
	{
		fprintf(log_attack, "        HP���޽���%.2f ʱ��%.2f\n", ratio, time/1000);
		return true;
	}
	bool SetIncmp(bool)
	{
		fprintf(log_attack, "        MP��������%.2f ʱ��%.2f\n", ratio, time/1000);
		return true;
	}
	bool SetIncdodge(bool)
	{
		fprintf(log_attack, "        ���Ӷ���� ����%.2f  ʱ��%.2f\n", ratio, time/1000);
		return true;
	}
	bool SetRepel(bool)
	{
		fprintf(log_attack, "        ����    ����%.2f  ����%.2f\n", value, probability);    
		return true;
	}
	bool SetBlind(bool)
	{
		fprintf(log_attack, "        Ŀä  ʱ��%.2f  ����%.2f\n", time/1000, probability);    
		return true;
	}
	bool SetDecresist(bool)
	{
		return true;
	}
	bool SetIncresist(bool)
	{
		return true;
	}
	bool SetInchurt(bool)
	{
		fprintf(log_attack, "        �˺��ӱ� ʱ��%.2f  ����%.2f\n", time/1000, ratio);    
		return true;
	}
	bool SetInaccurate(bool)
	{
		fprintf(log_attack, "        ����׼ȷ�� ����%.2f  ʱ��%.2f\n", ratio, time/1000);
		return true;
	}
	bool SetMagicleak(bool)
	{
		fprintf(log_attack, "        ħ��ȼ�� ʱ��%.2f  ����%.2f  ����%.2f\n", time/1000, probability, amount);    
		return true;
	}
	bool SetDecattack(bool) 
	{ 
		return true; 
	}
	bool SetDecdefence(bool)
	{
		fprintf(log_attack, "        ���ͷ����� ʱ��%.2f  ����%.2f  ����%.2f\n", time/1000, probability, ratio);    
		return true;
	}
	bool SetFrighten(bool)
	{
		return true;
	}
	bool SetDizzy(bool)
	{
		fprintf(log_attack, "        ���� ʱ��%.2f  ����%.2f\n", time/1000, probability);    
		return true;
	}
	bool SetSleep(bool)
	{
		fprintf(log_attack, "        ˯�� ʱ��%.2f  ����%.2f\n", time/1000, probability);    
		return true;
	}
	bool SetFix(bool)
	{
		fprintf(log_attack, "        ���� ʱ��%.2f  ����%.2f\n", time/1000, probability);    
		return true;
	}
	bool SetSealed(bool)
	{
		fprintf(log_attack, "        �޷����� ʱ��%.2f  ����%.2f\n", time/1000, probability);    
		return true;
	}
	bool SetSlow(bool)
	{
		fprintf(log_attack, "        �����ƶ��ٶ� ʱ��%.2f  ����%.2f  ����%.2f\n", time/1000, probability, ratio);    
		return true;
	}
	bool SetBleeding(bool)
	{
		fprintf(log_attack, "        ��Ѫ ʱ��%.2f  ����%.2f  ����%d\n", time/1000, probability, (int)amount);    
		return true;
	}
	bool SetThunder(bool)
	{
		fprintf(log_attack, "        �׻� ʱ��%.2f  ����%.2f  ����%d\n", time/1000, probability, (int)amount);    
		return true;
	}
	bool SetToxic(bool)
	{
		fprintf(log_attack, "        �ж� ʱ��%.2f  ����%.2f  ����%d\n", time/1000, probability, (int)amount);    
		return true;
	}
	bool SetBurning(bool)
	{
		fprintf(log_attack, "        ȼ�� ʱ��%.2f  ����%.2f  ����%d\n", time/1000, probability, (int)amount);    
		return true;
	}
	bool SetFallen(bool)
	{
		fprintf(log_attack, "        ��ʯ ʱ��%.2f  ����%.2f  ����%d\n", time/1000, probability, (int)amount);    
		return true;
	}
	bool SetFeathershield(bool)
	{
		fprintf(log_attack, "        ��� ʱ��%.2f  ����%.2f  ��ֵ%.2f\n", time/1000, ratio, value);    
		return true;
	}
	bool SetStoneshield(bool)
	{
		return true;
	}
	bool SetMagicshield(bool)
	{
		return true;
	}
	bool SetIceshield(bool)
	{
		return true;
	}
	bool SetRetort(bool)
	{
		fprintf(log_attack, "       ����  ʱ��%.2f  ����%.2f\n", time/1000, ratio);    
		return true;
	}
	bool SetSlowpray(bool)
	{
		return true;
	}
	bool SetFastmpgen(bool)
	{
		return true;
	}
	bool SetFasthpgen(bool)
	{
		return true;
	}
	bool SetCrazy(bool)
	{
		return true;
	}
	bool SetTardy(bool)
	{
		return true;
	}
	bool SetIceblade(bool)
	{
		return true;
	}
};
}


#endif


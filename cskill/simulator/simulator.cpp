#include <stdio.h>
#include <map>
#include <vector>
#include <assert.h>
#include "myobject.h"
#include "skill.h"
#include "mywrapper.h"
#include "basicinfo.h"

using namespace GNET;

void test_learn(int id,Skill* skill);
void test_run(char*, char*);
void read_name();
int test_skill(int id);

BasicInfo gplayer;
BasicInfo gvictim;

std::vector<unsigned int> gdepends;

MyWrapper gwrapper;
SkillInfo ginfo;

const char* range_str[] = { "���˺�", "���˺�", "��(����)", "��(Ŀ��)", "Բ׶", "����" };

FILE* log_learn;
FILE* log_error;
FILE* log_attack;

typedef std::map<int, std::string> NameMap;
std::map<int, std::string> gnames;

int main(int argc, char** argv)
{
	if(argc==2)
	{
		FILE* fp = fopen(argv[1], "r");
		if(!fp)
		{
			printf("Cannot open %s\n", argv[1]);
			exit(1);
		}
		fscanf(fp, "%d %d %d %d %d %f %d", &gplayer.level, &gplayer.hp, &gplayer.mp, &gplayer.attack,
				&gplayer.magicattack, &gplayer.range, &gvictim.level);
	}
	else
		gplayer = fighter10;
	printf("Generating .... \n");
	log_learn = fopen("./logs/learn.log", "w+");
	log_error = fopen("./logs/error.log", "w+");
	if(!log_learn||!log_error)
	{
		printf("Cannot open log files\n");
		exit(1);
	}
	read_name();
	
	int id = Skill::NextSkill(0);
	while(id>0)
	{
		Skill * skill = Skill::Create(id);
		test_learn(id,skill);
		id = Skill::NextSkill(id);
		skill->Destroy();
	}

	gwrapper.Clear();
	id = Skill::NextSkill(0);
	while(id>0)
	{
		Skill * skill = Skill::Create(id);
		gwrapper.AddSkill(id,skill->GetStub()->GetMaxLevel());
		id = Skill::NextSkill(id);
		skill->Destroy();
	}
	test_run("./logs/run_max.log", "./logs/attack_max.log");

	gwrapper.Clear();
	id = Skill::NextSkill(0);
	while(id>0)
	{
		gwrapper.AddSkill(id,1);
		id = Skill::NextSkill(id);
	}
	test_run("./logs/run_min.log", "./logs/attack_min.log");

	fclose(log_learn);
	fclose(log_error);
}

void test_run(char* log, char* attack)
{
	unsigned int id;
	FILE* log_dep = fopen("./logs/depend.log", "w+");
	FILE* log_run = fopen(log, "w+");
	log_attack = fopen(attack, "w+");
	fprintf(log_run,"- - - -��Ҽ���(%d) ����(%d) ħ��(%d) ������(%d) ħ������(%d) ���(%.2f)\n",
			gplayer.level, gplayer.hp, gplayer.mp, gplayer.attack,gplayer.magicattack,gplayer.range);
	for( NameMap::iterator it = gnames.begin(); it != gnames.end(); ++ it )
	{
		gdepends.clear();
		id = (*it).first;
		fprintf(log_attack, "[%03d] %-15s\n", id, gnames[id].c_str());
		bzero(&ginfo,sizeof(ginfo));
		Skill * skill = Skill::Create(id);
		if(!skill)
			continue;
		skill->SetLevel(gwrapper.GetLevel(id));
		const SkillStub* stub = skill->GetStub();
		if(stub->IsPassive()|| stub->IsEnabled() || stub->GetCls()==255)
			continue;
		int ret = test_skill(id);
		fprintf(log_run,"%3d   %-15s %3d  %5.1f %3d %5.1f   %-10s  %5.2f   %5.2f   %5.2f   %5d   %5d  %4d\n", 
				id, gnames[id].c_str(), stub->GetCls(),(float)ginfo.time/1000.0, skill->GetCoolingtime(),
				(float)skill->GetExecutetime()/1000, range_str[(int)(stub->GetRange().type)], ginfo.castrange, 
				ginfo.range, ginfo.radius, ginfo.damage, ginfo.magicdamage, ginfo.mpcost);
		if(ret==1 && skill->GetExecutetime()!=-1)
		{
			fprintf(log_error,"[%03d]!!!%-15s ��ѭ��״̬�ļ���ִ��ʱ��Ӧ����Ϊ��1.\n", id, gnames[id].c_str()); 
		}
		if((stub->IsBless()||stub->IsCurse()) && !stub->HasStateAttack())
		{
			fprintf(log_error,"[%03d]!!_%-15s ף��������ħ��û�ж�����н��.\n", id, gnames[id].c_str()); 
		}
		if(skill->GetCoolingtime()<=0 && stub->GetCls()!=127)
		{
			fprintf(log_error,"[%03d]!__%-15s ������ȴʱ��Ϊ%d.\n", id, gnames[id].c_str(), skill->GetCoolingtime()); 
		}
		if((stub->IsAttack()||stub->IsCurse()) && stub->GetRange().type==5)
		{
			fprintf(log_error,"[%03d]!!!%-15s ����������ħ����Ŀ�겻�����Լ�.\n", id, gnames[id].c_str()); 
		}
		if(stub->IsAttack())
		{
			if(!ginfo.damage && !ginfo.magicdamage)
				fprintf(log_error,"[%03d]!!!%-15s (%2d��) ������ħ�����˺�Ϊ0.\n", id, gnames[id].c_str(), 
						skill->GetLevel()); 
		}else
		{
			if(ginfo.damage || ginfo.magicdamage)
				fprintf(log_error,"[%03d]!!!%-15s (%2d��) �ǹ�����ֱ��ħ���˺�����Ϊ0(%d %d).\n", id, gnames[id].c_str(),
						skill->GetLevel(), ginfo.damage, ginfo.magicdamage); 
		}
		if(!ginfo.cast)
			fprintf(log_error,"[%03d]!!!%-15s (%2d��) û�з�������Ϣ.\n", id, gnames[id].c_str(), skill->GetLevel());
		if(ginfo.perform==0)
			fprintf(log_error,"[%03d]!!!%-15s (%2d��) û�з�ִ����Ϣ.\n", id, gnames[id].c_str(), skill->GetLevel());
		if(ginfo.perform>1)
			fprintf(log_error,"[%03d]!!!%-15s (%2d��) ִ����Ϣ������%d��.\n", id, gnames[id].c_str(), 
					skill->GetLevel(), ginfo.perform);
		if(stub->GetRange().type==0 || stub->GetRange().type==3)
		{
			if(ginfo.castrange>ginfo.range)
				fprintf(log_error,"[%03d]!!_%-15s (%2d��) ����������������Ч����.\n",id, gnames[id].c_str(),
					   	skill->GetLevel());
		}
		if(stub->GetRange().type==4)
		{
			if(ginfo.radius<0 || ginfo.radius > 1)
				fprintf(log_error,"[%03d]!!!%-15s (%2d��) ���Ͱ��COS��Чֵ %.2f.\n",id, gnames[id].c_str(),
					   	skill->GetLevel(), ginfo.radius);
			if(ginfo.range<0.1)
				fprintf(log_error,"[%03d]!!_%-15s (%2d��) ���͹���ɱ�˰뾶̫С%.2f.\n",id, gnames[id].c_str(),
					   	skill->GetLevel(), ginfo.range);
		}
		if(stub->GetRange().type==2 || stub->GetRange().type==3)
		{
			if(ginfo.radius<0.1)
				fprintf(log_error,"[%03d]!!_%-15s (%2d��) ����ɱ�˰뾶̫С%.2f.\n",id, gnames[id].c_str(),
					   	skill->GetLevel(), ginfo.radius);
		}
		fprintf(log_dep,"[%03d] %-15s ", id, gnames[id].c_str());
		for(std::vector<unsigned int>::iterator it=gdepends.begin();it!=gdepends.end();it++)
		{
			if(id!=*it)
			{
				const SkillStub* tmps =  SkillStub::GetStub(*it);
				if(!tmps)
				{
					fprintf(log_error,"[%03d]!!!%-15s �������ܲ�����%d.\n",id, gnames[id].c_str(),*it);
				}else if(stub->GetCls()!=tmps->GetCls()){
					fprintf(log_error,"[%03d]!!!%-15s �������ܲ����ڱ���%d.\n",id, gnames[id].c_str(),*it);
				}else{
					fprintf(log_dep," %s",  gnames[*it].c_str());
				}
			}
		}
		if(fabs(ginfo.mpcost-ginfo.mptotal) > 0.5*ginfo.mptotal)
		{
			fprintf(log_error,"[%03d]!__%-15s ʵ�����ĵ�ħ�����趨ֵ���ϴ� %d <> %.2f\n", id, gnames[id].c_str(),  
					ginfo.mpcost, ginfo.mptotal);
		}
		fprintf(log_dep,"\n");
		skill->Destroy();
	}
	fclose(log_run);
	fclose(log_dep);
	fclose(log_attack);
}

int test_skill(int id)
{
	int time, next;
	int ret = 0;
	bool warned = false;
	object_interface performer;
	gactive_imp imp(gplayer, true);

	XID target(1,1);
	SkillWrapper::Data data(id);
	performer.Attach(&imp);

	ginfo.id = id;
	ginfo.scope = 0;

	const GNET::SkillStub * stub = GNET::SkillStub::GetStub(id);
	if(stub->cls==127)
	{
		time = gwrapper.NpcStart(id, performer, &target, 1, next); 
		if(time<0)
			fprintf(log_error,"[%03d]!!!%-15s NPC��������ʱ��<0.\n", id, gnames[id].c_str());  
		gwrapper.NpcEnd(id, performer, 1, &target, 1);
		return 0;
	}

	int counter = 0;
	time = gwrapper.StartSkill(data, performer,&target, 1, next); 
	ginfo.time = time;
	if(time<=0)
	{
		fprintf(log_error,"[%03d]!!!%-15s ��Ҽ�������ʱ��<=0.\n", id, gnames[id].c_str());  
	}
	while(time>0 && counter< 100)
	{
		time = gwrapper.Run(data,performer,&target,1,next);
		if(data.stateindex==data.nextindex)
		{ 
			if(!gwrapper.Cancel(data, performer) && !warned)
			{
				fprintf(log_error,"[%03d]!!!%-15s ѭ��״̬Ӧ����Ϊ��ȡ��.\n", id, gnames[id].c_str());  
				warned = true;
			}
			ret = 1;
		}
		if(data.skippable)
		{
			if(!warned)
			{
				fprintf(log_error,"[%03d]!!!%-15s ֻ������״̬�ɱ���Ϊ����.\n", id, gnames[id].c_str());  
				warned = true;
			}
			ret = 2;
		}
		if(time>0)
			ginfo.time += time;
		counter++;
	}
	if(counter>=100)
	{
		fprintf(log_error,"[%03d]!!!%-15s ��⵽��ѭ��.\n", id, gnames[id].c_str()); 
	}
	return ret;
}

void test_learn(int id,Skill* skill)
{
	const SkillStub* stub = skill->GetStub();
	int max = stub->GetMaxLevel();
	int minlevel, maxlevel, minsp, maxsp, total = 0;
	skill->SetLevel(0);
	minlevel =  skill->GetRequiredLevel();
	minsp =  skill->GetRequiredSp();
	skill->SetLevel(max-1);
	maxlevel =  skill->GetRequiredLevel();
	maxsp =  skill->GetRequiredSp();
	for(int i=0;i<max;i++)
	{
		skill->SetLevel(i);
		total += skill->GetRequiredSp();
	}
	int preid = stub->GetPreId();
	if(preid>0)
	{
		fprintf(log_learn,"%3d   %-15s   %3d    %02d    %3d    %3d  %9d  %-20s\n", id, gnames[id].c_str(), stub->GetCls(), 
			  minlevel, maxlevel, max, total, gnames[preid].c_str());
	}else
	{
		fprintf(log_learn,"%3d   %-15s   %3d    %02d    %3d    %3d  %9d\n", id, gnames[id].c_str(), stub->GetCls(), 
			  minlevel, maxlevel, max, total);
	}
	if(maxsp<minsp || minsp <0)
		fprintf(log_error,"[%03d]!__%-15s 1����Ҫ��SP����߼��� %d > %d\n", id, gnames[id].c_str(),  
				minsp, maxsp);
	if(maxlevel<minlevel || minlevel <0)
		fprintf(log_error,"[%03d]!__%-15s 1����Ҫ�ļ������߼��� %d > %d\n", id, gnames[id].c_str(), 
				minlevel, maxlevel);
	if(preid>0)
	{
		const SkillStub* p = SkillStub::GetStub(preid);
		if(!p)
			fprintf(log_error,"[%03d]!!!%-15s ��Ҫ��ǰ�Ἴ��%d������.\n", id, gnames[id].c_str(), stub->GetPreId()); 
		else if(p->GetCls() != stub->GetCls())
			fprintf(log_error,"[%03d]!!!%-15s ��Ҫ��ǰ�Ἴ��%d������������.\n", id, gnames[id].c_str(), 
					p->GetCls()); 
	}
}

void read_name()
{
	int id = Skill::NextSkill(0);
	while(id>0)
	{
		Skill * skill = Skill::Create(id);
		gnames[id] = skill->GetStub()->nativename;
		id = Skill::NextSkill(id);
		skill->Destroy();
	}
}

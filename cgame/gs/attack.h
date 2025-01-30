#ifndef __ONLINEGAME_GS_ATTACK_H__
#define __ONLINEGAME_GS_ATTACK_H__


#include "config.h"
#include <common/types.h>

//地图收益时间级别
enum
{
	PROFIT_LEVEL_NONE,	//打怪巫无收益
	PROFIT_LEVEL_YELLOW,//位于收益地图且收益时间低于1小时
	PROFIT_LEVEL_NORMAL,//玩家能获得正常收益
};

//考虑在这里面加入宠物的内容 ， 以让宠物攻击算成玩家的攻击
struct attacker_info_t
{
	XID attacker;			//攻击者的ID 对于宠物， 此ID与消息ID不一致
	short level;			//攻击者级别
	short eff_level;		//有效级别 为组队做准备的
	int team_id;			//攻击者组id
	int team_seq;			//攻击者组的序号
	int cs_index;			//如果是player，表示了player的cs_index
	int sid;			//如果是player，表示了player的cs_index 对应的sid 
	int mafia_id;			//组团序号，非0则为组团攻击，此攻击不会影响团内成员
	int wallow_level;		//沉迷等级 npc
	int profit_level;		//收益等级
	int force_id;			//攻击者势力id, 非0表示不攻击同势力玩家
};

struct attack_msg
{
	attacker_info_t ainfo;		//攻击者的信息

	float attack_range;		//此次攻击的范围（攻击点在消息里面）
	float short_range;		//此次攻击的近身距离，近身时会进行伤害调整
	float short_range_adjust_factor;	//近距离伤害调整 >1 加深 >0&&<1 减弱 1 不变，仅技能攻击时使用
	int target_layer_adjust_mask;	//0x00-伤害不变 0x01-地面目标伤害加深 0x02-空中目标伤害加深 0x04-水中目标伤害加深 可复选
	float target_layer_adjust_factor;	//空地目标伤害加深倍数 >1
	int physic_damage;		//物理攻击的伤害力
	int attack_rate;		//物理攻击的命中
	int magic_damage[MAGIC_CLASS];	//魔法伤害力
	int attacker_faction;		//攻击者阵营
	int target_faction;		//攻击者的敌人阵营(自己的阵营只有符合这个阵营才能被非强制攻击伤害)
	int crit_rate;
	int crit_damage_bonus;	//额外暴击伤害加成
	int attack_degree;		//攻击级别
	int penetration;
	char attack_attr;		//攻击属性，数值由enum ATTACK_ATTR定义
	char force_attack;		//是否强制攻击
	char attacker_layer;		//攻击者处于什么位置 0 地上 1 天上 2 水上 
	int _attack_state;		//0x01 重击  0x02 攻击优化符
	short speed;
	char attacker_mode;		//攻击者的状态(0x01 PK , 0x02 FREE, 0x04 已进入PK模式)
	char is_invader;		//是否卑劣攻击,现在提前判定了
	int hp_steal_rate;		//吸血百分比，只对物理攻击有效
	int skill_id;			//是否技能攻击， 及其对应的技能号
	int skill_modify[3];
	struct
	{
		int skill;
		int level;
	} attached_skill;
	int skill_enhance;		//增强使用技能的伤害，只有目标是npc才生效
	int skill_enhance2;		//增强使用技能的伤害，所有目标
	int vigour;				//气魄
	unsigned char section;  //技能阶段
	int weapon_class;
	int feedback_filter;
	int anti_defense_degree; //无视物防等级
	int anti_resistance_degree; // 无视法防等级
	struct
	{
		int skill;
		int level;
	} infected_skill;

	enum 
	{
		PVP_ENABLE 	= 0x01,		//打开了PK开关
		PVP_FREE   	= 0x02,		//自由PK
		PVP_DURATION 	= 0x04,		//已经在和玩家PK
		PVP_DUEL	= 0x08,		//此次是决斗攻击
		PVP_FEEDBACK_KILL = 0x10,	//杀死玩家后需要将目标的信息发送给自己
	};

	enum ATTACK_ATTR
	{
		MAGIC_ATTACK = 0,		//法术攻击
		PHYSIC_ATTACK = 1,		//物理攻击
		MAGIC_ATTACK_GOLD = 2,		//法术攻击金
		MAGIC_ATTACK_WOOD = 3,		//法术攻击木
		MAGIC_ATTACK_WATER = 4,		//法术攻击水
		MAGIC_ATTACK_FIRE = 5,		//法术攻击火
		MAGIC_ATTACK_EARTH = 6,		//法术攻击土
		PHYSIC_ATTACK_HIT_DEFINITE = 7,	//物理攻击必中
	};
};

struct enchant_msg
{
	attacker_info_t ainfo;		//攻击者的信息

	int attacker_faction;		//攻击者阵营
	int target_faction;		//攻击者的敌人阵营(自己的阵营只有符合这个阵营才能被非强制攻击伤害)
	float attack_range;
	int skill;
	int skill_reserved1;		//技能内部使用
	int skill_modify[3];
	int attack_degree;		//攻击级别
	int penetration;
	char force_attack;		//是否强制攻击
	char skill_level;
	char attacker_layer;
	char helpful;			//0:诅咒 1:祝福 2:中性祝福
	char attacker_mode;		//攻击者的状态(0 非PK,1:PK)
	char is_invader;		//是否卑劣攻击,现在提前判定了
	int _attack_state;		//0x01 重击  0x02 攻击优化符  0x80 免疫此次公鸡
	int vigour;				//气魄
	unsigned char section;  //技能阶段
	struct
	{
		int skill;
		int level;
	} infected_skill;
};

struct damage_entry
{
	float physic_damage;
	float magic_damage[MAGIC_CLASS];
};

#endif


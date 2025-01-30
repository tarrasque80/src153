#ifndef _TASKSERVER_H_
#define _TASKSERVER_H_

#include "TaskInterface.h"

#ifndef _TASK_CLIENT

class elementdataman;

// 加载任务文件
bool LoadTasksFromPack(elementdataman* pDataMan, const char* szPackPath, const char* szDynPackPath);

// 释放任务文件
void ReleaseTaskTemplMan();

// 通知队长建立
void OnTeamSetup(TaskInterface* pTask);

// 通知队长解散
void OnTeamDisband(TaskInterface* pTask);

// 通知新队长队长转移
void OnTeamTransferCaptain(TaskInterface* pTask);

// 通知队长加入成员
void OnTeamAddMember(TaskInterface* pTask, const task_team_member_info* pInfo);

// 通知队长成员退出
void OnTeamDismissMember(TaskInterface* pTask, unsigned int ulPlayerId);

// 通知队员退出
void OnTeamMemberLeave(TaskInterface* pTask);

// 接收其他玩家通知
void OnPlayerNotify(TaskInterface* pTask, unsigned int ulPlayerId, const void* pBuf, unsigned int sz);

// 接收客户端通知
void OnClientNotify(TaskInterface* pTask, const void* pBuf, unsigned int sz);

// 检查任务能否发放
bool OnTaskCheckDeliver(TaskInterface* pTask, unsigned int ulTaskId,unsigned int ulStorageId,bool bNotifyErr=true);

// NPC发放任务物品
void OnNPCDeliverTaskItem(TaskInterface* pTask, unsigned int ulNPCId, unsigned int ulTaskId);

// 检查在NPC处完成的任务
void OnTaskCheckAward(TaskInterface* pTask, unsigned int ulTasklId, int nChoice);

// 玩家被杀
void OnTaskPlayerKilled(TaskInterface* pTask);

// 定时检查整个任务系统限时且有接收者上限任务的状态
void OnTaskCheckAllTimeLimits(unsigned int ulCurTime);

// 单人杀怪
void OnTaskKillMonster(TaskInterface* pTask, unsigned int ulTemplId, unsigned int ulLev, float fRand,int dps,int dph);

// 组队杀怪
void OnTaskTeamKillMonster(TaskInterface* pTask, unsigned int ulTemplId, unsigned int ulLev, float fRand);

// 挖任务矿
void OnTaskMining(TaskInterface* pTask, unsigned int ulTaskId);

// 请求任务全局数据
void TaskQueryGlobalData(
	unsigned int ulTaskId,
	unsigned int ulPlayerId,
	const void* pPreservedData,
	unsigned int size);

// 更新任务全局数据
void TaskUpdateGlobalData(
	unsigned int ulTaskId,
	const unsigned char pData[TASK_GLOBAL_DATA_SIZE]);

// 获得全局数据
void OnTaskReceivedGlobalData(
	TaskInterface* pTask,
	unsigned int ulTaskId,
	unsigned char pData[TASK_GLOBAL_DATA_SIZE],
	const void* pPreservedData,
	unsigned int size);

// 遗忘生活技能
void OnForgetLivingSkill(TaskInterface* pTask);

// 玩家升级
void OnTaskPlayerLevelUp(TaskInterface* pTask);

// PQ任务初始化
bool InitPublicQuestSystem();

// PQ任务的heartbeat
void OnTaskPublicQuestHeartbeat();

// PQ任务杀怪贡献度分配
void OnTaskKillPQMonster(TaskInterface* pTask, unsigned int ulTemplId, bool bOwner,float fTeamContrib,int iTeamMemberCnt,float fPersonalContib);

// PQ任务初始化link信息
void OnTaskCheckPQEnterWorldInit(TaskInterface* pTask, unsigned int ulTaskId);

// 离开势力任务失败
void OnTaskLeaveForce(TaskInterface* pTask);

enum{
	EX_TK_SENDAUMAIL_LEVEL1 = 1,	//	向AU发送召回好友邮件奖励1（依赖好友未登录天数）
	EX_TK_SENDAUMAIL_LEVEL2 = 2,
	EX_TK_SENDAUMAIL_LEVEL3 = 3,
	EX_TK_SENDAUMAIL_EXAWARD = 4,	//	向AU发送召回好友邮件数目奖励（依赖发送邮件数目）
	EX_TK_WELCOMBACK_REWARD	= 5,	//	上线回复召回邮件时收到的系统欢迎回归奖励
};
// 
void OnTaskExternEvent(TaskInterface* pTask, int Event);

// 某些奖励由任务发放，比如账号补填
void OnTaskSpecialAwardMask(TaskInterface* pTask,unsigned int ulTaskID);

// 杀人任务
void OnTaskKillPlayer(TaskInterface* pTask, int iOccupation, bool bGender, int iLevel, int iForce, float fRand);

// 称号任务
void OnTaskTitle(TaskInterface* pTask, unsigned int ulTaskID);

// 不通过NPC触发任务
void OnTaskManualTrig(TaskInterface* pTask,unsigned int ulTaskId, bool bNotifyErr=true);

// 清除所有的爬塔任务
void ClearAllTowerTask(TaskInterface* pTask);

#endif

#endif

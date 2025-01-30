#ifndef _TASKINTERFACE_H_
#define _TASKINTERFACE_H_

#include <stddef.h>
#include "vector.h"

#ifdef	WIN32
#define _TASK_CLIENT
#endif

// Task Prerequisite Error Code
#define TASK_PREREQU_FAIL_INDETERMINATE	1
#define TASK_PREREQU_FAIL_NOT_ROOT		2
#define TASK_PREREQU_FAIL_SAME_TASK		3
#define TASK_PREREQU_FAIL_NO_SPACE		4
#define TASK_PREREQU_FAIL_FULL			5
#define TASK_PREREQU_FAIL_CANT_REDO		6
#define TASK_PREREQU_FAIL_BELOW_LEVEL	7
#define TASK_PREREQU_FAIL_ABOVE_LEVEL	8
#define TASK_PREREQU_FAIL_NO_ITEM		9
#define TASK_PREREQU_FAIL_BELOW_REPU	10
#define TASK_PREREQU_FAIL_CLAN			11
#define TASK_PREREQU_FAIL_WRONG_GENDER	12
#define TASK_PREREQU_FAIL_NOT_IN_OCCU	13
#define TASK_PREREQU_FAIL_WRONG_PERIOD	14
#define TASK_PREREQU_FAIL_PREV_TASK		15
#define TASK_PREREQU_FAIL_MAX_RCV		16
#define TASK_PREREQU_FAIL_NO_DEPOSIT	17
#define TASK_PREREQU_FAIL_NO_TASK		18
#define TASK_PREREQU_FAIL_NOT_CAPTAIN	19
#define TASK_PREREQU_FAIL_ILLEGAL_MEM	20
#define TASK_PREREQU_FAIL_WRONG_TIME	21
#define TASK_PREREQU_FAIL_NO_SUCH_SUB	22
#define TASK_PREREQU_FAIL_MUTEX_TASK	23
#define TASK_PREREQU_FAIL_NOT_IN_ZONE	24
#define TASK_PREREQU_FAIL_WRONG_SUB		25
#define TASK_PREREQU_FAIL_OUTOF_DIST	26
#define TASK_PREREQU_FAIL_GIVEN_ITEM	27
#define TASK_PREREQU_FAIL_LIVING_SKILL	28
#define TASK_PREREQU_FAIL_SPECIAL_AWARD	29
#define	TASK_PREREQU_FAIL_GM			30
#define TASK_PREREQU_FAIL_GLOBAL_KEYVAL 31
#define TASK_PREREQU_FAIL_SHIELD_USER	32
#define TASK_PREREQU_FAIL_ALREADY_HAS_PQ 33
#define TASK_PREREQU_FAIL_MAX_ACC_CNT	34
#define TASK_PREREQU_FAIL_RMB_NOT_ENOUGH 35
#define TASK_PREREQU_FAIL_NOT_COUPLE	36
#define TASK_PREREQU_FAIL_ERR_CHAR_TIME	37
#define TASK_PREREQU_FAIL_NOT_IVTRSLOTNUM 38 // version 81
#define TASK_PREREQU_FAIL_BELOW_FACTION_CONTRIB 39//version 87
#define TASK_PREREQU_FAIL_BELOW_RECORD_TASKS_NUM 40 //version 91
#define TASK_PREREQU_FAIL_OVER_RECEIVE_PER_DAY 41
#define TASK_PREREQU_FAIL_TRANSFORM_MASK 42
#define TASK_PREREQU_FAIL_FORCE 43
#define TASK_PREREQU_FAIL_FORCE_REPUTATION 44
#define TASK_PREREQU_FAIL_FORCE_CONTRIBUTION 45
#define TASK_PREREQU_FAIL_EXP 46
#define TASK_PREREQU_FAIL_SP 47
#define TASK_PREREQU_FAIL_FORCE_AL 48
#define TASK_PREREQU_FAIL_WEDDING_OWNER 49
#define TASK_PREREQU_FAIL_CROSSSERVER_NO_ACOUNT_LIMIT 50
#define TASK_PREREQU_FAIL_CROSSSERVER_NO_MARRIAGE 51
#define TASK_PREREQU_FAIL_CROSSSERVER_NO_FORCE 52
#define TASK_PREREQU_FAIL_KING 53
#define TASK_PREREQU_FAIL_IN_TEAM 54
#define TASK_PREREQU_FAIL_TITLE 55
#define TASK_PREREQU_FAIL_HISTORYSTAGE	56
#define TASK_PREREQU_FAIL_NO_GIFTCARD_TASK 57
#define TASK_PREREQU_FAIL_BELOW_REINCARNATION	57
#define TASK_PREREQU_FAIL_ABOVE_REINCARNATION	58
#define TASK_PREREQU_FAIL_BELOW_REALMLEVEL	59
#define TASK_PREREQU_FAIL_ABOVE_REALMLEVEL	60
#define TASK_PREREQU_FAIL_REALM_EXP_FULL	61
#define TASK_PREREQU_FAIL_CARD_COUNT_COLLECTION	62
#define TASK_PREREQU_FAIL_MAX_ROLE_CNT	63
#define TASK_PREREQU_FAIL_CARD_COUNT_RANK	64
#define TASK_PREREQU_FAIL_TASK_FORBID 65
#define TASK_PREREQU_FAIL_NO_NAVIGATE_INSHPAED 66
#define TASK_PREREQU_FAIL_HAS_ICONSTATE_ID	67
#define TASK_PREREQU_FAIL_CHECK_VIP_LEVEL	68

#define TASK_AWARD_FAIL_GIVEN_ITEM		150
#define TASK_AWARD_FAIL_NEW_TASK		151
#define TASK_AWARD_FAIL_REPUTATION		152
#define TASK_AWARD_FAIL_GLOBAL_KEYVAL   153
#define TASK_AWARD_FAIL_CROSSSERVER_NO_ACOUNT_LIMIT 154
#define TASK_AWARD_FAIL_CROSSSERVER_NO_ACOUNT_STORAGE 155
#define TASK_AWARD_FAIL_CROSSSERVER_NO_DIVORCE 156
#define TASK_AWARD_FAIL_CROSSSERVER_NO_FACTION_RALATED 157
#define TASK_AWARD_FAIL_CROSSSERVER_NO_FORCE_RALATED 158
#define TASK_AWARD_FAIL_CROSSSERVER_NO_DIVIEND 159
#define TASK_AWARD_FAIL_LEVEL_CHECK 160


// Task messages
#define TASK_MSG_NEW					1
#define TASK_MSG_SUCCESS				2
#define TASK_MSG_FAIL					3

#define TASK_ACTIVE_LIST_HEADER_LEN		8
#define TASK_ACTIVE_LIST_MAX_LEN		175
#define TASK_FINISHED_LIST_MAX_LEN		4080
#define TASK_DATA_BUF_MAX_LEN			(24 + sizeof(size_t) + sizeof(size_t))
#define TASK_FINISH_TIME_MAX_LEN		2500
#define TASK_FINISH_COUNT_MAX_LEN		730

// 库任务
static const int	TASK_MAX_DELIVER_COUNT = 5;
static const int	TASK_STORAGE_COUNT = 32;
static const int	TASK_STORAGE_LEN   = 10;
static const float	TASK_STORAGE_WHELL_SCALE = 10000.f;

// 当前激活的任务列表缓冲区大小
#define TASK_ACTIVE_LIST_BUF_SIZE		(TASK_ACTIVE_LIST_MAX_LEN * TASK_DATA_BUF_MAX_LEN + TASK_ACTIVE_LIST_HEADER_LEN)
// 已完成的任务列表缓冲区大小
#define TASK_FINISHED_LIST_BUF_SIZE		16384
#define TASK_FINISHED_LIST_BUF_SIZE_OLD	4096
// 任务全局数据大小
#define TASK_GLOBAL_DATA_SIZE			256
// 任务完成时间
#define TASK_FINISH_TIME_LIST_BUF_SIZE	15100
//任务完成次数
#define TASK_FINISH_COUNT_LIST_BUF_SIZE	10240
// 库任务
#define TASK_STORAGE_LIST_BUF_SIZE		1024


// Masks
#define	TASK_MASK_KILL_MONSTER			0x00000001
#define TASK_MASK_COLLECT_ITEM			0x00000002
#define TASK_MASK_TALK_TO_NPC			0x00000004
#define TASK_MASK_REACH_SITE			0x00000008
#define TASK_MASK_ANSWER_QUESTION		0x00000010
#define TASK_MASK_TINY_GAME				0x00000020
#define TASK_MASK_KILL_PQ_MONSTER		0x00000040
#define TASK_MASK_KILL_PLAYER			0x00000080


#define MAX_MONSTER_WANTED				3	// 受ActiveTaskEntry大小限制，最大3
#define MAX_PLAYER_WANTED				MAX_MONSTER_WANTED
#define MAX_ITEM_WANTED					10
#define MAX_ITEM_AWARD					64
#define MAX_MONSTER_SUMMONED			32	// 最大召唤出的怪物数量

#define MAX_OCCUPATIONS					12  // 职业

#define TASK_MSG_CHANNEL_LOCAL			0
#define TASK_MSG_CHANNEL_WORLD			1
#define TASK_MSG_CHANNEL_BROADCAST		9

#define TASK_TEAM_RELATION_MARRIAGE		1

#define TASK_AWARD_MAX_CHANGE_VALUE     255
#define TASK_AWARD_MAX_DISPLAY_VALUE    64

#define TASK_AWARD_MAX_DISPLAY_EXP_CNT  32    //表达式的个数
#define TASK_AWARD_MAX_DISPLAY_CHAR_LEN	64	  //表达式的长度

#define TASK_WORLD_CONTRIBUTION_SPEND_PER_DAY 30 // 免费玩家每日消费贡献度上限

#ifdef _WINDOWS
typedef wchar_t task_char;
#else
typedef unsigned short task_char;
#endif

struct task_team_member_info
{
	unsigned int	m_ulId;
	unsigned int	m_ulLevel;
	unsigned int	m_ulOccupation;
	bool			m_bMale;
	unsigned int	m_ulWorldTag;
	unsigned int	m_ulWorldIndex;
	int				m_iForce;
	float			m_Pos[3];
};

struct special_award
{
	unsigned int	id1;
	unsigned int	id2;
	unsigned int	special_mask;
};

class Kill_Player_Requirements
{
	enum
	{
		MAX_OCCPU_MASK =  (1 << MAX_OCCUPATIONS) - 1,
	};
public:
	unsigned int	m_ulOccupations;
	int				m_iMinLevel;
	int				m_iMaxLevel;
	int				m_iGender;
	int				m_iForce;
	Kill_Player_Requirements():
	m_iMinLevel(10),
	m_iMaxLevel(100),
	m_iGender(0),
	m_iForce(0)
	{
		m_ulOccupations = MAX_OCCPU_MASK;
	}
	bool IsMeetAllOccupation() const { return m_ulOccupations == MAX_OCCPU_MASK;}
	bool CheckRequirements(int iOccupation, int iLevel, bool bGender, int iForce) const
	{
		bool bForce = false;
		// 编辑器里1为男，2为女
		int iGender = bGender ? 2 : 1;
		// 编辑器里0代表没有势力要求
		if (m_iForce == 0)
		{
			bForce = true;
		}
		// 非零表示有势力要求
		else
		{
			// 编辑器里华光、暗隐和辉夜分别用第一、二、三个二进制位表示
			int iForceMask = 0;
			if (iForce == 0)
			{
				return false;
			}
			else if (iForce == 1004)
			{
				iForceMask = 1 << 0;
			}
			else if (iForce ==  1005)
			{
				iForceMask = 1 << 1;
			}
			else if (iForce == 1006)
			{
				iForceMask = 1 << 2;
			}
			bForce = (m_iForce & iForceMask) != 0;
		}
		return (m_ulOccupations & (1 << iOccupation)) &&
			m_iMinLevel <= iLevel &&
			m_iMaxLevel >= iLevel &&
			(m_iGender ? m_iGender == iGender : true) &&
			bForce;
	}
};
#ifdef _TASK_CLIENT

struct Task_State_info
{
	unsigned int	m_ulTimeLimit;
	unsigned int	m_ulTimePassed;
	unsigned int	m_ulNPCToProtect;
	unsigned int	m_ulProtectTime;
	unsigned int	m_ulWaitTime;
	unsigned int	m_ulErrCode;
	unsigned int	m_ulGoldWanted;
	unsigned int	m_ulReachLevel;
	unsigned int	m_ulReachReincarnation;
	unsigned int	m_ulReachRealm;
	unsigned int	m_ulPremLevelMin;
	unsigned int	m_ulTMIconStateID;
	struct
	{
		unsigned int	m_ulMonsterId;
		unsigned int	m_ulMonstersToKill;
		unsigned int	m_ulMonstersKilled;
	} m_MonsterWanted[MAX_MONSTER_WANTED];

	struct
	{
		unsigned int	m_ulItemId;
		unsigned int	m_ulItemsToGet;
		unsigned int	m_ulItemsGained;
		unsigned int	m_ulMonsterId;
	} m_ItemsWanted[MAX_ITEM_WANTED];

	struct TASK_INFO_PLAYER 
	{
		unsigned int m_ulPlayersToKill;
		unsigned int m_ulPlayersKilled;
		Kill_Player_Requirements m_Requirements;
	} m_PlayerWanted[MAX_PLAYER_WANTED];

	abase::vector<wchar_t*> m_TaskCharArr;
	~Task_State_info() {
		unsigned int s = m_TaskCharArr.size();
		for (unsigned int i = 0; i < s; ++i)
			delete m_TaskCharArr[i];
	}
};

struct AWARD_DATA;

struct Task_Award_Preview
{
	unsigned int	m_ulGold;
	unsigned int	m_ulExp;
	unsigned int	m_ulRealmExp;
	unsigned int	m_ulSP;
	bool			m_bHasItem;
	bool			m_bItemKnown;
	unsigned int	m_ulItemTypes;
	unsigned int	m_ItemsId[MAX_ITEM_AWARD];
	unsigned int	m_ItemsNum[MAX_ITEM_AWARD];
	int				m_iForceActivity;
	int				m_iForceContrib;
	int				m_iForceRepu;
};

#endif

#ifdef _TASK_CLIENT

	struct talk_proc;

#else

	struct TaskTeamInterface
	{
		virtual void SetMarriage(int nPlayer) = 0;
	};
	
#endif

struct TaskInterface
{
#ifdef _TASK_CLIENT
	enum
	{
		CMD_EMOTION_BINDBUDDY = 1024, //如果有其他非表情的动作，则依次为1025,1026....
		CMD_EMOTION_SITDOWN,	// 打坐
		CMD_JUMP_TRICKACTION,	// 跳起翻滚
		CMD_RUN_TRICKACTION,	// 跑动翻滚
	};
#endif

	virtual ~TaskInterface(){}
	virtual int GetFactionContrib() = 0;
	virtual int GetFactionConsumeContrib() = 0;
	virtual int GetFactionExpContrib() = 0;
	virtual void TakeAwayFactionConsumeContrib(int ulNum) = 0;
	virtual void TakeAwayFactionExpContrib(int ulNum) = 0;
	virtual unsigned int GetPlayerLevel() = 0;
	virtual void* GetActiveTaskList() = 0;
	virtual void* GetFinishedTaskList() = 0;
	virtual void* GetFinishedTimeList() = 0;
	virtual void* GetFinishedCntList() = 0;
	virtual void* GetStorageTaskList() = 0;
	virtual unsigned int* GetTaskMask() = 0;
	virtual void DeliverGold(unsigned int ulGoldNum) = 0;
	virtual void DeliverExperience(unsigned int ulExp) = 0;
	virtual void DeliverSP(unsigned int ulSP) = 0;
	virtual void DeliverReputation(int lReputation) = 0;
	virtual void DeliverFactionContrib(int iConsumeContrib, int iExpContrib) = 0;
	virtual bool CastSkill(int skill_id, int skill_level) = 0;
	virtual int GetTaskItemCount(unsigned int ulTaskItem) = 0;
	virtual int GetCommonItemCount(unsigned int ulCommonItem) = 0;
	virtual bool IsInFaction(unsigned int ulFactionId) = 0;
	virtual int GetFactionRole() = 0;
	virtual unsigned int GetGoldNum() = 0;
	virtual void TakeAwayGold(unsigned int ulNum) = 0;
	virtual void TakeAwayTaskItem(unsigned int ulTemplId, unsigned int ulNum) = 0;
	virtual void TakeAwayCommonItem(unsigned int ulTemplId, unsigned int ulNum) = 0;
	virtual int GetReputation() = 0;
	virtual unsigned int GetCurPeriod() = 0;
	virtual unsigned int GetPlayerId() = 0;
	virtual unsigned int GetPlayerRace() = 0;
	virtual unsigned int GetPlayerOccupation() = 0;
	virtual bool IsDeliverLegal() = 0;
	virtual bool CanDeliverCommonItem(unsigned int ulItemTypes) = 0;
	virtual bool CanDeliverTaskItem(unsigned int ulItemTypes) = 0;
	virtual void DeliverCommonItem(unsigned int ulItemId, unsigned int ulCount, int lPeriod = 0) = 0;
	virtual void DeliverTaskItem(unsigned int ulItemId, unsigned int ulCount) = 0;
	virtual unsigned int GetPos(float pos[3]) = 0;
	virtual bool HasLivingSkill(unsigned int ulSkill) = 0;
	virtual int GetLivingSkillLevel(unsigned int ulSkill) = 0;
	virtual int GetLivingSkillProficiency(unsigned int ulSkill) = 0;
	virtual void GetSpecailAwardInfo(special_award* p) = 0;
	virtual bool IsGM() = 0;
	virtual bool IsShieldUser() = 0;
	virtual bool IsKing() = 0;
	virtual unsigned int GetInvEmptySlot() = 0;
	virtual void LockInventory(bool is_lock) = 0;
	virtual bool HasIconStateID(unsigned int ulIconStateID)=0;

	virtual unsigned char GetShapeMask() = 0;
	virtual bool IsAtCrossServer() = 0;

	/* 组队任务信息*/
	virtual int GetTeamMemberNum() = 0;
	virtual void GetTeamMemberInfo(int nIndex, task_team_member_info* pInfo) = 0;
	virtual unsigned int GetTeamMemberId(int nIndex) = 0;
	virtual bool IsCaptain() = 0;
	virtual bool IsInTeam() = 0;
	virtual bool IsMale() = 0;
	virtual bool IsMarried() = 0;
	virtual bool IsWeddingOwner() = 0;

	void InitActiveTaskList();
	unsigned int GetActLstDataSize();
	unsigned int GetFnshLstDataSize();
	unsigned int GetFnshTimeLstDataSize();
	unsigned int GetFnshCntLstDataSize();
	unsigned int GetStorageTaskLstDataSize();
	bool CanDoMining(unsigned int ulTaskId);

	bool CheckVersion();
	bool HasTask(unsigned int ulTaskId);
	bool HasTopTaskRelatingMarriage(abase::vector<unsigned int> *pTopTaskIDs);
	bool HasTopTaskRelatingWedding(abase::vector<unsigned int> *pTopTaskIDs);
	bool HasTopTaskRelatingSpouse(abase::vector<unsigned int> *pTopTaskIDs);
	bool HasTopTaskRelatingGender(abase::vector<unsigned int> *pTopTaskIDs);

	// static funcs
	static unsigned int GetCurTime();
	static void WriteLog(int nPlayerId, int nTaskId, int nType, const char* szLog);
	static void WriteKeyLog(int nPlayerId, int nTaskId, int nType, const char* szLog);
	
	//全局key/value
	virtual int GetGlobalValue(int lKey) = 0;

	virtual unsigned int GetRoleCreateTime() = 0;
	virtual unsigned int GetRoleLastLoginTime() = 0;
	virtual unsigned int GetAccountTotalCash() = 0;
	virtual unsigned int GetSpouseID() = 0;

	//势力相关
	virtual int GetForce() = 0;
	virtual int GetForceContribution() = 0;
	virtual int GetForceReputation() = 0;
	virtual bool ChangeForceContribution(int iValue) = 0;
	virtual bool ChangeForceReputation(int iValue) = 0;
	virtual bool SetForceReputation(int iValue);
	virtual int GetExp() = 0;
	virtual int GetSP() = 0;
	virtual bool ReduceExp(int iExp) = 0;
	virtual bool ReduceSP(int iSP) = 0;
	virtual int GetForceActivityLevel() = 0;
	virtual void AddForceActivity(int iActivity) = 0;

	// 称号相关
	virtual bool HaveGotTitle(unsigned int id_designation) = 0;
	virtual void DeliverTitle(unsigned int id_designation, int lPeriod = 0) = 0;
	virtual bool CheckRefine(unsigned int level_refine, unsigned int num_equips) = 0;

	virtual int  GetCurHistoryStageIndex() = 0; // 当前历史阶段的序号
	virtual bool CheckSimpleTaskFinshConditon(unsigned int task_id) const = 0; // 简单任务只客户端检查，服务器可直接 返回ture。
	// 转生
	virtual unsigned int GetMaxHistoryLevel() = 0;
	virtual unsigned int GetReincarnationCount() = 0;
	// 境界
	virtual unsigned int GetRealmLevel() = 0;
	virtual bool IsRealmExpFull() = 0;
	virtual void DeliverRealmExp(unsigned int exp) = 0;
	virtual void ExpandRealmLevelMax() = 0;
	virtual unsigned int	 GetObtainedGeneralCardCount() = 0; // 获得的卡牌数量， 卡牌图鉴中的数量
	virtual void AddLeaderShip(unsigned int leader_ship) = 0;
	virtual unsigned int GetObtainedGeneralCardCountByRank(int) = 0; // 身上、包裹、和卡牌仓库中某品阶卡牌的数量之和

	virtual bool CheckTaskForbid(unsigned int task_id) = 0; // 检查任务是否被屏蔽
	virtual int GetWorldContribution() = 0;	// 世界贡献度
	virtual void DeliverWorldContribution(int contribution) = 0;
	virtual void TakeAwayWorldContribution(int contribution) = 0;
	virtual int GetWorldContributionSpend() = 0;
	virtual bool PlayerCanSpendContributionAsWill() = 0;

	// 爬塔任务相关
	virtual void OnTowerTaskDeliver(bool bSuccess) = 0;		// 爬塔任务发放
	virtual void OnTowerTaskComplete(bool bSuccess) = 0;	// 爬塔任务完成
	virtual void DeliverSoloTowerChallengeScore(int score) = 0;	// 发放单人爬塔积分奖励(积分上限和当前积分都要增加)

	// VIP
	virtual int GetVIPLevel() = 0;

#ifdef _TASK_CLIENT
	static void UpdateTitleUI(unsigned int ulTask);
	static void PopChatMessage(int iMsg,int dwNum = 0);
	virtual void NotifyServerStorageTasks();
	static time_t m_tmFinishDlgShown;
	static void UpdateTaskUI(unsigned int idTask, int reason);
	static void TraceTask(unsigned int idTask);
	static void PopupTaskFinishDialog(unsigned int ulTaskId, const talk_proc* pTalk);
	static void ShowMessage(const wchar_t* szMsg);
	static void ShowPunchBagMessage(bool bSucced,bool bMax,unsigned int MonsterTemplID,int dps,int dph);
	static void ShowTaskMessage(unsigned int ulTask, int reason);
	static int GetFinishDlgShowTime() { return m_tmFinishDlgShown; }
	static void SetFinishDlgShowTime(int t) { m_tmFinishDlgShown = t; }
	static int GetTimeZoneBias();
	static void PopEmotionUI(unsigned int task_id,unsigned int uiEmotion,bool bShow);
	
	virtual void UpdateTaskEmotionAction(unsigned int task_id) = 0;
//	static void DisplayTaskCharInfo(const abase::vector<wchar_t*>& TaskCharArr);

	void OnUIDialogEnd(unsigned int ulTask);
	virtual void NotifyServer(const void* pBuf, unsigned int sz) = 0;
	unsigned int GetTaskCount();
	unsigned int GetTaskId(unsigned int ulIndex);
	bool CheckParent(unsigned int ulParent, unsigned int ulChild);

	// 无返回-1
	int GetFirstSubTaskPosition(unsigned int ulParentTaskId);
	unsigned int GetNextSub(int& nPosition);
	unsigned int GetSubAt(int nPosition);
	

	unsigned int CanDeliverTask(unsigned int ulTaskId);
	bool CanShowTask(unsigned int ulTaskId);
	bool CanFinishTask(unsigned int ulTaskId);
	void GiveUpTask(unsigned int ulTaskId);
	const unsigned short* GetStorageTasks(unsigned int uStorageId);

	void GetTaskStateInfo(unsigned int ulTaskId, Task_State_info* pInfo, bool bActiveTask = true);
	bool GetAwardCandidates(unsigned int ulTaskId, AWARD_DATA* pAward);
	void GetTaskAwardPreview(unsigned int ulTaskId, Task_Award_Preview* p, bool bActiveTask = true);

	unsigned int GetTaskFinishedTime(unsigned int) { return 0; }
	
	//全局key/value
	virtual void DisplayGlobalValue(int lKey, int lValue) = 0;

	// 进入世界时检查PQ任务初始信息
	void CheckPQEnterWorldInit();

	virtual void SetTaskReadyToConfirm(int iTaskID, bool bReady) = 0;
	virtual void UpdateConfirmTasksMap() = 0;
	virtual bool IsTitleDataReady() {return false;}

	virtual void OnGiveupTask(int iTaskID)=0;
	virtual void OnCompleteTask(int iTaskID)=0;
	virtual void OnNewTask(int iTaskID) = 0;


#else
	virtual void NotifyClient(const void* pBuf, unsigned int sz) = 0;
	virtual void NotifyPlayer(unsigned int ulPlayerId, const void* pBuf, unsigned int sz) = 0;
	virtual float UnitRand() = 0;
	virtual int  RandNormal(int lower, int upper) = 0;
	virtual int  RandSelect(const float * option, int size) = 0;
	virtual unsigned int GetTeamMemberPos(int nIndex, float pos[3]) = 0;
	virtual void SetCurPeriod(unsigned int ulPeriod) = 0;
	virtual void SetNewRelayStation(unsigned int ulStationId) = 0;
	virtual void SetStorehouseSize(unsigned int ulSize) = 0;
	virtual void SetStorehouseSize2(unsigned int ulSize) = 0;	// 时装
	virtual void SetStorehouseSize3(unsigned int ulSize) = 0;	// 材料
	virtual void SetAccountStorehouseSize(unsigned int ulSize) = 0;	// 账号
	virtual void AddDividend(int nDividend) = 0; // 鸿利值
	virtual void SetInventorySize(int lSize) = 0;
	virtual void SetFuryUpperLimit(unsigned int ulValue) = 0;
	virtual void TransportTo(unsigned int ulWorldId, const float pos[3], int lController) = 0;
	virtual void SetPetInventorySize(unsigned int ulSize) = 0;
	virtual void SetMonsterController(int id, bool bTrigger) = 0;
	virtual void CheckTeamRelationship(int nReason) = 0;
	virtual void Divorce() = 0;
	virtual void SendMessage(unsigned int task_id, int channel, int param) = 0;
	virtual int SummonMonster(int mob_id, int count, int distance, int remain_time, bool die_with_self) = 0;
	virtual void SetAwardDeath(bool bDeadWithLoss) = 0;

	void JP_Only_RemoveKeyTask();//移除玩家身上所有主线任务，慎用
	void JP_Only_RetrieveLostTasks();//根据玩家已完成任务，尝试恢复丢失的主线任务。自动任务自动发放，在NPC处接的任务要去NPC那接
	void BeforeSaveData();
	void PlayerSwitchScene();
	void PlayerLeaveScene();
	void PlayerLeaveWorld();
	void PQEnterWorldInit(unsigned int task_id);
	void TakeAwayItem(unsigned int ulTemplId, unsigned int ulNum, bool bCmnItem)
	{
		if (bCmnItem) TakeAwayCommonItem(ulTemplId, ulNum);
		else TakeAwayTaskItem(ulTemplId, ulNum);
	}

	bool OnCheckTeamRelationship(int nReason, TaskTeamInterface* pTeam);
	void OnCheckTeamRelationshipComplete(int nReason, TaskTeamInterface* pTeam);
	bool RefreshTaskStorage(unsigned int storage_id);

	//全局key/value	
	virtual void PutGlobalValue(int lKey, int lValue) = 0;
	virtual void ModifyGlobalValue(int lKey, int lValue) = 0;

	//历史推进相关的全局key/value	
	virtual void PutHistoryGlobalValue(int lKey, int lValue) = 0;
	virtual void ModifyHistoryGlobalValue(int lKey, int lValue) = 0;

	virtual void ExpandTaskLimit();
	virtual bool OnGiftCardTask(int type);
	void UpdateOneStorageDebug(int idStorage, bool bUseDayAsSeed);
	
#endif
};

class PublicQuestInterface
{
public:
	PublicQuestInterface(){}
 
	static bool InitAddQuest(int task_id, int child_task_id, int* global_value, int size);
 
	//QuestSetStart：参数 bNotChangeRanking: true， 不更改玩家排名， false, 更改玩家排名。
	static bool QuestSetStart(int task_id, int* first_child_global_value, int size,bool bNotChangeRanking);		//	开始一个PQ任务
	static bool QuestSetFinish(int task_id);	//	PQ任务结束
	//QuestSetNextChildTask： 参数 bNotChangeRanking: true， 不更改玩家排名， false, 更改玩家排名。
	static bool QuestSetNextChildTask(int task_id,  int child_task_id, int* child_global_value, int size,bool bNotChangeRanking);

	static int GetCurSubTask(int task_id);		// 获取PQ任务的当前子任务id
	static int GetCurTaskStamp(int task_id);	// 获取PQ任务的时戳
	static int GetCurContrib(int task_id, int role_id);	// 获取PQ任务当前贡献度

	static int GetCurAllPlace(int task_id, int role_id);	// 获取当前总体排名
	static int GetCurClsPlace(int task_id, int role_id);	// 获取当前职业排名

	static bool QuestAddPlayer(int task_id, int role_id);
	static bool QuestRemovePlayer(int task_id, int role_id);
	static bool QuestUpdatePlayerContrib(int task_id, int roleid, int inc_contrib);

	static int QuestGetGlobalValue(int lKey);	// 获取PQ任务全局变量

	static void QuestEnterWorldInit(int task_id, int role_id); // 进入世界初始化link相关数据
	
	static bool QuestSetRandContrib(int task_id, int fixed_contrib, int max_rand_contrib, int lowest_contrib); // 设置PQ随机贡献度

	static void SetMonsterController(int ctrl_id, bool trigger);	// 设置怪物控制器

	static void QuestLeaveWorld(int task_id, int role_id);
};

#endif

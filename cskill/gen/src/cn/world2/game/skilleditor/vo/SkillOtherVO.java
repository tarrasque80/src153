/*
 * Created on 2005-1-25
 *
 * TODO To change the template for this generated file go to
 * Window - Preferences - Java - Code Style - Code Templates
 */
package cn.world2.game.skilleditor.vo;

import cn.world2.game.skilleditor.vo.data.AttackWay;

/**
 * @author linxiaojun
 *
 * TODO To change the template for this generated type comment go to
 * Window - Preferences - Java - Code Style - Code Templates
 */
public class SkillOtherVO {
	private String id;//skillid
	private String isonland;//½���Ƿ���Ч��  1��  0�� ��ѡһ��Ĭ���ǣ� 
	private String isinsky;//�����Ƿ���Ч��  1��  0�� ��ѡһ��Ĭ�Ϸ� 
	private String isinwater;//ˮ���Ƿ���Ч��  1��  0�� ��ѡһ��Ĭ�Ϸ� 
	private String isAttackSkill;//�Ƿ�Ϊ�����Լ���:    1��  0�� ��ѡһ��Ĭ���ǣ�
	private String cannotusetogether;//����ͬʱ�����õļ��ܣ���;�ָ�	
	private String mainskillproperty;//����������  1���� 2�� 3ľ�� 4ˮ�� 5��6 �� ��ѡһ Ĭ�ϣ�1����    
	private String isAutoSearchDestination;//�Ƿ��Զ�Ѱ��Ŀ�� 1�ǡ� 0���ѡһ
	private String autosearchDestExp;//��Ѱ�뾶
	private String searchcenter;//1.������Ϊ����2.��Ŀ��Ϊ����
	private String continuoushit;//�Ƿ�������1��0��
	private String intonateActionName;//����������
	private String exeactionname;//ִ�ж�����
	private String exeactionnameselect;//����ѡ��� 1������� 2�����޹�
	private String exeactionweaponselect;//����ѡ��� 1������� 2�����޹�
	private String effectFileName;//Ч���ļ���
	private String effectfiletype;//Ч���ļ����ͣ�1ֱ�ߣ�2�����ߣ�3�㽶�ߣ�4��������
	private String fly_effect_proportion;//����Ч����С����ϵ��
	private String fly_effect_parameter;//���߸��Ӳ���
	private String fly_effect_behave;//����Ч�����ַ�ʽ��1�̶����˺���2�����ӵ��˺�
	private String fly_effect_time;//����ʱ��
	private String hitEnjureActionName;//�������˶�����
	private String hit_effect_proportion;//����Ч����С����ϵ��
	
	private String ridable;//����Ƿ���Ч1��Ч0��Ч
	private AttackWay attackway;//������Χ

	private String bianshenhou;//����״̬����(��ѡ)����;�ָ����1;2;4��ʾ1��2��4ѡ�� 1����״̬ 2����״̬һ 3����״̬�� 4����״̬��//�Ƿ��������1�ǡ�0��
	private String futihou;//�Ƿ���븽���1�ǡ�0��

	private String fenmingtime;//����ʱ��
	private String displayorder;//������ʾ˳��

	private String reserved1;	// ����
	private String reserved2;	// ����
	/**
	 * @return Returns the attackway.
	 */
	public AttackWay getAttackway() {
		return attackway;
	}
	/**
	 * @param attackway The attackway to set.
	 */
	public void setAttackway(AttackWay attackway) {
		this.attackway = attackway;
	}
	/**
	 * @return Returns the ridable.
	 */
	public String getRidable() {
		return ridable;
	}
	/**
	 * @param ridable The ridable to set.
	 */
	public void setRidable(String ridable) {
		this.ridable = ridable;
	}
	/**
	 * @return Returns the id.
	 */
	public String getId() {
		return id;
	}
	/**
	 * @param id The id to set.
	 */
	public void setId(String id) {
		this.id = id;
	}	
	/**
	 * @return Returns the effectFileName.
	 */
	public String getEffectFileName() {
		return effectFileName;
	}
	/**
	 * @param effectFileName The effectFileName to set.
	 */
	public void setEffectFileName(String effectFileName) {
		this.effectFileName = effectFileName;
	}
	/**
	 * @return Returns the hitEnjureActionName.
	 */
	public String getHitEnjureActionName() {
		return hitEnjureActionName;
	}
	/**
	 * @param hitEnjureActionName The hitEnjureActionName to set.
	 */
	public void setHitEnjureActionName(String hitEnjureActionName) {
		this.hitEnjureActionName = hitEnjureActionName;
	}
	/**
	 * @return Returns the intonateActionName.
	 */
	public String getIntonateActionName() {
		return intonateActionName;
	}
	/**
	 * @param intonateActionName The intonateActionName to set.
	 */
	public void setIntonateActionName(String intonateActionName) {
		this.intonateActionName = intonateActionName;
	}
	/**
	 * @return Returns the isAttackSkill.
	 */
	public String getIsAttackSkill() {
		return isAttackSkill;
	}
	/**
	 * @param isAttackSkill The isAttackSkill to set.
	 */
	public void setIsAttackSkill(String isAttackSkill) {
		this.isAttackSkill = isAttackSkill;
	}
	/**
	 * @return Returns the isAutoSearchDestination.
	 */
	public String getIsAutoSearchDestination() {
		return isAutoSearchDestination;
	}
	/**
	 * @param isAutoSearchDestination The isAutoSearchDestination to set.
	 */
	public void setIsAutoSearchDestination(String isAutoSearchDestination) {
		this.isAutoSearchDestination = isAutoSearchDestination;
	}
	/**
	 * @return Returns the isinsky.
	 */
	public String getIsinsky() {
		return isinsky;
	}
	/**
	 * @param isinsky The isinsky to set.
	 */
	public void setIsinsky(String isinsky) {
		this.isinsky = isinsky;
	}
	/**
	 * @return Returns the isinwater.
	 */
	public String getIsinwater() {
		return isinwater;
	}
	/**
	 * @param isinwater The isinwater to set.
	 */
	public void setIsinwater(String isinwater) {
		this.isinwater = isinwater;
	}
	/**
	 * @return Returns the isonland.
	 */
	public String getIsonland() {
		return isonland;
	}
	/**
	 * @param isonland The isonland to set.
	 */
	public void setIsonland(String isonland) {
		this.isonland = isonland;
	}
	/**
	 * @return Returns the mainskillproperty.
	 */
	public String getMainskillproperty() {
		return mainskillproperty;
	}
	/**
	 * @param mainskillproperty The mainskillproperty to set.
	 */
	public void setMainskillproperty(String mainskillproperty) {
		this.mainskillproperty = mainskillproperty;
	}
	/**
	 * @return Returns the autosearchDestExp.
	 */
	public String getAutosearchDestExp() {
		return autosearchDestExp;
	}
	/**
	 * @param autosearchDestExp The autosearchDestExp to set.
	 */
	public void setAutosearchDestExp(String autosearchDestExp) {
		this.autosearchDestExp = autosearchDestExp;
	}
	/**
	 * @return Returns the cannotusetogether.
	 */
	public String getCannotusetogether() {
		return cannotusetogether;
	}
	/**
	 * @param cannotusetogether The cannotusetogether to set.
	 */
	public void setCannotusetogether(String cannotusetogether) {
		this.cannotusetogether = cannotusetogether;
	}
	/**
	 * @return Returns the exeactionname.
	 */
	public String getExeactionname() {
		return exeactionname;
	}
	/**
	 * @param exeactionname The exeactionname to set.
	 */
	public void setExeactionname(String exeactionname) {
		this.exeactionname = exeactionname;
	}
	/**
	 * @return Returns the exeactionnameselect.
	 */
	public String getExeactionnameselect() {
		return exeactionnameselect;
	}
	/**
	 * @param exeactionnameselect The exeactionnameselect to set.
	 */
	public void setExeactionnameselect(String exeactionnameselect) {
		this.exeactionnameselect = exeactionnameselect;
	}
	/**
	 * @return Returns the searchcenter.
	 */
	public String getSearchcenter() {
		return searchcenter;
	}
	/**
	 * @param searchcenter The searchcenter to set.
	 */
	public void setSearchcenter(String searchcenter) {
		this.searchcenter = searchcenter;
	}
	/**
	 * @return Returns the continuoushit.
	 */
	public String getContinuoushit() {
		return continuoushit;
	}
	/**
	 * @param continuoushit The continuoushit to set.
	 */
	public void setContinuoushit(String continuoushit) {
		this.continuoushit = continuoushit;
	}
	/**
	 * @return Returns the exeactionweaponselect.
	 */
	public String getExeactionweaponselect() {
		return exeactionweaponselect;
	}
	/**
	 * @param exeactionweaponselect The exeactionweaponselect to set.
	 */
	public void setExeactionweaponselect(String exeactionweaponselect) {
		this.exeactionweaponselect = exeactionweaponselect;
	}
	/**
	 * @return Returns the effectfiletype.
	 */
	public String getEffectfiletype() {
		return effectfiletype;
	}
	/**
	 * @param effectfiletype The effectfiletype to set.
	 */
	public void setEffectfiletype(String effectfiletype) {
		this.effectfiletype = effectfiletype;
	}
	/**
	 * @return Returns the fly_effect_proportion.
	 */
	public String getFly_effect_proportion() {
		return fly_effect_proportion;
	}
	/**
	 * @param fly_effect_proportion The fly_effect_proportion to set.
	 */
	public void setFly_effect_proportion(String fly_effect_proportion) {
		this.fly_effect_proportion = fly_effect_proportion;
	}
	/**
	 * @return Returns the fly_effect_parameter.
	 */
	public String getFly_effect_parameter() {
		return fly_effect_parameter;
	}
	/**
	 * @param fly_effect_parameter The fly_effect_parameter to set.
	 */
	public void setFly_effect_parameter(String fly_effect_parameter) {
		this.fly_effect_parameter = fly_effect_parameter;
	}
	/**
	 * @return Returns the fly_effect_behave.
	 */
	public String getFly_effect_behave() {
		return fly_effect_behave;
	}
	/**
	 * @param fly_effect_behave The fly_effect_behave to set.
	 */
	public void setFly_effect_behave(String fly_effect_behave) {
		this.fly_effect_behave = fly_effect_behave;
	}
	/**
	 * @return Returns the fly_effect_time.
	 */
	public String getFly_effect_time() {
		return fly_effect_time;
	}
	/**
	 * @param fly_effect_time The fly_effect_time to set.
	 */
	public void setFly_effect_time(String fly_effect_time) {
		this.fly_effect_time = fly_effect_time;
	}
	/**
	 * @return Returns the hit_effect_proportion.
	 */
	public String getHit_effect_proportion() {
		return hit_effect_proportion;
	}
	/**
	 * @param hit_effect_proportion The hit_effect_proportion to set.
	 */
	public void setHit_effect_proportion(String hit_effect_proportion) {
		this.hit_effect_proportion = hit_effect_proportion;
	}
	/**
	 * @return Returns the bianshenhou.
	 */
	public String getBianshenhou() {
		return bianshenhou;
	}
	/**
	 * @param bianshenhou The bianshenhou to set.
	 */
	public void setBianshenhou(String bianshenhou) {
		this.bianshenhou = bianshenhou;
	}
	/**
	 * @return Returns the futihou.
	 */
	public String getFutihou() {
		return futihou;
	}
	/**
	 * @param futihou The futihou to set.
	 */
	public void setFutihou(String futihou) {
		this.futihou = futihou;
	}
	/**
	 * @return Returns the fenmingtime.
	 */
	public String getFenmingtime() {
		return fenmingtime;
	}
	/**
	 * @param fenmingtime The fenmingtime to set.
	 */
	public void setFenmingtime(String fenmingtime) {
		this.fenmingtime = fenmingtime;
	}
	/**
	 * @return Returns the displayorder.
	 */
	public String getDisplayorder() {
		return displayorder;
	}
	/**
	 * @param displayorder The displayorder to set.
	 */
	public void setDisplayorder(String displayorder) {
		this.displayorder = displayorder;
	}
}

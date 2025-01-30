/*
 * Created on 2004-12-21
 *
 * TODO To change the template for this generated file go to
 * Window - Preferences - Java - Code Style - Code Templates
 */
package cn.world2.game.skilleditor.DB.DAO;

import java.io.Serializable;
import java.util.List;

import cn.world2.game.skilleditor.vo.Skill;
import cn.world2.game.skilleditor.vo.data.SkillLevelVO;

/**
 * @author linxiaojun
 *
 * TODO To change the template for this generated type comment go to
 * Window - Preferences - Java - Code Style - Code Templates
 */
public interface ISkillDAO {
	/**
	 * ���ռ���idȡ��ĳ���������С�
	 * @param skillid
	 * @return
	 */
	Skill getSkillById(String skillid);
	/**
	 * ���뼼��
	 * @param skill
	 * @return
	 */
	String insertSkill(Skill skill);
	/**
	 * ɾ������
	 * @param skill
	 * @return
	 */
	int deleteSkill(Skill skill);
	/**
	 * ɾ������
	 * @param idlist
	 * @return
	 */
	int deleteSkill(List idlist);
	/**
	 * ���¼���
	 * @param skill
	 * @return
	 */
	boolean updateSkill(Skill skill);
	/**
	 * ��ü����б�
	 * @param condtion
	 * @return
	 */
	List getSkill(String condtion);
	/**
	 * ������м��ܼ���б�
	 * ��ֻ����id�����֣�
	 * @param condition
	 * @return
	 */
	List getSkillsShort(String condition);
	/**
	 * ����skill����
	 * @param slvo
	 * @return
	 */
	boolean updateSkillLevel(SkillLevelVO slvo,String skillid);
	/**
	 * ���skill����
	 * @param skillid
	 * @return
	 */
	SkillLevelVO getSkillLevel(String skillid);
	/**
	 * ���¼��ܻ����༭
	 * @param skill
	 * @return
	 */
	boolean updateSkillBasic(Skill skill);
	/**
	 * ��û������ܵ�����
	 * @param skillid
	 * @return
	 */
	Skill getSkillBasic(String skillid);
	/**
	 * ����Skill�Ĺ���
	 * @param p
	 * @return
	 */
	boolean updateSkillProcedure(List list,String skillid);
	/**
	 * ���SKILL�Ĺ���
	 * @param skillid
	 * @return
	 */
	List getSkillProcedure(String skillid);
	/**
	 * ͨ����ѯ���¼��ܣ�����ִ������INSERT,UPDATE,DELETE
	 * @param query
	 * @return
	 */
	boolean updateSkillByQuery(String query);
	/**
	 * ����������
	 * @param query
	 * @return
	 */
	String copySkillByQuery(String query,String oldrole);
	void updateBlob(Serializable s,String field,String skillid);
	Serializable getBlob(String field,String skillid);
}

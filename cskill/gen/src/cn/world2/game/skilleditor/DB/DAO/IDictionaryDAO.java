/*
 * Created on 2005-1-8
 *
 * TODO To change the template for this generated file go to
 * Window - Preferences - Java - Code Style - Code Templates
 */
package cn.world2.game.skilleditor.DB.DAO;

import java.util.List;

import cn.world2.game.skilleditor.vo.Dictionary;

/**
 * @author linxiaojun
 *
 * TODO To change the template for this generated type comment go to
 * Window - Preferences - Java - Code Style - Code Templates
 */
public interface IDictionaryDAO {
	Dictionary getDitionary(String key);
	/**
	 * ��ù����ֵ�
	 * @return
	 */
	List getAllDictionary();
	boolean insertDictionary(Dictionary d);
	/**
	 * ���¹����ֵ�
	 * @param list
	 * @return
	 */
	boolean updateDictionary(List list);
	/**
	 * ��øü��ܱ��ʽ˽�е��ֵ�
	 * @return
	 */
	List getSkillPrivateDict(String skillid);
	/**
	 * ��øü��ܱ��ʽ�������ֵ�
	 * @param list
	 * @return
	 */
	boolean updateSkillPrivateDict(List list,String skillid);	
}

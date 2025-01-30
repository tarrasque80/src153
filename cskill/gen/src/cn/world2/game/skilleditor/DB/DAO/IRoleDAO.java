/*
 * Created on 2004-12-21
 *
 * TODO To change the template for this generated file go to
 * Window - Preferences - Java - Code Style - Code Templates
 */
package cn.world2.game.skilleditor.DB.DAO;

import java.util.List;

import cn.world2.game.skilleditor.vo.Role;

/**
 * @author linxiaojun
 *
 * TODO To change the template for this generated type comment go to
 * Window - Preferences - Java - Code Style - Code Templates
 */
public interface IRoleDAO {
	/**
	 * ����һ��ɫ
	 * @param role
	 * @return
	 */
	boolean insertOneRole(Role role);
	/**
	 * ɾ��һ��ɫ
	 * @param role
	 * @return
	 */
	boolean deleteOneRole(Role role);
	/**
	 * ����һ��ɫ
	 * @param newRole
	 * @return
	 */
	int updateOneRole(Role newRole);
	/**
	 * ����������ý�ɫ��
	 * �������null�򷵻����н�ɫ
	 * @param condition
	 * @return
	 */
	List getRoles(String condition);
	/**
	 * ���غ���skills��Role�б�
	 * �������null�򷵻����н�ɫ
	 * @param condition
	 * @return
	 */
	List getRolesWithSkills(String condition);
	/**
	 * ȡ��һ��Role
	 * @param id
	 * @return
	 */
	Role retrieveRole(String id);
	/**
	 * ɾ��һ����role
	 * @param idlist
	 */
	int deleteRoles(List idlist);
}

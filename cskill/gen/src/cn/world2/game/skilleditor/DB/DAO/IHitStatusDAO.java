/*
 * Created on 2005-2-26
 *
 * TODO To change the template for this generated file go to
 * Window - Preferences - Java - Code Style - Code Templates
 */
package cn.world2.game.skilleditor.DB.DAO;

import java.util.LinkedList;

import cn.world2.game.skilleditor.vo.HitStatus;

/**
 * @author linxiaojun
 *
 * TODO To change the template for this generated type comment go to
 * Window - Preferences - Java - Code Style - Code Templates
 */
public interface IHitStatusDAO {
	/**
	 * ׼���ɱ༭���б�
	 * @return
	 */
	LinkedList<HitStatus> retrieveAllHitStatus();
	/**
	 * ����ɱ༭���б�
	 * @param list
	 * @return
	 */
	boolean saveAllHitStatus(LinkedList<HitStatus> list);
}

/*
 * Created on 2004-12-28
 *
 * TODO To change the template for this generated file go to
 * Window - Preferences - Java - Code Style - Code Templates
 */
package cn.world2.game.skilleditor.vo.data;

import java.io.Serializable;

/**
 * @author linxiaojun
 *
 * TODO To change the template for this generated type comment go to
 * Window - Preferences - Java - Code Style - Code Templates
 */
public class AttackWay implements Serializable {
	private String approach;//0,�㹥�� 1,�߹��� 2,����ΪԲ�ĵ����� 3,Ŀ���ΪԲ�ĵ����� 4,����ΪԲ�ĵ����� 5,����
	private String radious00;//Բ���뾶
	private String radious01;//��뾶
	private String radious02;//���ΰ뾶		
	private String angle;//���ΰ��Ž�
	private String killnum;//�Ѿ�������
	private String attackdistance00;//�����Ч����
	private String attackdistance01;//Բ������

	private String reserved1; // ����
	private String reserved2; // ����
	/**
	 * @return Returns the radious00.
	 */
	public String getRadious00() {
		return radious00;
	}
	/**
	 * @param radious00 The radious00 to set.
	 */
	public void setRadious00(String radious00) {
		this.radious00 = radious00;
	}
	/**
	 * @return Returns the radious01.
	 */
	public String getRadious01() {
		return radious01;
	}
	/**
	 * @param radious01 The radious01 to set.
	 */
	public void setRadious01(String radious01) {
		this.radious01 = radious01;
	}
	/**
	 * @return Returns the radious02.
	 */
	public String getRadious02() {
		return radious02;
	}
	/**
	 * @param radious02 The radious02 to set.
	 */
	public void setRadious02(String radious02) {
		this.radious02 = radious02;
	}
	/**
	 * @return Returns the attackdistance00.
	 */
	public String getAttackdistance00() {
		return attackdistance00;
	}
	/**
	 * @param attackdistance00 The attackdistance00 to set.
	 */
	public void setAttackdistance00(String attackdistance00) {
		this.attackdistance00 = attackdistance00;
	}
	/**
	 * @return Returns the attackdistance01.
	 */
	public String getAttackdistance01() {
		return attackdistance01;
	}
	/**
	 * @param attackdistance01 The attackdistance01 to set.
	 */
	public void setAttackdistance01(String attackdistance01) {
		this.attackdistance01 = attackdistance01;
	}
	/**
	 * @return Returns the angle.
	 */
	public String getAngle() {
		return angle;
	}
	/**
	 * @param angle The angle to set.
	 */
	public void setAngle(String angle) {
		this.angle = angle;
	}
	/**
	 * @return Returns the approach.
	 */
	public String getApproach() {
		return approach;
	}
	/**
	 * @param approach The approach to set.
	 */
	public void setApproach(String approach) {
		this.approach = approach;
	}
	/**
	 * @return Returns the killnum.
	 */
	public String getKillnum() {
		return killnum;
	}
	/**
	 * @param killnum The killnum to set.
	 */
	public void setKillnum(String killnum) {
		this.killnum = killnum;
	}
}


package Cpp;

import java.io.PrintStream;
import java.util.*;

import cn.world2.game.skilleditor.biz.*;
import cn.world2.game.skilleditor.vo.*;
import cn.world2.game.skilleditor.vo.data.*;
import expr.*;

public class ExportCsv
{
	private Skill skill;
	private List skills;
	private List weapons;
	private List allhitstatus;

	private String ESCExpr(String s)
	{
		if( null == s )
			s = "";
		s.trim();
		if( s.length() > 0 && s.charAt(s.length()-1) != ';' )
			s += ";";
		return s;
	}

	HitStatus GetHitStatusStatic( String id )
	{
		if( null == id )
			return null;
		for (Iterator it = this.allhitstatus.iterator(); it.hasNext(); )
		{
			HitStatus hss = (HitStatus)it.next();
			if( id.equals( hss.getId() ) )
				return hss;
		}
		return null;
	}

	String GetStateAttacks( List list )
	{
		String result = "";
		if( null == list )
			return "";

		for (Iterator it = list.iterator(); it.hasNext(); )
		{
			HitStatusParam hsp = (HitStatusParam)it.next();
			if( hsp.getId().equals("-1") )
				continue;

			HitStatus hss = GetHitStatusStatic(hsp.getId());
			if( null == hss )
			{
				System.out.println( "\tError GetHitStatusStatic: id="+hsp.getId() );
				continue;
			}

			String exp = "";
			if( hss.isNeed_probability() )
				exp += "victim.probability=1.0*" + ESCExpr(hsp.getProbability());
			if( hss.isNeed_time() )
				exp += "victim.time=" + ESCExpr(hsp.getTime());
			if( hss.isNeed_proportion() )
				exp += "victim.ratio=" + ESCExpr(hsp.getProportion());
			if( hss.isNeed_increasement() )
				exp += "victim.showicon=" + ESCExpr(hsp.getIncreasement());
			if( hss.isNeed_total() )
				exp += "victim.amount=" + ESCExpr(hsp.getTotal());
			if( hss.isNeed_value() )
				exp += "victim.value=" + ESCExpr(hsp.getValue());

			if( null != hss.getExecute() && hss.getExecute().length() > 0 )
				exp += "victim." + hss.getExecute() + "=" + "1;";

			if( exp.length() > 0 )
				result += exp;
		}
		return result;
	}

	public ExportCsv( Skill skill, List skills, List weapons, List allhitstatus )
	{
		this.skill = skill;
		this.skills = skills;
		this.weapons = weapons;
		this.allhitstatus = allhitstatus;
	}

	static public void writeheader(PrintStream ps)
	{
		ps.print( "��ɫID," );
		ps.print( "��������," );
		ps.print( "ͼ��," );
		ps.print( "������ʾ˳��," );
		ps.print( "����˵��," );
		ps.print( "ǰ�Ἴ��," );
		ps.print( "���漶��," );
		ps.print( "��������," );
		ps.print( "��󼶱�," );
		ps.print( "����������," );

		ps.print( "�Ƿ���ҪĿ��," );
		ps.print( "½���Ƿ���Ч," );
		ps.print( "�����Ƿ���Ч," );
		ps.print( "ˮ���Ƿ���Ч," );
		ps.print( "����Ƿ���Ч," );
		ps.print( "�Ƿ�˲������," );
		ps.print( "ʹ�ú��Զ�����," );
		ps.print( "Ŀ��Ϊʬ��," );
		ps.print( "����״̬����," );
		ps.print( "��������," );
		ps.print( "����ͬʱ�����õļ���," );

		int count = 0;
		while( count ++ < 3 )
			ps.print( "������,ʱ��,�˳�����,ѭ������,��·����,�������,�жϸ���,��ȡ��,����," );

		ps.print( "Ч���ļ���," );
		ps.print( "�������˶�����," );
		ps.print( "�˺���ʱ," );
		ps.print( "������׼ȷ����������," );

		ps.print( "������Χ," );
		ps.print( "�����Ч����," );
		ps.print( "Բ������," );
		ps.print( "Բ���뾶," );
		ps.print( "��뾶," );
		ps.print( "���ΰ뾶," );
		ps.print( "���ΰ��Ž�," );

		ps.print( "����MP," );
		ps.print( "��������," );
		ps.print( "ִ��ʱ��," );
		ps.print( "��ȴʱ��," );
		ps.print( "�Թ����޶ȵ�����," );
		ps.print( "���н��״̬," );
		ps.print( "��������Ч��," );

		ps.print( "ѧϰ����1," );
		ps.print( "ѧϰ����2," );
		ps.print( "ѧϰ����3," );
		ps.print( "ѧϰ����4," );
		ps.print( "ѧϰ����5," );
		ps.print( "ѧϰ����6," );
		ps.print( "ѧϰ����7," );
		ps.print( "ѧϰ����8," );
		ps.print( "ѧϰ����9," );
		ps.print( "ѧϰ����10," );

		ps.print( "ѧϰ�������1," );
		ps.print( "ѧϰ�������2," );
		ps.print( "ѧϰ�������3," );
		ps.print( "ѧϰ�������4," );
		ps.print( "ѧϰ�������5," );
		ps.print( "ѧϰ�������6," );
		ps.print( "ѧϰ�������7," );
		ps.print( "ѧϰ�������8," );
		ps.print( "ѧϰ�������9," );
		ps.print( "ѧϰ�������10," );

		ps.print( "����Ҫ��," );
		ps.print( "����Ԫ��," );
		ps.print( "ѧϰ��Ҫ��Ʒ," );
		ps.print( "ѧϰ���ѽ�Ǯ," );
		ps.print( "������Ҫ������," );

		ps.print("\n");
	}

	public void write(PrintStream ps)
	{
		SkillOtherVO other = skill.getOtherinfo();
		AttackWay range = other.getAttackway();
		if( null == range )	range = new AttackWay();
		SkillLevelVO levelinfo = skill.getLevelinfo();
		if( null == levelinfo ) levelinfo = new SkillLevelVO();

		String stateattacks = //ESCExpr( skill.getAttack() ) + 
			ESCExpr( GetStateAttacks( skill.getHitstatus() ) );

		int i;

		ps.print( skill.getRole().getId() + "," );
		ps.print( skill.getName() + "," );
		ps.print( skill.getIcon() + "," );
		ps.print( other.getDisplayorder() + "," );
		ps.print( skill.getSkilldesc() + "," );
		ps.print( skill.getPreconditonSkill() + "," );
		ps.print( skill.getReviselevel() + "," );
		ps.print( skill.getPassiveStatus() + "," );
		ps.print( skill.getMaxLevel() + "," );
		ps.print( other.getMainskillproperty() + "," );

		ps.print( skill.getNeedgoal() + "," );
		ps.print( other.getIsonland() + "," );
		ps.print( other.getIsinsky() + "," );
		ps.print( other.getIsinwater() + "," );
		ps.print( other.getRidable() + "," );
		ps.print( other.getIntonateActionName() + "," );
		ps.print( other.getFutihou() + "," );
		ps.print( other.getContinuoushit() + "," );
		ps.print( other.getBianshenhou() + "," );
		String strArmlimitation = new String();
		String[] arms = skill.getArmlimitation();
		for( i = 0; null!= arms && i<arms.length; i++ )
			strArmlimitation += (arms[i] + ";");
		ps.print( strArmlimitation + "," );
		ps.print( other.getCannotusetogether() + "," );

		int count = 0;
		List procedures = skill.getProcedures();
		if( null != procedures )
		{
			int stateid = 0;
			for(Iterator itp=procedures.iterator(); itp.hasNext(); )
			{
				ProcedureVO p = (ProcedureVO)itp.next();

				ps.print( p.getName() + "," );
				ps.print( p.getContinuetime() + "," );
				ps.print( p.getQuitCondition() + "," );
				ps.print( p.getLoopCondition() + "," );
				ps.print( p.getBypassCondition() + "," );
				ps.print( p.getComputeResult() + "," );	// calculate
				ps.print( p.getAsynchronousTerminate() + "," );	// interrupt
				ps.print( p.getKequxiao() + "," );	// cancel
				ps.print( p.getXuli() + "," );	// skip
				count ++;
			}
		}
		while( count ++ < 3 )	ps.print( ",,,,,,,,," );

		ps.print( other.getEffectFileName() + "," );
		ps.print( other.getHitEnjureActionName() + "," );
		ps.print( other.getFenmingtime() + "," );
		ps.print( other.getExeactionname() + "," );

		ps.print( range.getApproach() + "," );
		ps.print( range.getAttackdistance00() + "," );
		ps.print( range.getAttackdistance01() + "," );
		ps.print( range.getRadious00() + "," );
		ps.print( range.getRadious01() + "," );
		ps.print( range.getRadious02() + "," );
		ps.print( range.getAngle() + "," );

		ps.print( skill.getCondition() + "," );
		ps.print( skill.getDistance() + "," );
		ps.print( skill.getExecutetime() + "," );
		ps.print( skill.getCooltime() + "," );
		ps.print( skill.getHatedegree() + "," );
		ps.print( stateattacks + "," );
		ps.print( (null!=skill.getInitskilluse()&&skill.getInitskilluse().length()>0)?skill.getInitskilluse():skill.getChangeweaponskilluse() + "," );

		ps.print( levelinfo.getLevel1Learning() + "," );
		ps.print( levelinfo.getLevel2Learning() + "," );
		ps.print( levelinfo.getLevel3Learning() + "," );
		ps.print( levelinfo.getLevel4Learning() + "," );
		ps.print( levelinfo.getLevel5Learning() + "," );
		ps.print( levelinfo.getLevel6Learning() + "," );
		ps.print( levelinfo.getLevel7Learning() + "," );
		ps.print( levelinfo.getLevel8Learning() + "," );
		ps.print( levelinfo.getLevel9Learning() + "," );
		ps.print( levelinfo.getLevel10Learning() + "," );

		ps.print( levelinfo.getLevel1Cost() + "," );
		ps.print( levelinfo.getLevel2Cost() + "," );
		ps.print( levelinfo.getLevel3Cost() + "," );
		ps.print( levelinfo.getLevel4Cost() + "," );
		ps.print( levelinfo.getLevel5Cost() + "," );
		ps.print( levelinfo.getLevel6Cost() + "," );
		ps.print( levelinfo.getLevel7Cost() + "," );
		ps.print( levelinfo.getLevel8Cost() + "," );
		ps.print( levelinfo.getLevel9Cost() + "," );
		ps.print( levelinfo.getLevel10Cost() + "," );

		ps.print( "\"" + levelinfo.getLevel_requirement() + "\"," );
		ps.print( "\"" + levelinfo.getSkill_point_requirement() + "\"," );
		ps.print( "\"" + other.getFly_effect_proportion() + "\"," );
		ps.print( "\"" + other.getFly_effect_parameter() + "\"," );
		ps.print( "\"" + other.getFly_effect_time() + "\"," );

		ps.print("\n");
	}

}


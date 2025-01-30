#ifndef _GNET_ANALYZER_H
#define _GNET_ANALYZER_H
#include "syntaxtree.hpp"
#include "objectgraph.hpp"
namespace GNET
{
class Analyzer
{
	Analyzer() {}
	//some token is context relative, such as '[]'
	// in case of assignment, etc. 'a[5]=3;' [] means the fifth element of a is 3
	// in case of announcement, etc. 'a[5]={1,3,4,1,4}', [] means allocate memory space for 5 integers.
	// and in statement state, such definition as 'a[5]={1,3,4,1,4}' is forbidden
	enum Context_State //context relative state
	{
		_STATE_STMT,		//context is statement, such as calcuation, assignment,
		_STATE_VAR_ANNC, 	//annouce varible
		_STATE_FUN_ANNC,	//announce function
		_STATE_STRUCT_ANNC,	//annouce structure
	};
	static ObjectGraph* og;
	static unsigned int current_type;
	static TreeNode* current_treenode;
	static SyntaxTree* st;
	static Context_State context_state;
	
	enum ErrCode
	{
		ERR_ASSIGN_CHILD,		//�����ASSIGN�ڵ�
		ERR_TYPE_MISMATCH,		//���Ͳ�ƥ��
		ERR_MEMBER_ERROR,		//����ĳ�Ա����	
		ERR_NULL_SYNTREE,		//�յ��﷨��
		ERR_INVALID_NODEKIND,	//��Ч�Ľڵ�����
		ERR_INVALID_TYPE,		//��Ч������ ����
		ERR_INVALID_OPERATOR,	//��Ч�Ĳ������ṹ
		ERR_INVALID_CURTYPE,	//��Ч�ĵ�ǰ����
		ERR_INVALID_ANNOUCE,	//��Ч�ı�������
		ERR_INVALID_PRINT,		//��Ч�Ĵ�ӡ���
		ERR_INVALID_ARRAY,		//��Ч�����鶨��
		ERR_ANNOUNCE_OGARRAY,	//������ͼ�еĶ�����Ϊ���飬�����ǲ������
		ERR_VAR_ARRAY_UNINIT,	//����������û��ָ�����鳤��
		ERR_ARRAY_CONTEXT,		//��������鶨��λ�ã�����ֻ���������ж��壬������ִ������ж���
		ERR_ARRAY_OVERBOUND,	//����Խ��
		ERR_INIT_ARRAY_FAILED,	//��ʼ������ʧ��
		ERR_OPERATE_NONARRAY,	//�ڷ��������ͱ����Ͻ���Ѱַ����
		ERR_VARNAME_EXIST,		//�ظ��ı�������
		ERR_USE_OGVAR,			//ʹ��OG�ж���ı�����Ŀǰ��ʱ��֧�������ռ��ʶ��
	};
private:
	void TypeCheck(unsigned int type1,unsigned int type2,bool strict=true);
	unsigned int OGCheck(unsigned int parent_type,unsigned int child_name);
	void StmtCheck(TreeNode* t);
public:	
	static Analyzer& GetInstance() {static Analyzer instance; return instance; }
	static void Analyze(SyntaxTree& _st,ObjectGraph& _og,const std::string& start_point_type)
	{
		og = &_og;
		st = &_st;
		try
		{
			TreeNode* t=_st.GetRoot();
			if (NULL==t) 
				throw ERR_NULL_SYNTREE;
			if ((current_type = og->GetIdentifier(start_point_type))==INVALID_IDENTIFIER)
				throw ERR_INVALID_CURTYPE;
			while (t!=NULL)
			{
				current_treenode=t;
				if (t->nodekind == TreeNode::StmtK)
				{   
					switch (t->kind.stmt)
					{
						case TreeNode::AnnounceK:
							if (og->GetIdentifier(t->name)!=INVALID_IDENTIFIER)
								throw ERR_USE_OGVAR;
							if(!st->symtab.insert(t)) 
								throw ERR_VARNAME_EXIST;
							context_state=_STATE_VAR_ANNC;
							break;
						default:
							context_state=_STATE_STMT;
							break;
					}
				}				
				else
				{
					context_state=_STATE_STMT;
				}	
				GetInstance().StmtCheck(t);
				t=t->sibling;
			}
		}
		catch (ErrCode errcode)
		{
			printf("Analyzer:");
			switch (errcode)
			{
				case ERR_NULL_SYNTREE:
					printf("Empty syntax tree.\n");
					break;
				case ERR_ASSIGN_CHILD:
					printf("Assign node error. LINE: %d\n",current_treenode->lineno);
					break;
				case ERR_TYPE_MISMATCH:
					printf("Type mismatch. LINE: %d\n",current_treenode->lineno);
					break;
				case ERR_MEMBER_ERROR:
					printf("Member is not exist. LINE: %d\n",current_treenode->lineno);
					break;
				case ERR_INVALID_NODEKIND:
					printf("Invalid Node kind. LINE: %d\n",current_treenode->lineno);
					break;
				case ERR_INVALID_TYPE:
					printf("Invalid type value. LINE: %d\n",current_treenode->lineno);
					break;
				case ERR_INVALID_OPERATOR:
					printf("Invalid operator node. LINE: %d\n",current_treenode->lineno);
					break;
				case ERR_INVALID_CURTYPE:
					printf("Invalid start point type_id. LINE: %d\n",current_treenode->lineno);
					break;	
				case ERR_INVALID_ANNOUCE:
					printf("Invalid variable announcement. LINE: %d\n",current_treenode->lineno);	
					break;
				case ERR_INVALID_ARRAY:
					printf("Invalid array definition. Maybe no 'value' field in OG file. LINE: %d\n",current_treenode->lineno);
					break;
				case ERR_ANNOUNCE_OGARRAY:
					printf("Cann't announce array of a Class that defined in OG. LINE: %d\n",current_treenode->lineno);
					break;	
				case ERR_VAR_ARRAY_UNINIT:
					printf("Varialbe array maybe uninitialed. LINE: %d\n",current_treenode->lineno);
					break;
				case ERR_ARRAY_CONTEXT:
					printf("Cann't define a array without announcement context. LINE: %d\n",current_treenode->lineno);
					break;
				case ERR_ARRAY_OVERBOUND:
					printf("Array over bound. LINE: %d\n",current_treenode->lineno);	
					break;
				case ERR_INIT_ARRAY_FAILED:
					printf("Initialize array failed. LINE: %d\n",current_treenode->lineno);	
					break;
				case ERR_OPERATE_NONARRAY:
					printf("Cann't do '[]' operation on non-array varible. LINE: %d\n",current_treenode->lineno);	
					break;
				case ERR_VARNAME_EXIST:
					printf("Variable name has already be defined before. LINE: %d\n",current_treenode->lineno);
					break;	
				case ERR_USE_OGVAR:
					printf("Variable name has be defined in OG file. LINE: %d\n",current_treenode->lineno);
					break;	
				case ERR_INVALID_PRINT:
					printf("Invalid print statement. LINE: %d\n",current_treenode->lineno);
					break;	
			}
			if (NULL != current_treenode)
				SyntaxTree::DisplaySolitaryTree(current_treenode);
			throw errcode;
		}
	}
};//end of class	
};//end of namespace
#endif


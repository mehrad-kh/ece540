#include "cfg.h"
#include "basicblk.h"
#include "dfa.h"
#include <stdio.h>
#include <cstddef>
#include <iostream>
#include <assert.h>
#include <algorithm>	//std::sort
extern "C" { 
#include <simple.h>
}
extern "C" { 
#include "print.h"
}


using std::cout;
using std::endl;

void cfg::global_cse ()
{
	// The proccess is as follows:
	// Go through each basic block
	// for each basic block go through its intructions
	// if the instuction is a definition then
	// 1) find if that definition is available at the entry of the BB ae_ins[i]
	// 2) if YES  then move backward to locate the source of expression

	int i;
	for (i = 0; i < n; i++)
	{
		find_expr(i);
	}
}


void cfg::find_expr(int bb_number)
{
	simple_instr * leader_inst = vertexlist[bb_number]->get_leader_instr();
	simple_instr * last_inst = vertexlist[bb_number]->get_last_instr();
	int exprnum = -1;

	if (leader_inst == NULL)
		return;

	simple_instr * s = leader_inst;
	std::vector<simple_instr *> AVE;

	while(s != last_inst->next)
	{
		if ((exprnum = is_expression(s)) != -1)
		{
	
			if (is_global_common_subexpression(bb_number, exprnum, s, leader_inst))
			{
				simple_reg * dst = s->u.base.dst;
				simple_reg * pseudo_reg;
				cout << "global common subexpression in block # " << bb_number << ":-->>  ";
				fprint_instr(stdout, s , 0);

				if (dst->kind == TEMP_REG)
				{
					// since a temp register compute a common subexpression and that
					// temp is going to be used in only one place with in the basic block
					// what I do is that I'll go and change dst register type to PSEUDO reg
					// remove the common subexpression which is s
					dst->kind = PSEUDO_REG;
					pseudo_reg = dst;
					remove_common_subexpr_with_temp_reg(bb_number, s);
				}

				else
				{
					pseudo_reg = new_register(dst->var->type, PSEUDO_REG);
					s->opcode = CPY_OP;
					s->u.base.src1 = pseudo_reg;
					s->u.base.src2 = NO_REGISTER;
				}

				// Now I have a PSEUDO_REG. call recursively locate_sources to search backward in 
				// the flowgraph to find occurrences of expr, such as in the context of 
				// v<--expr, that caused it to be in AEin[exprnum];
				std::vector<int> pred = vertexlist[bb_number]->get_pred();
				int pred_size = pred.size();
				std::set<int> visit;

				int i;
				for (i = 0; i < pred_size; i++)
				{
					cout << "going to locate sources reachs " << bb_number << " of expr " << exprnum << endl;
					locate_sources(bb_number, exprnum, pseudo_reg, visit);
				}
	
			}

			else if (is_local_common_subexpression(s, AVE, bb_number))
			{
				;
			}

			else
			{
				// it is a expression but cannot be eliminated by means of global or local cse. 
				// so add it to AVE mybe it can eliminate some other expression later.
				AVE.push_back(s);
			}	
			
		}

		kill_AVE(AVE, s); // remove expressions from AVE which will be killed by s
		s = s->next;
	}
}


int cfg::is_expression(simple_instr *s)
{
	std::string key;
	char buffer[20];
	bool expr = FALSE;

	switch (s->opcode)
	{
		case CVT_OP:	
		case NEG_OP:
		case NOT_OP:
		{
			if (s->u.base.src1->kind == PSEUDO_REG)
			{
				sprintf(buffer,"%d#%d", s->u.base.src1->num, s->opcode);
				key.assign(buffer);
				expr = TRUE;
			}
			break;
		}

		case ADD_OP:
		case MUL_OP:
		case AND_OP:
		case IOR_OP:
		case XOR_OP:
		case SUB_OP:
		case DIV_OP:
		case REM_OP:
		case MOD_OP:
		case ASR_OP:
		case LSR_OP:
		case ROT_OP:
		case SEQ_OP:
		case SNE_OP:
		case SL_OP:
		case SLE_OP:
		{
			if (s->u.base.src1->kind == PSEUDO_REG && s->u.base.src2->kind == PSEUDO_REG)
			{
				sprintf(buffer,"%d#%d#%d", s->u.base.src1->num, s->opcode, s->u.base.src2->num);
				key.assign(buffer);
				expr = TRUE;
			}
			break;
		}
		
		default: break;
	}

	if (expr)
	{
		int exprnum = mydfa->expression_map_exprnum[key];
		return exprnum;
	}

	return -1;
}


bool cfg::is_global_common_subexpression(int bb_number, int exprnum, simple_instr * s, simple_instr * leader_inst)
{
	//
	//cout << "size of ae_ins is " << mydfa->ae_ins.size() << endl;
	//cout << "bb_number is " << bb_number << " and exprnum is =  " << exprnum << endl;
	if (mydfa->ae_ins[bb_number][exprnum] == 0)
		return FALSE;

	simple_instr * curr = s->prev;

	while (curr != leader_inst->prev)
	{
		switch(simple_op_format(curr->opcode))
		{
			case BASE_FORM:
			{
				if (curr->u.base.dst == s->u.base.src1 || curr->u.base.dst == s->u.base.src2)
					return FALSE;
			}

			case LDC_FORM:
			{
				if (curr->u.ldc.dst == s->u.base.src1 || curr->u.ldc.dst == s->u.base.src2)
					return FALSE;
			}	

			case CALL_FORM:
			{
				if (curr->u.call.dst == s->u.base.src1 || curr->u.call.dst == s->u.base.src2)
					return FALSE;
			}
		}

		curr = curr->prev;
	}

	return TRUE;
}


bool cfg::is_local_common_subexpression(simple_instr *s, std::vector<simple_instr *> &AVE, int bb_number)
{
	// note all expressions are of BASE_FROM.
	std::vector<simple_instr *>::iterator it;
	for (it = AVE.begin(); it != AVE.end(); it++)
	{
		if ((*it)->opcode == s->opcode && (*it)->u.base.src1 == s->u.base.src1 && (*it)->u.base.src2 == s->u.base.src2)
		{
			simple_reg * dst = s->u.base.dst;
			// since a temp register compute a common subexpression and that
			// temp is going to be used in only one place with in the basic block
			// what I do is that I'll go and change dst register type to PSEUDO reg
			// remove the common subexpression which is s
			dst->kind = PSEUDO_REG;
			remove_common_subexpr_with_temp_reg(bb_number, s);

			// (*it) -->  ti = src1 op src2
			// 1) find only use of ti and replace it with with r;
			// 2) create new instruction r = ti;
			// 3) insert after curr
			simple_reg * r = dst;

			simple_reg * t = (*it)->u.base.dst;
			replace_use_temp_with_pseudo ((*it), t, r);
			// insret new copy instruction
			
			simple_instr * copy_instr = new_instr(CPY_OP, s->u.base.dst->var->type);
			copy_instr->u.base.dst = r;
			copy_instr->u.base.src1 = t;
			copy_instr->u.base.src2 = NULL;

			// now insert the new instruction after curr
			copy_instr->next = (*it)->next;
			copy_instr->prev = (*it);
			(*it)->next = copy_instr;
			copy_instr->next->prev = copy_instr;

			return TRUE;
		}
	}
	return FALSE;
}


void cfg::kill_AVE(std::vector<simple_instr *> &AVE, simple_instr * s)
{
	// remove expressions from AVE which will be killed by s
	simple_reg * dst = NULL;

	switch(simple_op_format(s->opcode))
	{
		case BASE_FORM:
		{
			dst = s->u.base.dst;
			break;
		}

		case LDC_FORM:
		{
			dst = s->u.ldc.dst;
			break;
		}

		case CALL_FORM:
		{
			dst = s->u.call.dst;
			break;
		}

		default: break;
	}

	if (dst == NULL)
		return;

	if (dst->kind == TEMP_REG)
		return;

	std::vector<simple_instr *>::iterator it;
	it = AVE.begin();

	while (it != AVE.end())
	{	

		if ((*it)->u.base.src1 == dst || (*it)->u.base.src2 == dst)
		{
			it = AVE.erase(it);
		}

		else
		{
			it++;
		}
	}

}

void cfg::remove_common_subexpr_with_temp_reg(int bb_number, simple_instr  * s)
{
	simple_instr * leader_inst = vertexlist[bb_number]->get_leader_instr();
	simple_instr * last_inst = vertexlist[bb_number]->get_last_instr();

	s->prev->next = s->next;
	s->next->prev = s->prev;

	if (leader_inst == s)
	{
		vertexlist[bb_number]->set_leader_instr(s->next);
	}


}


void cfg::locate_sources(int bb_number, int exprnum, simple_reg * r, std::set<int> & visit)
{

	// To locate the source of expr we search backward in the flowgraph.
	// in each basic block we search backward for evaluation of exprnum.
	// Every flow path from entry block to block i must include at least
	// on of them. Once it's found for the first time in a flow path we stop search 
	// in that path. 

	if (visit.find(bb_number) != visit.end())
		return;
	visit.insert(bb_number);
	simple_instr * leader_inst = vertexlist[bb_number]->get_leader_instr();
	simple_instr * last_inst = vertexlist[bb_number]->get_last_instr();

	simple_instr * curr = last_inst;
	while(curr != leader_inst->prev)
	{
		if (is_expression(curr) == exprnum)
		{
			// curr --> ti = src1 op src2
			// 1) find only use of ti and replace it with with r;
			// 2) create new instruction r = ti;
			// 3) insert after curr
			simple_reg * t = curr->u.base.dst;
			replace_use_temp_with_pseudo (curr, t, r);

			// now create a new copy instruction r = ti and insert it after curr
			simple_instr *copy_instr = new_instr(CPY_OP, t->var->type);
			copy_instr->u.base.dst = r;
			copy_instr->u.base.src1 = t;
			copy_instr->u.base.src2 = NULL;

			// now insert the new instruction after curr
			copy_instr->next = curr->next;
			copy_instr->prev = curr;
			curr->next = copy_instr;
			copy_instr->next->prev = copy_instr;

			return;
		}

		curr = curr->prev;
	}


	// if exprnum is not found in this block, it implies that we need to still search backward.
	std::vector<int> pred;
	pred = vertexlist[bb_number]->get_pred();

	int pred_size = pred.size();
	int i;
	for (i = 0; i < pred_size; i++)
	{
		locate_sources(pred[i], exprnum, r, visit);
	}
}


void cfg::replace_use_temp_with_pseudo (simple_instr * s, simple_reg * t, simple_reg * r)
{
	bool replaced = FALSE;

	s = s->next;

	while (replaced == FALSE)
	{
		switch(simple_op_format(s->opcode))
		{
			case BASE_FORM:
			{
				if (s->u.base.src1 == t)
				{
					s->u.base.src1 = r;
					replaced = TRUE;
				}

				if (s->u.base.src2 == t)
				{
					s->u.base.src2 = r;
					replaced = TRUE;
				}
										
				break;	
			}

			case BJ_FORM:
			{
				if (s->u.bj.src == t)
				{
					s->u.bj.src = r;
					replaced = TRUE;
				}

				break;
			}

			case LDC_FORM:
			{
				break;
			}	

			case CALL_FORM:
			{
				if (s->u.call.proc == t)
				{
					s->u.call.proc = r;
					replaced = TRUE;
				}

				int nargs = s->u.call.nargs;
				int i;

				for (i = 0; i < nargs; i++)
				{
					if (s->u.call.args[i] == t)
					{
						s->u.call.args[i] = r;
						replaced = TRUE;
					}
				}
				break;
			}

			case MBR_FORM:
			{
				if (s->u.mbr.src == t)
				{
					s->u.mbr.src = r;
					replaced = TRUE;
				}
				break;
			}

			case LABEL_FORM:
			{
				break;
			}
		}

		s = s->next;
	}
}



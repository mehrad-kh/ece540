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


void cfg::local_cp ()
{
	// only 2 types of copy are important
	// 1) ri = rj
	// 2) ti  = rj

	int i;
	for (i = 0; i < n; i++)
	{
		local_cp_in_bb(i);
	}
}


void cfg::local_cp_in_bb (int bb_number)
{
	simple_instr * leader_inst = vertexlist[bb_number]->get_leader_instr();
	simple_instr * last_inst = vertexlist[bb_number]->get_last_instr();

	std::set<simple_instr *> ACP; // set of available copy 

	if (leader_inst == NULL)
		return;

	simple_instr * s = leader_inst;
	simple_reg * dst = NULL;

	while (s != last_inst->next)
	{
		// for each operand of an instruction check what copy reaches them
		//fprint_instr(stdout, s , 0);

		dst = NULL;
		switch(simple_op_format(s->opcode))
		{
			case BASE_FORM:
			{
				// replace operands that are copies
				s->u.base.src1 = copy_value(ACP, s->u.base.src1);
					
				s->u.base.src2 = copy_value(ACP, s->u.base.src2);
					
				dst = s->u.base.dst; 

				break;	
			}

			case BJ_FORM:
			{
				// replace operands that are copies
				s->u.bj.src = copy_value(ACP, s->u.bj.src);
				
				break;
			}

			case LDC_FORM:
			{
				dst = s->u.ldc.dst;
				break;
			}	

			case CALL_FORM:
			{
				// replace operands that are copies
				s->u.call.proc = copy_value(ACP, s->u.call.proc);
				
			
				int nargs = s->u.call.nargs;
				int i;

				for (i = 0; i < nargs; i++)
				{
					// replace operands that are copies
					s->u.call.args[i] = copy_value(ACP, s->u.call.args[i]);
				}

				dst = s->u.call.dst;
				break;
			}

			case MBR_FORM:
			{
				// replace operands that are copies
				s->u.mbr.src = copy_value(ACP, s->u.mbr.src);
									
				break;
			}

			case LABEL_FORM:
			{
				break;
			}
		}

		// delete copies from ACP that are invalidated by the current instruction
		//fprint_instr(stdout, s , 0);
		//cout << "remove?\n" ;
		remove_ACP(ACP, dst);
		//cout << "removeeeeeeeeeeeeeeeeeeeeeeee?\n" ;
		//fprint_instr(stdout, s , 0);
		if (s->opcode == CPY_OP && s->u.base.src1->kind == PSEUDO_REG)
		{
			// if is copy of ri = rj or ti = rj 
			ACP.insert(s);

		}
		//cout << "am i here ?"<<endl;
		s = s->next;
		//fprint_instr(stdout, s , 0);

	}
}



simple_reg * cfg::copy_value(std::set<simple_instr *> & ACP, simple_reg *opnd)
{
	// x = y
	// if opnd = x then if such a instruction of the form x = y exists in the 
	// ACP then return y'
	if (opnd == NO_REGISTER)
		return opnd;

	std::set<simple_instr *>::iterator it;
	for (it = ACP.begin(); it != ACP.end(); it++)
	{
		if ((*it)->u.base.dst == opnd)
		{
			if (opnd->kind == TEMP_REG)
			{
				// ti = rj copy avaialable and we are about to replace the only use
				// of ti with rj. So It's necessary to remove ti = rj from instruction list
				//cout << "Im here?" << endl;
				(*it)->prev->next = (*it)->next;
				(*it)->next->prev = (*it)->prev;
			}
			return (*it)->u.base.src1;
		}
	}
	return opnd;
}


void cfg::remove_ACP(std::set<simple_instr *> & ACP, simple_reg *opnd)
{
	if (opnd == NULL)
		return;

	std::set<simple_instr *>::iterator it;
	for (it = ACP.begin(); it != ACP.end(); it++)
	{
		if (((*it)->u.base.dst == opnd) || ((*it)->u.base.src1 == opnd))
		{
			ACP.erase(it);
		}
	}

}



void cfg::global_copy()
{
	std::vector<basicblk *>::iterator it;
	for (it = vertexlist.begin(); it != vertexlist.end(); it++)
	{
		propagate_copy_basicblk((*it));
	}

}


void cfg::propagate_copy_basicblk(basicblk *bb)
{
	// for each copy available at the beginning of this basicblk
	// propagate the copy as long as that copy is not killed by 
	// any other instruction. 

	int bb_number = bb->get_bb_number();
	simple_instr * leader_instr = bb->get_leader_instr();
	simple_instr * last_instr = bb->get_last_instr();

	int copy_num = mydfa->copy_instr_list.size();
	int i;
	for (i = 0; i < copy_num; i++)
	{
		if(mydfa->cp_ins[bb_number][i] == 1)
		{
			simple_instr * copy = mydfa->copy_instr_list[i];
			simple_instr * s = leader_instr;

			while(s != last_instr->next)
			{
				propagate_copy(s, copy);

				if (is_copy_killed(s, copy))
					break;

				s = s->next;
			}
		}
	}

}


void cfg::propagate_copy(simple_instr * s, simple_instr * copy)
{
	// copy t = r 
	simple_reg * t = copy->u.base.dst;
	simple_reg * r = copy->u.base.src1;

	switch(simple_op_format(s->opcode))
	{
		case BASE_FORM:
		{
			if (s->u.base.src1 == t)
			{
				s->u.base.src1 = r;
			}

			if (s->u.base.src2 == t)
			{
				s->u.base.src2 = r;
			}
									
			break;	
		}

		case BJ_FORM:
		{
			if (s->u.bj.src == t)
			{
				s->u.bj.src = r;
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
			}

			int nargs = s->u.call.nargs;
			int i;

			for (i = 0; i < nargs; i++)
			{
				if (s->u.call.args[i] == t)
				{
					s->u.call.args[i] = r;
				}
			}
			break;
		}

		case MBR_FORM:
		{
			if (s->u.mbr.src == t)
			{
				s->u.mbr.src = r;
			}
			break;
		}

		case LABEL_FORM:
		{
			break;
		}
	}
}


bool cfg::is_copy_killed(simple_instr * s, simple_instr * copy)
{
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
		return FALSE;

	if (dst->kind == TEMP_REG)
		return FALSE;

	if (copy->u.base.dst == dst || copy->u.base.src1 == dst)
		return TRUE;
}



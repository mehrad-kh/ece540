#include "use_def.h"
#include "dfa.h"
#include <iostream>
#include <assert.h>
#include <boost/dynamic_bitset.hpp>
#include <algorithm>
#include <stdio.h>
extern "C" { 
#include <simple.h>
}
#include <assert.h>


using std::cout;
using std::endl;


void use_def::build_use_def(cfg * mycfg_, dfa * mydfa_)
{
	mycfg = mycfg_;
	mydfa = mydfa_;

	// loop over each basic block for each instruction get its operand
	// if operand is of TEMP_REG then there is exactly one definition reaching it
	// else operand is of PSEUDO_REG then there are two cases R_i
	// case 1) 	if there is an unambiguous definition of R_i before reaching this instruction
	//			in the same basic block. Then that definition will be the only reaching definition
	//			for this use. 
	// case 2)	find RDin of the basic block the instruction belongs to. Get those definition 
	//			of R_i which are reaching beginning of this basic block. Those definition would 
	//			all reach that instruction

	//cout << "building use def\n";
	int n = mycfg->get_n();
	int i;

	for (i = 0; i < n; i++)
	{
		basicblk_use_def(i);
	}
}



void use_def::basicblk_use_def (int bb_number)
{
	basicblk * bb = mycfg->get_basicblk_addr(bb_number);

	if (bb->isEntryBlock() || bb->isExitBlock())
		return;

	simple_instr * leader_instr = bb->get_leader_instr();
	simple_instr * last_instr = bb->get_last_instr();

	simple_instr * curr_instr = leader_instr;

	while (curr_instr != last_instr->next)
	{
		identify_instr(curr_instr, bb_number);
		curr_instr = curr_instr->next;
	}
}


void use_def::identify_instr(simple_instr * s, int bb_number)
{
	// first we need to find type of the instruction
	switch(s->opcode)
	{
		//cases where both src1 and src2 are supposed to be used
		case STR_OP:
		case MCPY_OP:
		case ADD_OP:
		case SUB_OP:
		case MUL_OP:
		case DIV_OP:
		case REM_OP:
		case MOD_OP:
		case AND_OP:
		case IOR_OP:
		case XOR_OP:
		case ASR_OP:
		case LSR_OP:
		case LSL_OP:
		case ROT_OP:
		case SEQ_OP:
		case SNE_OP:
		case SL_OP:
		case SLE_OP:
		{
			if (s->u.base.src1 != NO_REGISTER && s->u.base.src1->kind == PSEUDO_REG)
				find_defs_reach_operand(s, bb_number, s->u.base.src1->num);

			if (s->u.base.src2 != NO_REGISTER && s->u.base.src2->kind == PSEUDO_REG)
				find_defs_reach_operand(s, bb_number, s->u.base.src2->num);
			return;
		}

		// cases where only one register (src1) will be used
		case LOAD_OP:
		case CPY_OP:
		case CVT_OP:
		case NEG_OP:
		case NOT_OP:
		case RET_OP: 	// src1 might be used so do a check
		{
			if (s->u.base.src1 != NO_REGISTER && s->u.base.src1->kind == PSEUDO_REG)
				find_defs_reach_operand(s, bb_number, s->u.base.src1->num);
			return;
		} 

		case BTRUE_OP:
		case BFALSE_OP:
		case MBR_OP:
		{
			if (s->u.bj.src != NO_REGISTER && s->u.bj.src->kind == PSEUDO_REG)
				find_defs_reach_operand(s, bb_number, s->u.bj.src->num);
			return;
		}

		case CALL_OP: // proc register
		{
			if (s->u.call.proc != NO_REGISTER && s->u.call.proc->kind == PSEUDO_REG)
				find_defs_reach_operand(s, bb_number, s->u.call.proc->num);

			 /* print the list of arguments */
		    int nargs = s->u.call.nargs;
		    if (nargs != 0) 
		    {
				for (int n = 0; n < nargs; n++) 
				{
				    if (s->u.call.args[n]->kind == PSEUDO_REG)
				    	find_defs_reach_operand(s, bb_number, s->u.call.args[n]->num);
				}
		    }

			return;
		}

		default: ;// do nothing

	}

}


void use_def::find_defs_reach_operand(simple_instr * s, int bb_number, int op_num)
{

	std::string key;
	std::vector<int> deflist;	

	char buffer[20];

	sprintf(buffer,"%x#%d", s, op_num);
	key.assign(buffer);

	basicblk * bb = mycfg->get_basicblk_addr(bb_number);
	simple_instr * leader_instr = bb->get_leader_instr();

	// first moving backward from s to leader instruction to check if there is 
	// any assignment to register with number equal to op_num. if there is then 
	// that is going to be the only defenition reaching operand of s.

	bool isKilled = false; 

	simple_instr * curr = s->prev;
	while (curr != leader_instr->prev && isKilled == false)
	{
		switch(simple_op_format(curr->opcode))
		{
			case BASE_FORM:
			{
				if (curr->u.base.dst != NO_REGISTER && curr->u.base.dst->kind == PSEUDO_REG && curr->u.base.dst->num == op_num)
				{
					isKilled = true;
				}
				break;
			}

			case BJ_FORM: break;

			case LDC_FORM: 
			{
				if (curr->u.ldc.dst != NO_REGISTER && curr->u.ldc.dst->kind == PSEUDO_REG && curr->u.ldc.dst->num == op_num)
				{
					isKilled = true;
				}
				break;
			}

			case CALL_FORM:
			{
				if (curr->u.call.dst != NO_REGISTER && curr->u.call.dst->kind == PSEUDO_REG && curr->u.call.dst->num == op_num)
				{
					isKilled = true;
				}
				break;
			}

			case MBR_FORM: break;

			case LABEL_FORM: break;
		}

		if (isKilled == true)
			break;

		curr = curr->prev;
	}

	// at this point if isKilled is true that implies that there is an instruction (curr points to it) in this basicblk
	// which define the variable (op_num). So this is the only definition reaching it. 

	if (isKilled == true)
	{
		// the instruction which kills a definition itself is a definition. So it exists
		// in the defnition list (instr_map_defnum).
		int defnum = -1;
		defnum = mydfa->instr_map_defnum[curr];
		assert(defnum != -1);
		deflist.push_back(defnum);
		use_def_chain[key] = deflist;
		return;
	}


	// find all definition reaching beginnig of this basicblk
	std::vector<int> reaching_defs;	// list of all def_number reaching begginig of this basic block
	std::vector<int> op_defs;	// list of all definition of the operand

	int def_size = (mydfa->rd_ins[bb_number]).size();

	int i;

	for (i = 0; i < def_size; i++)
	{
		if (mydfa->rd_ins[bb_number][i] == 1)
			reaching_defs.push_back(i);
	}

	op_defs = mydfa->var_map_defnum[op_num];

	sort(op_defs.begin(), op_defs.end());

	std::set_intersection(reaching_defs.begin(), reaching_defs.end(), op_defs.begin(), op_defs.end(), back_inserter(deflist));

	use_def_chain[key] = deflist;
	return;

}


void use_def::print_use_def_chain(char * proc_name)
{
	std::string key;
	std::vector<int> deflist;
	int i;	

	char buffer[20];

	cout << "ud chain " << proc_name << endl;
	simple_instr * s = mycfg->get_inlist();

	int counter = 0;

	while (s)
	{
			// first we need to find type of the instruction
		switch(s->opcode)
		{
			//cases where both src1 and src2 are supposed to be used
			case STR_OP:
			case MCPY_OP:
			case ADD_OP:
			case SUB_OP:
			case MUL_OP:
			case DIV_OP:
			case REM_OP:
			case MOD_OP:
			case AND_OP:
			case IOR_OP:
			case XOR_OP:
			case ASR_OP:
			case LSR_OP:
			case LSL_OP:
			case ROT_OP:
			case SEQ_OP:
			case SNE_OP:
			case SL_OP:
			case SLE_OP:
			{
				if (s->u.base.src1 != NO_REGISTER && s->u.base.src1->kind == PSEUDO_REG)
				{
					sprintf(buffer,"%x#%d", s, s->u.base.src1->num);
					key.assign(buffer);

					cout << "instr " << counter << " reach_defs:";
					deflist = use_def_chain[key];

					int deflist_size =  deflist.size();
					for (i = 0; i < deflist_size; i++)
					{
						cout << " " << deflist[i];
					}

					cout << endl;

				}

				if (s->u.base.src2 != NO_REGISTER && s->u.base.src2->kind == PSEUDO_REG)
				{
					sprintf(buffer,"%x#%d", s, s->u.base.src2->num);
					key.assign(buffer);

					cout << "instr " << counter << " reach_defs:";
					deflist = use_def_chain[key];

					int deflist_size =  deflist.size();
					for (i = 0; i < deflist_size; i++)
					{
						cout << " " << deflist[i];
					}

					cout << endl;
				}
				break;
			}

			// cases where only one register (src1) will be used
			case LOAD_OP:
			case CPY_OP:
			case CVT_OP:
			case NEG_OP:
			case NOT_OP:
			case RET_OP: 	// src1 might be used so do a check
			{
				if (s->u.base.src1 != NO_REGISTER && s->u.base.src1->kind == PSEUDO_REG)
				{
					sprintf(buffer,"%x#%d", s, s->u.base.src1->num);
					key.assign(buffer);

					cout << "instr " << counter << " reach_defs:";
					deflist = use_def_chain[key];

					int deflist_size =  deflist.size();
					for (i = 0; i < deflist_size; i++)
					{
						cout << " " << deflist[i];
					}

					cout << endl;
				}
				break;
			} 

			case BTRUE_OP:
			case BFALSE_OP:
			case MBR_OP:
			{
				if (s->u.bj.src != NO_REGISTER && s->u.bj.src->kind == PSEUDO_REG)
				{
					sprintf(buffer,"%x#%d", s, s->u.bj.src->num);
					key.assign(buffer);

					cout << "instr " << counter << " reach_defs:";
					deflist = use_def_chain[key];

					int deflist_size =  deflist.size();
					for (i = 0; i < deflist_size; i++)
					{
						cout << " " << deflist[i];
					}

					cout << endl;
				}
				break;
			}

			case CALL_OP: // proc register
			{
				if (s->u.call.proc != NO_REGISTER && s->u.call.proc->kind == PSEUDO_REG)
				{
					sprintf(buffer,"%x#%d", s, s->u.call.proc->num);
					key.assign(buffer);

					cout << "instr " << counter << " reach_defs:";
					deflist = use_def_chain[key];

					int deflist_size =  deflist.size();
					for (i = 0; i < deflist_size; i++)
					{
						cout << " " << deflist[i];
					}

					cout << endl;
				}


				int nargs = s->u.call.nargs;
		    	if (nargs != 0) 
		    	{
					for (int n = 0; n < nargs; n++) 
					{
				    	if (s->u.call.args[n]->kind == PSEUDO_REG)
				    	{
				    		sprintf(buffer,"%x#%d", s, s->u.call.args[n]->num);
							key.assign(buffer);

							cout << "instr " << counter << " reach_defs:";
							deflist = use_def_chain[key];

							int deflist_size =  deflist.size();
							for (i = 0; i < deflist_size; i++)
							{
								cout << " " << deflist[i];
							}

							cout << endl;
				    	}
					}
		    	}

				break;
			}

			default: ;// do nothing
		}

		counter++;
		s = s->next;
	}

}
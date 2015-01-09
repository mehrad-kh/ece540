#include "dfa.h"
#include <iostream>
#include <assert.h>
#include <boost/dynamic_bitset.hpp>
#include <algorithm>
#include <stdio.h>
extern "C" { 
#include <simple.h>
}


using std::cout;
using std::endl;


void dfa::reaching_definition_analyzer()
{
	number_definition();

	reaching_definition_local_analyzer();

	solve_forward (gen_rds, kill_rds, rd_ins, rd_outs, ANY_PATH);

}


void dfa::number_definition()
{
	int defnum = 0;
	int regnum; // stroes definiton of which variable
	int n = mycfg->get_n();
	simple_instr * curr;

	int i;
	for(i = 0; i < n; i++)
	{
		basicblk * bb = mycfg->get_basicblk_addr(i);
		simple_instr * leader_instr = bb->get_leader_instr();
		simple_instr * last_instr = bb->get_last_instr();

		if (leader_instr == NULL)
			continue;

		int bb_number = bb->get_bb_number();
		curr = leader_instr;

		while (curr != last_instr->next)
		{
			regnum = is_definition(curr);
			if(regnum >= 0)
			{
				instr_map_defnum[curr] = defnum;

				definition_info info;
				info.register_number = regnum;
				info.instr = curr;
				info.bb_number = bb_number;
				defnum_map_definfo[defnum] = info;

				if (var_map_defnum.find(regnum) != var_map_defnum.end())
				{
					// definition of the same variable was already exist somewhere
					std::vector<int> temp = var_map_defnum[regnum];
					temp.push_back(defnum);
					var_map_defnum[regnum] = temp;
				}

				else
				{
					// first definition of the variable encountered
					std::vector<int> temp;
					temp.push_back(defnum);
					var_map_defnum[regnum] = temp;
				}
				defnum++;
			}
			curr = curr->next;
		}
		
	}
}


int dfa::is_definition(simple_instr * s) 
{
	// given an instruction it checks if the instruction is a definition.
	// if it is then returns the register number of the variable whci gets assigned.
	switch(s->opcode)
	{
		case LOAD_OP:
		case CPY_OP:
		case CVT_OP:
		case NEG_OP:
		case ADD_OP:
		case SUB_OP:
		case MUL_OP:
		case DIV_OP:
		case REM_OP:
		case MOD_OP:
		case NOT_OP:
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
			if (s->u.base.dst != NO_REGISTER)
			{
				if (s->u.base.dst->kind ==  PSEUDO_REG)
					return s->u.base.dst->num;
			}

			return -1;
		}

		case LDC_OP:
		{
			if (s->u.ldc.dst != NO_REGISTER)
			{
				if (s->u.ldc.dst->kind ==  PSEUDO_REG)
					return s->u.ldc.dst->num;
			}
			return -1;
		}

		case CALL_OP:
		{
			if (s->u.call.dst != NO_REGISTER)
			{
				if (s->u.call.dst->kind ==  PSEUDO_REG)
					return s->u.call.dst->num;
			}
			return -1;
		}

		default:
		{
			return -1;
		}
		
	}
}


void dfa::reaching_definition_local_analyzer()
{
	int n = mycfg->get_n();

	basicblk * bb;
	int i;
	for (i = 0; i < n; i++)
	{
		bb = mycfg->get_basicblk_addr(i);
		basicblk_reaching_definition_analyzer(bb);
	}
}


void dfa::basicblk_reaching_definition_analyzer(basicblk * bb)
{
	boost::dynamic_bitset <> gen_rd;	// Gen set
	boost::dynamic_bitset <> kill_rd; 	// kill set
	boost::dynamic_bitset <> rd_in;	// in set 
	boost::dynamic_bitset <> rd_out;	// out set

	int n = instr_map_defnum.size();

	// Initialize set values
	gen_rd.resize(n, FALSE);
	kill_rd.resize(n, FALSE);
	rd_in.resize(n, FALSE);
	rd_out.resize(n, FALSE);

	find_gen_kill_rd(bb, gen_rd, kill_rd);

	gen_rds.push_back(gen_rd);
	kill_rds.push_back(kill_rd);
	rd_ins.push_back(rd_in);
	rd_outs.push_back(rd_out);

}


void dfa::find_gen_kill_rd(basicblk * bb, boost::dynamic_bitset <> & gen_rd, boost::dynamic_bitset <> & kill_rd)
{
	// perform local analysis to find gen_rd and kill_rd within a basic block
	simple_instr * leader_instr = bb->get_leader_instr();	
	simple_instr * last_instr = bb->get_last_instr();

	if (last_instr == NULL && leader_instr == NULL)
		return;

	simple_instr * s = leader_instr;
	while (s != last_instr->next)
	{
		int regnum = is_definition(s);
		if (regnum >= 0)
		{
			int definition_number = instr_map_defnum[s];
			std::vector<int> mydefs = var_map_defnum[regnum];
			int mydefs_size = mydefs.size();

			int i;
			for (i = 0; i < mydefs_size; i++)
			{
				kill_rd[mydefs[i]] = 1;
				gen_rd[mydefs[i]] = 0;
			}

			gen_rd[definition_number] = 1;
		}
		s = s->next;
	}

}


void dfa::print_reaching_definition_sets (char * proc_name)
{
	int n = mycfg->get_n(); 
	print_definition_list(proc_name);

	// function to print sets associated to reaching definitions
	int i;
	cout << "gen_rd_sets " << proc_name << " " << n << endl; 

	for (i = 0; i < n; i++)
	{
		int j;

		int gen_rd_size = gen_rds[i].size();

		cout << "gen_rd " << i << " ";

		for (j = 0; j < gen_rd_size; j++)
		{
			cout << gen_rds[i][j];
		}

		cout << endl;
	}

	cout << endl;

	cout << "kill_rd_sets " << proc_name << " " << n << endl; 

	for (i = 0; i < n; i++)
	{
		int j;
		int kill_rd_size = kill_rds[i].size();

		cout << "kill_rd " << i << " ";

		for (j = 0; j < kill_rd_size; j++)
		{
			cout << kill_rds[i][j];
		}

		cout << endl;
	}

	cout << endl;

	cout << "reaching_defs " << proc_name << " " << n << endl; 

	for (i = 0; i < n; i++)
	{
		// prints my live_out set
		int j;
		int rd_in_size = rd_ins[i].size();

		cout << "reach_defs_ins " << i << " ";

		for (j = 0; j < rd_in_size; j++)
		{
			cout << rd_ins[i][j];
		}

		cout << endl;
	}

	cout << endl;
}


void dfa::print_definition_list(char * proc_name)
{
	simple_instr *s = mycfg->get_inlist();
	int defs = instr_map_defnum.size();
	int counter = 0;

	cout << "definition " << proc_name << " " << defs << endl; 

	while (s)
	{
		if (instr_map_defnum.find(s) != instr_map_defnum.end())
		{
			int defnum = instr_map_defnum[s];
			cout << "def " << defnum << " " << counter << endl;
		}
		counter++;
		s = s->next;
	}
	//cout << "done printing" << endl;
}

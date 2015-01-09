#include "dfa.h"
#include <iostream>
#include <assert.h>
#include <boost/dynamic_bitset.hpp>
#include <algorithm>
#include <stdio.h>
extern "C" { 
#include <simple.h>
}
extern "C" { 
#include "print.h"
}

using std::cout;
using std::endl;

void dfa::available_copy_analyzer()
{

	copy_instr_list.clear();
	gen_cps.clear();		
	kill_cps.clear(); 	
	cp_ins.clear();		
	cp_outs.clear();		

	number_copy();

	available_copy_local_analyzer();

	// solve the dataflow problem to find in and out set in basic block granularity. 
	solve_forward (gen_cps, kill_cps, cp_ins, cp_outs, ALL_PATH);
}


void dfa::number_copy()
{
	simple_instr * curr = mycfg->get_inlist();
	while (curr)
	{
		if (curr->opcode == CPY_OP && 
			curr->u.base.dst->kind == PSEUDO_REG && 
			curr->u.base.src1->kind == PSEUDO_REG)
		{
			add_copy(curr);
		}
			
		curr = curr->next;
	}
}



void dfa::add_copy(simple_instr * s)
{
	std::vector<simple_instr *>::iterator it;
	for (it = copy_instr_list.begin(); it != copy_instr_list.end(); it++)
	{
		if ((*it)->u.base.dst == s->u.base.dst && (*it)->u.base.src1 == s->u.base.src1)
			return;
	}

	copy_instr_list.push_back(s);
}



void dfa::available_copy_local_analyzer()
{
	int n = mycfg->get_n();

	basicblk * bb;
	int i;
	for (i = 0; i < n; i++)
	{
		bb = mycfg->get_basicblk_addr(i);
		basicblk_available_copy_analyzer(bb);
	}
}



void dfa::basicblk_available_copy_analyzer(basicblk * bb)
{
	boost::dynamic_bitset <> gen_cp;	// Gen set
	boost::dynamic_bitset <> kill_cp; 	// kill set
	boost::dynamic_bitset <> cp_in;	// in set 
	boost::dynamic_bitset <> cp_out;	// out set

	int n = copy_instr_list.size();

	// Initialize set values
	gen_cp.resize(n, FALSE);
	kill_cp.resize(n, FALSE);
	cp_in.resize(n, FALSE);
	cp_out.resize(n, FALSE);
	//cout << "Im here\n";
	find_gen_kill_cp(bb, gen_cp, kill_cp);
	//cout <<"i end\n";

	gen_cps.push_back(gen_cp);
	kill_cps.push_back(kill_cp);
	cp_ins.push_back(cp_in);
	cp_outs.push_back(cp_out);

}



void dfa::find_gen_kill_cp(basicblk * bb, boost::dynamic_bitset <> & gen_cp, boost::dynamic_bitset <> & kill_cp)
{
	//cout << "Im here111\n";
	// perform local analysis to find gen_cp and kill_cp within a basic block
	int index;
	simple_instr * leader_instr = bb->get_leader_instr();	
	simple_instr * last_instr = bb->get_last_instr();

	if (leader_instr == NULL)
		return;

	simple_instr * curr = leader_instr;
	simple_reg * dst = NULL;
	//fprint_instr(stdout, curr , 0);
	//cout << "Im keeees\n";
	while(curr != last_instr->next)
	{
		//fprint_instr(stdout, curr , 0);
		//cout << "Im in while \n";
		dst = NULL;
		switch(simple_op_format(curr->opcode))
		{
			case BASE_FORM:
			{	
				dst = curr->u.base.dst; 
				break;	
			}

			case BJ_FORM:
			{
				
				break;
			}

			case LDC_FORM:
			{
				//cout << "i'm in ldc\n";
				dst = curr->u.ldc.dst;
				//cout << "i'm in ldc form\n";
				break;
			}	

			case CALL_FORM:
			{
				dst = curr->u.call.dst;
				break;
			}

			case MBR_FORM:
			{			
				break;
			}

			case LABEL_FORM:
			{
				break;
			}
		}
		//cout << "I get here to marke gen\n";
		mark_gen_kill_cp(gen_cp, kill_cp, dst);
		//cout << "finish marke gen\n";


		if (curr->opcode == CPY_OP && 
			curr->u.base.dst->kind == PSEUDO_REG && 
			curr->u.base.src1->kind == PSEUDO_REG)
		{
			//mark it in gen_cp
			int copynum = get_copy_number(curr);
			gen_cp[copynum] = 1;
		}

		curr = curr->next;
	}


}



void dfa::mark_gen_kill_cp(boost::dynamic_bitset <> & gen_cp, boost::dynamic_bitset <> & kill_cp, simple_reg * dst)
{
	// given a dst register find copy instruction in other blocks that have
	// one of their operands equal to dst
	if (dst == NULL)
		return;

	if (dst->kind == TEMP_REG)
		return;

	int i;
	int copy_instr_size = copy_instr_list.size();
	for (i = 0; i < copy_instr_size; i++)
	{
		if (copy_instr_list[i]->u.base.dst == dst || copy_instr_list[i]->u.base.src1 == dst)
		{
			kill_cp[i] = 1;
			gen_cp[i] = 0;
		}
	}
}


int dfa::get_copy_number(simple_instr * curr)
{
	//given a copy inst find its number in copy_instr_list
	int i;
	int copy_instr_size = copy_instr_list.size();
	for (i = 0; i < copy_instr_size; i++)
	{
		if (copy_instr_list[i]->u.base.dst == curr->u.base.dst && 
			copy_instr_list[i]->u.base.src1 == curr->u.base.src1)
		{
			return i;
		}
	}

}


void dfa::print_available_copy_sets (char * proc_name)
{
	int n = mycfg->get_n(); 

	print_copy_instr_list(proc_name);

	// function to print sets associated to live variables
	int i;
	cout << "gen_cp_sets " << proc_name << " " << n << endl; 

	for (i = 0; i < n; i++)
	{
		int j;

		int gen_cp_size = gen_cps[i].size();

		cout << "gen_cp " << i << " ";

		for (j = 0; j < gen_cp_size; j++)
		{
			cout << gen_cps[i][j];
		}

		cout << endl;
	}

	cout << endl;

	cout << "kill_cp_sets " << proc_name << " " << n << endl; 

	for (i = 0; i < n; i++)
	{
		int j;
		int kill_cp_size = kill_cps[i].size();

		cout << "kill_cp " << i << " ";

		for (j = 0; j < kill_cp_size; j++)
		{
			cout << kill_cps[i][j];
		}

		cout << endl;
	}

	cout << endl;

	cout << "avail_copies " << proc_name << " " << n << endl; 

	for (i = 0; i < n; i++)
	{
		// prints my live_out set
		int j;
		int cp_in_size = cp_ins[i].size();

		cout << "copy_in " << i << " ";

		for (j = 0; j < cp_in_size; j++)
		{
			cout << cp_ins[i][j];
		}

		cout << endl;
	}

	cout << endl;
}


void dfa::print_copy_instr_list(char * proc_name)
{
	// function to print copies in order of their appearance in copy_instr_list
	int i;
	int cps = copy_instr_list.size();
	cout << "copies " << proc_name << " " << cps << endl; ;
	
	for (i = 0; i < cps; i++)
	{
		cout << "copy " << (i+1) << ": " ;//<< variablelist[i]->num << endl;
		fprint_instr(stdout, copy_instr_list[i] , 0);
	}

	cout << endl;
}
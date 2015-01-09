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

typedef std::vector<int> int_type;


enum VAR_TYPE {SRC, DST};


void dfa::set_mycfg (cfg * mycfg_)
{
	// set mycfg pointer
	mycfg = mycfg_;
}



void dfa::solve_forward (bitset_vector & gen, bitset_vector & kill, bitset_vector & in, 
	bitset_vector & out, int path)
{
	// Algorithm to solve any/all path forward flow problems
	int i, j;
	int n = mycfg->get_n();
	int change = true;

	
	boost::dynamic_bitset <> oldout;
	int_type pred;

	while (change)
	{
		change = false;
		for (i = 1; i < n-1; i++)
		{
			
			oldout = out[i];
			pred = (mycfg->get_basicblk_addr(i))->get_pred();
			
			for (j = 0; j < pred.size(); j++)
			{
				if (path == ANY_PATH)
					in[i] = in[i] | out[pred[j]];
				else
					in[i] = in[i] & out[pred[j]];
			}
			
			out[i] = gen[i] | (in[i] - kill[i]);

			if (out[i] != oldout)
				change = true;
		}
	}

	pred = (mycfg->get_basicblk_addr(n-1))->get_pred();

	for (j = 0; j < pred.size(); j++)
	{
		if (path == ANY_PATH)
			in[n-1] = in[n-1] | out[pred[j]];
		else
			in[n-1] = in[n-1] & out[pred[j]];
	}

	out[n-1] = gen[n-1] | (in[n-1] - kill[n-1]);


}



void dfa::solve_backward (bitset_vector & gen, bitset_vector & kill, bitset_vector & in,
 bitset_vector & out, int path)
{
	// Algorithm to solve any/all path backward flow problems
	int i, j;
	int n = mycfg->get_n();
	int change = true;

	
	boost::dynamic_bitset <> oldin;
	int_type succ;

	while (change)
	{
		change = false;
		for (i = n-2; i >= 1; i--)
		{
			
			oldin = in[i];
			succ = (mycfg->get_basicblk_addr(i))->get_succ();
			
			for (j = 0; j < succ.size(); j++)
			{
				if (path == ANY_PATH)
					out[i] = out[i] | in[succ[j]];
				else
					out[i] = out[i] & in[succ[j]];
			}
			
			in[i] = gen[i] | (out[i] - kill[i]);

			if (in[i] != oldin)
				change = true;
		}
	}

	succ = (mycfg->get_basicblk_addr(0))->get_succ();

	for (j = 0; j < succ.size(); j++)
	{
		if (path == ANY_PATH)
			out[0] = out[0] | in[succ[j]];
		else
			out[0] = out[0] & in[succ[j]];
	}

}


/*****************************Live Variables************************/


void dfa::live_variable_analyzer()
{

	variablelist.clear();		
	variable_map_varnum.clear();  
	
	use_lvs.clear();		
	def_lvs.clear(); 		
	live_ins.clear();		
	live_outs.clear();
	// start of live variable analysis

	int var_num = 0;

	int n = mycfg->get_n();

	simple_instr * curr = mycfg->get_inlist();


	while (curr)
	{
		number_instr_var(curr, var_num);
		curr = curr->next;
	}

	// perfom live variable analysis locally in each basic block to find gen and kill sets
	live_variable_local_analyzer();

	// solve the dataflow problem to find in and out set in basic block granularity. 
	solve_backward (use_lvs, def_lvs, live_ins, live_outs, ANY_PATH);

}



void dfa::number_instr_var(simple_instr * s, int & var_num)
{
	//assigns a number to each variable of the instruction in inlist
	switch(s->opcode)
	{
		case LOAD_OP: 
		{
	    	add_variable(s->u.base.dst, var_num);
	    	add_variable(s->u.base.src1, var_num);
	    	break;
		}

		case STR_OP: 
		{
	   		add_variable(s->u.base.src1, var_num);
	    	add_variable(s->u.base.src2, var_num);
	    	break;
		}

		case MCPY_OP: 
		{
		    add_variable(s->u.base.src1, var_num);
		    add_variable(s->u.base.src2, var_num);
		    break;
		}

		case LDC_OP: 
		{
		    add_variable(s->u.ldc.dst, var_num);

	    	if (s->u.ldc.value.format == IMMED_SYMBOL)
	    	{
	    		if (s->u.ldc.value.u.s.symbol->kind == VAR_SYM)
	    		{
	    			//add_variable(s->u.ldc.value.u.s.symbol, var_num);
	    			;
	    		}
	    	}

	    	break; 
		}

		case JMP_OP: 
		{
		    //fprintf(fd, "\t%s\t\t%s\n",
		    //simple_op_name(s->opcode),
		    //s->u.bj.target->name); 
	   		break;
		}

		case BTRUE_OP:
		case BFALSE_OP: 
		{
	    	add_variable(s->u.bj.src, var_num);
	    	break;
		}

		case CALL_OP: 
		{
	    	unsigned n, nargs;

	    	if (s->u.call.dst != NO_REGISTER) 
	    	{
				add_variable(s->u.call.dst, var_num);
	    	}

	    	add_variable(s->u.call.proc, var_num); 

	    //print the list of arguments 
	    	nargs = s->u.call.nargs;
	    	if (nargs != 0) 
	    	{
				for (n = 0; n < nargs; n++) 
				{
		    		add_variable(s->u.call.args[n], var_num);
				}
	   		}
	    	break;
		}

		case MBR_OP: 
		{

		    add_variable(s->u.mbr.src, var_num);
		    break;
		}

		case LABEL_OP: 
		{
		    //fprintf(fd, "%s:\n", s->u.label.lab->name);
		    break; 
		}

		case RET_OP: 
		{
		 
	    	if (s->u.base.src1 != NO_REGISTER) 
	    	{
				add_variable(s->u.base.src1, var_num);
	    	}
	    	break;
		}

		case CVT_OP:
		case CPY_OP:
		case NEG_OP:
		case NOT_OP: 
		{
		    // unary base instructions 
		   
	    	add_variable(s->u.base.dst, var_num);
	    	add_variable(s->u.base.src1, var_num);
	    	break;
		}

		default:
		{
	   		// binary base instructions 
	    	assert(simple_op_format(s->opcode) == BASE_FORM);
	    	add_variable(s->u.base.dst, var_num);
	    	add_variable(s->u.base.src1, var_num);
	    	add_variable(s->u.base.src2, var_num);

		}
	}

}


void dfa::add_variable (simple_reg * r, int &var_num)
{
	// adds variable into the variablelist
	if (r->kind == PSEUDO_REG)
	{
		if (variable_map_varnum.find(r) == variable_map_varnum.end())
		{
			variable_map_varnum[r] = var_num;
			variablelist.push_back(r);
			var_num++;
		}
	}
}


void dfa::live_variable_local_analyzer()
{
	int n = mycfg->get_n();

	basicblk * bb;
	int i;
	for (i = 0; i < n; i++)
	{
		bb = mycfg->get_basicblk_addr(i);
		basicblk_live_variable_analyzer(bb);
	}
}


void dfa::basicblk_live_variable_analyzer(basicblk * bb)
{
	boost::dynamic_bitset <> use_lv;	// Gen set
	boost::dynamic_bitset <> def_lv; 	// kill set
	boost::dynamic_bitset <> live_in;	// in set 
	boost::dynamic_bitset <> live_out;	// out set

	int n = variablelist.size();

	// Initialize set values
	use_lv.resize(n, FALSE);
	def_lv.resize(n, FALSE);
	live_in.resize(n, FALSE);
	live_out.resize(n, FALSE);

	find_use_def_lv(bb, use_lv, def_lv);

	use_lvs.push_back(use_lv);
	def_lvs.push_back(def_lv);
	live_ins.push_back(live_in);
	live_outs.push_back(live_out);


}



void dfa::find_use_def_lv(basicblk * bb, boost::dynamic_bitset <> & use_lv, boost::dynamic_bitset <> & def_lv)
{
	// perform local analysis to find use_lv and def_lv within a basic block
	int index;
	simple_instr * leader_instr = bb->get_leader_instr();	
	simple_instr * last_instr = bb->get_last_instr();

	if (last_instr == NULL && leader_instr == NULL)
		return;

	simple_instr * s = last_instr;

	while (s != leader_instr->prev)
	{
		switch(s->opcode)
		{
			case LOAD_OP: 
			{
		    	add_to_use_def_lv(use_lv, def_lv, s->u.base.dst, DST);
		    	add_to_use_def_lv(use_lv, def_lv, s->u.base.src1, SRC);
		    	break;
			}

			case STR_OP: 
			{
		   		add_to_use_def_lv(use_lv, def_lv, s->u.base.src1, SRC);
	   		 	add_to_use_def_lv(use_lv, def_lv, s->u.base.src2, SRC);
	    		break;
			}

			case MCPY_OP: 
			{
			    add_to_use_def_lv(use_lv, def_lv, s->u.base.src1, SRC);
			    add_to_use_def_lv(use_lv, def_lv, s->u.base.src2, SRC);
		   	 break;
			}

			case LDC_OP: 
			{
			    add_to_use_def_lv(use_lv, def_lv, s->u.ldc.dst, DST);

	    		if (s->u.ldc.value.format == IMMED_SYMBOL)
	    		{
	    			if (s->u.ldc.value.u.s.symbol->kind == VAR_SYM)
	    			{
	    			//add_variable(s->u.ldc.value.u.s.symbol, var_num);
	    			;
	    			}
	    		}

	    		break;
			}

			case JMP_OP: 
			{
			    //fprintf(fd, "\t%s\t\t%s\n",
			    //simple_op_name(s->opcode),
			   // s->u.bj.target->name); 
	   			break;
			}

			case BTRUE_OP:
			case BFALSE_OP: 
			{
	    		add_to_use_def_lv(use_lv, def_lv, s->u.bj.src, SRC);
	    		break;
			}

			case CALL_OP: 
			{
	    		unsigned n, nargs;

	    		if (s->u.call.dst != NO_REGISTER) 
	    		{
					add_to_use_def_lv(use_lv, def_lv, s->u.call.dst, DST);
	    		}

	    		add_to_use_def_lv(use_lv, def_lv, s->u.call.proc, SRC);

	   		 // print the list of arguments 
	    		nargs = s->u.call.nargs;
	    		if (nargs != 0) 
	    		{
					for (n = 0; n < nargs; n++) 
					{
			    		add_to_use_def_lv(use_lv, def_lv, s->u.call.args[n], SRC);
					}
	   			}
	    		break;
			}

			case MBR_OP: 
			{

			    add_to_use_def_lv(use_lv, def_lv, s->u.mbr.src, SRC);
		   		break;
			}

			case LABEL_OP: 
			{
			    //fprintf(fd, "%s:\n", s->u.label.lab->name);
			    break;
			}

			case RET_OP: 
			{
		 
	    		if (s->u.base.src1 != NO_REGISTER) 
	    		{
					add_to_use_def_lv(use_lv, def_lv, s->u.base.src1, SRC);
	    		}
	    		break;
			}

			case CVT_OP:
			case CPY_OP:
			case NEG_OP:
			case NOT_OP: 
			{
			    // unary base instructions
		   
	    		add_to_use_def_lv(use_lv, def_lv, s->u.base.dst, DST);
	    		add_to_use_def_lv(use_lv, def_lv, s->u.base.src1, SRC);
	    		break;
			}

			default:
			{
		   		//binary base instructions 
		    	assert(simple_op_format(s->opcode) == BASE_FORM);
	   		 	add_to_use_def_lv(use_lv, def_lv, s->u.base.dst, DST);
	   	 		add_to_use_def_lv(use_lv, def_lv, s->u.base.src1, SRC);
	    		add_to_use_def_lv(use_lv, def_lv, s->u.base.src2, SRC);

			}
		}

		s = s->prev;
	}
}


void dfa::add_to_use_def_lv(boost::dynamic_bitset <> & use_lv, boost::dynamic_bitset <> & def_lv, simple_reg *r, int type)
{
	int var_num;

	if (r->kind == PSEUDO_REG)
	{
		var_num = variable_map_varnum[r];

		if (type == SRC)
		{
			use_lv[var_num] = 1;
		}

		else
		{
			def_lv[var_num] = 1;
			use_lv[var_num] = 0;
		}
	}
}


void dfa::print_live_variable_sets (char * proc_name)
{
	int n = mycfg->get_n(); 

	print_variable_list(proc_name);

	// function to print sets associated to live variables
	int i;
	cout << "def_lv_sets " << proc_name << " " << n << endl; 

	for (i = 0; i < n; i++)
	{
		int j;

		int def_lv_size = def_lvs[i].size();

		cout << "def_lv " << i << " ";

		for (j = 0; j < def_lv_size; j++)
		{
			cout << def_lvs[i][j];
		}

		cout << endl;
	}

	cout << endl;

	cout << "use_lv_sets " << proc_name << " " << n << endl; 

	for (i = 0; i < n; i++)
	{
		int j;
		int use_lv_size = use_lvs[i].size();

		cout << "use_lv " << i << " ";

		for (j = 0; j < use_lv_size; j++)
		{
			cout << use_lvs[i][j];
		}

		cout << endl;
	}

	cout << endl;

	cout << "live_variables " << proc_name << " " << n << endl; 

	for (i = 0; i < n; i++)
	{
		// prints my live_out set
		int j;
		int live_out_size = live_outs[i].size();

		cout << "live_out " << i << " ";

		for (j = 0; j < live_out_size; j++)
		{
			cout << live_outs[i][j];
		}

		cout << endl;
	}

	cout << endl;
}



void dfa::print_variable_list(char * proc_name)
{
	// function to print variables in order of their appearance in inlist
	int i;
	int vars = variablelist.size();
	cout << "variables " << proc_name << " " << vars << endl; ;
	
	for (i = 0; i < vars; i++)
	{
		cout << "var " << (i+1) << " r" << variablelist[i]->num << endl;
	}

	cout << endl;
}


/*****************************Avaialable Expresison************************/


void dfa::available_expression_analyzer()
{

	// whenever run data flow analyzer make sure remove previous stuff
	// from past passes
	expressionlist.clear();
	expression_map_exprnum.clear();
	operand_use.clear();

	eval_aes.clear();		//gen set
	kill_aes.clear(); 		// kill set
	ae_ins.clear();			// in set
	ae_outs.clear();

	int exprnum = 0;
	int instr_num = 0;;

	int n = mycfg->get_n();


	simple_instr * curr = mycfg->get_inlist();

	// adding expression to expressionlist
	while (curr)
	{
		number_expression(curr, exprnum, instr_num);
		instr_num ++;
		curr = curr->next;
	}

	available_expression_local_analyzer();
	solve_forward (eval_aes, kill_aes, ae_ins, ae_outs, ALL_PATH);
}



void dfa::number_expression(simple_instr * s, int & exprnum, int instr_num)
{
	switch (s->opcode)
	{
		case CVT_OP:	
		case NEG_OP:
		case NOT_OP:
		{
			add_expression(s, exprnum, instr_num, FALSE, TRUE);
			break;
		}
		
		case ADD_OP:
		case MUL_OP:
		case AND_OP:
		case IOR_OP:
		case XOR_OP:
		{
			add_expression(s, exprnum, instr_num, TRUE, FALSE);
			break;
		}
		
	
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
			add_expression(s, exprnum, instr_num, TRUE, TRUE);
			break;
		}
		
	}
}



void dfa::add_expression(simple_instr * s, int & exprnum, int instr_num,  bool isBinary, bool isOrdered)
{
	std::string key;
	char buffer[20];

	if (!isBinary)
	{
		// check if the only operand is PSEUDO_REG or not
		if(s->u.base.src1->kind == PSEUDO_REG)
		{
			sprintf(buffer,"%d#%d", s->u.base.src1->num, s->opcode);
			key.assign(buffer);
		}


	}

	else 
	{
		if (s->u.base.src1->kind == PSEUDO_REG && s->u.base.src2->kind == PSEUDO_REG)
		{
			if (isOrdered)
			{
				sprintf(buffer,"%d#%d#%d", s->u.base.src1->num, s->opcode, s->u.base.src2->num);
				key.assign(buffer);
			}

			else
			{
				int a = s->u.base.src1->num;
				int b = s->u.base.src2->num;

				/*if (a > b)
				{
					a = s->u.base.src2->num;
					b = s->u.base.src1->num;
				}*/
				sprintf(buffer,"%d#%d#%d", a, s->opcode, b);
				key.assign(buffer);
			}
		} 
	}

	if (key.size() > 0 && expression_map_exprnum.find(key) == expression_map_exprnum.end())
	{
		expression_map_exprnum[key] = exprnum;
		expressionlist.push_back(instr_num);

		operand_use[s->u.base.src1->num].push_back(exprnum);
		if (isBinary)
		{
			operand_use[s->u.base.src2->num].push_back(exprnum);
		}
		exprnum++;
	}

}


void dfa::available_expression_local_analyzer()
{
	int n = mycfg->get_n();

	basicblk * bb;
	int i;
	for (i = 0; i < n; i++)
	{
		bb = mycfg->get_basicblk_addr(i);
		basicblk_available_expression_analyzer(bb);
	}


}



void dfa::basicblk_available_expression_analyzer(basicblk * bb)
{
	boost::dynamic_bitset <> eval_ae;	// Gen set
	boost::dynamic_bitset <> kill_ae; 	// kill set
	boost::dynamic_bitset <> ae_in;	// in set 
	boost::dynamic_bitset <> ae_out;	// out set

	int n = expressionlist.size();

	// Initialize set values
	eval_ae.resize(n, FALSE);
	kill_ae.resize(n, FALSE);
	

	if (bb->isEntryBlock() || bb->isExitBlock())
	{
		
		ae_out.resize(n, FALSE);

		if (bb->isEntryBlock())
		{
			ae_in.resize(n, FALSE);
		}
		else
		{
			ae_in.resize(n, TRUE);
		}
		
	}

	else
	{
		ae_in.resize(n, TRUE);
		ae_out.resize(n, TRUE);
	}


	
	find_eval_kill_ae(bb, eval_ae, kill_ae);

	//ae_out = ae_in
	
	eval_aes.push_back(eval_ae);
	kill_aes.push_back(kill_ae);
	ae_ins.push_back(ae_in);
	ae_outs.push_back(ae_out);

}



void dfa::find_eval_kill_ae(basicblk * bb, boost::dynamic_bitset <> & eval_ae, boost::dynamic_bitset <> & kill_ae)
{
	int index;
	simple_instr * leader_instr = bb->get_leader_instr();	
	simple_instr * last_instr = bb->get_last_instr();

	if (last_instr == NULL && leader_instr == NULL)
		return;

	simple_instr * s = leader_instr;

	while (s != last_instr->next)
	{	
		switch (s->opcode)
		{
			case CVT_OP:	
			case NEG_OP:
			case NOT_OP:
			{
				add_to_eval_kill_ae(eval_ae, kill_ae, s, TRUE, FALSE);
				break;
			}
			

			case ADD_OP:
			case MUL_OP:
			case AND_OP:
			case IOR_OP:
			case XOR_OP:
			{
				add_to_eval_kill_ae(eval_ae, kill_ae, s, TRUE, TRUE);
				break;
			}
			
			

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
				add_to_eval_kill_ae(eval_ae, kill_ae, s, TRUE, TRUE);
				break;
			}

			default:
			{
				
				add_to_eval_kill_ae(eval_ae, kill_ae, s, FALSE, FALSE);
				
				break;
			}
		
		}

		s = s->next; 	// advancing to next instruction	
	}
}



void dfa::add_to_eval_kill_ae(boost::dynamic_bitset <> & eval_ae, boost::dynamic_bitset <> & kill_ae,
 simple_instr * s, bool isPotential, bool isBinary)
{
	char buffer[20];
	std::string key;
	int exprnum;

	if (isPotential)
	{
		if (isBinary)
		{
			// check if both operand are of PSEUDO_REG
			if (s->u.base.src1->kind == PSEUDO_REG && s->u.base.src2->kind == PSEUDO_REG)
			{
				// it is an expression so determine its key
				sprintf(buffer,"%d#%d#%d", s->u.base.src1->num, s->opcode, s->u.base.src2->num);
				key.assign(buffer);

				// look up its expression number from the dictionary
				exprnum = expression_map_exprnum[key];

				// make bit corresponding to expr 1
				eval_ae[exprnum] = 1; 

			}
		}

		if (!isBinary)
		{	
			// check if the only operand is of PSEUDO_REG
			if (s->u.base.src1->kind == PSEUDO_REG)
			{
				// it is an expression so determine its key
				sprintf(buffer,"%d#%d", s->u.base.src1->num, s->opcode);
				key.assign(buffer);

				// look up its expression number from the dictionary
				exprnum = expression_map_exprnum[key];

				// make bit corresponding to expr 1
				eval_ae[exprnum] = 1; 
			} 
		}
	}

	
	//find if destination register exist and if it is then check if is of PSEUDO_REG

	std::vector<int> killlist;		// expression list that will be killed by this instruction
	int dst_num = -1;

	switch (simple_op_format(s->opcode))
	{
		case BASE_FORM:
		{
			
			if (s->u.base.dst != NO_REGISTER && s->u.base.dst->kind == PSEUDO_REG)
				dst_num = s->u.base.dst->num;
			
			break;	
		}

		case LDC_FORM:
		{
			
			if (s->u.ldc.dst != NO_REGISTER && s->u.ldc.dst->kind == PSEUDO_REG)
				dst_num = s->u.ldc.dst->num;

			break;
		}

		case CALL_FORM:
		{
		
			if (s->u.call.dst != NO_REGISTER && s->u.call.dst->kind == PSEUDO_REG)
				dst_num = s->u.call.dst->num;

			break;
		}

		default:
		{
			;
		}
	}


	killlist = operand_use[dst_num];
	int i;

	int killlist_size = killlist.size();
	for (i = 0; i < killlist_size; i++)
	{
		eval_ae[killlist[i]] = 0;
		kill_ae[killlist[i]] = 1;
	}

}



void dfa::print_available_expression_sets (char * proc_name)
{
	int n = mycfg->get_n(); 

	print_expression_list(proc_name);

	// function to print sets associated to live variables
	int i;
	cout << "eval_ae_sets " << proc_name << " " << n << endl; 

	for (i = 0; i < n; i++)
	{
		int j;

		int eval_ae_size = eval_aes[i].size();

		cout << "eval_ae " << i << " ";

		for (j = 0; j < eval_ae_size; j++)
		{
			cout << eval_aes[i][j];
		}

		cout << endl;
	}

	cout << endl;

	cout << "kill_ae_sets " << proc_name << " " << n << endl; 

	for (i = 0; i < n; i++)
	{
		int j;
		int kill_ae_size = kill_aes[i].size();

		cout << "kill_ae " << i << " ";

		for (j = 0; j < kill_ae_size; j++)
		{
			cout << kill_aes[i][j];
		}

		cout << endl;
	}

	cout << endl;

	cout << "available_exprs " << proc_name << " " << n << endl; 

	for (i = 0; i < n; i++)
	{
		// prints my live_out set
		int j;
		int ae_out_size = ae_outs[i].size();

		cout << "avail_exprs_out " << i << " ";

		for (j = 0; j < ae_out_size; j++)
		{
			cout << ae_outs[i][j];
		}

		cout << endl;
	}

	cout << endl;
}



void dfa::print_expression_list(char * proc_name)
{
	// function to print expression in order of their appearance in inlist
	int i;
	int exprs = expressionlist.size();
	cout << "expressions " << proc_name << " " << exprs << endl; 
	
	for (i = 0; i < exprs; i++)
	{
		cout << "expr " << (i+1) << " " << expressionlist[i] << endl;
	}

	cout << endl;
}

#include "procedure.h"
#include <cstddef>
#include <iostream>


using std::cout;
using std::endl;

procedure::procedure ()
{
	//default constructor
	proc_name = NULL;
	inlist = NULL;
	next_proc = NULL;
	mydfa.set_mycfg(&mycfg);
}



procedure::procedure(char * proc_name_, simple_instr * inlist_)
{
	// constructor
	proc_name = proc_name_;
	inlist = inlist_;
	next_proc = NULL;
	mydfa.set_mycfg(&mycfg);
}



void procedure::set_next_proc(procedure * next_proc_) 
{
	// set my next_proc
	next_proc = next_proc_;
}



simple_instr * procedure::build_mycfg()
{
	//invokes mycfg member function to build its cfg.
	mycfg.set_my_dfa(&mydfa);
	mycfg.set_my_use_def(&my_use_def);

	//inlist = mycfg.build_cfg(inlist);
	//mycfg.process_loops();
	//mycfg.print_loops(proc_name);
	//print_proc();


	//dfa_analysis();
	//cout << "starting loop invariant vode motion\n";
	//mycfg.LICM_optimization();
	//cout << "returning licm"<< endl;
	//mycfg.global_cse ();
	//mycfg.local_cp();

	//inlist = mycfg.remove_dead_code();
	cp_pass();
	dce_pass();
	cp_pass();
	dce_pass();
	cse_pass();
	cp_pass();
	dce_pass();
	cp_pass();
	dce_pass();
	licm_pass();
	dce_pass();
	cse_pass();
	cp_pass();
	dce_pass();
	//licm_pass();
	cse_pass();
	cp_pass();
	dce_pass();
	cse_pass();
	cp_pass();
	dce_pass();
	cse_pass();
	cp_pass();
	dce_pass();
	cse_pass();
	cp_pass();
	dce_pass();
	cse_pass();
	cp_pass();
	dce_pass();
	cp_pass();
	dce_pass();
	cse_pass();
	cp_pass();
	dce_pass();






	return inlist;
}



void procedure::print_proc()
{
	//invokes function to print my cfg
	mycfg.print_cfg(proc_name);
}


void procedure::dfa_analysis()
{
	//perform live variable analysis

	//mydfa.live_variable_analyzer();
	//mydfa.print_live_variable_sets (proc_name);

	// perform dataflow analysis
	mydfa.available_expression_analyzer();
	//mydfa.print_available_expression_sets(proc_name);

	//perform reaching_definition analysis
	//cout << "start reaching definition" <<endl;
	mydfa.reaching_definition_analyzer();
	//mydfa.print_reaching_definition_sets(proc_name);

	//mydfa.available_copy_analyzer();
	//mydfa.print_available_copy_sets(proc_name);

	my_use_def.build_use_def(&mycfg, &mydfa);
	//my_use_def.print_use_def_chain(proc_name);
	//cout << "done reaching definition"<< endl;

}


void procedure::cp_pass()
{
	if (getenv("ECE540_DISABLE_CF") != 0)
	{
		return;
	}
	// local phase
	inlist = mycfg.build_cfg(inlist);
	
	mycfg.local_cp();


	// global phase
	inlist = mycfg.build_cfg(inlist);
	mydfa.available_copy_analyzer();
	mycfg.global_copy();

}


void procedure::cse_pass()
{
	if (getenv("ECE540_DISABLE_CSE") != 0)
	{
		return;
	}
	
	inlist = mycfg.build_cfg(inlist);
	mydfa.available_expression_analyzer();

	mycfg.global_cse();
}


void procedure::dce_pass()
{
	if (getenv("ECE540_DISABLE_dce") != 0)
	{
		return;
	}

	inlist = mycfg.build_cfg(inlist);
	mydfa.reaching_definition_analyzer();
	my_use_def.build_use_def(&mycfg, &mydfa);

	inlist = mycfg.remove_dead_code();

}


void procedure::licm_pass()
{
	if (getenv("ECE540_DISABLE_licm") != 0)
	{
		return;
	}
	inlist = mycfg.build_cfg(inlist);
	mycfg.process_loops();

	mydfa.reaching_definition_analyzer();
	my_use_def.build_use_def(&mycfg, &mydfa);


	mycfg.LICM_optimization();
}
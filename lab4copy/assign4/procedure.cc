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



void procedure::build_mycfg()
{
	//invokes mycfg member function to build its cfg.
	mycfg.build_cfg(inlist);
	//print_proc();
	dfa_analysis();
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
	mydfa.print_available_expression_sets(proc_name);
}
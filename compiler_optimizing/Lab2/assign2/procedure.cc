#include "procedure.h"
#include <cstddef>
#include <iostream>



procedure::procedure ()
{
	//default constructor
	proc_name = NULL;
	inlist = NULL;
	next_proc = NULL;
}



procedure::procedure(char * proc_name_, simple_instr * inlist_)
{
	// constructor
	proc_name = proc_name_;
	inlist = inlist_;
	next_proc = NULL;
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
	print_proc();
}



void procedure::print_proc()
{
	//invokes function to print my cfg
	mycfg.print_cfg(proc_name);
}
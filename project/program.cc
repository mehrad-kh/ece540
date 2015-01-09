#include "program.h"
#include <cstddef>
#include <iostream>



program::program ()
{
	beg_proc = NULL;
	end_proc = NULL;

}



simple_instr * program::add_proc(char * proc_name, simple_instr * inlist) 
{

	procedure * new_proc = new procedure(proc_name, inlist);

	if (beg_proc == NULL) 
	{
		beg_proc = new_proc;
		end_proc = new_proc;
		inlist = new_proc->build_mycfg();
		return inlist;
	} 

	end_proc->set_next_proc(new_proc);
	end_proc = new_proc;

	// Build cfg of new preocedure
	inlist = new_proc->build_mycfg();

	return inlist;
}
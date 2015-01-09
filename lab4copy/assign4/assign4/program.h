#ifndef PROGRAM_H
#define PROGRAM_H

#include "procedure.h"



class program {

	private:
		/* privare member data */
		procedure * beg_proc; // point to first procedure of my program
		procedure * end_proc; // point to last procedure of my program



	public:
		/* public member functions */
		program ();		// destructor
		void add_proc(char * proc_name, simple_instr * inlist);		// add new procedure to the list

};



#endif /* PROGRAM_H */
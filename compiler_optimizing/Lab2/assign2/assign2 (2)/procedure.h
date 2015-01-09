#ifndef PROCEDURE_H
#define PROCEDURE_H

#include "cfg.h"



class procedure {

	private:
		/* private member data */
		cfg mycfg;					// my control flow graph
		char * proc_name;			// procedure name
		simple_instr * inlist;		// points to my instuction list
		procedure * next_proc;		// points to my next procedure

		/* private member functions */
		void print_proc();



	public:
		/* public member functios */
		procedure (); 		// default constructor
		procedure(char * proc_name_, simple_instr * inlist_);	// constructor
		void set_next_proc(procedure * next_proc_);
		void build_mycfg();

};



#endif /* PROCEDURE_H */
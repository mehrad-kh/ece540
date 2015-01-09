#ifndef PROCEDURE_H
#define PROCEDURE_H

#include "cfg.h"
#include "dfa.h"
#include "use_def.h"



class procedure {

	private:
		/* private member data */
		cfg mycfg;					// my control flow graph
		dfa mydfa;					// my dfa solver
		use_def my_use_def;			// my use_def
		char * proc_name;			// procedure name
		simple_instr * inlist;		// points to my instuction list
		procedure * next_proc;		// points to my next procedure

		/* private member functions */
		void print_proc();
		
		void dfa_analysis();



	public:
		/* public member functios */
		procedure (); 		// default constructor
		procedure(char * proc_name_, simple_instr * inlist_);	// constructor
		
		simple_instr * build_mycfg();
		void set_next_proc(procedure * next_proc_);

		void cp_pass();
		void cse_pass();
		void dce_pass();
		void licm_pass();

		

};



#endif /* PROCEDURE_H */
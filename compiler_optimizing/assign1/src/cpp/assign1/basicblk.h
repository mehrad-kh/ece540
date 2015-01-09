#ifndef BASICBLK_H
#define BASICBLK_H

#include "cfg.h"
extern "C" { 
#include <simple.h>
}
#include <vector>


typedef std::vector<int> int_type;


class cfg; // Say foo exists without defining it.



class basicblk {

	private:
		/* private member data */
		cfg * graph;					// points to my graph

		simple_instr * leader_instr;	//points to my first instruction
		simple_instr * last_instr;		// points to my last instruction

		int bb_number;					// holds my basic block number starts from 0;

		int leader_instr_number; 		// holds my fist instruction number associated to the one in the inslist
		int last_instr_number; 

		bool isEntry;		// set to 1 if I am the entry block
		bool isExit;		// set to 1 if I am the exit block

		int_type succ;		// list of my successors 
		int_type pred;		// list of my predecessors
		int_type dom;		// list of my dominators

		int idom;			// my immidiate dominator

		/* private member functions */
		void print_instr();
		void print_succ();
		void print_pred();

		

	public:
		/* public member functions */		
		basicblk(cfg * graph_, simple_instr * leader_instr_, simple_instr * last_instr_,int bb_number_, 
			int leader_instr_number_,int last_instr_number_, bool isEntry_, bool isExit_);	// constructor

		void establish_succ_pred();
		void add_pred(int pred_num);
		void print_basicblk();
		void print_idom();		
		int_type get_pred();
		void initialize_dom();	
		int_type get_dom();
		void set_dom(int_type dom_);
		void find_idom();
		bool is_dom_me(int v);
		bool isReachable();

	
};



#endif /* BASICBLK_H */
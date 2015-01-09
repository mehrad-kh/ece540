#ifndef NATLOOP_H
#define NATLOOP_H

#include "basicblk.h"
#include "cfg.h"
#include <vector>
#include <set>
#include <tr1/unordered_map>
#include <stack>
extern "C" { 
#include <simple.h>
}
#include "use_def.h"
#include "dfa.h"


class cfg;
class basicblk;
class dfa;
class use_def;

struct invar_status
{
	int bb_number;
	simple_instr * dep_1;
	simple_instr * dep_2;
	bool movable;
};

class natloop {

	public:

		/* public member data */
		cfg * graph;		// points to my graph

		dfa * mydfa;

		use_def * my_use_def;

		basicblk * head;	// points to head block of my loop

		std::vector<basicblk *> tails;		// list of pointers to my tails

		int loop_number;			// my loop number

		std::vector<int> loop_blocks;		// list of loop's nodes bb_number

		std::vector<int> loop_blocks_bfs_order;  // list of loop's nodes bb_number in BFS order

		std::set<int> exit_blocks;

		std::set<int> dom_exits; // set of all basic block numbers that dominates exits of the loop 

		std::vector<simple_instr *> invar_order;

		std::tr1::unordered_map<simple_instr *, struct invar_status> invar_map_invarstat; 

		std::vector<simple_instr *> movable_instr;


		/* private member function */
		void print_backedge();			// prints back edges
		void print_preheader();			// prints preheader
		void print_loop_blocks();		// print nodes constituting this natloop

		void insert_on_stack(std::set<int> & loop_blocks_set, std::stack<int> & S, int a);



	public:
		/* private member function */
		natloop(cfg * graph_, int head_, std::vector<int> & tails_, int loop_number_);	// constructor

		void find_loop_blocks();	// find all nodes constituting my loop

		void print_natloop();		// print loop

		void BFS();

		void find_exit_blocks();

		void find_dom_exit_blocks();

		void LICM(dfa * mydfa_, use_def * my_use_def_);

		void mark_invar();

		void mark_invar_block(int bb_number);

		void mark_invar_inst(simple_instr * s, int bb_number);

		bool reach_defs_out(simple_instr * s, simple_reg * opnd);

		simple_instr * reach_defs_in(simple_instr * s, simple_reg * opnd);

		void mark_movable();  // mark movable the blocks which are movable

		bool is_dom_exits(int invar_bb_number);

		bool isMovable(simple_instr * s);

		bool is_one_def(simple_reg * dst);	// check if there is exactly one definition of dst in the loop

		int get_number_def(int bb_number, int op_num);

		bool only_def_reach(simple_instr * s, simple_reg * dst);

		bool only_def_reach_helper(int bb_number, int def_num, int op_num);

		void move_instr();

		simple_instr * make_dst_pseudo(simple_instr * s, simple_instr * s_next);

		void change_dst_reg(simple_instr * s, simple_reg * new_dst);

		bool is_my_use_movable (simple_instr * s, simple_reg * r, simple_instr * s_next);


};



#endif /* NATLOOP_H */

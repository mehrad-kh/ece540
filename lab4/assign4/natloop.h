#ifndef NATLOOP_H
#define NATLOOP_H

#include "basicblk.h"
#include "cfg.h"
#include <vector>
#include <set>
#include <stack>


class cfg;
class basicblk;

class natloop {

	private:

		/* private member data */
		cfg * graph;		// points to my graph

		basicblk * head;	// points to head block of my loop

		std::vector<basicblk *> tails;		// list of pointers to my tails

		int loop_number;			// my loop number

		std::vector<int> loop_blocks;		// list of loop's nodes bb_number


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
};



#endif /* NATLOOP_H */

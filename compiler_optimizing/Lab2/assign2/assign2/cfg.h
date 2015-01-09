#ifndef CFG_H
#define CFG_H

#include <vector>
#include <set>
#include <tr1/unordered_map>
#include "basicblk.h"
#include "natloop.h"
extern "C" { 
#include <simple.h>
}


class basicblk;
class natloop;


typedef std::vector<int> int_type;
typedef std::vector<basicblk *> container_type;


class cfg {

	private:

		/* private member data */
		simple_instr * inlist;
		
		container_type vertexlist; 	// list of pointers to basicblk
		// need a hash map for mapping symbole to basic block number 
		std::tr1::unordered_map<simple_sym *, int> lab_map_bbnum; 	

		std::vector<natloop *> looplist;	// list of pointers to my loops
		int n; // number of vertices 

		unsigned int natloop_num;		// number natural loops in cfg



		/* public member data */
		int_type set_intersection(int_type set1, int_type set2);

		// loop processing related function
		void process_loops();
		void DFS (std::tr1::unordered_map<int, int_type> & back_edge);
		void DFS_visit(std::tr1::unordered_map<int, int_type> & back_edge, std::set<int> & parent, int v);
		void insert_instruction(int_type & loop_heads, std::tr1::unordered_map<int, int_type> & back_edge );


	public:
		/* public member functions */
		void build_cfg (simple_instr * inlist_);
		void build_cfg_helper();

		void add_pred_to_succ(int pred_num, int succ_num);		
		int get_bb_number(simple_sym * lab);  // Return bb_number corresponding to the label
		int get_n();	// Returns number of vertices in the graphas
		basicblk * get_basicblk_addr (int i);
		void set_edges();
		void print_cfg(char * proc_name);
		void find_doms();  // iterative algorithm to find dominators of every vertex
		bool is_v1_dom_v2(int v1, int v2);		// returns true if v1 dom v2

};



#endif /* CFG_H */
#ifndef CFG_H
#define CFG_H

#include <vector>
#include <set>
#include <tr1/unordered_map>
#include "basicblk.h"
#include "natloop.h"
#include <boost/dynamic_bitset.hpp>
extern "C" { 
#include <simple.h>
}


class basicblk;
class natloop;
class dfa;
class use_def;


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

		std::vector<simple_reg *> variablelist;

		// dictionary mapping variable address to variable number	
		std::tr1::unordered_map<simple_reg *, int> variable_map_varnum;  

		/* private member data */
		void set_edges();
		int_type set_intersection(int_type set1, int_type set2);
		

		// loop processing related function
		
		void DFS (std::tr1::unordered_map<int, int_type> & back_edge);
		void DFS_visit(std::tr1::unordered_map<int, int_type> & back_edge, std::set<int> & parent, int v);
		void insert_instruction(int_type & loop_heads, std::tr1::unordered_map<int, int_type> & back_edge );



	public:
		/* public member functions */
		simple_instr * build_cfg (simple_instr * inlist_);
		void build_cfg_helper();

		void add_pred_to_succ(int pred_num, int succ_num);		
		int get_bb_number(simple_sym * lab);  // Return bb_number corresponding to the label
		int get_n();	// Returns number of vertices in the graph
		basicblk * get_basicblk_addr (int i);
		
		void print_cfg(char * proc_name);
		void find_doms();  // iterative algorithm to find dominators of every vertex
		bool is_v1_dom_v2(int v1, int v2);		// returns true if v1 dom v2

		//int get_var_num(simple_reg * r);  // return var_num correponding to a variable 
		simple_instr * get_inlist();

		void process_loops();
		void print_loops(char * proc_name);

		void LICM_optimization (dfa * mydfa_, use_def * my_use_def_);

		static bool way_to_sort(natloop * i, natloop * j);

};



#endif /* CFG_H */
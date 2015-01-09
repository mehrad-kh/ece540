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

struct worklist_item {
public:
    basicblk *bb;
    simple_instr *instr;

    worklist_item(basicblk *bb_, simple_instr *instr_) 
    {
  		bb = bb_;
  		instr = instr_;
    }  
};

typedef std::vector<worklist_item> worklist;


class cfg {

	private:
		/* private member data */
		simple_instr * inlist;
		
		container_type vertexlist; 	// list of pointers to basicblk

		dfa * mydfa; // pointer to my data flow graph

		use_def * my_use_def; // my use_def chain

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

		void set_my_dfa(dfa * mydfa_);

		void set_my_use_def(use_def * my_use_def_);

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

		void LICM_optimization ();

		void global_cse ();
		void find_expr (int bb_number);
		int is_expression(simple_instr *s);
		bool is_global_common_subexpression(int bb_number, int exprnum, simple_instr * s, simple_instr * leader_inst);
		bool is_local_common_subexpression(simple_instr *s, std::vector<simple_instr *> &AVE, int bb_number);
		void kill_AVE(std::vector<simple_instr *> &AVE, simple_instr * s);
		void remove_common_subexpr_with_temp_reg(int bb_number, simple_instr  * s);
		void locate_sources(int bb_number, int exprnum, simple_reg * r, std::set<int> & visit);
		void replace_use_temp_with_pseudo (simple_instr * s, simple_reg * t, simple_reg * r);



		static bool way_to_sort(natloop * i, natloop * j);

		void local_cp ();
		void local_cp_in_bb (int bb_number);
		simple_reg * copy_value(std::set<simple_instr *> & ACP, simple_reg *opnd);
		void remove_ACP(std::set<simple_instr *> & ACP, simple_reg *opnd);

		void global_copy();
		void propagate_copy_basicblk(basicblk *bb);
		void propagate_copy(simple_instr * s, simple_instr * copy);
		bool is_copy_killed(simple_instr * s, simple_instr * copy);


		// dead code elimination
		simple_instr * remove_dead_code();
		void mark_nop_unreachable_block();
		void worklist_initial_essential_instr(basicblk *bb, worklist &wl); 
		void mark_label_initial_essential_instr(std::set<simple_instr *> & essential_instr);
		void perform_dce();
		void instr_usage_reaching_def(simple_instr * s, basicblk * bb, std::vector<worklist_item> &wl);
		void opnd_reaching_def(simple_instr * s, basicblk * bb, simple_reg *opnd,
 			std::vector<worklist_item> &wl);
		void nop_out_non_essentials_instr(std::set<simple_instr *> & essential_instr);
		void remove_jmps();
		void remove_nops();  





};



#endif /* CFG_H */
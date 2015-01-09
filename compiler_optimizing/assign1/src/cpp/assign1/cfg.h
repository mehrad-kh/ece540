#ifndef CFG_H
#define CFG_H

#include <vector>
#include <tr1/unordered_map>
#include "basicblk.h"
extern "C" { 
#include <simple.h>
}


class basicblk;


typedef std::vector<int> int_type;
typedef std::vector<basicblk *> container_type;



class cfg {

	private:
		/* private member data */
		container_type vertexlist; 	// list of pointers to basicblk
		// need a hash map for mapping symbole to basic block number 
		std::tr1::unordered_map<simple_sym *, int> lab_map_bbnum; 	
		int n; // number of vertices 

		/* public member data */
		int_type set_intersection(int_type set1, int_type set2);



	public:
		/* public member functions */
		void build_cfg (simple_instr * inlist);
		void add_pred_to_succ(int pred_num, int succ_num);		
		int get_bb_number(simple_sym * lab);  // Return bb_number corresponding to the label
		int get_n();	// Returns number of vertices in the graph
		void set_edges();
		void print_cfg(char * proc_name);
		void find_doms();  // iterative algorithm to find dominators of every vertex
		bool is_v1_dom_v2(int v1, int v2);

};



#endif /* CFG_H */
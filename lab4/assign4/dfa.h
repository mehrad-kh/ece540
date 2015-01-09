#ifndef DFA_H
#define DFA_H

#include <vector>
#include <set>
#include <tr1/unordered_map>
#include <string>
#include <boost/dynamic_bitset.hpp>
#include "basicblk.h"
#include "cfg.h"

typedef std::vector < boost::dynamic_bitset <> > bitset_vector;
typedef std::vector<basicblk *> container_type;

enum path {ANY_PATH, ALL_PATH};

enum direction {FORWARD, BACKWARD};

class cfg;

class dfa
{
	private:	
		cfg * mycfg; 	// pointer to mycfg

		void solve_forward (bitset_vector & gen,
		 bitset_vector & kill, bitset_vector & in, bitset_vector & out, int path);

		void solve_backward (bitset_vector & gen,
		 bitset_vector & kill, bitset_vector & in, bitset_vector & out, int path);


		/* data structres used for live variable analysis */
		std::vector<simple_reg *> variablelist;

		// dictionary mapping variable address to variable number	
		std::tr1::unordered_map<simple_reg *, int> variable_map_varnum;  

		// live variable vector sets
		bitset_vector use_lvs;		// gen set
		bitset_vector def_lvs; 		// kill set
		bitset_vector live_ins;		// in set
		bitset_vector live_outs;	// out set

		
		/* data structres used for available expressions analysis */
		std::vector<int> expressionlist;

		// dictionary mapping expressions to expression number	
		std::tr1::unordered_map<std::string, int> expression_map_exprnum; 

		std::tr1::unordered_map<int, std::vector<int> > operand_use; 

		// available expression vector sets
		bitset_vector eval_aes;		//gen set
		bitset_vector kill_aes; 	// kill set
		bitset_vector ae_ins;		// in set
		bitset_vector ae_outs;		// out set


		/* live variable routines */
		void number_instr_var(simple_instr * s, int & var_num);
		void add_variable (simple_reg * r, int &var_num);

		void live_variable_local_analyzer();
		void basicblk_live_variable_analyzer(basicblk * bb);

		void find_use_def_lv(basicblk * bb, boost::dynamic_bitset <> & use_lv, boost::dynamic_bitset <> & def_lv);
		void add_to_use_def_lv(boost::dynamic_bitset <> & use_lv, boost::dynamic_bitset <> & def_lv, simple_reg *r, int type);

		void print_variable_list(char * proc_name);


		/* available expression routines */
		void number_expression(simple_instr * s, int & exprnum, int  instr_num);
		void add_expression(simple_instr * s, int & exprnum, int  instr_num, bool isBinary, bool isOrdered);

		void available_expression_local_analyzer();
		void basicblk_available_expression_analyzer(basicblk * bb);


		void find_eval_kill_ae(basicblk * bb, boost::dynamic_bitset <> & eval_ae, boost::dynamic_bitset <> & kill_ae);
		void add_to_eval_kill_ae(boost::dynamic_bitset <> & eval_ae, boost::dynamic_bitset <> & kill_ae, simple_instr * s, bool isPotential, bool isBinary);


		void print_expression_list(char * proc_name);



	public:
		void set_mycfg (cfg * mycfg_);

		void live_variable_analyzer();

		void print_live_variable_sets (char * proc_name);

		void available_expression_analyzer();

		void print_available_expression_sets (char * proc_name);
};



#endif /* DFA_H */
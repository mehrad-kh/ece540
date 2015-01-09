#ifndef USE_DEF_H
#define USE_DEF_H

extern "C" { 
#include <simple.h>
}
#include <vector>
#include <set>
#include <tr1/unordered_map>
#include <string>
#include <boost/dynamic_bitset.hpp>
#include "basicblk.h"
#include "cfg.h"
#include "dfa.h"

class cfg;
class dfa;
class use_def{

	public:
		cfg *mycfg;
		dfa *mydfa;

		// key : instruction addrees+operand number.
		std::tr1::unordered_map<std::string, std::vector<int> > use_def_chain; 


		void build_use_def(cfg * mycfg_, dfa * mydfa_);

		void basicblk_use_def (int bb_number);

		void identify_instr(simple_instr * s, int bb_number);

		void find_defs_reach_operand(simple_instr * s, int bb_number, int op_num);

		void print_use_def_chain(char * proc_name);
};


#endif /* USE_DEF_H*/

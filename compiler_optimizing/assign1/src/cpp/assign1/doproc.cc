#include <stdio.h>
#include "program.h"
#include <iostream>

#include<vector>
//#include<hash_map>
// data structures you should consider using are vector and hash_map from the STL
// refer to the following link as a starting point if you are not familiar with them: 
// http://www.sgi.com/tech/stl/Vector.html
// http://www.sgi.com/tech/stl/hash_map.html 
program prog;

simple_instr* do_procedure (simple_instr *inlist, char *proc_name)
{
	prog.add_proc(proc_name, inlist);

    return inlist;
}

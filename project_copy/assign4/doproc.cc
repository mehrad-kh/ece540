#include <stdio.h>
#include "program.h"
#include <iostream>
extern "C" { 
#include "print.h"
}

#include<vector>
//#include<hash_map>
// data structures you should consider using are vector and hash_map from the STL
// refer to the following link as a starting point if you are not familiar with them: 
// http://www.sgi.com/tech/stl/Vector.html
// http://www.sgi.com/tech/stl/hash_map.html 
program prog;

simple_instr* do_procedure (simple_instr *inlist, char *proc_name)
{
	inlist = prog.add_proc(proc_name, inlist);

	printf("\nProcedure %s:\n", proc_name);
    int counter = 0;

    simple_instr *i = inlist;
    while (i) {
	fprint_instr(stdout, i, counter);
	i = i->next;
    counter++;
    }

    return inlist;
}

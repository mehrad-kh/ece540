/*  Print Out a Procedure */

/*  Copyright (c) 1994 Stanford University

    All rights reserved.

    This software is provided under the terms described in
    the "suif_copyright.h" include file. */

#include <suif_copyright.h>

#include <stdio.h>
#include <simple.h>
#include "print.h"



/*  Read through the list of instructions and print them out.  */

simple_instr *
do_procedure (simple_instr *inlist, char *proc_name)
{
    simple_instr *i;

    printf("\nProcedure %s:\n", proc_name);
    int counter = 0;

    i = inlist;
    while (i) {
	fprint_instr(stdout, i, counter);
	i = i->next;
    counter++;
    }

    return inlist;
}




/*  Simple Compiler Pass Main Program */

/*  Copyright (c) 1994 Stanford University

    All rights reserved.

    This software is provided under the terms described in
    the "suif_copyright.h" include file. */

#include <suif_copyright.h>

#include <stdlib.h>
#include <stdio.h>


extern void init_suif(int& argc, char * argv[]);
extern void compile(char *infile, char *outfile);
static void usage(char *progname);

/*
 *  The following variables are normally defined in a separate file created
 *  by the standard SUIF makefiles.  However, the stand-alone makefile does
 *  not support that.  Instead, it sets the DEF_VERSION macro so that the
 *  variables will be defined here.
 */

#ifdef DEF_VERSION
char *prog_ver_string = "(unnumbered version)";
char *prog_who_string = "compiled at " __TIME__ " on " __DATE__ ;
char *prog_suif_string = "(unnumbered version)";
#endif /* DEF_VERSION */


int
main (int argc, char *argv[])
{
    char *infile, *outfile;

    init_suif(argc, argv);

    /* read the command line arguments */
    if ((argc < 2) || (argc > 3)) usage(argv[0]);
    infile = argv[1];
    if (argc > 2) {
	outfile = argv[2];
    } else {
        /*If no outfile, write to bit bucket instead*/
        /*Using NULL triggers an assert failure in Simple-SUIF*/
        outfile = "/dev/null";
    }

    compile(infile, outfile);
    return 0;
}



void
usage (char *progname)
{
    fprintf(stderr, "Usage: %s infile outfile\n", progname);
    exit(-1);
}




/*  Label Hash Table Implementation */

#include <stdio.h>
#include <stdlib.h>
#include <simple.h>
#include "labeltab.h"



/*  local function prototypes */
static int hash(simple_sym *s);



/*  global variables */
struct labeltab_entry *labeltab[LABELTAB_SIZE];



/*  Initialize the label hash table.  */

void
init_labeltab ()
{
    int i;

    for (i = 0; i < LABELTAB_SIZE; i++) {
	labeltab[i] = NULL;
    }
}



/*  Enter a label into the hash table along with the corresponding basic
    block number.  Return FALSE if the label was already in the table.  */

boolean
enter_label (simple_sym *label, int blknum)
{
    struct labeltab_entry *entry;
    int ndx = hash(label);

    /* check for duplicate labels */
    if (lookup_label(label) >= 0) return FALSE;

    /* create a new entry for the hash table */
    entry = (struct labeltab_entry *)malloc(sizeof(struct labeltab_entry));
    entry->label = label;
    entry->blknum = blknum;
    entry->next = labeltab[ndx];
    labeltab[ndx] = entry;

    return TRUE;
}



/*  Lookup a label in the hash table.  Return the corresponding basic block
    number or -1 if the label was not found.  */

int
lookup_label (simple_sym *label)
{
    int ndx = hash(label);
    struct labeltab_entry *e = labeltab[ndx];

    while (e) {
	if (e->label == label) return e->blknum;
	e = e->next;
    }

    return -1;
}



/*  Clear out the label hash table.  */

void
empty_labeltab ()
{
    int i;
    struct labeltab_entry *e, *tmp;

    for (i = 0; i < LABELTAB_SIZE; i++) {
	e = labeltab[i];
	while (e) {
	    tmp = e;
	    e = e->next;
	    free(tmp);
	}
	labeltab[i] = NULL;
    }
}



int
hash (simple_sym *s)
{
    return ((int)s >> 2) % LABELTAB_SIZE;
}




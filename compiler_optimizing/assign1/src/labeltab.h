/*  Definitions for a Hash Table to Record Labels */

#ifndef LABELTAB_H
#define LABELTAB_H

#include <simple.h>

struct labeltab_entry {
    simple_sym *label;
    int blknum;
    struct labeltab_entry *next;
};

#define LABELTAB_SIZE 100
extern struct labeltab_entry *labeltab[];

extern void init_labeltab();
extern boolean enter_label(simple_sym *label, int blknum);
extern int lookup_label(simple_sym *label);
extern void empty_labeltab();

#endif /* LABELTAB_H */

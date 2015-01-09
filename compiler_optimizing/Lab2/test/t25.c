#include <stdlib.h>
#include <stdio.h>

int loop_label_first_instruction_2nd (int a)
{
L1:
  printf("*");
  if (a < 6)
	goto Lend;
  goto L1;

Lend:
  return a;
}


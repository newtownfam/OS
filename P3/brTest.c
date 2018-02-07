/* Bathroom.c tester */

#include "Bathroom.h"
#include <stdlib.h>
#include <stdio.h>

int main()
{
	struct brGlobal = {-1, 0, 0, 0, 0, 0};

	int male = 1;
	int female = 0;

	enter(brGlobal, male);
	printf("GF: %d, MC: %d, FC: %d, TU, %d\n", brGlobal->gender, brGlobal->mCount, brGlobal->fCount, brGlobal->totalUsages);

	enter(brGlobal, female);
	printf("GF: %d, MC: %d, FC: %d, TU, %d\n", brGlobal->gender, brGlobal->mCount, brGlobal->fCount, brGlobal->totalUsages);

	leave(brGlobal, male);
	printf("GF: %d, MC: %d, FC: %d, TU, %d\n", brGlobal->gender, brGlobal->mCount, brGlobal->fCount, brGlobal->totalUsages);

}
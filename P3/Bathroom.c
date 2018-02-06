/* Bathroom.c
 * Andrew Morrison
 * Peter Christakos */

#include "Bathroom.h"

struct brGlobal
{
	int gender; // -1 for vacant, 0 for female, and 1 for male
	int mCount;
	int fCount;
	int totalUsages;
	long vacantTime;
	long occupiedTime;
	//int averageQL;
	//int avePeople;
};

/* enter
 * g is the gender of the thread
 * compare g with global bathroom gender
 * if brGlobal is vacant, then enter, set flag, increment counters, return 1
 * if g == brGlobal, then enter, increment counters, return 1
 * if g != brGlobal, return 0
 * if gender is invalid, return -1
 */
int enter(int g) // needs lockageeeeee
{
	if(brGlobal.gender == -1) // check if the bathroom is vacant, if so enter
	{
		brGlobal->gender = g; // set gender flag
		assert(mCount == 0 && fCount == 0);
		brGLobal->totalUsages++;
		switch(g)
		{
			case 0:
				brGlobal->fCount++; // increment female count
				break;
			case 1:
				brGlobal->mCount++; // increment male count
				break;
			default:
				return -1; // invalid gender
				break;
		}
		return 1; // success
	}
	if(brGlobal.gender == g) // correct gender so enter
	{
		brGlobal->totalUsages++;
		switch(g)
		{
			case 0:
				assert(mCount == 0);
				brGlobal->fCount++; // increment female count
				break;
			case 1:
				assert(fCount == 0);
				brGlobal->mCount++; // increment male count
				break;
			default:
				return -1; // invalid gender
				break;
		}
		return 1; // success
	}
	else // would be inappropriate to enter at this time, begin waiting
	{
		return 0; // failure
	}
}

/* leave
 * if global bathroom is flagged as female then decrement the female counter 
 *  and check if bathroom is vacant, if so then set the flag to vacant
 * same idea for male
 */
void leave() // needs lockaging
{
	if(brGlobal.gender == 0) // if its a female
	{
		assert(brGlobal.mCount == 0);
		brGlobal.fCount--; // decrement females
		if(brGlobal.fCount == 0) // if last female to leave
		{
			brGlobal.gender == -1; // set flag to vacant
		}
	}
	if(brGlobal.gender == 1) // if its a male
	{
		assert(brGLobal.fCount == 0);
		brGlobal.mCount--; // decrement males
		if(brGlobal.mCount == 0) // if last male to leave
		{
			brGlobal.gender == -1; // set flag to vacant
		}
	}
}

void initialize()
{

}

void finalize()
{

}
/* Bathroom.h
 * Andrew Morrison
 * Peter Christakos
 * Enter(gender G) 
 *		- check for gender flag
 *		- enter if vacant or same gender
 *		- increment people in bathroom
 *		- wait if not
 *	Leave(void)
 *		- decrement the people in bathroom
 *		- set flag to vacant if last person
 *	Global bathroom object (PROTECT FROM RACE CONDITIONS)
 *		- counter for people in bathroom (both men and women)
 *		- gender flag or vacant
 *		- use synchronization primitives
 *	 	- holds poeple in bathroom
 * 		- holds bathroom flag
 *		- # of usages of bathroom
 *		- total time bathroom vacant
 *		- total time occupied
 *		- average queue length
 * 		- Average # of people in the bathroom at the same time
 *	Initialize(...)
 *		- called by master thread only
 *	Finalize(...)
 *		- after all threads have been called
 *		- print out statistics gathered by global bathroom object */

#ifndef BATHROOM_SEEN
#define BATHROOM_SEEN

struct br();
struct br * brGlobal;
int enter(struct brGlobal, int g);
void leave();
void initialize();
void finalize();

#endif /* BATHROOM_SEEN */
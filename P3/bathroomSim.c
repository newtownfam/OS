/* bathroomSim
 * Andrew Morrison
 * Peter Christakos
 * 	takes in - bathroomSim nUsers meanLoopCount meanArrival meanStay
 *	use Box-Muller transform to create random numvers with a normal distribution
 * 	
 *	Calls Initialize
 *	Uses pthread_create to create all threads
 *	Waits until all threads have finished
 *	Calls finalized
 *
 *	Individual(Gender, arrival time, time to stay, loop count) - used to implement the threads
 * 		Gender - defined by random variable
 *		loop count - random based on the mean specified by the command line
 *		arrival and stay - random
 *			sleeps until arrival time
 *			invoke enter() - may have to wait a long time
 *			sleeps stay amount of time
 *			invokes leave
 *			repeat loop time amount of times
 *			each thread keeps track of how long it is in the bathroom
 *			after exiting thread prints:
 *				Its own thread number
 *				Its gender and number of loops
 *				The min average and max time spent in the queue */

#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include "Bathroom.h"

/* individual will create each thread individually, calling pthread_create to do so
 * It will create the thread with the function parameter specifications such as:
 *	g - a random num 0 or 1 that defines the gender of the thread
 *	arrival - the arrival time of the thread
 *	stay - the amount of time the thread will stay in the bathroom
 *	lCount - the number of times the thread will loop through the bathroom
 */
void individual(int g, long arrival, long stay, int lCount)
{

}

int main(int argc, char* argv[])
{
	assert(argc == 5);
}
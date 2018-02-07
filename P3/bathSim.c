/* bathSim
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

// RANDOM FUNCTIONS

//use this for loop count, based on mean from command line
int looprand(int meanLoopCount){
  int rangeLow, rangeHigh, mean, stdDev randNum;
  mean = meanArrival;
  stdDev = mean/2;
  rangeLow = mean - (3*stdDev);
  rangeHigh = mean + (3*stdDev);
  if (rangeLow < 0){
    rangeLow = 0;
  }
  randNum = (sqrt(-2 * log(rangeLow))) * cos(2*pi*rangeHigh);
  printf("random number generated with looprand: %d", randNum);
  //randNum = (rangeHigh * drand48());
  return randNum;
}

int arrivalrand(int meanArrival){
  int rangeLow, rangeHigh, mean, stdDev;
  mean = meanArrival;
  stdDev = mean/2;
  rangeLow = mean - (3*stdDev);
  rangeHigh = mean + (3*stdDev);
  if (rangeLow < 0){
    rangeLow = 0;
  }
  useconds_t randNum;
  randNum = (sqrt(-2 * log(rangeLow))) * cos(2*pi*rangeHigh);
  printf("random number generated with arrivalrand: %d", randNum);
  //randNum = (rangeHigh * drand48());
  return randNum;
}

int stayrand(int meanStay){
  int rangeLow, rangeHigh, mean, stdDev;
  mean = meanStay;
  stdDev = mean/2;
  rangeLow = mean - (3*stdDev);
  rangeHigh = mean + (3*stdDev);
  if (rangeLow < 0){
    rangeLow = 0;
  }
  useconds_t randNum;
  randNum = (sqrt(-2 * log(rangeLow))) * cos(2*pi*rangeHigh);
  printf("random number generated with stayrand: %d", randNum);
  //randNum = (rangeHigh * drand48());
  return randNum;
}


/* individual
 * The thread will run with the given specifications:
 *	g - a random num 0 or 1 that defines the gender of the thread
 *	arrival - the arrival time of the thread
 *	stay - the amount of time the thread will stay in the bathroom
 *	lCount - the number of times the thread will loop through the bathroom
 */
void *individual(pthread_cond_t vacant, pthread_mutex_t lock, struct br* brGlobal, int g, long arrival, long stay, int lCount, int threadNum)
{
	long minQueue, aveQueue, maxQueue, brTime; // variables to keep track of thread statistics
	int rc; // use for assert statements

	// lock
  	int rc = pthread_mutex_lock(&lock);
	assert(rc == 0);
  	// unlock and wait for each thread to be created, then lock
 	rc = pthread_cond_wait(&vacant, &lock);
	if (rc == 0) 
	{
		// wait
	}
	// unlock
	pthread_mutex_unlock(&lock);

	for(int i = 0; i<lCount; i++)
	{
		// wait for arrival time
		usleep(arrival);

		while(1)
		{
			// check if can enter bathroom
			rc = pthread_mutex_lock(&lock);
			assert(rc == 0);
			if(enter(brGlobal, g)==1) // if can enter
			{
				// go to the bathroom
				rc = pthread_mutex_unlock(&lock);
				assert(rc == 0);

				usleep(stay);

				rc = pthread_mutex_lock(&lock);
				assert(rc == 0);
				leave(brGlobal);

				/* QUESTION: How do we handle broadcast so that when it signals the other threads,
				 * the lock is freed from this thread, and all other threads can grab the lock */
				if(brGlobal->gender == -1) // Bathroom is vacant so signal threads
				{
					//rc = pthread_mutex_lock(&broad); // make sure broadcast and unlock happen at the same time... Is this necessary?

					rc = pthread_cond_broadcast(&vacant); // broadcast to all other threads
					assert(rc == 0);
					// what if we pause here?
					rc = pthread_cond_unlock(&lock);
					assert(rc == 0);

					//rc = pthread_mutex_unlock(&broad); // is this necessary?
				}
				else
				{
					rc = pthread_mutex_unlock(&lock);
					assert(rc == 0);
					break;
				}
			}
			else
			{
				// wait for the bathroom to be vacant so one can enter
				rc = pthread_cond_wait(&vacant, &lock);
			}
		}
	}
}


int main(int argc, char* argv[])
{
  //check if valid inputs
	assert(argc == 5);
  //assign inputs
  int nUsers = argv[1];
  int meanLoopCount = argv[2]; //avg number of times	user repeats user bathroom loop
  int meanArrival = argv[3];
  int meanStay = argv[4];
  int rc = 0;
  
  Intitialize();
  
  // create general mutex and condition variable
  pthread_mutex_t lock = PTHREAD_RECURSIVE_MUTEX_INITIALIZER_NP;
  pthread_cond_t vacant = pthread_cond_init();
  
  // create n threads
  pthread_t tid[nUsers];  
  for(int i=0; i<nUsers; i++) 
  {
    //pthread_t thread;
    gender g = rand() & 1
    // create a thread
    thr_id = pthread_create(&tid[i], NULL, Individual, vacant, lock, g, meanArrival, meanStay, meanLoopCount);
  }
  
  pthread_cond_broadcast(&vacant);
  
  for(int i = 0; i<nUsers; i++)
  {
		pthread_join(tid[i], NULL);
  }
  
  Finalize();
  return 0;
}
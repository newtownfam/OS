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
#include "Bathroom.h"
#ifndef M_PI
#define M_PI = #define M_PI 3.14159265358979323846
#endif

/**************************************************************** STRUCTS **********************************************************************/

struct argstruct
{
	pthread_mutex_t printLock;
	pthread_cond_t vacant;
	pthread_mutex_t lock;
	int gender;
	useconds_t arrival;
	useconds_t stay;
	int lCount;
	int threadNum;
};

/*********************************************************** HELPER FUNCTIONS **********************************************************************/

//use this for loop count, based on mean from command line
int loopRand(int meanLoopCount){
  double a = ((rand() % 10000) / 10000.0);
  double b = ((rand() % 10000) / 10000.0);
  double randNum = ((sqrt(-2 * log(a))) * cos(2*M_PI*b));
  //printf("STATS: randNum Val: [%lf]\n", randNum);
  return randNum + meanLoopCount;
}

// rng for arrival
double arrivalRand(double meanArrival){
  double a = ((rand() % 10000) / 10000.0);
  double b = ((rand() % 10000) / 10000.0);
  double randNum = ((sqrt(-2 * log(a))) * cos(2*M_PI*b))/10;
  return randNum + meanArrival;
}

// rng for stay
double stayRand(double meanStay){
  double a = ((rand() % 10000) / 10000.0);
  double b = ((rand() % 10000) / 10000.0);
  double randNum = ((sqrt(-2 * log(a))) * cos(2*M_PI*b))/10;
  return randNum + meanStay;
}

/***************************************************** INDIVIDUAL (THREAD SIMULATOR) *************************************************************/

/* individual
 * The thread will run with the given specifications:
 *	g - a random num 0 or 1 that defines the gender of the thread
 *	arrival - the arrival time of the thread
 *	stay - the amount of time the thread will stay in the bathroom
 *	lCount - the number of times the thread will loop through the bathroom
 */
void *individual(void* arguments)
{
	// struct argstruct *args = arguments;
	// long minQueue = 0;
	// long aveQueue = 0;
	// long maxQueue = 0;
 //    int rc = 0; // use for assert statements
 //    int br = 0; // know if bathroom is empty

	// /* Wait for all threads to be created so they all start at the same time (NOT WORKING)
 //  	rc = pthread_mutex_lock(&args->lock);
	// assert(rc == 0);

	// //printf("INDIVIDUAL: about to enter first while loop\n");
	// while(br != -1)
	// {
	// 	 rc = pthread_cond_wait(&args->vacant, &args->lock);
	// 	 br = 0;
	// }
	// //printf("INDIVIDUAL: exited first while loop\n");
	// pthread_mutex_unlock(&args->lock);
	// */

	// /* Begin for loop to go through lCount iterations of going to the bathroom */
	// for(int i = 0; i<args[threadNum]->lCount; i++)
	// {
	// 	/* Quick nap... wait for arrival time */
	// 	usleep(args[threadNum]->arrival);
	// 	printf("INDIVIDUAL: about to enter while loop for locks and shit\n");

	// 	while(1)
	// 	{
	// 		/* check if can enter bathroom <- that grammar */
	// 		rc = pthread_mutex_lock(&args[threadNum]->lock);
	// 		assert(rc == 0);
	// 		if(enter(args[threadNum]->gender)==1) // if can enter
	// 		{
	// 			/* go to the bathroom */
	// 			rc = pthread_mutex_unlock(&args[threadNum]->lock);
	// 			assert(rc == 0);

	// 			/* sleep (lol) in the bathroom... */
	// 			usleep(args[threadNum]->stay);
	// 			printf("INDIVIDUAL: in bathroom\n");

	// 			/* leave the bathrooom */
	// 			rc = pthread_mutex_lock(&args[threadNum]->lock);
	// 			assert(rc == 0);
	// 			leave();
	// 			printf("INDIVIDUAL: leaving bathroom\n");


	// 			/* Chcek if bathroom is vacant */
	// 			/* QUESTION: How do we handle broadcast so that when it signals the other threads,
	// 			 * the lock is freed from this thread, and all other threads can grab the lock */
	// 			if(getGender() == -1) // Bathroom is vacant so signal threads
	// 			{
	// 				rc = pthread_cond_broadcast(&args[threadNum]->vacant); // broadcast to all other threads
	// 				assert(rc == 0);
	// 				// what if we pause here?
	// 				rc = pthread_mutex_unlock(&args[threadNum]->lock);
	// 				assert(rc == 0);
	// 				printf("INDIVIDUAL: bathroom vacant\n");
	// 				br = -1;
	// 				break; // break out of waiting to do next lCount
	// 			}
	// 			else
	// 			{
	// 				rc = pthread_mutex_unlock(&args[threadNum]->lock);
	// 				assert(rc == 0);
	// 				break; // break out of waiting to do next lCount
	// 			}
	// 		}
	// 		else
	// 		{
	// 			/* wait for the bathroom to be vacant so one can enter */
	// 			while(br != -1)
	// 			{
	// 				rc = pthread_cond_wait(&args[threadNum]->vacant, &args[threadNum]->lock);
	// 				br = 0;
	// 			}
	// 		}
	// 	}
	// }

	/* Should wait for all threads to be created, is this too slow? (prolly not) */
	pthread_mutex_lock(&lock);
	int try = 0;
	god--;
	while(god>0)
	{
		pthread_mutex_lock(&lock);
		yield();
		pthread_mutex_unlock(&lock);
	}
	/* try is so that while the threads are waiting for other threads to be created, 
	 *it can at least assign random variables to speed it up slightly */
	if(int try = 0)
	{
		/* Assign random variables to the thread */
		printf("INDIVIDUAL: assigning random values...\n");
		arguments->lCount = loopRand(arguments->lCount);
		arguments->arrival = arrivalRand(arguments->arrival);
		arguments->stay = stayRand(arguments->stay);
	}
	pthread_mutex_unlock(&lock);

	/* Loop through the lCount times and simulate entering and leaving a bathroom */
	for(int i = 0; i<arguments->lCount; i++)
	{
		usleep(arguments->arrival);
		enter(arguments->gender);
		usleep(arguments->stay);
		leave();
	}


	/* print statistics (do not want more than one thread printing at once so lock) */
	pthread_mutex_lock(&args->printLock);
	printStats(arguments->gender, arguments->threadNum, arguments->lCount, minQueue, aveQueue, maxQueue);
	pthread_mutex_unlock(&args->printLock);
	return 0;
}

/**************************************************************** MAIN **********************************************************************/

int main(int argc, char* argv[])
{

/************************************************************** VARIABLES *********************************************************************/
  /* check if valid inputs */
  assert(argc == 5);
  /* assign inputs */
  int nUsers;
  int meanLoopCount;
  double meanArrival;
  double meanStay;
  
  nUsers = atoi(argv[1]);
  printf("nUsers:[%i]\n", nUsers);
  meanLoopCount = atoi(argv[2]);
  printf("meanLoopCount:[%i]\n", meanLoopCount);
  meanArrival = atof(argv[3]);
  printf("meanArrival:[%f]\n", meanArrival);
  meanStay = atof(argv[4]);
  printf("meanStay:[%f]\n", meanStay);
  
  initialize();
  printf("Initialize function ran...\n");
  
  /* create general mutex and condition variable */
  pthread_mutex_t lock;
  pthread_mutex_t printLock;
  pthread_mutex_t godLock;
  pthread_cond_t vacant;
  pthread_mutex_init(&lock, NULL);
  pthread_mutex_init(&printLock, NULL);
  pthread_cond_init(&vacant, NULL);

  /* args will hold all the thread information, and will be used to pass parameters to individual */
  /* This may cause a lot of memory accesses and slow down the threads considerably... */
  /* If this doesn't work try making it an array instead of a pointer */

	/* god and godLock will ensure all threads are created together */
	pthread_mutex_t godLock;
	pthread_mutex_init(&godLock);
	int god = 0;

	pthread_mutex_lock(&godLock);
  pthread_t tid[nUsers];

  /************************************************************ BEGIN SIMULATION *******************************************************************/  
  for(int i=0; i<nUsers; i++) 
  {
	//pthread_t thread;
	int gender = rand() & 1;
	args[i]->gender = gender;
	args[i]->printLock = printLock;
	args[i]->vacant = vacant;
	args[i]->lock = lock;
	args[i]->arrival = meanArrival;
	args[i]->stay = meanStay;
	args[i]->lCount = meanLoopCount;
	args[i]->threadNum = i;
	//printf("TN; %d\n", args->threadNum);

	/* CREATE THE THREADS */
	printf("About to create thread #%i\n", i+1);
	god++; // increment god (number of threads created)
	pthread_create(&tid[i], NULL, &individual, (void*) args[i]);
	printf("Thread #%i created!\n", i+1);
  }

  pthread_mutex_unlock(&godLock);
 
/************************************************************** END SIMULATION **********************************************************************/
 
  for(int i = 0; i<nUsers; i++)
  {
  		printf("Calling pthread_join for thread #%i\n", i+1);
		pthread_join(tid[i], NULL);
  }
  printf("About to run finalize() and end bathSim Program\n");
  finalize();
  printf("Goodbye, Commander\n");
  printf("What? Too Soon?\n");
  return 0;
}
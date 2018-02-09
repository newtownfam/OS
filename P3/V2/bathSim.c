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

/* individual
 * The thread will run with the given specifications:
 *	g - a random num 0 or 1 that defines the gender of the thread
 *	arrival - the arrival time of the thread
 *	stay - the amount of time the thread will stay in the bathroom
 *	lCount - the number of times the thread will loop through the bathroom
 */
//pthread_mutex_t printLock, pthread_cond_t vacant, pthread_mutex_t lock, int gender, long arrival, long stay, int lCount, int threadNum
void *individual(void* arguments)
{
	struct argstruct *args = arguments;
	long minQueue = 0;
	long aveQueue = 0;
	long maxQueue = 0;
	//long brTime = 0; // variables to keep track of thread statistics
	//useconds_t arrival = arrivalRand(args->arrival);
    //useconds_t stay = stayRand(args->stay);
    //int loopcount = loopRand(args->lCount);
    int rc = 0; // use for assert statements
    int br = 0; // know if bathroom is empty

	/* Wait for all threads to be created so they all start at the same time
  	rc = pthread_mutex_lock(&args->lock);
	assert(rc == 0);

	//printf("INDIVIDUAL: about to enter first while loop\n");
	while(br != -1)
	{
		 rc = pthread_cond_wait(&args->vacant, &args->lock);
		 br = 0;
	}
	//printf("INDIVIDUAL: exited first while loop\n");
	pthread_mutex_unlock(&args->lock);
	*/

	// loop for lCount
	printf("INDIVIDUAL: assigning random values...\n");
	args->lCount = loopRand(args->lCount);
	args->arrival = arrivalRand(args->arrival);
	args->stay = stayRand(args->stay);

	for(int i = 0; i<args->lCount; i++)
	{
		// wait for arrival time
		usleep(args->arrival);
		printf("INDIVIDUAL: about to enter while loop for locks and shit\n");

		while(1)
		{
			// check if can enter bathroom
			rc = pthread_mutex_lock(&args->lock);
			assert(rc == 0);
			if(enter(args->gender)==1) // if can enter
			{
				// go to the bathroom
				rc = pthread_mutex_unlock(&args->lock);
				assert(rc == 0);

				usleep(args->stay);

				printf("INDIVIDUAL: in bathroom\n");

				rc = pthread_mutex_lock(&args->lock);
				assert(rc == 0);
				leave();
				printf("INDIVIDUAL: leaving bathroom\n");


				/* QUESTION: How do we handle broadcast so that when it signals the other threads,
				 * the lock is freed from this thread, and all other threads can grab the lock */
				if(getGender() == -1) // Bathroom is vacant so signal threads
				{
					rc = pthread_cond_broadcast(&args->vacant); // broadcast to all other threads
					assert(rc == 0);
					// what if we pause here?
					rc = pthread_mutex_unlock(&args->lock);
					assert(rc == 0);
					printf("INDIVIDUAL: bathroom vacant\n");
					br = -1;
					break; // break out of waiting to do next lCount
				}
				else
				{
					rc = pthread_mutex_unlock(&args->lock);
					assert(rc == 0);
					break; // break out of waiting to do next lCount
				}
			}
			else
			{
				// wait for the bathroom to be vacant so one can enter
				while(br != -1)
				{
					rc = pthread_cond_wait(&args->vacant, &args->lock);
					br = 0;
				}
			}
		}
	}
/* print statistics (do not want more than one thread printing at once so lock) */
	/* TO_DO FIX THE STATISTICS PRINTING */
  pthread_mutex_lock(&args->printLock);
  printStats(args->gender, args->threadNum, args->lCount, minQueue, aveQueue, maxQueue); // give shit
  pthread_mutex_unlock(&args->printLock);
  return 0;
}



int main(int argc, char* argv[])
{
  //check if valid inputs
  assert(argc == 5);
  //assign inputs
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
  
  // create general mutex and condition variable
  pthread_mutex_t lock;
  pthread_mutex_t printLock;
  pthread_cond_t vacant;
  pthread_mutex_init(&lock, NULL);
  pthread_mutex_init(&printLock, NULL);
  pthread_cond_init(&vacant, NULL);

  // create n threads
  struct argstruct *args = malloc(sizeof (struct argstruct));
	args->printLock = printLock;
	args->vacant = vacant;
	args->lock = lock;
	args->arrival = meanArrival;
	args->stay = meanStay;
	args->lCount = meanLoopCount;
  
  pthread_t tid[nUsers];  
  for(int i=0; i<nUsers; i++) 
  {
	//pthread_t thread;
	int gender = rand() & 1;
	args->gender = gender;
	args->threadNum = i;
	printf("TN; %d\n", args->threadNum);
	// create a thread
	printf("About to create thread #%i\n", i+1);
	pthread_create(&tid[i], NULL, &individual, (void*) args);
	printf("Thread #%i created!\n", i+1);
  }
  //pthread_cond_broadcast(&vacant);

  /* TO_DO Make sure all threads are created at the same time */
  
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
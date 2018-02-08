/* Bathroom.c tester */

#include "Bathroom.h"
#include <stdlib.h>
#include <stdio.h>
#include <pthread.h>
#include <math.h>
#include <sys/types.h>

//use this for loop count, based on mean from command line
int loopRand(int meanLoopCount){
  int rangeLow, rangeHigh, mean, stdDev, randNum;
  mean = meanLoopCount;
  stdDev = mean/2;
  rangeLow = mean - (3*stdDev);
  rangeHigh = mean + (3*stdDev);
  if (rangeLow < 0){
    rangeLow = 0;
  }
  randNum = (sqrt(-2 * log(rangeLow))) * cos(2*M_PI*rangeHigh);
  printf("random number generated with looprand: %d", randNum);
  //randNum = (rangeHigh * drand48());
  return randNum;
}

// rng for arrival
useconds_t arrivalRand(int meanArrival){
  int rangeLow, rangeHigh, mean, stdDev;
  mean = meanArrival;
  stdDev = mean/2;
  rangeLow = mean - (3*stdDev);
  rangeHigh = mean + (3*stdDev);
  if (rangeLow < 0){
    rangeLow = 0;
  }
  useconds_t randNum;
  randNum = (sqrt(-2 * log(rangeLow))) * cos(2*M_PI*rangeHigh);
  printf("random number generated with arrivalrand: %d", randNum);
  //randNum = (rangeHigh * drand48());
  return randNum;
}

// rng for stay
useconds_t stayRand(int meanStay){
  int rangeLow, rangeHigh, mean, stdDev;
  mean = meanStay;
  stdDev = mean/2;
  rangeLow = mean - (3*stdDev);
  rangeHigh = mean + (3*stdDev);
  if (rangeLow < 0){
    rangeLow = 0;
  }
  useconds_t randNum;
  randNum = (sqrt(-2 * log(rangeLow))) * cos(2*M_PI*rangeHigh);
  printf("random number generated with stayrand: %d", randNum);
  //randNum = (rangeHigh * drand48());
  return randNum;
}


int main()
{
	printf("~~~~~~~~~~~~~~~LETS TEST SOME SHIT~~~~~~~~~~~~~~~\n");
	// test the initialize function
	initialize();

	printf("\ninitialize of brGlobal completed...\n");
	printf("BRGLOBAL INFO -> GF: %i, MC: %i, FC: %i, TU, %i\n", getGender(), getMCount(), getFCount(), getTotalU());

	//test enter and leave functions
	printf("\n~~~TESTING ENTER AND LEAVE FUNCTIONS~~~\n");
	printf("GF: %i, MC: %i, FC: %i, TU, %i\n", getGender(), getMCount(), getFCount(), getTotalU());
	enter(0);
	printf("GF: %i, MC: %i, FC: %i, TU, %i\n", getGender(), getMCount(), getFCount(), getTotalU());
	
	enter(0);
	printf("GF: %i, MC: %i, FC: %i, TU, %i\n", getGender(), getMCount(), getFCount(), getTotalU());
	
	leave();
	printf("GF: %i, MC: %i, FC: %i, TU, %i\n", getGender(), getMCount(), getFCount(), getTotalU());
	
	//brGlobal->gender = 0;

	enter(0);
	printf("GF: %i, MC: %i, FC: %i, TU, %i\n", getGender(), getMCount(), getFCount(), getTotalU());
	
//	brGlobal->gender = 1;
	enter(1);
	printf("GF: %i, MC: %i, FC: %i, TU, %i\n", getGender(), getMCount(), getFCount(), getTotalU());
	
	leave(1);
	printf("GF: %i, MC: %i, FC: %i, TU, %i\n", getGender(), getMCount(), getFCount(), getTotalU());
	
	leave(1);
	printf("GF: %i, MC: %i, FC: %i, TU, %i\n", getGender(), getMCount(), getFCount(), getTotalU());
	
	leave(1);
	printf("GF: %i, MC: %i, FC: %i, TU, %i\n", getGender(), getMCount(), getFCount(), getTotalU());
	
	enter(0);
	printf("GF: %i, MC: %i, FC: %i, TU, %i\n", getGender(), getMCount(), getFCount(), getTotalU());

	//test random functions
	printf("\n~~~TESTING RANDOM FUNCTIONS~~~\n");
	printf("Successfully ran loopRand function with input: [%d] and output: [%d]\n", 0, loopRand(0));
	printf("Successfully ran loopRand function with input: [%d] and output: [%d]\n", 5, loopRand(5));
	printf("Successfully ran loopRand function with input: [%d] and output: [%d]\n", 10, loopRand(10));
	printf("Successfully ran loopRand function with input: [%d] and output: [%d]\n", 15, loopRand(15));
  printf("Successfully ran loopRand function with input: [%d] and output: [%d]\n", 20, loopRand(20));
  printf("Successfully ran loopRand function with input: [%d] and output: [%d]\n", 25, loopRand(25));
  printf("Successfully ran loopRand function with input: [%d] and output: [%d]\n", 30, loopRand(30));
	printf("Successfully ran stayRand function with input: [%d] and output: [%d]\n", 0, stayRand(0));
	printf("Successfully ran stayRand function with input: [%d] and output: [%d]\n", 5, stayRand(5));
	printf("Successfully ran stayRand function with input: [%d] and output: [%d]\n", 10, stayRand(10));
	printf("Successfully ran stayRand function with input: [%d] and output: [%d]\n", 15, stayRand(15));
  printf("Successfully ran loopRand function with input: [%d] and output: [%d]\n", 20, stayRand(20));
  printf("Successfully ran loopRand function with input: [%d] and output: [%d]\n", 25, stayRand(25));
  printf("Successfully ran loopRand function with input: [%d] and output: [%d]\n", 30, stayRand(30));
  printf("Successfully ran arrivalRand function with input: [%d] and output: [%d]\n", 0, arrivalRand(0));
	printf("Successfully ran arrivalRand function with input: [%d] and output: [%d]\n", 5, arrivalRand(5));
	printf("Successfully ran arrivalRand function with input: [%d] and output: [%d]\n", 10, arrivalRand(10));
	printf("Successfully ran arrivalRand function with input: [%d] and output: [%d]\n", 15, arrivalRand(15));
  printf("Successfully ran arrivalRand function with input: [%d] and output: [%d]\n", 20, arrivalRand(20));
  printf("Successfully ran arrivalRand function with input: [%d] and output: [%d]\n", 25, arrivalRand(25));
  printf("Successfully ran arrivalRand function with input: [%d] and output: [%d]\n", 30, arrivalRand(30));
	return 0;

}
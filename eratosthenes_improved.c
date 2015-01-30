/*
 * =====================================================================================
 *
 *       Filename:  eratosthenes.cpp
 *
 *    Description:  
 *
 *        Version:  1.0
 *        Created:  01/30/2015 01:43:03
 *       Revision:  none
 *       Compiler:  gcc/g++
 *
 *         Author:  Marius-Constantin Melemciuc  
 *          email:  
 *   Organization:  
 *
 * =====================================================================================
 */

#include <mpi.h>
#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include "MyMPI.h"


int main(int argc, char** argv)
{
	int 	count;        /* local prime count */ 
	double  elapsed_time; /* parallel execution time */
	int 	first;        /* index of first multiple */
	int 	global_count; /* global prime count */
	int     high_value;   /* highest value on this proc */
	int 	i;            /* */
	int 	id;           /* process id number */
	int 	index;        /* index of current prime */ 
	int 	low_value;    /* lowest value on this proc */
	char* 	marked;       /* portion of 2, ..., 'n' */
	int 	n;            /* sieving from 2, ..., 'n' */
	int 	p;            /* number of processes */
	int 	proc0_size;   /* size of proc 0's subarray */
	int 	prime;        /* current prime */
	int 	size;         /* elements in marked string */
	int     first_value_index;
	int 	prime_step;
	int     prime_doubled;

	MPI_Init(&argc, &argv);

	/* start the timer */
	MPI_Barrier(MPI_COMM_WORLD);
	elapsed_time = -MPI_Wtime();

	MPI_Comm_rank(MPI_COMM_WORLD, &id);
	MPI_Comm_size(MPI_COMM_WORLD, &p);

	if (argc != 2)
	{
		if (id == 0) /* parent process */
			printf("Command line: %s <m>\n", argv[0]);
		MPI_Finalize();
		exit(1);
	} /* if (argc != 2) */

	n = atoi(argv[1]);

	/* figure out this process's share of the array, as well as the 
   	   integers represented by the first and last array elements */
	low_value  = BLOCK_FIRST + BLOCK_LOW(id, p, n - 1) * BLOCK_STEP;	
	high_value = BLOCK_FIRST + BLOCK_HIGH(id, p, n - 1) * BLOCK_STEP;
	size = BLOCK_SIZE(id, p, n - 1);
	
	/* bail out if all the primes used for sieving are not all held
	   by process 0 */
	proc0_size = (n - 1) / p;

	if ((2 + proc0_size) < (int)sqrt((double)n))
	{
		if (id == 0) /* parent process */
			printf("Too many processes\n");
		MPI_Finalize();
		exit(1);
	} /* if */

	/* allocate this process's share of the array */
	marked = (char*)malloc(size * sizeof(char));
	if (marked == NULL)
	{
		printf("Cannot allocate enough memory\n");
		MPI_Finalize();
		exit(1);
	} /* if */

	for (i = 0; i < size; i++)
		marked[i] = 0;

	if (id == 0) /* parent process */
		index = 0;
	prime = 3;

	do 
	{
		if (prime * prime > low_value)
		{
			first = prime * prime;
		}
		else 
		{
			if (!(low_value % prime))
				first = low_value;
			else 
				first = prime - (low_value % prime) + low_value;
		}
		
		// Optimization1, consider only odd multiples of prime number
		if ((first + prime) & 1) // is odd
		   first += prime; 
		
		first_value_index = (first - BLOCK_FIRST) / BLOCK_STEP - 
							BLOCK_LOW(id, p, n - 1);
		prime_doubled = prime << 1;
		prime_step = prime_doubled / BLOCK_STEP;	
		for (i = first; i <= high_value; i += prime_doubled)
		{
			marked[first_value_index] = 1;
			first_value_index += prime_step;
		} /* for */
		
		if (id == 0) /* parent process */
		{
			do 
			{
				index++;
			} /* do */
			while (marked[index]);
			prime = index * BLOCK_STEP + BLOCK_FIRST;
		} /* if */

		MPI_Bcast(&prime, 1, MPI_INT, 0, MPI_COMM_WORLD);
	} /* do */
	while (prime * prime <= n);

	/* count the number of primes numbers found on this process */
	count = 0;
	for (i = 0; i < size; i++)
		if (!marked[i])
			count++;

	MPI_Reduce(&count, &global_count, 1, MPI_INT, 
			   MPI_SUM, 0, MPI_COMM_WORLD);

	/* stop the timer */
	elapsed_time += MPI_Wtime();

	/* print the results */
	if (id == 0) /* parent process */
	{
		global_count += 1; // for the first prime, 2
		printf("%d primes are less than or equal to %d\n", 
			   global_count, n);
		printf("Total elapsed time: %10.6f\n", 
			   elapsed_time);
	} /* if */

	MPI_Finalize();

	return 0;
}

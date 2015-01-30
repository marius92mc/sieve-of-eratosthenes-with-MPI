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
	low_value  = 2 + BLOCK_LOW(id, p, n - 1);	
	high_value = 2 + BLOCK_HIGH(id, p, n - 1);
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
	prime = 2;

	do 
	{
		if (prime * prime > low_value)
		{
			first = prime * prime - low_value;
		}
		else 
		{
			if (!(low_value % prime))
				first = 0;
			else 
				first = prime - (low_value % prime);
		}

		for (i = first; i < size; i += prime)
			marked[i] = 1; // ?? 
		
		if (id == 0) /* parent process */
		{
			while (marked[++index])
				;
			prime = index + 2;
		}

		MPI_Bcast(&prime, 1, MPI_INT, 0, MPI_COMM_WORLD);
	} /* do */
	while (prime * prime <= n);

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
		printf("%d primes are less than or equal to %d\n", 
			   global_count, n);
		printf("Total elapsed time: %10.6f\n", 
			   elapsed_time);
	} /* if */

	MPI_Finalize();

	return 0;
}

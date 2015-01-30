# MPI Sieve of Eratosthenes
###### MPI Parallel program in C for The Sieve of Eratosthenes - project for Parallel and Concurrent Programming course

Based on the initial version of the problem presented in the book 
"Parallel programming in C with MPI and openMP", by Quinn.


##### Compile 
    mpicc -o eratosthenes_improved eratosthenes_improved.c

##### Run
	mpirun -np 5 eratosthenes_improved 152512534

###### Sample Output
8577481 primes are less than or equal to 152512534
Total elapsed time:   5.247906

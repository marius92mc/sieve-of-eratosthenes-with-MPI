# MPI Sieve of Eratosthenes
###### MPI Parallel program in C for The Sieve of Eratosthenes - project for Parallel and Concurrent Programming course

Based on the initial version of the problem presented in the book 
"Parallel programming in C with MPI and openMP", by Quinn.


##### Compile 
	mpicc -o eratosthenes eratosthenes.c 

##### Run
	mpirun -np 5 eratosthenes 152512534

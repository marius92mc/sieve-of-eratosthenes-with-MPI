# MPI Sieve of Eratosthenes

[![Build Status](https://travis-ci.org/marius92mc/sieve-of-eratosthenes-with-MPI.svg?branch=master)](https://travis-ci.org/marius92mc/sieve-of-eratosthenes-with-MPI)

MPI Parallel program in C for The Sieve of Eratosthenes - project for Parallel and Concurrent Programming course


Based on the initial version of the problem presented in the book 
"Parallel programming in C with MPI and openMP", by Quinn.


## Compile 
    $ cd src/  
    $ mpicc -o eratosthenes_improved eratosthenes_improved.c

## Run
    $ cd src/ 
    $ mpirun -np 5 eratosthenes_improved 152512534



### Sample Output

    8577481 primes are less than or equal to 152512534
    Total elapsed time:   5.247906s


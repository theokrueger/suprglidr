//! util.c
//!
//! helper functions and stuff

#include "pico/stdlib.h"
#include "pico/rand.h"

#include "../include/input.h"

/// Maximum of two integers
int imax(int a, int b)
{
	return a > b ? a : b;
}

/// Minimum of two integers
int imin(int a, int b)
{
	return a < b ? a : b;
}

/// Return a random number on range [low, high]
int get_rand_inclusive(int low, int high)
{
	return (get_rand_32() % (high - low + 1)) + low;
}

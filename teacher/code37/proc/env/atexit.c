#include <stdio.h>
#include <stdlib.h>

static void
fn1(void)
{
	puts("fn1() is running.");
}

static void
fn2(void)
{
	puts("fn2() is running.");
}

static void
fn3(void)
{
	puts("fn3() is running.");
}

int
main()
{
	puts("Begin.");
	atexit(fn1);
	atexit(fn2);
	atexit(fn3);
	puts("End.");
	exit(0);
}


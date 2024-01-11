#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <pthread.h>

// cc -lpthread main.c

void *routine(void)
{
 printf("Test Start:\n");
 sleep(3);
 printf("End Test.\n");
}

int main(int argc, char **argv)
{
	(void)argc;
	(void)argv;

	pthread_t t1, t2;

	pthread_create(&t1, NULL, routine, NULL);
	pthread_create(&t2, NULL, routine, NULL);

	pthread_join(t1, NULL);
	pthread_join(t2, NULL);
}

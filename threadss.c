#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <pthread.h>
#include <time.h>

// cc -lpthread main.c
// cc -s to compile into assembly

/*
what is a process?
what is a thread?
what are race condition?
what is a mutex? / threads and mutex must be init and destroyed
lock mutex before chaging values, unlock after.
what is a conditional variable?
what is a signaling variable?

pthread_t thread[argv[1]]; ft_atoi(argv[1]);
*/

int mails = 0;

void *routine(void)
{
	for (int i = 0; i < 500; i++)
	{
		mails++;
	}
}

int main(int argc, char **argv)
{
	(void)argc;
	(void)argv;

	pthread_t t1, t2;

	if (pthread_create(&t1, NULL, routine, NULL) != 0)
		return (1);
	if (pthread_create(&t2, NULL, routine, NULL) != 0)
		return (2);


	pthread_join(t1, NULL);
	pthread_join(t2, NULL);

	printf("Number of mails: %d\n", mails);
}


void create_threads(void)
{
	pthread_t th[4];

	for (int i = 0; i < 4; i++)
	{
		if (pthread_create(th[i], NULL, routine, NULL) != 0)
			return (0);
	}
}

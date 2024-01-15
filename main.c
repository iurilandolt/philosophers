#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <pthread.h>
#include <sys/time.h>
#include <stdbool.h>
#include <limits.h>

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
*/

typedef struct s_fork
{
	int				index;
	pthread_mutex_t	fork;
} t_fork;

// include pointer to sim struct in philo struct?
typedef struct s_philo
{
	pthread_t	th_id;
	int			index; // must start at 1, (i + 1)
	long		meals;
	long		last_meal;
	bool		full;
	t_fork		*left;
	t_fork		*right;

} t_philo;

typedef struct s_simu
{
	int		seats;
	int		meal_goal; // [5] or -1 (amount or flag)
	int		time_to_die;
	int		time_to_eat;
	int		time_to_sleep;
	int		sim_start;
	bool	sim_end; // sim ends when a philo dies or all philos are full
	t_philo	*philosophers;
	t_fork	*forks;
}	t_simu;


int	ft_atoi(char *str)
{
	int	n;
	int	sign;

	n = 0;
	sign = 1;
	while (*str == 32)
		str++;
	if (*str == '+' || *str == '-')
		if (*str++ == '-')
			sign = -1;
	while (*str >= '0' && *str <= '9')
		n = n * 10 + (*str++ - '0');
	return (n * sign);
}

/*
pthread_t thread[argv[1]]; ft_atoi(argv[1]);

argv[1] -> number of philosophers
argv[2] -> time to die
argv[3] -> time to eat
argv[4] -> time to sleep
argv[5] -> meal goal

state change of a philosopher must be formatted as follows:
◦ timestamp_in_ms X has taken a fork
◦ timestamp_in_ms X is eating
◦ timestamp_in_ms X is sleeping
◦ timestamp_in_ms X is thinking
◦ timestamp_in_ms X died

displayed state message should not be mixed up with another message.

message announcing a philosopher died should be displayed no more than 10 ms
after the actual death of the philosopher.

*/

int main(int argc, char **argv)
{
//	t_philo	*philos;
//	t_fork	*forks;
	t_simu	simu;

	if (argc == 6)
	{
		simu.seats = ft_atoi(argv[1]);
		simu.time_to_die = ft_atoi(argv[2]);
		simu.time_to_eat = ft_atoi(argv[3]);
		simu.time_to_sleep = ft_atoi(argv[4]);
		simu.meal_goal = ft_atoi(argv[5]);
		printf("%d\n", simu.seats);
		printf("%d\n", simu.time_to_die);
		printf("%d\n", simu.time_to_eat);
		printf("%d\n", simu.time_to_sleep);
		printf("%d\n", simu.meal_goal);
	}
	return (0);
}


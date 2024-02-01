# Philosophers

This project is a possible solution for E.W. Dijkstra's dinnig philosophers problem.
This project serves as an introduction to multithreaded programing, concurrency, 
shared memory space, data-races, deadlocks and other related concepts.

![arguments](https://github.com/iurilandolt/philosophers/blob/main/args.png?raw=true)

• The philosophers alternatively eat, think, or sleep.

• There are as many forks on the table as there are philosophers.

• A philosopher takes their right and their left forks to eat, one in each hand.

• When a philosopher has finished eating, they put their forks back on the table and start sleeping. 

• Once awake, they start thinking again. The simulation stops when a philosopher dies of starvation or all philosophers are full.

• Every philosopher needs to eat and should never starve.

• Philosophers don’t speak with each other.

• Philosophers don’t know if another philosopher is about to die.

• Global variables are forbidden

• Each philosopher should be a thread.

• A status message cannot be showned with a delay of more than 10ms.

### Structs Layout and Logic.

The core of this Dining Philosophers implementation revolves around several key data structures designed to encapsulate the state and behavior of forks, philosophers, and the simulation environment itself.

```c
	typedef struct s_fork
	{
		int		index;
		pthread_mutex_t	mtx;
	}	t_fork;
	
	typedef struct s_sim	t_sim;
	
	typedef struct s_philo
	{
		pthread_t			th_id;
		int				index;
		int				meals;
		int				limit;
		int				lspan;
		long				last_meal;
		bool				full;
		bool				dead;
		t_fork				*left;
		t_fork				*right;
		t_sim				*sim;
		pthread_mutex_t		mtx;
	}	t_philo;
	
	typedef struct s_sim
	{
		t_philo			*philosophers;
		t_fork			*forks;
		int			seats;
		int			goal;
		int			served;
		int			time_to_die;
		int			time_to_eat;
		int			time_to_sleep;
		long			start;
		bool			ended;
		pthread_mutex_t	mtx;
	}	t_sim;
```

### Mutexes, pthread Creation, and Thread Behavior.

The implementation leverages POSIX threads (pthreads) and mutexes to handle concurrency and synchronization in the Dining Philosophers problem. Here’s an overview of how these components are utilized:

`Mutexes` are used to control access to shared resources (the forks and the state of each philosopher).
In the t_fork struct, a mutex (pthread_mutex_t mtx) ensures that only one philosopher can hold a fork at any given time.
In the t_philo and t_sim structs, mutexes are used to protect the state of the philosopher and the simulation, respectively, allowing for safe updates and checks of shared variables in a multithreaded environment.

`pthread_create` and Thread Behavior.

Each philosopher is represented by a thread. The pthread_create function is used to create these threads, passing each philosopher’s unique data structure
The thread runs the philosopher function, taking a pointer to the philosopher's data (t_philo) as an argument.
This approach allows each philosopher (thread) to operate independently while sharing the same simulation environment.
Passing Structs as Arguments
The void * argument in pthread_create is a standard way in C to pass arbitrary data to a thread function. It allows passing structured data (like t_philo) to the thread.
Inside the thread function (philosopher), this pointer is cast back to the appropriate type to access the philosopher's data.


```c
    void	threads_create(t_sim *sim)
    {
    	int			i;
    	pthread_t	moni;
    
    	i = 0;
    	while (i < sim->seats)
    	{
    		if (pthread_create(&(sim->philosophers + i)->th_id, NULL, philosopher,
    				(void *)(sim->philosophers + i)) != 0)
    			ft_error("pthread_create() failed.");
    		i++;
    	}
    	ft_sleep(sim->seats);
    	if (pthread_create(&moni, NULL, monitor, (void *)sim) != 0)
    		ft_error("pthread_create() failed.");
    	if (pthread_join(moni, NULL) != 0)
    		ft_error("pthread_join() failed.");
    	i = 0;
    	while (i < sim->seats)
    	{
    		if (pthread_join((sim->philosophers + i)->th_id, NULL) != 0)
    			ft_error("pthread_join() failed.");
    		i++;
    	}
    }
```
`pthread_join` and Main Thread Behavior.

The pthread_join function is used to wait for the completion of philosopher threads. It ensures that the main thread (which initiates the simulation) waits until all philosopher threads have finished their execution.

The main thread's behavior is crucial in managing the lifecycle of the simulation. It initiates the philosopher threads, possibly a monitoring thread, and waits for their completion. It may also be responsible for initiating and ending the simulation, handling any cleanup or final state reporting.

```c
	void	*philosopher(void *data)
	{
	t_philo	*philo;
	
	philo = (t_philo *)data;
	pthread_mutex_lock(&philo->sim->mtx);
	philo->last_meal = get_time();
	pthread_mutex_unlock(&philo->sim->mtx);
	while (1)
	{
		if (is_full(philo))
			break ;
		if (get_bool(&philo->sim->mtx, &philo->sim->ended))
			break ;
		eat(philo);
		rest(philo);
		think(philo);
	}
	return (NULL);
	}
```

### Philosopher routine:

Life Cycle Management: The life cycle (eating, sleeping, thinking) is managed in a loop, reflecting the continuous nature of the philosophers' actions. The loop exits based on specific conditions (fullness or simulation end), illustrating how a philosopher's thread may conclude.

```c
    void	*monitor(void *data)
    {
    	t_sim	*sim;
    	int		i;
    
    	sim = (t_sim *)data;
    	sim->served = 0;
    	while (1)
    	{
    		i = -1;
    		while (++i < sim->seats)
    		{
    			if (get_bool(&sim->mtx, &sim->ended))
    				return (NULL);
    			if (get_bool(&sim->mtx, &(sim->philosophers + i)->full))
    				sim->served++;
    			if (sim->served == sim->seats)
    				return (NULL);
    			if (is_dead(sim->philosophers + i))
    			{
    				pthread_mutex_lock(&sim->mtx);
    				sim->ended = true;
    				pthread_mutex_unlock(&sim->mtx);
    				return (NULL);
    			}
    		}
    	}
    }
```
### Monitor routine:

continuously checks each philosopher's status to identify if any have died from starvation or if all philosophers have successfully completed their meal goals.

### further considerations:

### Fork assignment:
Philosophers pick up forks in a specific order based on their position: odd-numbered philosophers pick up their left fork first, while even-numbered pick up their right fork first. 
This method ensures that not all philosophers attempt to pick up adjacent forks simultaneously, reducing deadlock risk and facilitating smoother execution of the simulation.

### Sleep() Inaccuracy: 

Traditional sleep functions like usleep may introduce inaccuracy due to system scheduling and wake-up latency. The ft_sleep function mitigates this by periodically checking if the desired sleep duration has passed, improving the precision of sleep intervals in the context of the simulation's requirements.

```c
    long	get_time(void)
    {
    	struct timeval	tv;
    
    	if (gettimeofday(&tv, NULL))
    		ft_error("gettimeofday() failed.");
    	return ((tv.tv_sec * (long)1000) + (tv.tv_usec / 1000));
    }
    
    void	ft_sleep(long usecs)
    {
    	long	start;
    
    	start = get_time();
    	while ((get_time() - start) < usecs)
    		usleep(usecs / 10);
    	return ;
    }
```

### Addressing Resource Starvation:

In a multithreaded simulation like the Dining Philosophers, resource starvation can occur when one or more threads (philosophers) are perpetually denied access to necessary resources (forks), leading to uneven progress among threads. 
Even with mutexes for fork handling an even destribuition of resources is not guaranteed. In some cases you can try to "desynchronize" the philosophers actions or try to make use of the remaining time when time_to_eat + time_to_sleep < time_to die.

```c
    void	think(t_philo *philo)
    {
    	int	think;
    
    	if (get_bool(&philo->sim->mtx, &philo->sim->ended))
    		return ;
    	print_status(philo, Y"is thinking"RST);
    	if (philo->sim->seats % 2 == 0)
    		return ;
    	think = philo->sim->time_to_eat * 2 - philo->sim->time_to_sleep;
    	if (think < 0)
    		think = 0;
    	ft_sleep(think * 0.40);
    }
```

This project is not a perfect solution, and even though it was thoroughly tested, it will most certainly be inconsistent with very tight timings.

![output](https://github.com/iurilandolt/philosophers/blob/main/output.png?raw=true)


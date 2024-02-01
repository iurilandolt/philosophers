# philosophers

This project is a possible solution for E.W. Dijkstra's dinnig philosophers problem.
This project serves as an introduction to multithreaded programing, concurrency, 
shared memory space, data-races, deadlocks and other related concepts.


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

## pre sim

alloc mem for all philos and forks
init "table" mutex
get sim "start" time
```c
    void	startup(t_sim *sim)
    {
    	sim->ended = false;
    	sim->philosophers = alloc_philos(sim);
    	sim->forks = alloc_forks(sim);
    	sim->start = get_time();
    	if (pthread_mutex_init(&sim->mtx, NULL) != 0)
    		ft_error("pthread_mutex_init() failed.");
    }
```
init threads with respective routine 
N philosophers / 1 monitor
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
philosopher thread routine
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
monitor routine
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
considerations:

sleep() time inacuracy // gettimeofday & timevalstructs
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
resource starvation / using remaining time when time_to_eat + time_to_sleep < time_to die
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
data races & conditional variables
```c
    bool	get_bool(pthread_mutex_t *mtx, bool *value)
    {
    	bool	ret;
    
    	pthread_mutex_lock(mtx);
    	ret = *value;
    	pthread_mutex_unlock(mtx);
    	return (ret);
    }
```

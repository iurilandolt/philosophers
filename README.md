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

    int	main(int argc, char **argv)
    {
    	t_sim	*sim;
    
    	if (argc > 4 && argc < 7)
    	{
    		sim = (t_sim *)malloc(sizeof(t_sim));
    		if (!sim)
    			return (1);
    		if (!parse(sim, argc, argv))
    		{
    			exit_safe(sim);
    			return (1);
    		}
    		startup(sim);
    		threads_create(sim);
    		cleanup(sim);
    		free(sim);
    	}
    	else
    		exit_safe(NULL);
    	return (0);
    }

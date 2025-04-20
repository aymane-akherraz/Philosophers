/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aakherra <aakherra@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/27 12:36:20 by aakherra          #+#    #+#             */
/*   Updated: 2025/04/20 10:45:11 by aakherra         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

int	join_and_free(t_data *p)
{
	int	i;

	pthread_join(p->monitor_id, NULL);
	i = 0;
	while (i < p->philos->info->num_of_philos)
	{
		pthread_join(p->philos[i].thread_id, NULL);
		i++;
	}
	i = 0;
	while (i < p->philos->info->num_of_philos)
	{
		pthread_mutex_destroy(&(p->forks[i]));
		pthread_mutex_destroy(&(p->philos[i].mutex));
		i++;
	}
	c(&(p->philos->info->mutex));
	free(p->philos);
	free(p->forks);
	return (0);
}

void	init_philos(t_data *p, t_info *info, int i)
{
	p->philos[i].philo_id = i;
	p->philos[i].left_fork = &(p->forks[i]);
	p->philos[i].right_fork = &(p->forks[(i + 1) % info->num_of_philos]);
	p->philos[i].info = info;
	p->philos[i].has_left_fork = false;
	p->philos[i].has_right_fork = false;
	if (info->num_of_meals > 0)
		p->philos[i].meals_count = 0;
	else
		p->philos[i].meals_count = -2;
}

int	create_threads(t_data *p, t_info *info)
{
	int	i;

	p->philos = malloc(sizeof(t_philo) * info->num_of_philos);
	if (!(p->philos))
		return (1);
	p->forks = malloc(sizeof(pthread_mutex_t) * info->num_of_philos);
	if (!(p->forks))
		return (free_philos(p->philos));
	if (init_mutex(p, info))
		return (1);
	info->time_to_think = 0;
	i = 0;
	while (i < info->num_of_philos)
		init_philos(p, info, i++);
	i = 0;
	if (info->num_of_philos % 2)
	{
		if (info->time_to_eat == info->time_to_sleep)
			info->time_to_think = info->time_to_eat / 2;
		else if (info->time_to_eat > info->time_to_sleep)
			info->time_to_think = info->time_to_eat;
	}
	if (pthread_create(&(p->monitor_id), NULL, &do_monitor, p->philos))
	{
		destroy_and_free(p, info);
		return (1);		
	}
	while (i < info->num_of_philos)
	{
		pthread_create(&(p->philos[i].thread_id), NULL,
				&do_routine, &(p->philos[i]));
		i++;
	}
	init_time(p->philos);
	pthread_mutex_lock(&info->mutex);
	info->start_simulation = true;
	pthread_mutex_unlock(&info->mutex);
	return (join_and_free(p));
}

int	check_args(t_info *p, char **av)
{
	p->num_of_philos = ft_atoi(av[1]);
	p->time_to_die = ft_atoi(av[2]);
	p->time_to_eat = ft_atoi(av[3]);
	p->time_to_sleep = ft_atoi(av[4]);
	if (av[5])
		p->num_of_meals = ft_atoi(av[5]);
	else
		p->num_of_meals = -2;
	if (p->num_of_philos <= 0 || p->time_to_die <= 0
		|| p->time_to_eat <= 0 || p->time_to_sleep <= 0
		|| (p->num_of_meals <= 0 && p->num_of_meals != -2))
		return (1);
	p->start_simulation = false;
	p->philo_died = false;
	p->full = false;
	return (0);
}

int	main(int ac, char **av)
{
	t_info	info;
	t_data	data;

	if (ac == 5 || ac == 6)
	{
		if (check_args(&info, av))
			return (ft_error("invalid arguments"));
		else
		{
			if (create_threads(&data, &info))
				return (1);
		}
	}
	else
		return (ft_error(
				"Usage: <number_of_philos> <time_to_die> <time_to_eat> "
				"<time_to_sleep> [number_of_meals]"));
}

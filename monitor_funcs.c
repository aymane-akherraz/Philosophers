/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   monitor_funcs.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aakherra <aakherra@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/12 17:39:33 by aakherra          #+#    #+#             */
/*   Updated: 2025/04/13 20:04:31 by aakherra         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

void	init_time(t_philo *philos)
{
	int		i;
	long	time;
	i = 0;
	philos->info->start_time = get_time();
	time = get_time() - philos->info->start_time;
	while (i < philos->info->num_of_philos)
	{
		pthread_mutex_lock(&philos[i].mutex);
		philos[i].last_meal_time = time;
		pthread_mutex_unlock(&philos[i].mutex);
		i++;
	}
}

int	monitor_philos(t_philo *philos, int i, long my_var, bool *full)
{
	long timestamp;
	long	last;
	while (i < philos->info->num_of_philos)
	{
		pthread_mutex_lock(&philos[i].mutex);
		last = philos[i].last_meal_time;
		pthread_mutex_unlock(&philos[i].mutex);
		timestamp = get_time() - philos->info->start_time;
		if ((timestamp - last) > philos->info->time_to_die)
		{
			printf("%ld\n", timestamp);
			printf("%ld\n", last);
			pthread_mutex_lock(&philos->info->mutex);
			philos[i].info->philo_died = true;
			printf("%ld %d died\n", timestamp,
				philos[i].philo_id + 1);
			//write(1, "done\n", 5);
			pthread_mutex_unlock(&philos->info->mutex);
			return (1);
		}
		if (philos->info->num_of_meals > 0)
		{
			pthread_mutex_lock(&philos[i].mutex);
			my_var = philos[i].meals_count;
			pthread_mutex_unlock(&philos[i].mutex);
			if (my_var < philos->info->num_of_meals)
				*full = false;
		}
		i++;
	}
	return (0);
}

void	*do_monitor(void *arg)
{
	int		i;
	bool	full;
	long	my_var;
	t_philo	*philos;

	philos = arg;
	pthread_mutex_lock(&philos->info->mutex);
	pthread_mutex_unlock(&philos->info->mutex);
	full = false;
	my_var = 0;
	while (!full)
	{
		i = 0;
		if (philos->info->num_of_meals > 0)
			full = true;
		if (monitor_philos(philos, i, my_var, &full))
			return (arg);
	}
	pthread_mutex_lock(&philos->info->mutex);
	philos->info->full = true;
	pthread_mutex_unlock(&philos->info->mutex);
	return (arg);
}

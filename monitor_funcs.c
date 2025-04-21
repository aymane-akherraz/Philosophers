/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   monitor_funcs.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aakherra <aakherra@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/12 17:39:33 by aakherra          #+#    #+#             */
/*   Updated: 2025/04/21 15:45:42 by aakherra         ###   ########.fr       */
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
		philos[i].last_meal_time = time;
		i++;
	}
}

void	check_full(t_philo *philos, long meals_count, bool *full, int i)
{
	pthread_mutex_lock(&philos[i].mutex);
	meals_count = philos[i].meals_count;
	pthread_mutex_unlock(&philos[i].mutex);
	if (meals_count < philos->info->num_of_meals)
		*full = false;
}

int	monitor_philos(t_philo *philos, int i, long meals_count, bool *full)
{
	long	timestamp;
	long	last;

	while (i < philos->info->num_of_philos)
	{
		pthread_mutex_lock(&philos[i].mutex);
		last = philos[i].last_meal_time;
		pthread_mutex_unlock(&philos[i].mutex);
		timestamp = get_time() - philos->info->start_time;
		if ((timestamp - last) > philos->info->time_to_die)
		{
			pthread_mutex_lock(&philos->info->mutex);
			philos[i].info->philo_died = true;
			printf("%ld %d died\n", timestamp,
				philos[i].philo_id + 1);
			pthread_mutex_unlock(&philos->info->mutex);
			return (1);
		}
		if (philos->info->num_of_meals > 0)
			check_full(philos, meals_count, full, i);
		i++;
	}
	return (0);
}

void	start(t_philo *philos)
{
	bool	start;

	pthread_mutex_lock(&philos->info->mutex);
	start = philos->info->start_simulation;
	pthread_mutex_unlock(&philos->info->mutex);
	while (!start)
	{
		pthread_mutex_lock(&philos->info->mutex);
		start = philos->info->start_simulation;
		pthread_mutex_unlock(&philos->info->mutex);
	}
}

void	*do_monitor(void *arg)
{
	int		i;
	bool	full;
	long	meals_count;
	t_philo	*philos;

	philos = arg;
	start(philos);
	full = false;
	meals_count = 0;
	while (!full)
	{
		i = 0;
		if (philos->info->num_of_meals > 0)
			full = true;
		if (monitor_philos(philos, i, meals_count, &full))
			return (arg);
	}
	pthread_mutex_lock(&philos->info->mutex);
	philos->info->full = true;
	pthread_mutex_unlock(&philos->info->mutex);
	return (arg);
}

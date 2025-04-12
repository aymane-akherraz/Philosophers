/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   monitor_funcs.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aakherra <aakherra@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/12 17:39:33 by aakherra          #+#    #+#             */
/*   Updated: 2025/04/12 18:46:04 by aakherra         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

void	init_time(t_philo *philos)
{
	int	i;

	i = 0;
	philos->info->start_time = get_time();
	while (i < philos->info->num_of_philos)
	{
		pthread_mutex_lock(&philos[i].mutex);
		philos[i].last_meal_time = get_time() - philos->info->start_time;
		pthread_mutex_unlock(&philos[i].mutex);
		i++;
	}
	pthread_mutex_lock(&philos->info->mutex);
	philos->info->start_simulation = true;
	pthread_mutex_unlock(&philos->info->mutex);
}

int	monitor_philos(t_philo *philos, int i, long my_var, bool *full)
{
	while (i < philos->info->num_of_philos)
	{
		pthread_mutex_lock(&philos[i].mutex);
		my_var = philos[i].last_meal_time;
		pthread_mutex_unlock(&philos[i].mutex);
		if (((get_time() - philos->info->start_time) - my_var)
			> philos->info->time_to_die)
		{
			pthread_mutex_lock(&philos->info->mutex);
			philos[i].info->philo_died = true;
			printf("%ld %d died\n", get_time() - philos->info->start_time,
				philos[i].philo_id + 1);
			pthread_mutex_unlock(&philos->info->mutex);
			return (1);
		}
		pthread_mutex_lock(&philos[i].mutex);
		my_var = philos[i].meals_count;
		pthread_mutex_unlock(&philos[i].mutex);
		if (my_var < philos->info->num_of_meals)
			*full = false;
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
	init_time(philos);
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

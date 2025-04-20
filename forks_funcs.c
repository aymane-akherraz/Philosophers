/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   forks_funcs.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aakherra <aakherra@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/12 17:45:52 by aakherra          #+#    #+#             */
/*   Updated: 2025/04/20 10:18:42 by aakherra         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

int	init_mutex(t_data *p, t_info *info)
{
	int	i;

	i = 0;
	while (i < info->num_of_philos)
	{
		if (pthread_mutex_init(&(p->forks[i]), NULL))
		{
			destroy_forks(p->forks, i - 1);
			return (1);
		}
		if (pthread_mutex_init(&(p->philos[i].mutex), NULL))
		{
			destroy_philos(p->philos, i - 1);
			return (1);			
		}
		i++;
	}
	if (pthread_mutex_init(&(info->mutex), NULL))
	{
		destroy_forks(p, info);
		return (1);
	}
	return (0);
}

int	helper(t_philo *philo)
{
	pthread_mutex_lock(philo->left_fork);
	if (print_philo_state("has taken a fork", philo, 0))
		return (1);
	philo->has_left_fork = true;
	if (philo->info->num_of_philos > 1)
	{
		pthread_mutex_lock(philo->right_fork);
		if (print_philo_state("has taken a fork", philo, 0))
			return (1);
		philo->has_right_fork = true;
	}
	else
	{
		while (!is_died_full(philo))
			;
		return (1);
	}
	return (0);
}

int	pick_up_fork(t_philo *philo)
{
	if (philo->philo_id % 2)
	{
		pthread_mutex_lock(philo->right_fork);
		if (print_philo_state("has taken a fork", philo, 0))
			return (1);
		philo->has_right_fork = true;
		pthread_mutex_lock(philo->left_fork);
		if (print_philo_state("has taken a fork", philo, 0))
			return (1);
		philo->has_left_fork = true;
	}
	else
		return (helper(philo));
	return (0);
}

void	put_down_fork(t_philo *philo)
{
	if (philo->philo_id % 2)
	{
		if (!pthread_mutex_unlock(philo->left_fork))
			philo->has_left_fork = false;
		if (!pthread_mutex_unlock(philo->right_fork))
			philo->has_right_fork = false;
	}
	else
	{
		if (!pthread_mutex_unlock(philo->right_fork))
			philo->has_right_fork = false;
		if (!pthread_mutex_unlock(philo->left_fork))
			philo->has_left_fork = false;
	}
}

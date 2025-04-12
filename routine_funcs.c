/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   routine_funcs.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aakherra <aakherra@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/12 17:50:20 by aakherra          #+#    #+#             */
/*   Updated: 2025/04/12 18:47:53 by aakherra         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

int	is_died_full(t_philo *p)
{
	bool	died;
	bool	full;

	pthread_mutex_lock(&p->info->mutex);
	died = p->info->philo_died;
	full = p->info->full;
	pthread_mutex_unlock(&p->info->mutex);
	if (died || full)
	{
		if (p->has_left_fork)
			pthread_mutex_unlock(p->left_fork);
		if (p->has_right_fork)
			pthread_mutex_unlock(p->right_fork);
		return (1);
	}
	return (0);
}

int	print_philo_state(char *state, t_philo *p, long sleep_time)
{
	if (is_died_full(p))
		return (1);
	pthread_mutex_lock(&p->info->mutex);
	printf("%ld %d %s\n", get_time() - p->info->start_time,
		p->philo_id + 1, state);
	pthread_mutex_unlock(&p->info->mutex);
	usleep(sleep_time);
	return (0);
}

void	start(t_philo *philo, bool *died, bool *full)
{
	bool	start;

	pthread_mutex_lock(&philo->info->mutex);
	start = philo->info->start_simulation;
	pthread_mutex_unlock(&philo->info->mutex);
	while (!start)
	{
		usleep(100);
		pthread_mutex_lock(&philo->info->mutex);
		start = philo->info->start_simulation;
		pthread_mutex_unlock(&philo->info->mutex);
	}
	get_info(philo, died, full);
}

void	set_philo_info(t_philo *philo)
{
	pthread_mutex_lock(&philo->mutex);
	philo->last_meal_time = get_time() - philo->info->start_time;
	if (philo->meals_count >= 0)
		philo->meals_count++;
	pthread_mutex_unlock(&philo->mutex);
}

void	*do_routine(void *arg)
{
	bool	died;
	bool	full;
	t_philo	*philo;

	philo = arg;
	start(philo, &died, &full);
	while (!died && !full)
	{
		if (pick_up_fork(philo))
			break ;
		if (print_philo_state("is eating", philo,
				philo->info->time_to_eat * 1000))
			break ;
		set_philo_info(philo);
		put_down_fork(philo);
		if (print_philo_state("is sleeping", philo,
				philo->info->time_to_sleep * 1000))
			break ;
		if (print_philo_state("is thinking", philo, 100))
			break ;
		get_info(philo, &died, &full);
	}
	return (arg);
}

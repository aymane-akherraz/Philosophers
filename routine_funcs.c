/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   routine_funcs.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aakherra <aakherra@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/12 17:50:20 by aakherra          #+#    #+#             */
/*   Updated: 2025/04/13 20:05:27 by aakherra         ###   ########.fr       */
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

void	set_philo_info(t_philo *philo, long last_meal_time)
{
	pthread_mutex_lock(&philo->mutex);
	philo->last_meal_time = last_meal_time;
	if (philo->meals_count >= 0)
		philo->meals_count++;
	pthread_mutex_unlock(&philo->mutex);
}

int	print_eat_state(char *state, t_philo *p, long sleep_time)
{
	long timestamp;

	timestamp = get_time() - p->info->start_time;
	// if (is_died_full(p))
	// 	return (1);
	pthread_mutex_lock(&p->info->mutex);
	printf("%ld %d %s\n", timestamp, p->philo_id + 1, state);
	set_philo_info(p, timestamp);
	pthread_mutex_unlock(&p->info->mutex);
	usleep(sleep_time);
	return (0);
}

void	*do_routine(void *arg)
{
	bool	died = 0;
	bool	full = 0;
	t_philo	*philo;

	philo = arg;
	pthread_mutex_lock(&philo->info->mutex);
	pthread_mutex_unlock(&philo->info->mutex);
	while (!died && !full)
	{
		if (pick_up_fork(philo))
			break ;
		if (print_eat_state("is eating", philo,
				philo->info->time_to_eat * 1000))
			break ;
		put_down_fork(philo);
		if (print_philo_state("is sleeping", philo,
				philo->info->time_to_sleep * 1000))
			break ;
		if (print_philo_state("is thinking", philo, 0))
			break ;
		get_info(philo, &died, &full);
	}
	return (arg);
}

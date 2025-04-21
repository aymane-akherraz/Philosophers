/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   routine_funcs.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aakherra <aakherra@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/12 17:50:20 by aakherra          #+#    #+#             */
/*   Updated: 2025/04/21 15:45:17 by aakherra         ###   ########.fr       */
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
	long	timestamp;

	if (is_died_full(p))
		return (1);
	timestamp = get_time() - p->info->start_time;
	pthread_mutex_lock(&p->info->mutex);
	printf("%ld %d %s\n", timestamp, p->philo_id + 1, state);
	pthread_mutex_unlock(&p->info->mutex);
	set_philo_info(p, timestamp + sleep_time);
	usleep(sleep_time);
	return (0);
}

void	*do_routine(void *arg)
{
	bool	died;
	bool	full;
	t_philo	*philo;

	philo = arg;
	start(philo);
	died = 0;
	full = 0;
	while (!died && !full)
	{
		if (print_philo_state("is thinking", philo,
				philo->info->time_to_think * 1000))
			break ;
		if (pick_up_fork(philo))
			break ;
		if (print_eat_state("is eating", philo,
				philo->info->time_to_eat * 1000))
			break ;
		put_down_fork(philo);
		if (print_philo_state("is sleeping", philo,
				philo->info->time_to_sleep * 1000))
			break ;
	}
	return (arg);
}

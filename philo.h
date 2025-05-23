/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aakherra <aakherra@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/27 12:49:35 by aakherra          #+#    #+#             */
/*   Updated: 2025/04/23 08:54:47 by aakherra         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PHILO_H
# define PHILO_H

# include <stdio.h>
# include <unistd.h>
# include <stdlib.h>
# include <pthread.h>
# include <sys/time.h>
# include <stdbool.h>
# include <limits.h>

typedef struct info
{
	int				num_of_philos;
	int				num_of_meals;
	int				time_to_die;
	int				time_to_eat;
	int				time_to_sleep;
	int				time_to_think;
	bool			start_simulation;
	long			start_time;
	bool			philo_died;
	bool			full;
	pthread_mutex_t	mutex;
}	t_info;

typedef struct philo
{
	int				meals_count;
	int				philo_id;
	long			last_meal_time;
	bool			has_left_fork;
	bool			has_right_fork;
	t_info			*info;
	pthread_t		thread_id;
	pthread_mutex_t	mutex;
	pthread_mutex_t	*left_fork;
	pthread_mutex_t	*right_fork;
}	t_philo;

typedef struct data
{
	t_philo			*philos;
	pthread_t		monitor_id;
	pthread_mutex_t	*forks;
}	t_data;

int		ft_error(char *s);
int		free_philos(t_philo *p);
int		is_died_full(t_philo *p);
int		print_philo_state(char *state, t_philo *p, long sleep_time);
int		init_mutex(t_data *p, t_info *info);
int		pick_up_fork(t_philo *philo);
int		check_args(t_info *p, char **av);
int		monitor_philos(t_philo *philos, int i, long my_var, bool *full);
void	destroy_forks(pthread_mutex_t *mutex, int i);
void	destroy_and_free(t_data *p, t_info *info);
void	destroy_philos(t_philo *philos, int i);
void	*do_routine(void *arg);
void	start(t_philo *philos);
void	put_down_fork(t_philo *philo);
void	*do_monitor(void *arg);
long	ft_atoi(const char *s);
long	get_time(void);
void	init_time(t_philo *philos);
size_t	ft_strlen(char *s);

#endif

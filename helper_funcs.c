/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   helper_funcs.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aakherra <aakherra@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/12 17:50:38 by aakherra          #+#    #+#             */
/*   Updated: 2025/04/21 15:43:34 by aakherra         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

long	ft_atoi(const char *s)
{
	int		i;
	long	n;

	i = 0;
	n = 0;
	while ((s[i] >= 9 && s[i] <= 13) || s[i] == ' ')
		i++;
	if (s[i] == '+')
		i++;
	while (s[i] >= '0' && s[i] <= '9')
	{
		n = (n * 10) + s[i] - '0';
		i++;
	}
	if (s[i] || n > INT_MAX)
		return (-1);
	return (n);
}

size_t	ft_strlen(char *s)
{
	size_t	i;

	i = 0;
	while (s[i])
		i++;
	return (i);
}

int	ft_error(char *s)
{
	write(2, s, ft_strlen(s));
	write(2, "\n", 1);
	return (1);
}

long	get_time(void)
{
	struct timeval	tv;

	if (gettimeofday(&tv, NULL))
		return (-1);
	return ((tv.tv_sec * 1000) + (tv.tv_usec / 1000));
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

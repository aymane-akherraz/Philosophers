/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   free_funcs.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aakherra <aakherra@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/20 09:37:28 by aakherra          #+#    #+#             */
/*   Updated: 2025/04/21 15:50:25 by aakherra         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

int	free_philos(t_philo *p)
{
	free(p);
	return (1);
}

void	destroy_forks(pthread_mutex_t *mutex, int i)
{	
	while (i)
	{
		pthread_mutex_destroy(&mutex[i]);
		i--;
	}
	pthread_mutex_destroy(&mutex[i]);
	free(mutex);
}

void	destroy_philos(t_philo *philos, int i)
{
	while (i)
	{
		pthread_mutex_destroy(&philos[i].mutex);
		i--;
	}
	pthread_mutex_destroy(&philos[i].mutex);
	free(philos);
}

void	destroy_and_free(t_data *p, t_info *info)
{
	destroy_forks(p->forks, info->num_of_philos - 1);
	destroy_philos(p->philos, info->num_of_philos - 1);
	free(p->forks);
	free(p->philos);
}

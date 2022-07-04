/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   mortician.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: eshana <eshana@student.21-school.ru>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/01/27 18:13:18 by eshana            #+#    #+#             */
/*   Updated: 2022/01/27 18:16:15 by eshana           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

static int	ft_check_death(t_state *st)
{
	int			i;
	u_int64_t	time;

	i = 0;
	while (i < st->number_of_philos)
	{
		pthread_mutex_lock(&st->philos[i].dead);
		time = ft_gettimestamp(st) - st->philos[i].last_eat;
		if (time >= st->time_to_die)
		{
			pthread_mutex_lock(&st->ctrl_mtx);
			st->philos[i].alive = 0;
			st->death = 1;
			st->limit = 2;
			ft_postmortem(st, i + 1, MSG_DIED);
			ft_usleep(5000);
			pthread_mutex_unlock(&st->ctrl_mtx);
			pthread_mutex_unlock(&st->philos[i].dead);
			return (1);
		}
		pthread_mutex_unlock(&st->philos[i].dead);
		i++;
	}
	return (0);
}

static void	ft_check_eatings(t_state *st)
{
	int	i;

	i = 0;
	pthread_mutex_lock(&st->ctrl_mtx);
	while (i < st->number_of_philos && !st->death && st->limit < 2)
	{
		if (st->philos[i].ready == 0)
			break ;
		pthread_mutex_unlock(&st->ctrl_mtx);
		i++;
		if (i == st->number_of_philos)
		{
			pthread_mutex_lock(&st->ctrl_mtx);
			st->limit = 2;
			pthread_mutex_unlock(&st->ctrl_mtx);
		}
		pthread_mutex_lock(&st->ctrl_mtx);
	}
	pthread_mutex_unlock(&st->ctrl_mtx);
}

void	*ft_mortician(void *state)
{
	t_state	*st;

	st = (t_state *)state;
	pthread_mutex_lock(&st->mort_check_mtx);
	while (!st->mortician_done)
	{
		pthread_mutex_unlock(&st->mort_check_mtx);
		if (ft_check_death(st))
			return (NULL);
		ft_check_eatings(st);
		ft_usleep(600);
		pthread_mutex_lock(&st->mort_check_mtx);
	}
	pthread_mutex_unlock(&st->mort_check_mtx);
	return (NULL);
}

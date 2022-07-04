/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   waiter.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: eshana <eshana@student.21-school.ru>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/01/27 18:18:03 by eshana            #+#    #+#             */
/*   Updated: 2022/01/27 20:16:37 by eshana           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

static void	ft_unlock_next_queue(t_state *st)
{
	int	i;
	int	j;

	i = 0;
	j = st->queue_start;
	pthread_mutex_lock(&st->ctrl_mtx);
	while ((i < st->number_of_philos / 2) && !st->death)
	{
		pthread_mutex_unlock(&st->ctrl_mtx);
		pthread_mutex_lock(&st->ctrl_mtx);
		st->philos[j].can_eat = 1;
		pthread_mutex_unlock(&st->ctrl_mtx);
		pthread_mutex_unlock(&st->philos[j].eating);
		j = (j + 2) % st->number_of_philos;
		i++;
		pthread_mutex_lock(&st->ctrl_mtx);
	}
	pthread_mutex_unlock(&st->ctrl_mtx);
}

static void	ft_wait_for_eating_philos(t_state *st)
{
	int	i;

	i = 0;
	pthread_mutex_lock(&st->ctrl_mtx);
	while (i < st->number_of_philos && !st->death)
	{
		pthread_mutex_unlock(&st->ctrl_mtx);
		pthread_mutex_lock(&st->ctrl_mtx);
		if (st->philos[i].can_eat == 1 && !st->death)
		{
			pthread_mutex_unlock(&st->ctrl_mtx);
			ft_usleep(100);
			pthread_mutex_lock(&st->ctrl_mtx);
			continue ;
		}
		i++;
	}
	pthread_mutex_unlock(&st->ctrl_mtx);
}

static void	ft_unlock_on_exit(t_state *st)
{
	int	i;

	i = 0;
	while (i < st->number_of_philos)
	{
		pthread_mutex_lock(&st->ctrl_mtx);
		st->philos[i].alive = 0;
		if (!st->philos[i].can_eat)
			pthread_mutex_unlock(&st->philos[i].eating);
		i++;
		pthread_mutex_unlock(&st->ctrl_mtx);
	}
	pthread_mutex_lock(&st->mort_check_mtx);
	st->mortician_done = 1;
	pthread_mutex_unlock(&st->mort_check_mtx);
}

static void	ft_lock_cur_queue(t_state *st)
{
	int	i;
	int	j;

	i = 0;
	j = st->queue_start;
	pthread_mutex_lock(&st->ctrl_mtx);
	while ((i < st->number_of_philos / 2) && !st->death)
	{
		pthread_mutex_unlock(&st->ctrl_mtx);
		pthread_mutex_lock(&st->philos[j].eating);
		j = (j + 2) % st->number_of_philos;
		i++;
		pthread_mutex_lock(&st->ctrl_mtx);
	}
	pthread_mutex_unlock(&st->ctrl_mtx);
}

void	*ft_waiter(void *state)
{
	t_state	*st;

	st = (t_state *)state;
	ft_lock_eatings(st);
	pthread_mutex_lock(&st->ctrl_mtx);
	while (st->threads_cnt != st->number_of_philos && !st->death)
	{
		pthread_mutex_unlock(&st->ctrl_mtx);
		ft_usleep(500);
		pthread_mutex_lock(&st->ctrl_mtx);
	}
	pthread_mutex_unlock(&st->ctrl_mtx);
	pthread_mutex_lock(&st->ctrl_mtx);
	while (!st->death && st->limit != 2)
	{
		pthread_mutex_unlock(&st->ctrl_mtx);
		st->queue_start = (st->queue_start + 1) % st->number_of_philos;
		ft_unlock_next_queue(st);
		ft_wait_for_eating_philos(st);
		ft_lock_cur_queue(st);
		pthread_mutex_lock(&st->ctrl_mtx);
	}
	pthread_mutex_unlock(&st->ctrl_mtx);
	ft_unlock_on_exit(st);
	return (NULL);
}

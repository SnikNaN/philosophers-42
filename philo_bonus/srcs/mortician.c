/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   mortician.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: eshana <eshana@student.21-school.ru>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/01/27 18:13:18 by eshana            #+#    #+#             */
/*   Updated: 2022/01/29 00:44:34 by eshana           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

static int	ft_launch_cleaning(t_state *st)
{
	int	i;

	sem_post(st->philo.ctrl);
	ft_postmortem(st, &st->philo, MSG_DIED);
	sem_post(st->philo.dead);
	ft_msleep(1000);
	i = 0;
	while (i < st->number_of_philos)
	{
		sem_post(st->fed);
		i++;
	}
	return (1);
}

static int	ft_check_death(t_state *st)
{
	size_t	time;

	while (1)
	{
		sem_wait(st->philo.ctrl);
		if (!st->philo.alive)
		{
			sem_post(st->philo.ctrl);
			break ;
		}
		sem_post(st->philo.ctrl);
		sem_wait(st->checker);
		sem_wait(st->philo.dead);
		sem_wait(st->philo.ctrl);
		time = ft_gettimestamp(st->start_time);
		if (st->philo.alive && time - st->philo.last_eat >= st->time_to_die)
			return (ft_launch_cleaning(st));
		sem_post(st->philo.ctrl);
		sem_post(st->philo.dead);
		sem_post(st->checker);
		ft_msleep(1);
	}
	return (0);
}

void	*ft_mortician(void *state)
{
	t_state	*st;

	st = (t_state *)state;
	ft_check_death(st);
	return (NULL);
}

void	*ft_cleaner(void *state)
{
	t_state	*st;

	st = (t_state *)state;
	sem_wait(st->clean);
	sem_wait(st->philo.ctrl);
	st->philo.alive = 0;
	sem_post(st->philo.ctrl);
	sem_post(st->death);
	sem_post(st->forks);
	return (0);
}

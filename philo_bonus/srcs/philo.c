/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: eshana <eshana@student.21-school.ru>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/01/20 23:40:31 by eshana            #+#    #+#             */
/*   Updated: 2022/01/29 00:49:33 by eshana           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

static void	ft_eating(t_philo *phil)
{
	sem_wait(phil->state->eat);
	sem_wait(phil->state->forks);
	ft_putmsg(phil->state, phil, MSG_FORK, 0);
	sem_wait(phil->state->forks);
	ft_putmsg(phil->state, phil, MSG_FORK, 0);
	ft_putmsg(phil->state, phil, MSG_EAT, 1);
	sem_wait(phil->ctrl);
	if (phil->alive)
	{
		sem_post(phil->ctrl);
		ft_msleep(phil->state->time_to_eat);
	}
	else
		sem_post(phil->ctrl);
	if (phil->state->limit && !phil->state->number_of_eats--)
		sem_post(phil->state->fed);
	sem_post(phil->state->forks);
	sem_post(phil->state->forks);
	sem_post(phil->state->eat);
}

void	*ft_philo(t_philo *phil)
{
	if (phil->num != 1)
		ft_msleep(phil->state->time_to_eat / 2);
	sem_wait(phil->ctrl);
	while (phil->alive)
	{
		sem_post(phil->ctrl);
		ft_eating(phil);
		ft_putmsg(phil->state, phil, MSG_SLEEP, 0);
		ft_msleep(phil->state->time_to_sleep);
		ft_putmsg(phil->state, phil, MSG_THINK, 0);
		sem_wait(phil->ctrl);
	}
	sem_post(phil->ctrl);
	pthread_join(phil->state->cleaner, NULL);
	pthread_join(phil->state->mortician, NULL);
	sem_close(phil->state->clean);
	sem_close(phil->state->forks);
	sem_close(phil->state->writer);
	sem_close(phil->state->fed);
	sem_close(phil->state->death);
	sem_close(phil->state->eat);
	sem_close(phil->state->checker);
	sem_close(phil->dead);
	sem_close(phil->ctrl);
	exit(1);
}

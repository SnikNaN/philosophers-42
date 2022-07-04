/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   fed_monitor.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: eshana <eshana@student.21-school.ru>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/01/28 13:00:24 by eshana            #+#    #+#             */
/*   Updated: 2022/01/29 00:35:56 by eshana           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

int	ft_fed_monitor(t_state *st)
{
	int	i;

	i = 0;
	while (i++ < st->number_of_philos)
		sem_wait(st->fed);
	i = 0;
	while (i++ < st->number_of_philos)
		sem_post(st->clean);
	i = 0;
	while (i++ < st->number_of_philos)
		sem_wait(st->death);
	i = 0;
	while (i++ < st->number_of_philos)
		sem_post(st->writer);
	sem_post(st->checker);
	sem_close(st->forks);
	sem_close(st->writer);
	sem_close(st->fed);
	sem_close(st->death);
	sem_close(st->checker);
	sem_close(st->eat);
	sem_close(st->clean);
	exit(1);
}

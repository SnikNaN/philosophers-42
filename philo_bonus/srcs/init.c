/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   init.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: eshana <eshana@student.21-school.ru>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/01/27 18:22:25 by eshana            #+#    #+#             */
/*   Updated: 2022/01/28 21:27:49 by eshana           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

void	ft_sem_unlink(const t_state *st)
{
	if (st->forks != SEM_FAILED)
		sem_unlink("/forks");
	if (st->death != SEM_FAILED)
		sem_unlink("/death");
	if (st->checker != SEM_FAILED)
		sem_unlink("/checker");
	if (st->eat != SEM_FAILED)
		sem_unlink("/eat");
	if (st->writer != SEM_FAILED)
		sem_unlink("/writer");
	if (st->clean != SEM_FAILED)
		sem_unlink("/clean");
	if (st->fed != SEM_FAILED)
		sem_unlink("/fed");
}

int	ft_init_sems(t_state *st)
{
	st->forks = sem_open("/forks", O_CREAT | O_EXCL, 0, st->number_of_philos);
	st->death = sem_open("/death", O_CREAT | O_EXCL, 0, 1);
	st->eat = sem_open("/eat", O_CREAT | O_EXCL, 0,
			st->number_of_philos / 2 + st->number_of_philos % 2);
	st->checker = sem_open("/checker", O_CREAT | O_EXCL, 0, 1);
	st->writer = sem_open("/writer", O_CREAT | O_EXCL, 0, 1);
	st->clean = sem_open("/clean", O_CREAT | O_EXCL, 0, 0);
	st->fed = sem_open("/fed", O_CREAT | O_EXCL, 0, 0);
	ft_sem_unlink(st);
	if (st->forks == SEM_FAILED || st->death == SEM_FAILED
		|| st->eat == SEM_FAILED || st->checker == SEM_FAILED
		|| st->writer == SEM_FAILED || st->clean == SEM_FAILED
		|| st->fed == SEM_FAILED)
		exit(ft_error("SEM_FAILED\n"));
	return (0);
}

int	ft_init_state(t_state *st, int argc, char **argv)
{
	if (argc != 5 && argc != 6)
		return (ft_error("wrong arguments\n"));
	st->number_of_eats = 1;
	st->limit = 0;
	st->number_of_philos = ft_atoi(argv[1]);
	st->time_to_die = ft_atoi(argv[2]);
	st->time_to_eat = ft_atoi(argv[3]);
	st->time_to_sleep = ft_atoi(argv[4]);
	st->death = 0;
	st->queue_start = -1;
	st->philo.alive = 1;
	st->philo.state = st;
	if (argc == 6)
	{
		st->number_of_eats = ft_atoi(argv[5]);
		st->limit = 1;
	}
	if (!(st->number_of_philos && st->number_of_philos < 201 && st->time_to_die
			&& st->time_to_eat && st->time_to_sleep && st->number_of_eats))
		return (ft_error("wrong arguments\n"));
	st->number_of_eats--;
	ft_init_sems(st);
	return (0);
}

/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   init.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: eshana <eshana@student.21-school.ru>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/01/27 18:22:25 by eshana            #+#    #+#             */
/*   Updated: 2022/01/27 18:39:16 by eshana           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

int	ft_init_forks(t_state *state)
{
	int	i;

	i = 0;
	state->forks = (pthread_mutex_t *)malloc(sizeof(pthread_mutex_t)
			* state->number_of_philos);
	if (!state->forks)
		return (ft_error("malloc fail\n"));
	memset(state->forks, 0, sizeof(pthread_mutex_t) * state->number_of_philos);
	while (i < state->number_of_philos)
	{
		if (pthread_mutex_init(&state->forks[i], NULL))
		{
			free(state->forks);
			state->forks = NULL;
			return (ft_error("mutex fail\n"));
		}
		i++;
	}
	return (0);
}

static void	ft_philos_set_values(t_philo *philo)
{
	philo->thread = 0;
	philo->can_eat = 0;
	philo->ready = 0;
	philo->alive = 1;
	philo->last_eat = 0;
}

int	ft_init_philos(t_state *st)
{
	int	i;
	int	ret;

	ret = 0;
	st->philos = (t_philo *)malloc(sizeof(t_philo) * st->number_of_philos);
	if (!st->philos)
		return (ft_error("malloc fail\n"));
	memset(st->philos, 0, sizeof(t_philo) * st->number_of_philos);
	ret += pthread_mutex_init(&st->write_stream, NULL);
	ret += pthread_mutex_init(&st->ctrl_mtx, NULL);
	ret += pthread_mutex_init(&st->mort_check_mtx, NULL);
	i = 0;
	while (i < st->number_of_philos)
	{
		ft_philos_set_values(&st->philos[i]);
		st->philos[i].num = i + 1;
		st->philos[i].fork_l = i;
		st->philos[i].fork_r = (i + 1) % st->number_of_philos;
		st->philos[i].state = st;
		ret += pthread_mutex_init(&st->philos[i].eating, NULL);
		ret += pthread_mutex_init(&st->philos[i++].dead, NULL);
	}
	if (ret)
		return (ft_error("mutex init fail"));
	return (0);
}

int	ft_init_state(t_state *st, int argc, char **argv)
{
	if (argc != 5 && argc != 6)
		return (ft_error("wrong arguments\n"));
	st->forks = NULL;
	st->philos = NULL;
	st->number_of_eats = 1;
	st->limit = 0;
	st->number_of_philos = ft_atoi(argv[1]);
	st->time_to_die = ft_atoi(argv[2]);
	st->time_to_eat = ft_atoi(argv[3]);
	st->time_to_sleep = ft_atoi(argv[4]);
	st->death = 0;
	st->queue_start = -1;
	if (argc == 6)
	{
		st->number_of_eats = ft_atoi(argv[5]);
		st->limit = 1;
	}
	if (!(st->number_of_philos && st->time_to_die
			&& st->time_to_eat && st->time_to_sleep && st->number_of_eats))
		return (ft_error("wrong arguments\n"));
	return (0);
}

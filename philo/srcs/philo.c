/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: eshana <eshana@student.21-school.ru>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/01/20 23:40:31 by eshana            #+#    #+#             */
/*   Updated: 2022/01/27 18:35:32 by eshana           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

static void	ft_wait_on_start(t_philo *phil)
{
	pthread_mutex_lock(&phil->state->ctrl_mtx);
	phil->state->threads_cnt++;
	pthread_mutex_unlock(&phil->state->ctrl_mtx);
	pthread_mutex_lock(&phil->state->ctrl_mtx);
	while (!phil->can_eat && phil->alive)
	{
		pthread_mutex_unlock(&phil->state->ctrl_mtx);
		usleep(100);
		pthread_mutex_lock(&phil->state->ctrl_mtx);
	}
	pthread_mutex_unlock(&phil->state->ctrl_mtx);
}

static void	ft_eating(t_philo *phil)
{
	pthread_mutex_lock(&phil->eating);
	pthread_mutex_lock(&phil->state->forks[phil->fork_l]);
	ft_putmsg(phil->state, phil->num, MSG_FORK);
	pthread_mutex_lock(&phil->state->forks[phil->fork_r]);
	ft_putmsg(phil->state, phil->num, MSG_FORK);
	pthread_mutex_lock(&phil->dead);
	ft_putmsg(phil->state, phil->num, MSG_EAT);
	phil->last_eat = ft_gettimestamp(phil->state);
	pthread_mutex_unlock(&phil->dead);
	ft_usleep(phil->state->time_to_eat * 1000);
	phil->eats_cnt++;
	pthread_mutex_lock(&phil->state->ctrl_mtx);
	if (!phil->ready && phil->state->limit
		&& phil->eats_cnt >= phil->state->number_of_eats)
		phil->ready = 1;
	pthread_mutex_unlock(&phil->state->ctrl_mtx);
	pthread_mutex_unlock(&phil->state->forks[phil->fork_l]);
	pthread_mutex_unlock(&phil->state->forks[phil->fork_r]);
	pthread_mutex_unlock(&phil->eating);
	pthread_mutex_lock(&phil->state->ctrl_mtx);
	phil->can_eat = 0;
	pthread_mutex_unlock(&phil->state->ctrl_mtx);
}

static void	*ft_philo(void *philo)
{
	t_philo	*phil;

	phil = (t_philo *)philo;
	ft_wait_on_start(phil);
	pthread_mutex_lock(&phil->state->ctrl_mtx);
	while (phil->state->number_of_philos > 1 && phil->alive)
	{
		pthread_mutex_unlock(&phil->state->ctrl_mtx);
		ft_eating(phil);
		ft_putmsg(phil->state, phil->num, MSG_SLEEP);
		ft_usleep(phil->state->time_to_sleep * 1000);
		ft_putmsg(phil->state, phil->num, MSG_THINK);
		pthread_mutex_lock(&phil->state->ctrl_mtx);
		while (!phil->can_eat && phil->alive)
		{
			pthread_mutex_unlock(&phil->state->ctrl_mtx);
			usleep(100);
			pthread_mutex_lock(&phil->state->ctrl_mtx);
		}
		pthread_mutex_unlock(&phil->state->ctrl_mtx);
		pthread_mutex_lock(&phil->state->ctrl_mtx);
	}
	pthread_mutex_unlock(&phil->state->ctrl_mtx);
	return (NULL);
}

int	ft_run_philos(t_state *state)
{
	int		i;
	void	*philo;

	gettimeofday(&state->start_time, NULL);
	i = 0;
	philo = NULL;
	while (i < state->number_of_philos)
	{
		philo = (void *)(&state->philos[i]);
		if (pthread_create(&state->philos[i].thread, NULL, &ft_philo, philo))
			return (ft_error("thread creation fail\n"));
		i++;
	}
	return (0);
}

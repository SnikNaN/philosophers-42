/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: eshana <eshana@student.21-school.ru>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/01/27 18:21:52 by eshana            #+#    #+#             */
/*   Updated: 2022/01/27 19:10:26 by eshana           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

static int	ft_free(t_philo *ph, pthread_mutex_t *forks)
{
	if (ph)
		free(ph);
	if (forks)
		free(forks);
	return (1);
}

static int	ft_exit(t_state *state)
{
	int	i;

	pthread_join(state->mortician, NULL);
	pthread_join(state->waiter, NULL);
	i = 0;
	while (i < state->number_of_philos)
	{
		pthread_join(state->philos[i].thread, NULL);
		i++;
	}
	pthread_mutex_destroy(&state->mort_check_mtx);
	pthread_mutex_destroy(&state->ctrl_mtx);
	i = 0;
	while (i < state->number_of_philos)
	{
		pthread_mutex_destroy(&state->forks[i]);
		pthread_mutex_destroy(&state->philos[i].eating);
		pthread_mutex_destroy(&state->philos[i].dead);
		i++;
	}
	ft_free(state->philos, state->forks);
	pthread_mutex_destroy(&state->write_stream);
	return (0);
}

int	main(int argc, char **argv)
{
	t_state	state;

	memset(&state, 0, sizeof(state));
	if (ft_init_state(&state, argc, argv))
		return (1);
	if (ft_init_forks(&state) || ft_init_philos(&state))
		return (ft_free(state.philos, state.forks));
	if (pthread_create(&state.waiter, NULL, &ft_waiter, &state))
	{
		ft_free(state.philos, state.forks);
		return (ft_error("thread creation fail\n"));
	}
	if (ft_run_philos(&state))
		return (ft_free(state.philos, state.forks));
	if (pthread_create(&state.mortician, NULL, &ft_mortician, &state))
	{
		ft_free(state.philos, state.forks);
		return (ft_error("thread creation fail\n"));
	}
	exit(ft_exit(&state));
}

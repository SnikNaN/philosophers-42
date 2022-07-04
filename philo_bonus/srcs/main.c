/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: eshana <eshana@student.21-school.ru>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/01/27 18:21:52 by eshana            #+#    #+#             */
/*   Updated: 2022/01/28 14:30:26 by eshana           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

static void	ft_create_name(int n, char str[7])
{
	str[3] += n / 100;
	str[4] += n / 10 % 10;
	str[5] += n % 10;
}

static void	ft_fork(t_state *state, int i)
{
	char	str[7];

	memset(&str, '0', sizeof(char) * 7);
	str[0] = '/';
	str[1] = 'd';
	str[2] = '_';
	str[6] = '\0';
	state->philo.num = i + 1;
	ft_create_name(i, str);
	state->philo.dead = sem_open(str, O_CREAT | O_EXCL, 0, 1);
	if (state->philo.dead != SEM_FAILED)
		sem_unlink(str);
	else
		exit(ft_error("SEM_FAILED"));
	str[1] = 'w';
	state->philo.ctrl = sem_open(str, O_CREAT | O_EXCL, 0, 1);
	if (state->philo.ctrl != SEM_FAILED)
		sem_unlink(str);
	else
		exit(ft_error("SEM_FAILED"));
	if (pthread_create(&state->mortician, NULL, &ft_mortician, state))
		exit(ft_error("thread creation fail\n"));
	if (pthread_create(&state->cleaner, NULL, &ft_cleaner, state))
		exit(ft_error("thread creation fail\n"));
	ft_philo(&state->philo);
}

static void	ft_run_monitor(t_state *state, int *pid)
{
	*pid = fork();
	if (*pid == -1)
		exit(ft_error("fork() fail\n"));
	if (*pid == 0)
		ft_fed_monitor(state);
}

static void	ft_run_philos(t_state *state)
{
	int		i;
	int		status;
	int		pid[201];

	gettimeofday(&state->start_time, NULL);
	ft_run_monitor(state, &pid[200]);
	i = 0;
	while (i < state->number_of_philos)
	{
		pid[i] = fork();
		if (pid[i] == -1)
		{
			while (--i >= 0)
			{
				kill(pid[i], SIGTERM);
				waitpid(pid[i], &status, 0);
			}
			kill(pid[200], SIGTERM);
			waitpid(pid[200], &status, 0);
			exit(ft_error("fork() fail\n"));
		}
		if (!pid[i])
			ft_fork(state, i);
		i++;
	}
}

int	main(int argc, char **argv)
{
	t_state	state;
	int		status;
	int		i;

	memset(&state, 0, sizeof(state));
	if (ft_init_state(&state, argc, argv))
		return (1);
	ft_run_philos(&state);
	i = 0;
	while (i++ <= state.number_of_philos)
		waitpid(-1, &status, 0);
	sem_close(state.forks);
	sem_close(state.writer);
	sem_close(state.fed);
	sem_close(state.death);
	sem_close(state.clean);
	sem_close(state.eat);
	sem_close(state.checker);
	exit(EXIT_SUCCESS);
}

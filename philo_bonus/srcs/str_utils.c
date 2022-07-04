/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   str_utils.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: eshana <eshana@student.21-school.ru>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/01/21 09:27:37 by eshana            #+#    #+#             */
/*   Updated: 2022/01/29 00:52:25 by eshana           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

int	ft_atoi(const char *str)
{
	unsigned int	i;
	int				res;

	i = 0;
	res = 0;
	if (!str[i])
		return (-1);
	while (str[i])
	{
		if (!(str[i] >= '0' && str[i] <= '9'))
			return (0);
		res = res * 10 + str[i] - '0';
		i++;
	}
	return (res);
}

static void	ft_putstr_fd(int fd, const char *str)
{
	size_t	i;

	i = 0;
	while (str[i])
	{
		write(fd, &str[i], 1);
		i++;
	}
}

int	ft_error(const char *str)
{
	ft_putstr_fd(2, "Error: ");
	ft_putstr_fd(2, str);
	return (1);
}

void	ft_putmsg(t_state *state, t_philo *philo, const char *msg, int eat)
{
	size_t	timestamp;

	sem_wait(state->writer);
	sem_wait(philo->ctrl);
	if (philo->alive)
	{
		timestamp = ft_gettimestamp(state->start_time);
		printf("%lu %d %s", timestamp, philo->num, msg);
		if (eat)
			philo->last_eat = timestamp;
	}
	sem_post(philo->ctrl);
	sem_post(state->writer);
}

void	ft_postmortem(t_state *state, t_philo *philo, const char *msg)
{
	sem_wait(state->writer);
	sem_wait(philo->ctrl);
	if (philo->alive)
		printf("%lu %d %s",
			ft_gettimestamp(state->start_time), philo->num, msg);
	else
		sem_post(state->writer);
	sem_post(philo->ctrl);
}

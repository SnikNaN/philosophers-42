/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   str_utils.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: eshana <eshana@student.21-school.ru>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/01/21 09:27:37 by eshana            #+#    #+#             */
/*   Updated: 2022/01/27 19:28:08 by eshana           ###   ########.fr       */
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

void	ft_putmsg(t_state *state, int philo, const char *msg)
{
	pthread_mutex_lock(&state->ctrl_mtx);
	pthread_mutex_lock(&state->write_stream);
	if (!state->death && state->limit != 2)
		printf("%lu %d %s", ft_gettimestamp(state), philo, msg);
	pthread_mutex_unlock(&state->write_stream);
	pthread_mutex_unlock(&state->ctrl_mtx);
}

void	ft_postmortem(t_state *state, int philo, const char *msg)
{
	pthread_mutex_lock(&state->write_stream);
	printf("%lu %d %s", ft_gettimestamp(state), philo, msg);
	pthread_mutex_unlock(&state->write_stream);
}

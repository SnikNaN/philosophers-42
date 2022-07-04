/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   time.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: eshana <eshana@student.21-school.ru>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/01/27 18:28:03 by eshana            #+#    #+#             */
/*   Updated: 2022/01/27 18:37:49 by eshana           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

void	ft_usleep(size_t usec)
{
	struct timeval	start;
	struct timeval	end;

	gettimeofday(&start, NULL);
	end = start;
	while ((size_t)(end.tv_sec * 1000000 + end.tv_usec)
		< (size_t)(start.tv_sec * 1000000 + start.tv_usec) + usec)
	{
		gettimeofday(&end, NULL);
		usleep(200);
	}
}

u_int64_t	ft_gettimestamp(t_state *state)
{
	struct timeval	*x;
	struct timeval	*y;
	u_int64_t		result;

	x = &state->curr_time;
	y = &state->start_time;
	gettimeofday(x, NULL);
	result = (x->tv_sec * 1000 + x->tv_usec / 1000)
		- (y->tv_sec * 1000 + y->tv_usec / 1000);
	return (result);
}

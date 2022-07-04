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

void	ft_msleep(size_t msec)
{
	struct timeval	start;
	struct timeval	end;

	gettimeofday(&start, NULL);
	end = start;
	while ((size_t)(end.tv_sec * 1000 + end.tv_usec / 1000)
		- (size_t)(start.tv_sec * 1000 + start.tv_usec / 1000) < msec)
	{
		gettimeofday(&end, NULL);
		usleep(msec * 3);
	}
}

size_t	ft_gettimestamp(struct timeval start)
{
	struct timeval	x;
	struct timeval	y;
	size_t			result;

	y = start;
	gettimeofday(&x, NULL);
	result = (x.tv_sec * 1000 + x.tv_usec / 1000)
		- (y.tv_sec * 1000 + y.tv_usec / 1000);
	return (result);
}

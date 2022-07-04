/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   waiter_2.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: eshana <eshana@student.21-school.ru>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/01/27 18:18:03 by eshana            #+#    #+#             */
/*   Updated: 2022/01/27 20:16:37 by eshana           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

void	ft_lock_eatings(t_state *st)
{
	int	i;

	i = 0;
	while (i < st->number_of_philos)
	{
		pthread_mutex_lock(&st->philos[i].eating);
		i++;
	}
}

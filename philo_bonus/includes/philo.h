/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: eshana <eshana@student.21-school.ru>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/01/20 23:40:44 by eshana            #+#    #+#             */
/*   Updated: 2022/01/28 23:28:16 by eshana           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */
#ifndef PHILO_H
# define PHILO_H

# define MSG_EAT "is eating\n"
# define MSG_SLEEP "is sleeping\n"
# define MSG_FORK "has taken a fork\n"
# define MSG_THINK "is thinking\n"
# define MSG_DIED "died\n"

# include <stdio.h>
# include <stdlib.h>
# include <string.h>
# include <unistd.h>
# include <sys/stat.h>
# include <semaphore.h>
# include <sys/time.h>
# include <pthread.h>
# include <signal.h>
# include <sys/wait.h>
# include <fcntl.h>

typedef struct s_philo
{
	int				num;
	pid_t			pid;
	size_t			last_eat;
	size_t			eats_cnt;
	size_t			can_eat;
	size_t			ready;
	size_t			done;
	struct s_state	*state;
	int				alive;
	sem_t			*dead;
	sem_t			*ctrl;
}	t_philo;

typedef struct s_state
{
	int				number_of_philos;
	size_t			time_to_die;
	size_t			time_to_eat;
	size_t			time_to_sleep;
	size_t			number_of_eats;
	t_philo			philo;
	sem_t			*forks;
	sem_t			*writer;
	sem_t			*ready;
	sem_t			*fed;
	sem_t			*eat;
	sem_t			*checker;
	size_t			limit;
	struct timeval	start_time;
	struct timeval	curr_time;
	pthread_t		waiter;
	int				waiter_done;
	pthread_t		mortician;
	pthread_t		cleaner;
	sem_t			*death;
	sem_t			*clean;
	int				queue_start;
}	t_state;

int		ft_atoi(const char *str);
int		ft_error(const char *str);
void	ft_putmsg(t_state *state, t_philo *philo, const char *msg, int eat);
void	ft_postmortem(t_state *state, t_philo *philo, const char *msg);
size_t	ft_gettimestamp(struct timeval start);
void	*ft_mortician(void *state);
void	*ft_waiter(void *state);
void	ft_msleep(size_t msec);
int		ft_init_state(t_state *st, int argc, char **argv);
int		ft_init_philos(t_state *st);
int		ft_init_forks(t_state *state);
void	*ft_philo(t_philo *phil);
int		ft_fed_monitor(t_state *st);
void	*ft_cleaner(void *state);

#endif

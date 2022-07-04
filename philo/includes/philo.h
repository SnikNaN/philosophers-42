/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: eshana <eshana@student.21-school.ru>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/01/20 23:40:44 by eshana            #+#    #+#             */
/*   Updated: 2022/01/27 20:21:18 by eshana           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */
#ifndef PHILO_H
# define PHILO_H

# define MSG_EAT "is eating\n"
# define MSG_SLEEP "is sleeping\n"
# define MSG_FORK "has taken a fork\n"
# define MSG_THINK "is thinking\n"
# define MSG_DIED "died\n"

# include <unistd.h>
# include <pthread.h>
# include <sys/time.h>
# include <stdlib.h>
# include <string.h>
# include <stdio.h>

typedef struct s_philo
{
	pthread_t		thread;
	int				num;
	size_t			fork_l;
	size_t			fork_r;
	u_int64_t		last_eat;
	size_t			eats_cnt;
	size_t			can_eat;
	size_t			ready;
	pthread_mutex_t	eating;
	pthread_mutex_t	dead;
	struct s_state	*state;
	int				alive;
}	t_philo;

typedef struct s_state
{
	int				number_of_philos;
	int				threads_cnt;
	pthread_mutex_t	ctrl_mtx;
	u_int64_t		time_to_die;
	size_t			time_to_eat;
	size_t			time_to_sleep;
	size_t			number_of_eats;
	t_philo			*philos;
	pthread_mutex_t	*forks;
	pthread_mutex_t	write_stream;
	size_t			limit;
	struct timeval	start_time;
	struct timeval	curr_time;
	pthread_t		waiter;
	pthread_t		mortician;
	int				mortician_done;
	pthread_mutex_t	mort_check_mtx;
	int				death;
	int				queue_start;
}	t_state;

int			ft_atoi(const char *str);
int			ft_error(const char *str);
void		ft_putmsg(t_state *state, int philo, const char *msg);
void		ft_postmortem(t_state *state, int philo, const char *msg);
u_int64_t	ft_gettimestamp(t_state *state);
void		*ft_mortician(void *state);
void		*ft_waiter(void *state);
void		ft_lock_eatings(t_state *st);
void		ft_usleep(size_t usec);
int			ft_init_state(t_state *st, int argc, char **argv);
int			ft_init_philos(t_state *st);
int			ft_init_forks(t_state *state);
int			ft_run_philos(t_state *state);

#endif

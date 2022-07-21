NAME	= philo

SRCS	=	srcs/main.c	srcs/check_integers.c srcs/utils.c srcs/init_philos.c srcs/init_threads.c \
			srcs/philo_actions.c 	srcs/philo_sleep.c	srcs/philo_eat.c srcs/philo_forks.c	\
			srcs/take_first_fork.c \

OBJS = $(SRCS:.c=.o)
CC = gcc
RM = rm -f
CFLAGS = -Wall -Wextra -Werror
INC = -I./includes

.c.o:
		$(CC) $(CFLAGS) $(INC) -c $< -o $(<:.c=.o)

all:	$(NAME)

$(NAME):	$(OBJS)
		$(CC) $(CFLAGS) $(OBJS) -lpthread -D_REENTRANT -l c -o $(NAME)

clean:
		$(RM) $(OBJS)

fclean:	clean
		$(RM) $(NAME)

re: fclean all

.PHONY: all clean fclean re

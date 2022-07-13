NAME	= philo

SRCS	=	srcs/main.c	srcs/check_integers.c srcs/utils.c srcs/init_philos.c srcs/init_threads.c srcs/philo_actions.c 	\

OBJS = $(SRCS:.c=.o)
CC = gcc
RM = rm -f
CFLAGS = -Wall -Wextra -Werror #-fsanitize=thread
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

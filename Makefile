NAME = philo
CC = cc
CFLAGS = -Wall -Wextra -Werror
RM = rm -f
SRC = philo.c helper_funcs.c forks_funcs.c monitor_funcs.c routine_funcs.c
OBJ = $(SRC:%.c=%.o)

all: $(NAME)

$(NAME): $(OBJ)
	$(CC) -pthread -o $@ $^

%.o: %.c
	$(CC) $(CFLAGS) -pthread -c $< -o $@

clean:
	$(RM) $(OBJ)

fclean: clean
	$(RM) $(NAME)

re: fclean all

.PHONY:  all clean fclean re bonus

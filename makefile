SRC = main.c
OBJ = main.o
CC = gcc
CFLAGS = -Wall -Wextra -Werror -fsanitize=address -g


NAME = philo
all: $(OBJ)
	$(CC) $(CFLAGS) $(OBJ) -o $(NAME)

clean:
	rm -f $(OBJ)

fclean: clean
	rm -f $(NAME)

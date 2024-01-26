# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: rlandolt <rlandolt@student.42lisboa.com    +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2024/01/15 18:21:04 by rlandolt          #+#    #+#              #
#    Updated: 2024/01/25 12:06:51 by rlandolt         ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

NAME = philo

SRC_PATH = ./srcs

SRC = $(SRC_PATH)/main.c \
	$(SRC_PATH)/parse.c \
	$(SRC_PATH)/data.c \
	$(SRC_PATH)/time.c \
	$(SRC_PATH)/routine.c \
	$(SRC_PATH)/status.c \
	$(SRC_PATH)/forks.c

SANITIZER = -g -fsanitize=thread

CC = cc -pthread
CFLAGS = -Wall -Wextra -Werror

OBJ = $(SRC:.c=.o)

all: $(NAME)

$(NAME): $(OBJ)
		$(CC) $(CFLAGS) -o $(NAME) $(OBJ)

clean:
		rm -f $(OBJ)

fclean: clean
		rm -f $(NAME)

debug: CFLAGS += ${SANITIZER}
debug: re

re: fclean all

.PHONY: all, clean, fclean, re

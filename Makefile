# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: rlandolt <rlandolt@student.42lisboa.com    +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2024/01/15 18:21:04 by rlandolt          #+#    #+#              #
#    Updated: 2024/01/20 18:42:52 by rlandolt         ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

NAME = philosophers.a

SRC_PATH = ./srcs

SRC = $(SRC_PATH)/main.c \
	$(SRC_PATH)/parse.c \
	$(SRC_PATH)/operations.c \
	$(SRC_PATH)/structs.c \
	$(SRC_PATH)/time.c \
	$(SRC_PATH)/threads.c \
	$(SRC_PATH)/monitor.c \
	$(SRC_PATH)/routines.c

SANITIZER = -fsanitize=thread

CC = cc -g -pthread
CFLAGS = -Wall -Wextra -Werror

OBJ = $(SRC:.c=.o)

$(NAME): $(OBJ)
		@ar -rcs $@ $(OBJ)
		@${CC} ${CFLAGS} -o philosophers ${NAME} ${SANITIZER}
		@rm -f $(NAME)

all: $(NAME)

clean:
		rm -f $(OBJ)

fclean: clean
		rm -f $(NAME)

re: fclean all

.PHONY: all, clean, fclean, re

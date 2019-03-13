# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: lbopp <marvin@42.fr>                       +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2019/03/12 10:03:24 by lbopp             #+#    #+#              #
#    Updated: 2019/03/13 16:42:16 by lbopp            ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

NAME = ft_nm
CFLAGS = -Wall -Werror -Wextra
LIBFT = libft/
CC = clang
SRC_NAME = ft_nm.c
SRC_PATH = srcs
SRC = $(addprefix $(SRC_PATH)/,$(SRC_NAME))
OBJ = $(SRC:.c=.o)
RM = rm -f

.PHONY: all, clean, fclean, re

all: $(NAME)

$(NAME): $(OBJ)
	make -C libft
	$(CC) $(CFLAGS) -o $(NAME) $(OBJ) -I includes -I libft/includes -lft -L libft

%.o: %.c
	$(CC) $(CFLAGS) -c $^ -o $@ -I includes -I libft/includes

clean:
	make -C libft clean

fclean: clean
	make -C libft fclean
	$(RM) $(NAME)

re: fclean all

# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: lbopp <marvin@42.fr>                       +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2019/03/12 10:03:24 by lbopp             #+#    #+#              #
#    Updated: 2019/05/10 18:02:05 by lbopp            ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

NAME = ft_nm
CFLAGS = -Wall -Werror -Wextra -g -fsanitize=address
LIBFT = libft/
CC = clang
SRC_NAME = nm/ft_nm.c\
		   nm/fill_section.c\
		   nm/tools1.c\
		   nm/tools2.c\
		   nm/fill_data.c\
		   nm/handle_macho.c\
		   nm/browse_symtab.c\
		   nm/get_cpu_type.c\
		   nm/print.c\
		   nm/qs_data.c
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
	$(RM) $(OBJ)

fclean: clean
	make -C libft fclean
	$(RM) $(NAME)

re: fclean all

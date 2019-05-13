# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: lbopp <marvin@42.fr>                       +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2019/03/12 10:03:24 by lbopp             #+#    #+#              #
#    Updated: 2019/05/13 14:25:30 by lbopp            ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

NM = ft_nm
OTOOL = ft_otool
CFLAGS = -Wall -Werror -Wextra -g -fsanitize=address
LIBFT = libft/
CC = clang
SRC_NAME_NM = nm/ft_nm.c\
			  nm/fill_section.c\
			  nm/tools1.c\
		   	  nm/tools2.c\
		   	  nm/fill_data.c\
		   	  nm/handle_macho.c\
		   	  nm/handle_fat.c\
		   	  nm/handle_arch.c\
		   	  nm/browse_symtab.c\
		   	  nm/get_cpu_type.c\
		   	  nm/search_host.c\
		   	  nm/print.c\
		   	  nm/qs_data.c\
		   	  nm/launch_nm.c
SRC_NAME_OTOOL = otool/ft_otool.c\
				 otool/handle_macho.c\
		   	  	 otool/handle_fat.c\
		   	  	 otool/handle_arch.c\
				 otool/search_host.c\
				 otool/launch_otool.c\
				 otool/tools1.c\
		   	  	 otool/tools2.c\
				 otool/browse_symtab.c\
				 otool/get_cpu_type.c
SRC_PATH = srcs
SRC_NM = $(addprefix $(SRC_PATH)/,$(SRC_NAME_NM))
OBJ_NM = $(SRC_NM:.c=.o)
SRC_OTOOL = $(addprefix $(SRC_PATH)/,$(SRC_NAME_OTOOL))
OBJ_OTOOL = $(SRC_OTOOL:.c=.o)
RM = rm -f

.PHONY: all, clean, fclean, re

all: nm_otool

nm_otool: $(OBJ_NM) $(OBJ_OTOOL)
	make -C libft
	$(CC) $(CFLAGS) -o $(NM) $(OBJ_NM) -I includes/ft_nm.h -I libft/includes -lft -L libft
	$(CC) $(CFLAGS) -o $(OTOOL) $(OBJ_OTOOL) -I includes/ft_otool.h -I libft/includes -lft -L libft

%.o: %.c
	$(CC) $(CFLAGS) -c $^ -o $@ -I includes -I libft/includes

clean:
	make -C libft clean
	$(RM) $(OBJ_NM)
	$(RM) $(OBJ_OTOOL)

fclean: clean
	make -C libft fclean
	$(RM) $(NM)
	$(RM) $(OTOOL)

re: fclean all

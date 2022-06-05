# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: pirabaud <pirabaud@student.42angoulem      +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2022/05/27 16:34:26 by pirabaud          #+#    #+#              #
#    Updated: 2022/05/31 16:45:23 by pirabaud         ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

SRCS 	= main.c

OBJS	= ${SRCS:.c=.o}

NAME	= pipex

CC 		= gcc -g -opipex

RM		= rm -f

CFLAGS	= -Wall -Wextra -Werror -I . -I libft

$(NAME):	$(OBJS)
	make -sC libft
	$(CC) $(CFLAGS) $(OBJS) -L ./libft -lft

all:	$(NAME)

clean:
	${RM} ${OBJS}
	make clean -sC libft

fclean: clean
	${RM} ${NAME}
	make fclean -sC libft

re: fclean all

.PHONY: all clean fclean re

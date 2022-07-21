# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: pirabaud <pirabaud@student.42angoulem      +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2022/05/27 16:34:26 by pirabaud          #+#    #+#              #
#    Updated: 2022/07/18 15:01:23 by pirabaud         ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

SRCS 	= main.c call.c check_path.c init_cmd.c check_error.c here_doc.c

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

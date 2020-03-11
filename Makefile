# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: alkozma <marvin@42.fr>                     +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2019/03/13 18:06:36 by alkozma           #+#    #+#              #
#    Updated: 2020/03/11 08:52:06 by alkozma          ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

NAME = patchy

SRCS = fileparser.c

OBJECTS = $(patsubst %.c,%.o,$(SRCS))

INCLUDES = .

FLAGS = -Wall -Wextra -Werror

all: $(NAME)

$(NAME):
	gcc $(SRCS) -I$(INCLUDES) $(FLAGS) -o $(NAME)

clean:
	rm -rf $(OBJECTS)

fclean: clean
	rm -rf $(NAME)

re: fclean all

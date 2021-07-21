# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: tpetit <tpetit@student.s19.be>             +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2021/07/19 15:53:42 by ldelmas           #+#    #+#              #
#    Updated: 2021/07/21 15:03:49 by tpetit           ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

NAME = minishell

CC = gcc

#CFLAGS = -Wall -Wextra -Werror -I includes/

CFLAGS = -I includes/ 

HEADER = minishell.h

MAIN = main

PIPE = pipe exec

UTIL = basics

PARSE = parsing

SRC =	${addsuffix .c, ${addprefix srcs/, ${MAIN}}} \
		${addsuffix .c, ${addprefix srcs/pipe/, ${PIPE}}} \
		${addsuffix .c, ${addprefix srcs/utils/, ${UTIL}}} \
		${addsuffix .c, ${addprefix srcs/parsing/, ${PARSE}}}

OBJ = ${SRC:c=o}

%.o: %.c
	@echo "Compiling binaries..."
	@${CC} ${CFLAGS} -c $< -o $@
	@echo "Binary files created."
	
all : ${NAME}

${NAME}: ${OBJ}
	@echo "\nMinishell is compiling..."
	@${CC} ${CFLAGS} -o ${NAME} ${OBJ} -lreadline
	@echo "Minishell is ready!"

clean :
	@echo "Removing binaries..."
	@rm -f ${OBJ}
	@echo "Binaries removed."

fclean : clean
	@echo "\nRemoving minishell executable..."
	@rm -f ${NAME}
	@echo "Minishell removed."
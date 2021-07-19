# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: ldelmas <ldelmas@student.42.fr>            +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2021/07/19 15:53:42 by ldelmas           #+#    #+#              #
#    Updated: 2021/07/19 17:46:40 by ldelmas          ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

NAME = minishell

CC = gcc

CFLAGS = -Wall -Wextra -Werror -I includes/ 

HEADER = minishell.h

MAIN = main

PIPE = pipe

UTIL = basics

SRC = ${addsuffix .c, ${addprefix srcs/, ${MAIN}}} \
	${addsuffix .c, ${addprefix srcs/pipe/, ${PIPE}}} \
	${addsuffix .c, ${addprefix srcs/utils/, ${UTIL}}}

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
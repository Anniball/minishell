# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: ldelmas <ldelmas@student.42.fr>            +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2021/07/19 15:53:42 by ldelmas           #+#    #+#              #
#    Updated: 2021/07/19 16:10:58 by ldelmas          ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

NAME = minishell

CC = gcc

CFLAGS = -Wall -Wextra -Werror -lreadline -I includes/

HEADER = minishell.h

MAIN = main

PIPE = pipe

SRC = ${addsuffix .c, ${addprefix srcs/, ${MAIN}}} \
	${addsuffix .c, ${addprefix srcs/pipe/, ${PIPE}}}

OBJ = ${SRC:c=o}

%.c: %.o
	@echo "\nCompiling binaries..."
	@${CC} ${CFLAGS} -c $< -o $@
	@echo "\nBinary files created."
	
all : ${NAME}

${NAME}: ${OBJ}
	@echo "\nMinishell is compiling..."
	@${CC} ${CFLAGS} -o ${NAME} ${OBJ}
	@echo "\nMinishell is ready!"

clean :
	@echo "\nRemoving binaries..."
	@rm -f ${OBJ}
	@echo "\nBinaries removed."

fclean : clean
	@echo "\nRemoving minishell executable..."
	@rm -f ${NAME}
	@echo "\nMinishell removed."
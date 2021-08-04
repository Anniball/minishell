# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: ldelmas <ldelmas@student.42.fr>            +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2021/07/19 15:53:42 by ldelmas           #+#    #+#              #
#    Updated: 2021/08/04 15:05:54 by ldelmas          ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

NAME = minishell

CC = gcc

#CFLAGS = -Wall -Wextra -Werror -I includes/

CFLAGS = -I includes/ -L /Users/$(USER)/.brew/opt/readline/lib -I/Users/$(USER)/.brew/opt/readline/include

# RFLAGS = -L /Users/$(USER)/.brew/opt/readline/lib -I/Users/$(USER)/.brew/opt/readline/include

HEADER = minishell.h

MAIN = main

PIPE = pipe exec command n_pipe builtins

UTIL = basics cmd_lst str_lst env_utils str_utils

PARSE = parsing parsing_print parsing_basic_utils parsing_split

REBUILT = pwd echo env cd export unset exit

SIGNAL = signals

SRC =	${addsuffix .c, ${addprefix srcs/, ${MAIN}}} \
		${addsuffix .c, ${addprefix srcs/pipe/, ${PIPE}}} \
		${addsuffix .c, ${addprefix srcs/utils/, ${UTIL}}} \
		${addsuffix .c, ${addprefix srcs/parsing/, ${PARSE}}} \
		${addsuffix .c, ${addprefix srcs/rebuilt/, ${REBUILT}}} \
		${addsuffix .c, ${addprefix srcs/signals/, ${SIGNAL}}}

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

re: clean all
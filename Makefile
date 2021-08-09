# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: ldelmas <ldelmas@student.42.fr>            +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2021/07/19 15:53:42 by ldelmas           #+#    #+#              #
<<<<<<< HEAD
#    Updated: 2021/08/06 15:38:45 by tpetit           ###   ########.fr        #
=======
#    Updated: 2021/08/06 15:49:26 by ldelmas          ###   ########.fr        #
>>>>>>> multifiles_test
#                                                                              #
# **************************************************************************** #

NAME = minishell

CC = gcc

#CFLAGS = -Wall -Wextra -Werror -I includes/

CFLAGS = -I includes/

RPATH = /Users/$(USER)/.brew/opt/readline/

RFLAGS = -L $(RPATH)lib -I $(RPATH)include -Wno-unused-command-line-argument

HEADER = minishell.h

MAIN = main

PIPE = exec command n_pipe builtins multifiles

UTIL = basics cmd_lst str_lst env_utils str_utils basics2

PARSE = parsing parsing_print parsing_basic_utils parsing_split parsing_utils check_line

REBUILT = pwd echo env cd export unset exit

SIGNAL = signals

SRC =	${addsuffix .c, ${addprefix srcs/, ${MAIN}}} \
		${addsuffix .c, ${addprefix srcs/pipe/, ${PIPE}}} \
		${addsuffix .c, ${addprefix srcs/utils/, ${UTIL}}} \
		${addsuffix .c, ${addprefix srcs/parsing/, ${PARSE}}} \
		${addsuffix .c, ${addprefix srcs/rebuilt/, ${REBUILT}}} \
		${addsuffix .c, ${addprefix srcs/signals/, ${SIGNAL}}}

OBJ = ${SRC:c=o}

START = 0

%.o: %.c
	@${CC} ${CFLAGS} ${RFLAGS} -c $< -o $@ 
	
all : ${NAME}

${NAME}: ${OBJ}
	@echo "Minishell is compiling..."
	@${CC} ${CFLAGS} ${RFLAGS} -o ${NAME} ${OBJ} -lreadline
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
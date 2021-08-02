# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: tpetit <tpetit@student.s19.be>             +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2021/07/19 15:53:42 by ldelmas           #+#    #+#              #
#    Updated: 2021/08/02 14:29:20 by tpetit           ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

NAME = minishell

CC = gcc

#CFLAGS = -Wall -Wextra -Werror -I includes/

CFLAGS = -I includes/ 

# RFLAGS = -L /Users/$(USER)/.brew/opt/readline/lib -I/Users/$(USER)/.brew/opt/readline/include

HEADER = minishell.h

MAIN = main

PIPE = pipe exec command n_pipe builtins

UTIL = basics cmd_lst

PARSE = parsing parsing_print

REBUILT = pwd echo env cd export unset exit

SRC =	${addsuffix .c, ${addprefix srcs/, ${MAIN}}} \
		${addsuffix .c, ${addprefix srcs/pipe/, ${PIPE}}} \
		${addsuffix .c, ${addprefix srcs/utils/, ${UTIL}}} \
		${addsuffix .c, ${addprefix srcs/parsing/, ${PARSE}}} \
		${addsuffix .c, ${addprefix srcs/rebuilt/, ${REBUILT}}}

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
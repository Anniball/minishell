/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ldelmas <ldelmas@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/07/19 13:41:49 by tpetit            #+#    #+#             */
/*   Updated: 2021/07/26 09:31:41 by ldelmas          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MINISHELL_H
# define MINISHELL_H

# define RED   "\x1B[31m"
# define GRN   "\x1B[32m"
# define YEL   "\x1B[33m"
# define BLU   "\x1B[34m"
# define MAG   "\x1B[35m"
# define CYN   "\x1B[36m"
# define WHT   "\x1B[37m"
# define RESET "\x1B[0m"

# define SUCCESS 0
# define ERROR 1

# include <stdio.h>
# include <unistd.h>
# include <stdlib.h>
# include <fcntl.h>
# include <readline/readline.h>
# include <readline/history.h>
# include <sys/wait.h>
#include <signal.h>

# ifdef __linux
#  include <linux/limits.h>
# elif __APPLE__
#  include <limits.h>
# endif

typedef struct s_cmd
{
	char			*cmd;
	char			**flags;
	struct s_cmd	*next;
}				t_cmd;

typedef struct s_shell
{
	t_cmd	*start_cmd;
}				t_shell;

/*
** ------------------UTILS------------------------------
*/

size_t	my_strlen(char *str);
char	*my_strcpy(char *src, char *dst);
char	*my_strncpy(char *src, char *dst, int n);
char	*my_strjoin(char *str, char *add);
char	*my_strdup(char *str);
int		my_strcmp(char *s1, char *s2);

/*
** ------------------PARSING-----------------------------
*/

char 	**parse_split(char *str, char c);
int		parse_line(t_shell *shell, char *line);

/*
** ------------------LST---------------------------------
*/

void	cmd_clear(t_cmd **lst);
void	cmd_add_back(t_cmd **alst, t_cmd *new);
t_cmd	*cmd_new(char *cmd, char **flags);

/*
** ------------------REBUILT-----------------------------
*/

int		get_pwd(void);

/*
** ------------------DEBUG-------------------------------
*/

void	print_cmd(t_shell *shell);

/*
** ------------------PIPING------------------------------
*/

/*
**	Work without pipe and without infile or outfile.
**	If needed infile and outfile will be STDIN_FILENO and STDOUT_FILENO.
*/
void	my_command(char *cmd, char **argv, char **env);

/*
**	Work without pipe and with or without infile & outfile, if there is no file
**	juste put a NULL pointer as parameter, default files will be STDIN_FILENO
**	and STDOUT_FILENO.
**	/!\ Some command needs an explicit outfile or infile and will crash if they
**	don't. Example : grep need an infile since it can't just read STDIN_FILENO.
*/
int		my_exec(t_cmd pip, char **env, char *infile, char *outfile);

/*
**	Work with only one pipe.
**	Same behaviour for infile and outfile than in my_exec.
**	/!\ Will crash if there is less than 2 commands in the chained list!
**	Use my_command, my_exec  or n_piper instead!
*/
int		piper(t_cmd pip, char **env, char *infile, char *outfile);

/*
**	Work with n pipes.
**	Same behaviour for infile and outfile than in my_exec.
*/
int		n_piper(t_cmd *pip, char **env, char *infile, char *outfile);

/*
** -----------------------------------------------------
*/

#endif
/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ldelmas <ldelmas@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/07/19 13:41:49 by tpetit            #+#    #+#             */
/*   Updated: 2021/08/04 15:10:56 by ldelmas          ###   ########.fr       */
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

typedef struct s_lst
{
	char			*str;
	struct s_lst	*next;
}				t_lst;

typedef struct s_shell
{
	t_cmd	*start_cmd;
	char	**env;
	int		status;
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
char	*get_env_value(t_shell *shell, char **env, char *var);
char	*ft_itoa(int n);

/*
** ------------------PARSING-----------------------------
*/

char 	**parse_split(char *str, char c);
int		parse_line(t_shell *shell, char *line);
char	**parse_split(char *str, char c);
char	**parse_split_with_quotes(char *str, char c);
int		is_in_str(char *str, char c);
char	*strdup_until(const char *str, char *c_lst);
char	*strdup_until_c(const char *str, char c);
char	*parse_join(char *s1, char *s2);
char	*ft_substr(char *s, unsigned int start, size_t len);

/*
** ------------------LST---------------------------------
*/

void	cmd_clear(t_cmd **lst);
void	cmd_add_back(t_cmd **alst, t_cmd *new);
t_cmd	*cmd_new(char *cmd, char **flags);

/*
** ------------------REBUILT-----------------------------
*/

int		get_pwd(t_cmd *cmd);
int		get_echo(t_cmd *cmd);
int		get_env(char **env, t_cmd *cmd);

/*
**	/!\Like in the real shell, cd is NOT a process since changing the PWD of
**	a subprocess wouldn't change the PWD of the main process. cd has to be
**	called in the main process if written alone in the shell and is useless
**	if used with pipes!
*/
int		get_cd(t_cmd *cmd, char ***env);

/*
**	/!\ 'export' and 'unset' are NOT processes since they're modifiying the
**	environment for the next subprocesses. If they were in a subprocess they
**	wouldn't be able to modify the environment of their parent.
**	/!\ 'export' and 'unset' are supposing that the environment is malloc
**	and will try to free it after replacing it.
*/
int		get_export(t_cmd *cmd, char ***env);
int		get_unset(t_cmd *cmd, char ***env);

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
void	my_command(t_cmd pip, char *cmd, char **argv, char ***env);

/*
**	Work without pipe and with or without infile & outfile, if there is no file
**	juste put a NULL pointer as parameter, default files will be STDIN_FILENO
**	and STDOUT_FILENO.
**	/!\ Some command needs an explicit outfile or infile and will crash if they
**	don't. Example : grep need an infile since it can't just read STDIN_FILENO.
*/
int		my_exec(t_cmd pip, char ***env, char *infile, char *outfile);

/*
**	Work with only one pipe.
**	Same behaviour for infile and outfile than in my_exec.
**	/!\ Will crash if there is less than 2 commands in the chained list!
**	Use my_command, my_exec  or n_piper instead!
*/
int		piper(t_cmd pip, char ***env, char *infile, char *outfile);

/*
**	Work with n pipes.
**	Same behaviour for infile and outfile than in my_exec.
*/
int		n_piper(t_shell *shell, char *infile, char *outfile);

int		get_exit(t_cmd *cmd, char **env);
int		exec_builtin(t_cmd *cmd, char ***env);
int		check_builtins(char *cmd);

/*
** ------------------SIGNALS------------------------------
*/

void	receive_signal(void);

/*
** -----------------------------------------------------
*/

#endif
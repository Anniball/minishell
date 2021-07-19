/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ldelmas <ldelmas@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/07/19 13:41:49 by tpetit            #+#    #+#             */
/*   Updated: 2021/07/19 17:59:53 by ldelmas          ###   ########.fr       */
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

# include <stdio.h>
# include <unistd.h>
# include <stdlib.h>
# include <fcntl.h>
# include <readline/readline.h>
# include <readline/history.h>

typedef struct s_cmd
{
	char			*cmd;
	char			**flags;
	struct s_cmd	*next;
}				t_cmd;

/*
** ------------------UTILS------------------------------
*/

size_t	my_strlen(char *str);
char	*my_strcpy(char *src, char *dst);
char	*my_strncpy(char *src, char *dst, int n);
char	*my_strjoin(char *str, char *add);
char	*my_strdup(char *str);

/*
** -----------------------------------------------------
*/



/*
** ------------------PIPING------------------------------
*/

/*
** Work without pipe
*/
void	my_command(char *cmd, char **argv, char **env);

/*
** Work with only one pipe 
*/
int		piper(t_cmd pip, char **env, char *infile, char *outfile);

/*
** -----------------------------------------------------
*/

#endif
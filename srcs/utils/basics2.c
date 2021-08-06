/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   basics2.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tpetit <tpetit@student.s19.be>             +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/08/05 16:05:03 by tpetit            #+#    #+#             */
/*   Updated: 2021/08/06 16:58:20 by tpetit           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

int	ft_strcmp(const char *s1, const char *s2)
{
	while (1)
	{
		if (!*s1 || !*s2)
			return (0);
		if (*s1 == *s2)
		{
			s1++;
			s2++;
		}
		else
			return ((int)((unsigned char)(*s1) - (unsigned char)(*s2)));
	}
}

/*
** my_strip remove char c from start and end of str.
** This function free str.
*/

char	*my_strip(char *str, char c)
{
	int			i;
	int			start;
	int			end;
	const int	len = my_strlen(str);
	char		*ret;

	if (!str || !str[0])
		return (str);
	i = -1;
	start = 0;
	end = len;
	while (str[++i] && str[i] == c)
		start = i + 1;
	i = -1;
	while (++i < len && str[len - 1 - i] == c)
		end = len - 1 - i;
	str[end] = 0;
	ret = my_strdup(&str[start]);
	free(str);
	return (ret);
}

char	*ft_substr(char *s, unsigned int start, size_t len)
{
	int		i;
	size_t	str_len;
	char	*str;

	if (!s || start > my_strlen(s))
		return (NULL);
	else
	{
		i = -1;
		str_len = my_strlen(s) - start;
		if (len < my_strlen(s) - start)
			str_len = len;
		str = malloc(sizeof(char) * (str_len + 1));
		if (!str)
			return (NULL);
		while (s[++i] && i < (int)len)
			str[i] = s[start + i];
		str[str_len] = 0;
	}
	return (str);
}

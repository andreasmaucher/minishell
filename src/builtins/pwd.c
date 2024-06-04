/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pwd.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: amaucher <amaucher@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/08/23 10:13:39 by amaucher          #+#    #+#             */
/*   Updated: 2024/06/04 23:20:22 by amaucher         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

/*
memory management is handled by getcwd function, which returns the path of
the current directory
*/
int	pwd(void)
{
	char	current_dir[PATH_MAX];

	if (getcwd(current_dir, PATH_MAX) == NULL)
		return (1);
	printf("%s\n", current_dir);
	return (0);
}

char	*pwd_path(void)
{
	char	current_dir[PATH_MAX];
	char	*return_str;

	if (getcwd(current_dir, PATH_MAX) == NULL)
		return (NULL);
	return_str = ft_strdup(current_dir);
	return (return_str);
}

/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   file_handling.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mrizakov <mrizakov@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/21 23:47:52 by mrizakov          #+#    #+#             */
/*   Updated: 2023/11/22 04:13:27 by mrizakov         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

int	if_file_exists_delete(void *filename)
{
	if (access(filename, R_OK | W_OK) != 0)
		return (1);
	if (unlink(filename) != 0)
		return (1);
	else
		return (0);
}

int	check_if_file_or_dir(char *path)
{
	struct stat	file_stat;

	if (stat(path, &file_stat) == 0) 
	{
		if (S_ISREG(file_stat.st_mode)) 
			return (0);
		else if (S_ISDIR(file_stat.st_mode)) 
			return (1);
		else 
			return (2);
	}
	return (3);
}

int	check_if_file_can_be_opened(char *file)
{
	if (check_if_file_or_dir(file) == 1)
	{
		errno = 126;
		return (1);
	}
	if (access(file, X_OK) == 0)
		return (0);
	else
		return (1);
}

int	check_file_rights(char *filename)
{
	if (access(filename, R_OK | W_OK) != 0)
	{
		return (1);
	}
	return (0);
}

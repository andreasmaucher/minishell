/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: amaucher <amaucher@student.42berlin.d      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/08/23 10:13:39 by amaucher          #+#    #+#             */
/*   Updated: 2023/08/23 10:13:42 by amaucher         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

# include "../minishell.h"

//CHECK AGAIN WHEN CD WORKS; BUT SHOULD BE GOOD

/*
memory management is handled by getcwd function, which returns the path of
the current directory
*/
int    pwd(void)
{
    char    current_dir[PATH_MAX];

    if (getcwd(current_dir, PATH_MAX) == NULL)
        return (1);
    printf("%s\n", current_dir);
    return(0);
}

//! NEED TO FREE IN EXECUTION
char    *pwd_path(void)
{
    char    current_dir[PATH_MAX];
    char    *return_str;

    if (getcwd(current_dir, PATH_MAX) == NULL)
        return (NULL);
    return_str = ft_strdup(current_dir);
    return(return_str);
}
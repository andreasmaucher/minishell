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

char **create_env_library(char **envp)
{
    char **buf = NULL;
    char target = '=';
    int len = 0;
    int substr_len;

    while (envp[len] != NULL)
        len++;
    buf = malloc(sizeof(char *) * (len + 1));
    if (!buf)
        return (NULL);
    len = 0; //! Don't get that logic fully 
    while (envp[len] != NULL)
    {
        char *target_pos = strchr(envp[len], target); //! LIBFT
        if (target_pos != NULL)
        {
            substr_len = target_pos - envp[len];
            buf[len] = malloc(substr_len + 1);
            if (!buf[len])
                return (NULL);
            strncpy(buf[len], envp[len], substr_len); //! LIBFT
            buf[len][substr_len] = '\0';
            //printf("SUBSTR: %s\n", buf[len]);
        }
        else
        {
            buf[len] = NULL;
        }
        len++;
    }
    buf[len] = NULL;
    return (buf);
}

char **create_envp_library(char **envp)
{
    char **buf = NULL;
    char target = '\0';
    int len = 0;
    int substr_len;

    while (envp[len] != NULL)
        len++;
    buf = malloc(sizeof(char *) * (len + 1));
    if (!buf)
        return (NULL);
    len = 0;
    while (envp[len] != NULL)
    {
        char *target_pos = strchr(envp[len], target); //! LIBFT
        if (target_pos != NULL)
        {
            substr_len = target_pos - envp[len];
            buf[len] = malloc(substr_len + 1);
            if (!buf[len])
                return (NULL);
            strncpy(buf[len], envp[len], substr_len); //! LIBFT
            buf[len][substr_len] = '\0';
            //printf("SUBSTR: %s\n", buf[len]);
        }
        //! not needed?!?!??!?!?!?
        /* else
        {
            buf[len] = NULL;
        } */
        len++;
    }
    buf[len] = NULL;
    return (buf);
}
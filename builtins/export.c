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

/*
function that mimics the behavior of the 'export' command;
export is used to set or export environment variables;
once an environment variable is exported it can be accessed by other programs
within the same session or shell;
environment variables that were set using export are available to the current
shell session and andy child processes spawned from it;
by closing the terminal or shell session these variables are discarded;

Usage examples:
export MY_VARIABLE=new -> creates a new var and sets its value to 'new'
export USER=new_user -> this will change the already existing env 'USER' and 
                        set it to 'new_user'
export MY_VARIABLE= -> will be added, even though not path specified
export KEY8=m KEYFAI KEY7=true -> 7+8 will be added, even though the middle var
                                    is in the wrong format and will not be added
export KEY12=m KEYFAI. KEY11=true -> no variables added, because of invalid
                                    input for the middle var
export LOVE ME= -> will only add ME= but show no error

Function below only adds variables if all variables have the format MY_VAR=
*/

bool	ft_isalpha(int c)
{
	if ((c >= 'a' && c <= 'z') || (c >= 'A' && c <= 'Z'))
		return (true);
	return (false);
}

bool	ft_is_digit(int c)
{
	if (c >= '0' && c <= '9')
		return (true);
	return (false);
}

bool    check_valid_export_input(char **args)
{
    int i;
    int j;

    i = 1; // need to start after the command
    j = 0;
    while (args[i] != NULL)
    {   
        j= 0;
        while (args[i][j] != '\0')
        {
            if (args[i][0] != '_' && ft_isalpha(args[i][0]) == false) // first letter needs to be _ or character
            {
                printf("export '%s' : not a valid identifier", args[i]);
                return(false);
            }
            if (args[i][j] == '=') // stop at equal sign, more characters allowed  afterwards
                break;
            // if not character, digit or _ its invalid input
            if ((ft_isalpha(args[i][j]) == false && ft_is_digit(args[i][j] == false && args[i][j] != '_')))
            {
                printf("export '%s' : not a valid identifier", args[i]);
                return(false);
            }
            j++;
        }
        // after moving to the next arg
        i++;
    }
    return(true);
}

int calculate_old_env_lib_size(t_minishell m)
{
    int i;

    i = 0;
    while (m.envp_lib[i] != NULL)
        i++;
    return(i - 1);
}

/*
function extracts the part until '=' to later compare it with m.env_lib
*/
char *extract_search_str(t_command *cmd, int i)
{
    int search_len;
    char *search_str;
    int j;

    j = 0;
    search_len = 0;
    while (cmd->args[i][j] != '\0' && cmd->args[i][j] != '=')
    {
        search_len++;
        j++;
    }
    search_str = malloc(sizeof(char) * (search_len + 1));
    if (search_str == NULL)
        return (0);
    j = 0;
    while (cmd->args[i][j] != '\0' && cmd->args[i][j] != '=')
    {
        search_str[j] = cmd->args[i][j];
        j++;
    }
    search_str[j] = '\0';
    return (search_str);
}

bool    check_if_existing_env(t_minishell *m, t_command *cmd, int len)
{
    char    *search_str;
    int i;

    search_str = NULL;
    i = 1;
    while(cmd->args[i])
    {   
        search_str = extract_search_str(cmd, i); //! UPDATE ENV LIB!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
        if (ft_strcmp(m->env_lib[len], search_str) == 0) //! DOES LEN HERE STILL MAKE SENSE IN CASE OF DOUBLES?!
        {
            printf("SEARCH STR IF FOUND: %s\n", search_str);
            printf("M.env-lib found: %s\n", search_str);
            free(search_str);
            return(true);
        }
        i++;
    }
    free(search_str);
    return (false);
}

int calc_length_of_old_env_arr(t_minishell *m, t_command *cmd)
{
    int full_len;
    int double_count;

    full_len = 0;
    double_count = 0;
    while(m->envp_lib[full_len] != NULL)
    {
        if (check_if_existing_env(m, cmd, full_len) == true)
            double_count++;
        full_len++;
    }
    full_len -= 1; // reduce by one for NULL;
    //full_len += arg_count(cmd->args) - 2; // minus the command and the NULL
    //printf("DOUBLE COUNT: %d\n", double_count);
    full_len -= double_count;
    return (full_len);
}

/*
part that adds new vars within cmd->arg the new lib;
first size of the old lib - doubles is counted to determine index starting point for additions;
*/
char    **add_args_to_env_lib(t_minishell *m, char **tmp_envp, t_command *cmd, int full_len)
{
    //int env_lib_size;
    int i;
    /* int len;
    int substr_len; */

    //env_lib_size = calc_length_of_old_env_arr(m, cmd);
    /* i = 1;
    len = env_lib_size + 1; //! should it be plus one since we want to start at the next one or is included because of '\0'?
    substr_len = 0;
    printf("START INDEX FOR ADDING: %d\n", len);
    printf("FULL LEN UNTIL THAT CAN BE ADDED: %d\n", full_len); */
    /* while (len <= full_len) //! should be equal since null terminator is separated, but everythign else not
    {
        substr_len = ft_strlen(cmd->args[i]);
        tmp_envp[len] = malloc(substr_len + 1);
        if (!tmp_envp[len])
            return(NULL);
        strncpy(tmp_envp[len], cmd->args[i], substr_len); //! LIBFT
        tmp_envp[len][substr_len] = '\0';
        printf("ARG THAT IS ADDED: %s\n", tmp_envp[len]);
        len++;
        i++;
    } */
    (void)m;
    i = 1;
    while (tmp_envp[full_len] != NULL)
    {
        tmp_envp[full_len] = ft_strdup(cmd->args[i]);
        i++;
        full_len++;
    }
    return(tmp_envp);
}   

//! EVEN WORKS FOR ADDITIONS ITS ALL IN TMP_ENVP ONLY PRINTING IS WRONG!!!!!!!!!!!!!!!
char **copy_existing_env_lib(t_minishell *m, char **tmp_envp, t_command *cmd, int full_len)
{
    int len;
    /* int substr_len;
    char target = '\0';
    char *target_pos;
    int i; */

    //i = 0;
    len = 0;
    /* substr_len = 0;
    target_pos = NULL;
    printf("WITHIN copy existing env lib\n"); */
    (void)cmd;
    while (m->envp_lib[len] != NULL)
    {
        // I want to check if m.envp_lib at position len is equal to one arg
        //if (check_if_existing_env(m, cmd, len) == false)
        /* {
            target_pos = strchr(m->envp_lib[len], target); //! LIBFT
            if (target_pos != NULL)
            {
                substr_len = target_pos - m->envp_lib[len];
                tmp_envp[i] = malloc(substr_len + 1);
                if (!tmp_envp[i])
                    return(NULL);
                strncpy(tmp_envp[i], m->envp_lib[len], substr_len); //! LIBFT
                tmp_envp[i][substr_len] = '\0';
                printf("EXISTING ENV TO COPY: %s\n", tmp_envp[i]);
                i++;
            }
        } */
        tmp_envp[len] = ft_strdup(m->envp_lib[len]);
        len++;
    }
    int i = 1;
    while (len <= full_len)
    {
        tmp_envp[len] = ft_strdup(cmd->args[i]);
        printf("CMD_ARGS %s\n", cmd->args[i]);
        printf("TMP_ENVP %s\n", tmp_envp[len]);
        i++;
        len++;
    }
    tmp_envp[len] = NULL;
    return(tmp_envp);
}

/*
calculates the number of args that need to be added to the env_lib while taking into account
already existing variables that are supposed to be changed
*/
int calc_length_of_new_env_arr(t_minishell *m, t_command *cmd)
{
    int full_len;
   /// int double_count;

    full_len = 0;
  //  double_count = 0;
    while(m->envp_lib[full_len] != NULL)
    {
/*         if (check_if_existing_env(m, cmd, full_len) == true)
            double_count++; */
        full_len++;
    }
    full_len += arg_count(cmd->args) - 2; // minus the command and the NULL
    //printf("DOUBLE COUNT: %d\n", double_count);
    //full_len -= double_count;
    return (full_len);
}

//! DO THE SAME FOR ENV LIB!!!!!!!!!!!!!!!!!!!!!!!!! need to cut at '='
void    update_env_lib(t_minishell *m, t_command *cmd)
{
    char   **tmp_envp;
    int    len;
    int full_len;

    tmp_envp = NULL;
    full_len = calc_length_of_new_env_arr(m, cmd);
    tmp_envp = malloc(sizeof(char *) * (full_len + 1)); // without +1?
    if (!tmp_envp)
        return ;
    ///tmp_envp[full_len + 1] = NULL;
    tmp_envp = copy_existing_env_lib(m, tmp_envp, cmd, full_len);
    //tmp_envp = add_args_to_env_lib(m, tmp_envp, cmd, full_len);
    //! testing
    len = 0;
    while (tmp_envp[len])
    {
        printf("ENVP AFTER UPDATE: %s\n", tmp_envp[len]);
        len++;
    }
    free_env(m->envp_lib);
    m->envp_lib = malloc(sizeof(char *) * (full_len + 1));
    if (!m->envp_lib)
        return ;
    len = 0;
    while (len <= full_len)
    {
        m->envp_lib[len] = ft_strdup(tmp_envp[len]);
        len++;
    }
    m->envp_lib[len] = NULL;
    len = 0;
    while (m->envp_lib[len])
    {
        printf("ENVP NEW LIST: %s\n", m->envp_lib[len]);
        len++;
    }
    len = 0;
    while (tmp_envp[len] != NULL) {
        free(tmp_envp[len]); // Free each string
        len++;
    }
    free(tmp_envp); // Free the array of strings
	//free_env(tmp_envp);
}

//! does not cover case where = is missing, so they will be added currently
//! do all builtins need to return a value?
int export(t_minishell *m, t_command *cmd)
{
    //if (check_valid_export_input(cmd->args) == true)
    {
        int len = 0;
        while (m->envp_lib[len])
        {
            printf("ENVP BEFORE UDPATE: %s\n", m->envp_lib[len]);
            len++;
        }
        update_env_lib(m, cmd);
        return (0); //! exit_codes
    }
    //return (1); //! exit_codes
}
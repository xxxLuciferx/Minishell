/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: msodor <msodor@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/07/03 15:54:58 by msodor            #+#    #+#             */
/*   Updated: 2023/07/09 10:45:52 by msodor           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

/**
 * put_error - Helper function to print error messages
 * @cmd: The command causing the error
 * @error: The error message
 * Return: void
 */
void	put_error(char *cmd, char *error)
{
	write(2, "minishell: ", 11);
	write(2, cmd, ft_strlen(cmd));
	write(2, error, ft_strlen(error));
	write(2, "\n", 1);
}

/**
 * get_cmd_path - Function to get the full path of the command
 * @parser: A pointer to the parser structure
 * Return: The commands full path, or NULL if not found or an error occurred
 */
char	*get_cmd_path(t_parser *parser, t_cmd *cmd)
{
	struct stat	buf;

	if (is_file(cmd->cmd))
	{
		if (access(cmd->cmd, F_OK) == 0 && stat(cmd->cmd, &buf) == 0)
		{
			if (access(cmd->cmd, X_OK) == -1)
			{
				put_error(cmd->cmd, ": Permission denied");
				return (parser->exit_s = 126, NULL);
			}
			if (S_ISDIR(buf.st_mode))
			{
				put_error(cmd->cmd, ": is a directory");
				return (parser->exit_s = 126, NULL);
			}
			return (cmd->cmd);
		}
		put_error(cmd->cmd, ": No such file or directory");
		return (parser->exit_s = 127, NULL);
	}
	return (if_not_path(parser, cmd));
}

/**
 * if_not_path - Function to get the command path if it is not a file path
 * @parser: A pointer to the parser structure
 * Return: The command path, or NULL if not found
 */
char	*if_not_path(t_parser *parser, t_cmd *cmds)
{
	char	*cmd_path;
	char	**path;
	char	*cmd_file;
	int		i;

	path = get_path(parser->env);
	i = 0;
	while (path[i])
	{
		cmd_file = ft_strjoin("/", cmds->cmd);
		cmd_path = ft_strjoin(path[i], cmd_file);
		free(cmd_file);
		if (access(cmd_path, F_OK) == 0)
		{
			free_array(path);
			return (cmd_path);
		}
		free(cmd_path);
		i++;
	}
	free_array(path);
	put_error(cmds->cmd, ": command not found");
	parser->exit_s = 127;
	return (NULL);
}

int	exec_cmd(t_parser *parser, t_cmd *cmd)
{
	char	**env;
	int		status;
	int		id;

	env = list_to_array(parser->env);
	if (!get_cmd_path(parser, cmd))
		return (free_array(env), 1);
	if (parser->cmd_nbr > 1)
	{
		execve(get_cmd_path(parser, cmd), cmd->full_cmd, env);
		free_array(env);
	}
	id = fork();
	if (id == 0)
	{
		execve(get_cmd_path(parser, cmd), cmd->full_cmd, env);
		free_array(env);
	}
	waitpid(id, &status, 0);
	parser->exit_s = WEXITSTATUS(status);
	return (free_array(env), 0);
}

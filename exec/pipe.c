/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipe.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: msodor <msodor@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/06/27 23:47:43 by msodor            #+#    #+#             */
/*   Updated: 2023/07/07 18:39:17 by msodor           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"
#include <fcntl.h>
void	create_pipes(int (*fd)[2], t_parser *parser)
{
	int	i;

	i = 0;
	while (i < parser->cmd_nbr - 1)
	{
		if (pipe(fd[i]))
			return ;
		i++;
	}
}

void	wait_for_childs(int *arra, t_parser *parser)
{
	int	i;
	int	status;

	i = 0;
	while (i < parser->cmd_nbr)
	{
		waitpid(arra[i], &status, 0);
		i++;
	}
	parser->exit_s = WEXITSTATUS(status);
}
void handle_here_document(t_redir *redir)
{
	int fd;
	char *line = NULL;
	size_t len = 0;
	ssize_t read;

	while (redir)
	{
		if (redir->type == HERE_DOC)
		{
			// Open a temporary file for writing
			fd = open("/tmp/minishell_heredoc", O_WRONLY | O_CREAT | O_TRUNC, S_IRUSR | S_IWUSR | S_IRGRP | S_IWGRP);
			if (fd == -1)
			{
				perror("Error opening temporary file for here document");
				exit(1);
			}
			// Read lines from stdin until we encounter the delimiter
			while ((read = getline(&line, &len, stdin)) != -1)
			{
				if (ft_strncmp(line, redir->file, ft_strlen(redir->file)) == 0) 
				{ // Assumes that the delimiter is stored in redir->file
					break;
				}
				write(fd, line, read);
			}
			close(fd);
			free(line);
		}
		redir = redir->next;
	}
}

void handle_input_redirection(t_redir *redir)
{
	int fd;
	char *file;

	while (redir)
	{
		if (redir->type == REDIR_IN || redir->type == HERE_DOC)
		{
			file = (redir->type == REDIR_IN) ? redir->file : "/tmp/minishell_heredoc";
			fd = open(file, O_RDONLY);
			if (fd == -1)
			{
				perror("Error opening file for reading");
				exit(1);
			}
			if (dup2(fd, STDIN_FILENO) == -1)
			{
				perror("dup2 failed");
				exit(1);
			}
			close(fd);
		}
		redir = redir->next;
	}
}

void handle_output_redirection(t_redir *redir)
{
	int fd;

	while (redir)
	{
		if (redir->type == REDIR_OUT)
		{
			fd = open(redir->file, O_WRONLY | O_CREAT | O_TRUNC, S_IRUSR | S_IWUSR | S_IRGRP | S_IWGRP);
			if (fd == -1)
			{
				perror("Error opening file for writing");
				exit(1);
			}
			if (dup2(fd, STDOUT_FILENO) == -1)
			{
				perror("dup2 failed");
				exit(1);
			}
			close(fd);
		}
		redir = redir->next;
	}

}

void handle_append_output_redirection(t_redir *redir)
{
	int fd;

	while (redir)
	{
		if (redir->type == AREDIR_OUT)
		{
			fd = open(redir->file, O_WRONLY | O_CREAT | O_APPEND, S_IRUSR | S_IWUSR | S_IRGRP | S_IWGRP);
			if (fd == -1)
			{
				perror("Error opening file for appending");
				exit(1);
			}
			if (dup2(fd, STDOUT_FILENO) == -1)
			{
				perror("dup2 failed");
				exit(1);
			}
			close(fd);
		}
		redir = redir->next;
	}
}


void	exec_commands(t_parser *parser)
{
	t_cmd	*cmds;
	int		fd[parser->cmd_nbr][2];
	int		arra[parser->cmd_nbr];
	int		id;

	cmds = parser->cmds;
	
	if (parser->cmd_nbr > 1)
		create_pipes(fd, parser);
	// if (parser->cmd_nbr == 1) had condition li knti dyr mablanch hit i9der idir lk echo n >> c ola ay redirect fhmti laffaire o dima kansb9o redirection b3da
	else
	{
		while (cmds)
		{
			id = fork();
			if (id == 0)
			{
				handle_here_document(cmds->redir);
				handle_input_redirection(cmds->redir);
				handle_output_redirection(cmds->redir);
				handle_append_output_redirection(cmds->redir);
				builtins(cmds, parser, fd);
				exit (1);
				// hanta wst had exit li lfo9 dir exitstatus besh salat command ra kanexiti f child besh parent i3rfo sala lamadrtihach ayb9a dkchi mhangi
			}
			else
				if (cmds->cmd)
				{
					if (!ft_strncmp(cmds->cmd, "exit", ft_strlen("exit")))
						ft_exit(parser);
				}
				arra[cmds->index] = id;
			cmds = cmds->next;
		}
		close_pipes(fd, parser);
		wait_for_childs(arra, parser);
	}
}


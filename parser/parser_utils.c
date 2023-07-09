/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser_utils.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: msodor <msodor@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/06/17 17:53:37 by msodor            #+#    #+#             */
/*   Updated: 2023/07/07 11:11:47 by msodor           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

/**
 * cmd_nbr - count the number of cmds
 * @elems: the list of elems
 * @path: the path of the cmd
 */
void	cmd_nbr(t_elems *elems, t_parser *parser)
{
	parser->cmds = NULL;
	parser->cmd_nbr = 1;
	while (elems)
	{
		if (elems->type == PIPE)
			parser->cmd_nbr++;
		elems = elems->next;
	}
}

/**
 * cmd_list_add - add a new cmd to the list
 * @elems: the list of elems
 * @path: the path of the cmd
 */
void	init_cmds(t_elems *elems, t_parser *parser)
{
	int			i;
	int			argc;
	t_cmd		**cms;

	cmd_nbr(elems, parser);
	i = 0;
	while (i++ < parser->cmd_nbr)
	{
		argc = 0;
		elems = elems->next;
		while (elems && elems->type != PIPE && argc++ >= 0)
		{
			if (is_redir(elems))
			{
				elems = elems->next->next;
				continue ;
			}
			elems = elems->next;
		}
		cms = &parser->cmds;
		cmd_list_add(cms, cmd_new(NULL, argc, (i - 1)));
	}
}

/**
 * rm_redir - removes redirect tokens from a linked list of elements
 * @elems: A pointer to a pointer to the head element of the token list
 */
void	rm_redir(t_elems **elems)
{
	t_elems	*current;
	t_elems	*tmp;

	current = *elems;
	tmp = NULL;
	while (current)
	{
		if (is_redir(current))
		{
			tmp = current->next->next;
			token_del(elems, current->next);
			token_del(elems, current);
			current = tmp;
		}
		else
			current = current->next;
	}
}

/**
 * set_redir - function that sets up redirections for the commands in the parser
 * @parser: A pointer to the parser structure
 * @elems: The linked list of elements representing the tokens
 */
void	set_redir(t_parser *parser, t_elems *elems)
{
	t_elems	*current;
	t_redir	*new_redir;
	t_cmd	*cmds;

	current = elems;
	cmds = parser->cmds;
	while (cmds)
	{
		current = current->next;
		while (current && current->type != PIPE)
		{
			if (is_redir(current))
			{
				new_redir = redir_new(current->next->content, current->type);
				redir_add(&cmds->redir, new_redir);
				current = current->next->next;
				continue ;
			}
			current = current->next;
		}
		cmds = cmds->next;
	}
	rm_redir(&elems);
}

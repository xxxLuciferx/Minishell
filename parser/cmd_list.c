/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cmd_list.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: msodor <msodor@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/06/07 14:56:55 by msodor            #+#    #+#             */
/*   Updated: 2023/07/14 13:14:04 by msodor           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

/**
 * cmd_new - function that creates a new cmd element
 * @content: a pointer to the content string
 * @len: the length of the content
 * @type: the cmd type
 * @state: the cmd state
 * Return: a pointer to the newly created cmd element
 */
t_cmd	*cmd_new(t_redir *redir, int argc, int index)
{
	t_cmd	*cmds;

	cmds = malloc(sizeof(t_cmd));
	if (!cmds)
		return (NULL);
	cmds->cmd = NULL;
	cmds->args = (char **)malloc(sizeof(char *) * argc);
	cmds->full_cmd = (char **)malloc(sizeof(char *) * (argc + 2));
	if (!cmds->args || !cmds->full_cmd)
		return (NULL);
	cmds->redir = redir;
	cmds->index = index;
	cmds->argc = argc;
	cmds->next = NULL;
	return (cmds);
}

/**
 * cmd_list_add - function that adds a new cmd element to a cmd list
 * @lst: a pointer to the head of the cmd list
 * @new: a pointer to the new cmd element
 */
void	cmd_list_add(t_cmd **lst, t_cmd *new)
{
	t_cmd	*ptr;

	if (!lst || !new)
		return ;
	if (*lst)
	{
		ptr = *lst;
		while (ptr->next != NULL)
			ptr = ptr->next;
		ptr->next = new;
	}
	else
		*lst = new;
}

/**
 * cmd_del - function that deletes a cmd element from a cmd list
 * @head: a pointer to the head of the cmd list
 * @node: a pointer to the cmd element to be deleted
 */
void	cmd_del(t_cmd **head, t_cmd *node)
{
	t_cmd	*current;

	current = *head;
	if (*head == NULL)
		return ;
	if (node == *head)
	{
		*head = node->next;
		free(node->cmd);
		free(node);
		return ;
	}
	while (current->next != NULL && current->next != node)
		current = current->next;
	if (current->next == node)
	{
		current->next = node->next;
		free(node->cmd);
		free(node);
	}
}

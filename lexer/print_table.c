/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   print_table.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: msodor <msodor@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/05/24 20:39:28 by msodor            #+#    #+#             */
/*   Updated: 2023/06/25 15:38:19 by msodor           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

char	*get_type_string(enum e_token type)
{
	if (type == QUOTE)
		return ("QUOTE");
	else if (type == DQUOTE)
		return ("DQUOTE");
	else if (type == _SPACE)
		return ("SPACE");
	else if (type == VAR)
		return ("VAR");
	else if (type == PIPE)
		return ("PIPE");
	else if (type == REDIR_IN)
		return ("REDIR_IN");
	else if (type == REDIR_OUT)
		return ("REDIR_OUT");
	else if (type == WORD)
		return ("WORD");
	else if (type == HERE_DOC)
		return ("HERE_DOC");
	else if (type == AREDIR_OUT)
		return ("AREDIR_OUT");
	else
		return (NULL);
}

char	*get_state_string(enum e_state state)
{
	if (state == IN_DQUOTE)
		return ("IN_DQUOTE");
	else if (state == IN_QUOTE)
		return ("IN_QUOTE");
	else if (state == DEFAULT)
		return ("DEFAULT");
	else
		return (NULL);
}

void	print_table_row(t_elems *elem)
{
	char	*type_str;
	char	*state_str;
	char	*content_str;

	type_str = get_type_string(elem->type);
	state_str = get_state_string(elem->state);
	if (elem->content != NULL)
		content_str = elem->content;
	else
		content_str = "";
	printf("| %-9s | %-9d | %-9s | %-9s |\n", \
	content_str, elem->len, type_str, state_str);
	printf(BLUE"------------------------------------------------\n"RESET);
}

void	print_table(t_elems *lst)
{
	t_elems	*current;

	printf(BLUE"================================================\n"RESET);
	printf(MAGENTA"| %-9s | %-9s | %-9s | %-9s |\n"RESET, \
	"Content", "Len", "Type", "State");
	printf(BLUE"================================================\n"RESET);
	if (lst == NULL)
	{
		printf("Token table is empty.\n");
		return ;
	}
	current = lst->next;
	while (current != NULL)
	{
		print_table_row(current);
		current = current->next;
	}
}

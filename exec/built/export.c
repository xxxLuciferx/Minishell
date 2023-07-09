/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   export.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: msodor <msodor@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/06/13 01:42:00 by msodor            #+#    #+#             */
/*   Updated: 2023/07/05 12:59:30 by msodor           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

/**
 * is_correct - checks if a word is a valid identifier for the export command
 * @word: The word to be checked
 * Return: 1 if the word is a valid identifier, 0 otherwise
 */
int	is_correct(char *word)
{
	int	i;

	i = 0;
	if (!ft_isalpha(word[i]) && word[i] != '_')
	{
		write(2, "export: `", 9);
		write(2, word, ft_strlen(word));
		write(2, "': not a valid identifier\n", 26);
		return (0);
	}
	i++;
	while (word[i] && word[i] != '=')
	{
		if (!ft_isalnum(word[i]) && word[i] != '_')
		{
			write(2, "export: `", 9);
			write(2, word, ft_strlen(word));
			write(2, "': not a valid identifier\n", 26);
			return (0);
		}
		i++;
	}
	return (1);
}

/**
 * set_value - function that sets the value of an environment variable
 * @var: The variable string in the format "key=value"
 * @env: A pointer to the head of the environment variable linked list
 * Return: void
 */
void	set_value(char *var, t_env *env)
{
	t_env	*new_env;
	t_env	*tmp;

	new_env = env_new(var);
	tmp = env;
	while (tmp && tmp->next && new_env->value)
	{
		tmp = tmp->next;
		if (!ft_strncmp(new_env->key, tmp->key, ft_strlen(tmp->key) + 1))
		{
			free(tmp->value);
			tmp->value = new_env->value;
			return ;
		}
	}
	tmp = env;
	while (tmp && tmp->next && !new_env->value)
	{
		tmp = tmp->next;
		if (!ft_strncmp(new_env->key, tmp->key, ft_strlen(tmp->key) + 1))
			return ;
	}
	env_list_add(&tmp, new_env);
	return ;
}

/**
 * print_export - prints the export command for each environment variable
 * @env: A pointer to the head of the environment variable linked list
 * Return: void
 */
void	print_export(t_env *env)
{
	while (env && env->next)
	{
		env = env->next;
		if (env->value)
			printf("declare -x %s=\"%s\"\n", env->key, env->value);
		else
			printf("declare -x %s\n", env->key);
	}
}

/**
 * ft_export - function that handles the export command
 * @cmd: A pointer to the command structure
 * @parser: A pointer to the parser structure
 * Return: void
 */
void	ft_export(t_cmd *cmd, t_parser *parser)
{
	int		i;
	int		trig;

	trig = 0;
	if (!cmd->args[0])
	{
		print_export(parser->env);
		parser->exit_s = 0;
	}
	i = 0;
	while (cmd->args[i])
	{
		if (!is_correct(cmd->args[i]))
		{
			trig = 1;
			i++;
		}
		else
		{
			set_value(cmd->args[i], parser->env);
			i++;
		}
		parser->exit_s = trig;
	}
}

/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   header_utils.h                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: msodor <msodor@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/05/22 13:10:15 by msodor            #+#    #+#             */
/*   Updated: 2023/07/09 18:26:46 by msodor           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef HEADER_UTILS_H
# define HEADER_UTILS_H

# include <signal.h>
# include <stdio.h>
# include <stdlib.h>
# include <string.h>
# include <unistd.h>
# include <dirent.h>
# include <limits.h>
# include <sys/types.h>
# include <sys/wait.h>
# include <sys/stat.h>
# include <fcntl.h>
# include <readline/readline.h>
# include <readline/history.h>

# define BLACK   "\033[30m"
# define RED     "\033[31m"
# define GREEN   "\033[32m"
# define YELLOW  "\033[33m"
# define BLUE    "\033[34m"
# define MAGENTA "\033[35m"
# define CYAN    "\033[36m"
# define WHITE   "\033[37m"
# define RESET   "\033[0m"

typedef enum e_token
{
	QUOTE = '\'',
	DQUOTE = '\"',
	_SPACE = ' ',
	VAR = '$',
	PIPE = '|',
	REDIR_IN = '<',
	REDIR_OUT = '>',
	WORD,
	HERE_DOC,
	AREDIR_OUT,
}	t_token;

typedef enum e_state
{
	IN_DQUOTE,
	IN_QUOTE,
	DEFAULT,
}	t_state;

typedef struct s_elems
{
	char					*content;
	int						len;
	enum e_token			type;
	enum e_state			state;
	struct s_elems			*next;
}	t_elems;

/*parser*/

typedef struct s_redir
{
	char			*file;
	t_token			type;
	struct s_redir	*next;
	int				old_infd;
	int				old_outfd;
}	t_redir;

typedef struct s_cmd
{
	int				index;
	char			*cmd;
	char			**args;
	char			**full_cmd;
	t_redir			*redir;
	int				fd[2];
	int				argc;
	struct s_cmd	*next;
}	t_cmd;

typedef struct s_env
{
	char			*key;
	char			*value;
	struct s_env	*next;
}	t_env;

typedef struct s_parser
{
	t_env	*env;
	t_cmd	*cmds;
	int		cmd_nbr;
	int		exit_s;
}	t_parser;

#endif

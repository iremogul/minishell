/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   heredoc_utils.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: skaraosm <skaraosm@student.42kocaeli.co    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/03/10 15:58:25 by iogul             #+#    #+#             */
/*   Updated: 2026/03/15 13:57:24 by skaraosm         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"
#include "libft/libft.h"
#include <readline/readline.h>
#include <stdlib.h>
#include <unistd.h>

void	unlink_heredocs(t_cmd *cmd)
{
	t_redir	*tmp;

	while (cmd)
	{
		tmp = cmd->redirs;
		while (tmp)
		{
			if (tmp->type == R_HEREDOC && tmp->file)
				unlink(tmp->file);
			tmp = tmp->next;
		}
		cmd = cmd->next;
	}
}

static void	heredoc_warn(char *delimiter)
{
	ft_putstr_fd("minishell: warning: here-document \
		delimited by EOF (wanted `", 2);
	ft_putstr_fd(delimiter, 2);
	ft_putstr_fd("')\n", 2);
}

void	heredoc_loop(t_redir *node, t_env *env, int lst_sts, int fd)
{
	char	*line;
	char	*out;

	while (1)
	{
		line = readline("> ");
		if (!line)
		{
			heredoc_warn(node->file);
			break ;
		}
		if (!ft_strncmp(line, node->file, ft_strlen(node->file) + 1))
			break ;
		out = line;
		if (node->is_expandable)
			out = expand_string(line, env, lst_sts);
		write(fd, out, ft_strlen(out));
		write(fd, "\n", 1);
		free(line);
	}
	if (line)
		free(line);
}

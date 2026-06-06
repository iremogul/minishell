/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   executer_redir.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: skaraosm <skaraosm@student.42kocaeli.co    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/03/09 13:21:05 by iogul             #+#    #+#             */
/*   Updated: 2026/03/15 13:04:29 by skaraosm         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"
#include <fcntl.h>
#include <unistd.h>
#include <stdio.h>
#include "libft/libft.h"

int	check_heredocs(t_cmd *cmd, t_env *env, int last_exit_status)
{
	t_cmd	*tmp_cmd;
	t_redir	*tmp_redir;
	int		i;

	tmp_cmd = cmd;
	i = 0;
	while (tmp_cmd)
	{
		tmp_redir = tmp_cmd->redirs;
		while (tmp_redir)
		{
			if (tmp_redir->type == R_HEREDOC)
			{
				if (run_heredoc(tmp_redir, env, i++, last_exit_status) == -2)
					return (1);
			}
			tmp_redir = tmp_redir->next;
		}
		tmp_cmd = tmp_cmd->next;
	}
	return (0);
}

static int	open_file(t_redir *node)
{
	int	fd;

	fd = -1;
	if (node->type == R_INPUT)
		fd = open(node->file, O_RDONLY);
	else if (node->type == R_TRUNC)
		fd = open(node->file, O_WRONLY | O_CREAT | O_TRUNC, 0644);
	else if (node->type == R_APPEND)
		fd = open(node->file, O_WRONLY | O_CREAT | O_APPEND, 0644);
	else if (node->type == R_HEREDOC)
		fd = open(node->file, O_RDONLY);
	if (fd == -1)
	{
		ft_putstr_fd("minishell: ", 2);
		perror(node->file);
	}
	return (fd);
}

static int	process_redir_node(t_cmd *cmd, t_redir *tmp)
{
	int	fd;

	fd = open_file(tmp);
	if (fd == -1)
		return (-1);
	if (tmp->type == R_INPUT || tmp->type == R_HEREDOC)
	{
		if (cmd->infile > 0)
			close(cmd->infile);
		cmd->infile = fd;
	}
	else
	{
		if (cmd->outfile > 1)
			close(cmd->outfile);
		cmd->outfile = fd;
	}
	return (0);
}

static void	apply_fds(t_cmd *cmd)
{
	if (cmd->infile != 0)
	{
		dup2(cmd->infile, 0);
		close(cmd->infile);
	}
	if (cmd->outfile != 1)
	{
		dup2(cmd->outfile, 1);
		close(cmd->outfile);
	}
}

int	setup_file_descriptors(t_cmd *cmd)
{
	t_redir	*tmp;

	tmp = cmd->redirs;
	while (tmp)
	{
		if (process_redir_node(cmd, tmp) == -1)
		{
			if (cmd->infile > 0)
				close(cmd->infile);
			if (cmd->outfile > 1)
				close(cmd->outfile);
			return (-1);
		}
		tmp = tmp->next;
	}
	apply_fds(cmd);
	return (0);
}

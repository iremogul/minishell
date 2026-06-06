/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: skaraosm <skaraosm@student.42kocaeli.co    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/03/09 12:44:12 by iogul             #+#    #+#             */
/*   Updated: 2026/03/15 14:05:08 by skaraosm         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"
#include <readline/readline.h>
#include <readline/history.h>
#include <unistd.h>
#include <stdlib.h>
#include "libft/libft.h"

static char	*read_raw_input(int *bytes_read)
{
	char	*line;
	char	c;
	int		i;

	line = gc_malloc(10000);
	if (!line)
		return (NULL);
	i = 0;
	while (read(STDIN_FILENO, &c, 1) > 0)
	{
		*bytes_read = 1;
		if (c == '\n')
			break ;
		line[i++] = c;
	}
	line[i] = '\0';
	return (line);
}

static char	*get_tester_input(void)
{
	char	*line;
	char	*gc_line;
	int		bytes_read;

	bytes_read = 0;
	if (isatty(STDIN_FILENO))
	{
		line = readline("minishell$ ");
		if (!line)
			return (NULL);
		gc_line = ft_strdup(line);
		free(line);
		return (gc_line);
	}
	line = read_raw_input(&bytes_read);
	if (line && line[0] == '\0' && bytes_read <= 0)
		return (NULL);
	return (line);
}

static void	process_input(char *input, t_env *env_list, int *last_exit_status)
{
	t_token	*tokens;
	t_cmd	*command;

	if (isatty(STDIN_FILENO))
		add_history(input);
	tokens = lexer(input, last_exit_status);
	if (tokens)
	{
		expander(tokens, env_list, *last_exit_status);
		command = parser(tokens, last_exit_status);
		if (command)
			*last_exit_status = executer(command, env_list, *last_exit_status);
	}
}

int	main(int argc, char **argv, char **envp)
{
	char	*input;
	t_env	*env_list;
	int		last_exit_status;

	(void)argc;
	(void)argv;
	last_exit_status = 0;
	env_list = init_env(envp);
	init_shell_env(&env_list);
	init_signals();
	while (1)
	{
		input = get_tester_input();
		if (!input)
		{
			if (isatty(STDIN_FILENO))
				ft_putstr_fd("exit\n", 1);
			break ;
		}
		if (*input)
			process_input(input, env_list, &last_exit_status);
	}
	gc_free_all();
	return (last_exit_status);
}

/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: iogul <iogul@student.42kocaeli.com.tr>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/01/15 20:52:41 by iogul             #+#    #+#             */
/*   Updated: 2026/01/15 20:58:55 by iogul            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */


#ifndef MINISHELL_H
# define MINISHELL_H

# include <stdlib.h>
# include <unistd.h>
# include <stdio.h>
# include <readline/readline.h>
# include <readline/history.h>

typedef struct s_gc
{
	void			*ptr;
	struct s_gc		*next;
}	t_gc;

typedef enum e_type
{
	WORD,
	PIPE,
	INPUT,
	TRUNC,
	HEREDOC,
	APPEND,			
}	t_type;

typedef struct s_token
{
	char			*value;
	t_type			type;
	struct s_token	*next;
	struct s_token	*prev;
}	t_token;

void	*gc_malloc(size_t size);
void	gc_add_node(void *ptr);
void	gc_free_all(void);
t_gc	**get_gc_list(void);

#endif
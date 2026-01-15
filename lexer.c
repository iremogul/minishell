#include "minishell.h"


static int	handle_operator(char *input, int i, t_token **tokens)
{
	if (input[i] == '>' && input[i + 1] == '>')
	{
		ft_token_add_back(tokens, ft_token_new(">>", APPEND));
		return (i + 2);
	}
	if (input[i] == '<' && input[i + 1] == '<')
	{
		ft_token_add_back(tokens, ft_token_new("<<", HEREDOC));
		return (i + 2);
	}
	if (input[i] == '>')
		ft_token_add_back(tokens, ft_token_new(">", TRUNC));
	else if (input[i] == '<')
		ft_token_add_back(tokens, ft_token_new("<", INPUT));
	else if (input[i] == '|')
		ft_token_add_back(tokens, ft_token_new("|", PIPE));
	return (i + 1);
}

static int	handle_quotes(char *input, int i, t_token **tokens)
{
	int		start;
	char	quote;
	t_type	type;

	quote = input[i];
	type = (quote == '\'') ? S_QUOTE : D_QUOTE;
	start = ++i;
	while (input[i] && input[i] != quote)
		i++;
	ft_token_add_back(tokens, ft_token_new(ft_substr_gc(input, start, i - start), type));
	if (input[i])
		i++;
	return (i);
}

static int	handle_word(char *input, int i, t_token **tokens)
{
	int	start;

	start = i;
	while (input[i] && input[i] != ' ' && !(input[i] >= 9 && input[i] <= 13)
		&& input[i] != '|' && input[i] != '<' && input[i] != '>')
	{
		if (input[i] == '\'' || input[i] == '\"')
			break ;
		i++;
	}
    //libft ekle ve substr gibi fonksiyonlstın malloclarını fc ile değiş
	ft_token_add_back(tokens, ft_token_new(ft_substr_gc(input, start, i - start), WORD));
	return (i);
}

t_token	*lexer(char *input)
{
	t_token	*tokens;
	int		i;

	tokens = NULL;
	i = 0;
	while (input[i])
	{
		while (input[i] && (input[i] == ' ' || (input[i] >= 9 && input[i] <= 13)))
			i++;
		if (!input[i])
			break ;
		if (input[i] == '\'' || input[i] == '\"')
			i = handle_quotes(input, i, &tokens);
		else if (input[i] == '|' || input[i] == '<' || input[i] == '>')
			i = handle_operator(input, i, &tokens);
		else
			i = handle_word(input, i, &tokens);
	}
	return (tokens);
}
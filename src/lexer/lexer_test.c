#include "../../include/minishell.h"

void	print_token_list(t_token **tokens)
{
	int	i;

	if (!tokens)
		return ;
	i = 0;
	while (tokens[i])
	{
		printf("Token %d: %s\n", i, tokens[i]->value);
		printf("Quoted: %d\n", tokens[i]->quoted);
		i++;
	}
}

void	test_lexer_cases()
{
	const char *tests[] = {
		"echo hello world",
		"echo \"hello world\" 'foo bar'",
		"echo hello\"world\"foo",
		"cat < infile | grep foo > outfile",
		"cat << EOF | grep bar >> out",
		"   ls    -l	  |	 wc -l",
		"echo \"\"",
		"echo ''",
		"echo \"hello",
		"echo 'world",
		"| < > << >>",
		"echo \"foo|bar\" 'baz>qux'",
		"echo foo|||bar",
		"      ",
		"",
		"echo \"\" ''",
		"echo \"   \" '   '",
		NULL
	};
	int i = 0;
	t_token **tokens;

	printf("==== LEXER TEST CASES ====\n");
	while (tests[i])
	{
		printf("\nTest %d: [%s]\n", i + 1, tests[i]);
		tokens = tokenize_input((char *)tests[i]);
		print_token_list(tokens);
		free_tokens(tokens);
		i++;
	}
	printf("==========================\n");
}
#include "includes/minishell.h"

int main(void)
{
	char *input;
	int i;

	i = -1;
	while (++i < 5)
	{
		input = readline("Put something: ");
		add_history(input);
		printf("You say: %s\n", input);
		free(input);
	}
	return (0);
}
#include "includes/minishell.h"

int main(void)
{
	char *input;

	while (1)
	{
		input = readline("Put something: ");
		printf("You say: %s\n", input);
		free(input);
	}
	return (0);
}
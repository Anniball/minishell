
#include <stdio.h>
#include <readline/readline.h>
#include <readline/history.h>
#include <unistd.h>
#include <stdlib.h>

int main(int argc, char** argv) {
	while (1) {
		char *input = readline("➜ minishell > ");
		add_history(input);
		printf("%s\n", input);
	}

	return 0;
}
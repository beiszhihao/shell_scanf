#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>
#include <shell/shell.h>

int main() {
	char command[256] = {0};

	/* init */
	shell_init();
	
	/* add assoc */
	shell_add_assoc("help");
	shell_add_assoc("version");
	shell_add_assoc("ls");
	shell_add_assoc("cd");
	shell_add_assoc("pwd");
	shell_add_assoc("gcc");
	shell_add_assoc("g++");

	/* config */
	shell_set_tran(4);
	shell_set_sei(20);

	/* test */
	system("clear");
	while(true) {
		shell_scanf("TEST#", command, sizeof(command));
		printf("command: %s\n", command);
	}

	/* free */
	shell_free();

	return 0;

}

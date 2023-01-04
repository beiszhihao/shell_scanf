#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <stdint.h>
#include <termios.h>
#include <unistd.h>
#include <signal.h>
#include <termios.h>
#include <limits.h>
#include <shell/shell.h>
#include <lib/list.h>

/* Global terminal handle */
static struct 	termios g_handle;
static struct 	list*   g_list;
static struct 	list*   g_assoc;
static struct   list*	g_temp;
static struct 	termios term, ttmp;
static bool   	is_init = false;
static char     *g_head;
static char     *g_buff;
static uint32_t g_command_index;
static uint32_t g_rec;
static uint32_t g_tran = 4;
static uint32_t g_sei  = 20;

/* Init terminal */
void shell_init() {
	
	if (is_init) return;

	/* Set the current terminal as an unconventional terminal, and allow response without entering */
	tcgetattr(STDIN_FILENO, &term);
	tcgetattr(STDIN_FILENO, &g_handle);
	ttmp = term;
	ttmp.c_lflag &= ~(ICANON|ECHO);
	tcsetattr(STDIN_FILENO, TCSANOW, &ttmp);

	/* init list */
	g_list  = list_create();
	g_assoc = list_create();
	
	is_init = true;
}

void shell_free() {
	
	if (!is_init) return;

	/* reset stdin */
	tcsetattr(STDIN_FILENO, TCSANOW, &term);
	
	/* free list */
	if(g_list != NULL) {
		list_free(&g_list);
	}
	if(g_assoc != NULL) {
		list_free(&g_assoc);
	}
	is_init = false;
}

static uint8_t recv_char() {

	return CHAR_SOURCE;
}

static void delete_char(uint32_t index, char* str) {
	char* new_str = (char*)malloc(strlen(str));
	uint32_t s_i  = 0;

	memset(new_str, 0, strlen(str));
	for(int i = 0; i < strlen(str); ++i) {
		if(i == index - 1) {
			continue;
		}

		new_str[s_i++] = str[i];
	}

	memset(str, 0, strlen(str));
	strcpy(str, new_str);
	free(new_str);
}

void shell_set_tran(uint32_t val) {
	g_tran = val;
}

void shell_set_sei(uint32_t val) {
	g_sei = val;
}

static bool central_logic_processing(uint8_t logic, char *buff) {
	char* list_str = NULL;
	int temp_tp = 0;
	char* temp_str = NULL;
	char  temp_cmd[256] = {0};

	//printf("<%0x>\n", logic);
	switch (logic) {
		case KEY_MAIN:
		case KEY_SEC:
			return false;
		break;
		case KEY_WARP:
			printf("\n");
			return true;
		break;
		case KEY_UP:
			if(list_len(&g_list) == 0) {
				return false;
			}
			g_rec--;	
			if(g_rec >= INT_MAX) {
				g_rec = 0;
			}	
			OPT_CLS((int)(strlen(g_head) + strlen(buff)) + 1);
			PRINT_HEAD(g_head);
			memset(buff, 0, strlen(buff));
			strcpy(buff, (char*)list_get_data(&g_list, g_rec));
			g_command_index = strlen(g_head) + strlen(buff);
			printf("%s", buff);
			return false;
		break;
		case KEY_LOW:
			g_rec++;
			if(g_rec >= list_len(&g_list)) {
				g_rec = list_len(&g_list) - 1;
				return false;
			}
			OPT_CLS((int)(strlen(g_head) + strlen(buff)) + 1);
			PRINT_HEAD(g_head);
			memset(buff, 0, strlen(buff));
			strcpy(buff, (char*)list_get_data(&g_list, g_rec));
			g_command_index = strlen(g_head) + strlen(buff);
			printf("%s", buff);
			return false;
		break;
		case KEY_LEFT:
			if(g_command_index == strlen(g_head)) {
				return false;
			}
			OPT_LEFT;
			g_command_index--;
			return false;
		break;
		case KEY_RIGHT:
			if(g_command_index == (strlen(g_head) + strlen(buff))) {
				return false;
			}
			OPT_RIGHT;
			g_command_index++;
			return false;
		break;
		case KEY_DEL:
			if(g_command_index == strlen(g_head)) {
				return false;
			}
			OPT_CLS((int)(strlen(g_head) + strlen(buff)) + 1);
			delete_char((g_command_index - strlen(g_head)), buff);
			PRINT_HEAD(g_head);
			g_command_index--;
			printf("%s", buff);
			if(g_command_index != strlen(g_head) + strlen(buff)) {
				MOVE_LEFT((int)(strlen(g_head) + strlen(buff)) - g_command_index);
			}
			return false;
		case KEY_TAB:
			if(strlen(buff) == 0) {
				printf("\n");
				for(int i = 0; i < list_len(&g_assoc); ++i) {
					if(i == temp_tp + g_tran) {
						printf("\n");
						temp_tp = i;
					}
					temp_str = (char*)list_get_data(&g_assoc, i);
					sprintf(temp_cmd, "%d.%s", i, temp_str);
					printf("%s", temp_cmd);
					for(int j = 0; j < g_sei - strlen(temp_cmd);++j) {
						printf(" ");
					}
				}
				printf("\n");
				PRINT_HEAD(g_head);
				g_command_index = strlen(g_head);
				return false;
			}else {
				g_temp = list_create();
				int temp_index = 0;
				for(int i = 0; i < list_len(&g_assoc); ++i) {
					list_str = (char*)list_get_data(&g_assoc, i);
					if(strstr(list_str, buff) != NULL) {
							temp_index = i;
							list_push_back(&g_temp, list_str, strlen(list_str));
					}
				}

				if(list_len(&g_temp) != 1) {
					printf("\n");
					for(int i = 0; i < list_len(&g_temp); ++i) {
						if(i == temp_tp + g_tran) {
							printf("\n");
							temp_tp = i;
						}
						temp_str = (char*)list_get_data(&g_temp, i);
						sprintf(temp_cmd, "%d.%s", i, temp_str);
						printf("%s", temp_cmd);
						for(int j = 0; j < g_sei - strlen(temp_cmd);++j) {
							printf(" ");
						}
					}
					printf("\n");
				} else {
					memset(buff, 0, strlen(buff));
					strcpy(buff, (char*)list_get_data(&g_assoc, temp_index));
				}			
				OPT_CLS((int)(strlen(g_head) + strlen(buff)) + 1);
				PRINT_HEAD(g_head);
				printf("%s", buff);
				g_command_index = strlen(g_head) + strlen(buff);
				list_free(&g_temp);
			}

			return false;
		break;
		case KEY_CTRL_A:
			if(g_command_index == 0) {
				return false;
			}
			MOVE_LEFT((int)(g_command_index - strlen(g_head)));
			g_command_index = strlen(g_head);
			return false;
		break;
		case KEY_CTRL_E:
			if(g_command_index == strlen(g_head) + strlen(buff)) {
				return false;
			}
			MOVE_RIGHT((int)(strlen(buff) - (g_command_index - strlen(g_head))));
			g_command_index = strlen(g_head) + strlen(buff);
			return false;
		break;

	}	
	
	SHELL_ADD(buff, logic);
	printf("%c", logic);
	g_command_index++;

	return false;
}

void shell_add_assoc(const char* str) {
	if(str == NULL || g_assoc == NULL) {
		return;
	}
	
	if(strlen(str) == 0) {
		return;
	}

	list_push_back(&g_assoc, (char*)str, strlen(str));
}

static void sig_usr(int signo) {
	if (signo == SIGINT) {
		printf("^c\n");
		PRINT_HEAD(g_head);
		memset(g_buff, 0, strlen(g_buff));
		g_command_index = 0;
		fflush(stdout);
	}

}

uint32_t shell_scanf(const char* txt, char* buff, size_t len) {
	
	if (!is_init) return 0;
	
	if (buff == NULL || len <= 0 || txt == NULL) return 0;
	
	memset(buff, 0, len);

	/* > */
	PRINT_HEAD(txt);
	g_head = (char*)txt;
	g_command_index = strlen(g_head);
	g_buff = buff;

	/* sig */
	signal(SIGINT, sig_usr);

	/* Receive String */
	do {
			if (strlen(buff) >= len) {
				break;
			}

			if (central_logic_processing (recv_char(), buff)) {
				list_delete_repeat(&g_list, (char*)buff);
				list_push_back(&g_list, (char*)buff, strlen(buff));
				g_rec = list_len(&g_list);
				break;			
			}

	} while (1);

	return strlen(buff);

}


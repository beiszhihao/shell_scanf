#ifndef __SHELL_H__
#define __SHELL_H__
#include <string.h>
#include <stdint.h>
#define CHAR_SOURCE     getchar()
#define PRINT_HEAD(s)   printf("\033[1;36m%s \033[m", s)

#define KEY_MAIN	0x1B
#define KEY_SEC		0x5B
#define KEY_WARP        0x0A
#define KEY_UP		0x41
#define KEY_LOW		0x42
#define KEY_LEFT	0x44
#define KEY_RIGHT	0x43
#define KEY_DEL		0x7F
#define KEY_TAB		0x09
#define KEY_CTRL_A	0x01
#define KEY_CTRL_E	0x05

#define OPT_LEFT		printf("\033[1D")
#define OPT_RIGHT		printf("\033[1C")
#define OPT_CLS(s_len)		printf("\r");\
				printf("%*s", s_len, " ");\
				printf("\r");
#define SHELL_ADD(s, c)		strncat(s, (char*)&c, 1)
#define MOVE_LEFT(size)		printf("\033[%dD", size);
#define MOVE_RIGHT(size)	printf("\033[%dC", size);

extern void shell_init();
extern void shell_free();
extern uint32_t shell_scanf(const char* txt, char* buff, size_t len);
extern void shell_add_assoc(const char* str);
extern void shell_set_tran(uint32_t val);
extern void shell_set_sei(uint32_t val);

#endif

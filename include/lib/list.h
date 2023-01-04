#ifndef __LIST_H__
#define __LIST_H__

#include <stdint.h>
#include <stddef.h>

struct list {
	size_t      len;
	void        *data;
	struct list *next;
	void 	(*free)		(struct list*);
	int  	(*push_back)	(struct list*, void*, size_t);
	size_t 	(*size)		(struct list*);
	void* 	(*get_data)	(struct list*, uint32_t);
};

extern struct list*  	list_create();
extern void 		list_free(struct list** m_list);
extern int  		list_push_back(struct list** m_list, void* data, size_t len);
extern size_t 		list_len(struct list **m_list);
extern void* 		list_get_data(struct list **m_list, uint32_t index);
extern void 		list_delete(struct list **m_list, uint32_t index);
extern void 		list_delete_repeat(struct list** m_list, void* data);
#endif

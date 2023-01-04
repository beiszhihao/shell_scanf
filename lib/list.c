#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <limits.h>
#include <lib/list.h>

struct list* list_create () {
	struct list* cr_list = (struct list*)malloc(sizeof(struct list));
	memset(cr_list, 0, sizeof(struct list));
	
	return cr_list;
}

void list_free (struct list** m_list) {

	struct list* temp_list;
	struct list* temp_next;

	if ((*m_list) == NULL) {
		return;
	}

	if(list_len(m_list) <= 0) {
		return;
	}

	temp_list = *m_list;
	for(int i = 0; i < list_len(m_list); ++i) {
		temp_next = temp_list->next;
		free(temp_list->data);
		free(temp_list);
		temp_list = temp_next;
	}

	(*m_list) = NULL;
	
}

static struct list *get_end (struct list** m_list) {
	struct list* temp_list = NULL;

	temp_list = *m_list;
	for (int i = 0; temp_list->next != NULL; ++i) {
		temp_list = temp_list->next;

	}
	
	return temp_list;
}

static void insert_data (struct list* m_list, void* data, size_t len) {
	void 	     *copy_data = NULL;
	struct list  *next = NULL;
	
	/* for string */
	if ((copy_data = malloc(len + 1)) == NULL) {
		return;
	}
	memset(copy_data, 0, len + 1);

	next = list_create();
	memcpy(copy_data, data, len);
	m_list->data = copy_data;
	m_list->len  = len;
	m_list->next = next;
}

int list_push_back (struct list** m_list, void* data, size_t len) {
	struct list *temp_list = NULL;
	void *copy_data = NULL;

	if (*m_list == NULL || data == NULL || len <= 0) return -1;

	if ((temp_list = get_end(m_list)) == NULL) return -1;
	
	insert_data(temp_list, data, len);

	return 0;
}

void list_delete_repeat(struct list** m_list, void* data) {
	struct list *temp_list = NULL;

	if(*m_list == NULL || data == NULL) {
		return;
	}

	temp_list = *m_list;
	for(int i = 0; i < list_len(m_list); ++i) {
		if(memcmp(temp_list->data, data, temp_list->len) == 0) {
			list_delete(m_list, i);
		}

		temp_list = temp_list->next;
	}

}

size_t list_len(struct list **m_list) {
	int i = 0;
	struct list* temp_list = NULL;

	if(*m_list == NULL) {
		return 0;
	}
	
	temp_list = *m_list;
	for(; temp_list->next != NULL; ++i) {
		temp_list = temp_list->next;
	}
	return i;	
}

static struct list* jmp_index(struct list **m_list, uint32_t index) {
	struct list *temp_list = *m_list;

	for(int i = 0; i < index; ++i) {
		temp_list = temp_list->next;
	}

	return temp_list;

}

void* list_get_data(struct list **m_list, uint32_t index) {
	
	if (*m_list == NULL) {
		return NULL;
	}
	
	if(index < 0 || index > list_len(m_list)) {
		return NULL;
	}
	
	return jmp_index(m_list, index)->data;

}

void list_delete(struct list **m_list, uint32_t index) {
	struct list* temp_list;
	struct list* temp_del;
	struct list* temp_next_node;
	uint32_t     temp_index;

	if (*m_list == NULL) {
		return;
	}
	
	if(index < 0 || index > list_len(m_list)) {
		return;
	}
	
	temp_list = *m_list;
	if((temp_index = index - 1) >= INT_MAX) temp_index = 0;
	
	if(index == 0) {
		temp_next_node = temp_list->next;
		free(temp_list->data);
		free(temp_list);
		(*m_list) = temp_next_node;
		return;
	}
	
	for(int i = 0;i < temp_index; ++i) {
		temp_list = temp_list->next;
	}

	temp_del = temp_list->next;
	/*
	if(index == list_len(m_list)) {
		free(temp_del->data);
		free(temp_del);
		temp_list->next = NULL;
		return;
	}
	*/
	temp_del = temp_list->next;
	temp_next_node = temp_del->next;
	free(temp_del->data);
	free(temp_del);
	temp_list->next = temp_next_node;
	
}

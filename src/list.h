#ifndef LIST_H
#define LIST_H

#include <string.h>

#include "common.h"

typedef struct _List {
    void *head;
    struct _List *tail;
} List;

List List_new() {
    return (List) { .head = 0, .tail = 0 };
}

List *List_newm() {
    List *list = (List*)myalloc(sizeof(List));
    *list = List_new();
    return list;
}

bool List_isempty(List list) {
    return list.head == 0;
}

void List_append(List *list, void *item) {
    
    if (List_isempty(*list)) {
        List *newnode = List_newm();

        list->head = item;
        list->tail = newnode;
    } else {
        List_append(list->tail, item);
    }
}

size_t List_size(List list) {
    if (List_isempty(list)) {
        return 0;
    } else {
        return 1 + List_size(*list.tail);
    }
}

void List_appendList(List *list1, List *list2) {
    if (List_isempty(*list1)) {
        list1->head = list2->head;
        list1->tail = list2->tail;
    } else {
        List_appendList(list1->tail, list2);
    }
}

void List_insert(List *list, void *item) {
    List *newnode = List_newm();
    
    if (List_isempty(*list)) {
        list->head = item;
        list->tail = newnode;
    } else {
        newnode->head = item;
        newnode->tail = list->tail;
        list->tail = newnode;
    }
}

#endif

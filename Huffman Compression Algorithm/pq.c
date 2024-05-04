#include "pq.h"

#include "node.h"

#include <stdio.h>
#include <stdlib.h>
typedef struct ListElement ListElement;
struct ListElement {
    Node *tree;
    ListElement *next;
};
struct PriorityQueue {
    ListElement *list;
};

PriorityQueue *pq_create(void) {
    PriorityQueue *new_pq = (PriorityQueue *) malloc(sizeof(PriorityQueue));
    if (new_pq == NULL) {
        return NULL;
    }
    new_pq->list = NULL;
    return new_pq;
}
void pq_free(PriorityQueue **q) {
    if (*q != NULL) {
        ListElement *current = (*q)->list;
        while (current != NULL) {
            ListElement *temp = current;
            current = current->next;
            free(temp);
        }
        free(*q);
        *q = NULL;
    }
}
bool pq_is_empty(PriorityQueue *q) {
    return (q == NULL || q->list == NULL);
}
bool pq_size_is_1(PriorityQueue *q) {
    return (q != NULL && q->list != NULL && q->list->next == NULL);
}

bool pq_less_than(ListElement *e1, ListElement *e2) {
    if (e1 == NULL || e2 == NULL) {
        return false;
    }

    if (e1->tree->weight < e2->tree->weight) {
        return true;
    } else if (e1->tree->weight > e2->tree->weight) {
        return false;
    }
    return (e1->tree->symbol < e2->tree->symbol);
}
void enqueue(PriorityQueue *q, Node *tree) {
    ListElement *new_element = (ListElement *) malloc(sizeof(ListElement));
    if (new_element == NULL) {
        return;
    }
    new_element->tree = tree;
    if (q->list == NULL || pq_less_than(new_element, q->list)) {
        new_element->next = q->list;
        q->list = new_element;
    } else {
        ListElement *current = q->list;
        while (current->next != NULL && !pq_less_than(new_element, current->next)) {
            current = current->next;
        }
        new_element->next = current->next;
        current->next = new_element;
    }
}
Node *dequeue(PriorityQueue *q) {
    if (q == NULL || q->list == NULL) {
        fprintf(stderr, "Fatal error\n");
        exit(1);
    }
    Node *dequeued_tree = q->list->tree;
    ListElement *temp = q->list;
    q->list = q->list->next;
    free(temp);

    return dequeued_tree;
}
void pq_print(PriorityQueue *q) {
    assert(q != NULL);

    ListElement *e = q->list;
    int position = 1;

    while (e != NULL) {
        if (position++ == 1) {
            printf("=============================================\n");
        } else {
            printf("---------------------------------------------\n");
        }

        node_print_tree(e->tree);
        e = e->next;
    }

    printf("=============================================\n");
}

/* RLL Singly-Linked List library
 * 
 * This library implements a very basic singly-linked list
 * that may be used like a stack. It is designed for use in
 * the Raft interpreter, but may be used elsewhere.
 * 
 * Implemented as a single-file library in the style of the 
 * stb libraries: https://github.com/nothings/stb
 * 
 * Current features:
 *   - Initialize (creates head link)
 *   - Push ------- LinkPush()
 *   - Pop -------- LinkPop()
 *   - Length ----- LinkLength()
 * 
 * Planned features:
 *   - Insert
 *   - Delete
 *   - Maybe more in the future
 * 
 * by Nolan Adams, 25 January 2022
*/

#ifndef RLL_H_INCLUDED
#define RLL_H_INCLUDED

struct _Node {
    int data;
    enum {
        head = 0,
        link
    } type;
    struct _Node * ptr;
};

extern struct _Node Initialize(void);
extern struct _Node * _walk(struct _Node * n);
extern void LinkPush(struct _Node * n, int i);
extern int LinkPop(struct _Node * n);
extern int LinkLength(struct _Node * n);
extern void PrintNodes(struct _Node * n);

typedef struct _Node Link;

#endif // RLL_H_INCLUDED

#ifndef RLL_C_INCLUDED
#define RLL_C_INCLUDED

struct _Node Initialize(void) {
    struct _Node to_init;
    to_init.data = 0;
    to_init.type = head;
    to_init.ptr = NULL;
    return(to_init);
}

struct _Node * _walk(struct _Node * n) {
    struct _Node * curr = n;
    while (curr->ptr != NULL) {
        curr = curr->ptr;
    }
    return(curr);
}

void LinkPush(struct _Node * n, int i) {
    // Push operation for using linked list like a stack
    struct _Node * new__Node = malloc(sizeof(struct _Node));
    new__Node->data = i;
    new__Node->type = link;
    new__Node->ptr = NULL;

    struct _Node * last__Node = _walk(n);

    last__Node->ptr = new__Node;
}

int LinkPop(struct _Node * n) {
    // Pop operation for using linked list like a stack
    struct _Node * curr = n;
    struct _Node * next;

    if (curr->ptr == NULL) {
        fprintf(stderr, "Error: Insufficient list contents for operation: pop\n");
        return(0);
    } else {
        next = curr->ptr;
        while (next->ptr != NULL) {
            curr = curr->ptr;
            next = next->ptr;
        }
    }

    int m = next->data;
    curr->ptr = NULL;

    free(next); // After storing the contents of the last link, we can free its memory.
    return(m);
}

int LinkLength(struct _Node * n) {
    int link_length = 0;

    struct _Node * curr = n;
    while (curr->ptr != NULL) {
        curr = curr->ptr;
        link_length++;
    }
    return(link_length);
}

void PrintNodes(struct _Node * n) {
    struct _Node * curr = n;
    while (curr != NULL) {
        printf("== Node == \n");
        printf("  data: %d\n", curr->data);
        printf("  type: %s\n", curr->type? "link" : "head");
        printf("   ptr: %p\n\n", curr->ptr);
        curr = curr->ptr;
    }
}

#endif // RLL_C_INCLUDED
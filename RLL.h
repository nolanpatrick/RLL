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
 *   - Initialize - Initialize(void)
 *   - Push ------- LinkPush()
 *   - Pop -------- LinkPop()
 *   - Peek ------- LinkPeek()
 *   - Insert ----- LinkInsert()
 *   - Length ----- LinkLength()
 * 
 * Planned features:
 *   - Delete
 *   - Maybe more in the future
 * 
 * by Nolan Adams, 26 January 2022
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

extern struct _Node Initialize(void);                     // Initialize the 'head' node
extern struct _Node * _walk(struct _Node * n);            // Returns a pointer to the end node
extern void LinkPush(struct _Node * n, int d);            // Push (stack operation)
extern int LinkPop(struct _Node * n);                     // Pop (stack operation)
extern int LinkLength(struct _Node * n);                  // Returns length of linked list
extern int LinkPeek(struct _Node * n);                    // Returns value of end node
extern void LinkInsert(struct _Node * n, int d, int ind); // Insert a node at specified index
extern void PrintNodes(struct _Node * n);                 // Print attributes of all nodes

typedef struct _Node Link;

#endif // RLL_H_INCLUDED

#ifndef RLL_C_INCLUDED
#define RLL_C_INCLUDED

struct _Node Initialize(void) {
    // Initialize empty list
    struct _Node to_init;
    to_init.data = 0;
    to_init.type = head;
    to_init.ptr = NULL;
    return(to_init);
}

struct _Node * _walk(struct _Node * n) {
    // Returns pointer to last node in list
    struct _Node * curr = n;
    while (curr->ptr != NULL) {
        curr = curr->ptr;
    }
    return(curr);
}

void LinkPush(struct _Node * n, int d) {
    // Push operation for using linked list like a stack
    struct _Node * new_node = malloc(sizeof(struct _Node));
    new_node->data = d;
    new_node->type = link;
    new_node->ptr = NULL;

    struct _Node * last_node = _walk(n);

    last_node->ptr = new_node;
}

int LinkPop(struct _Node * n) {
    // Pop operation for using linked list like a stack
    struct _Node * curr = n;
    struct _Node * next;

    if (curr->ptr == NULL) {
        fprintf(stderr, "Error: Insufficient list contents for operation: pop\n");
        exit(1);
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
    // Get the current length of a list
    int link_length = 0;

    struct _Node * curr = n;
    while (curr->ptr != NULL) {
        curr = curr->ptr;
        link_length++;
    }
    return(link_length);
}

int LinkPeek(struct _Node * n) {
    struct _Node * end = _walk(n);
    return(end->data);
}

void LinkInsert(struct _Node * n, int d, int ind) {
    // Insert a link at the specified index
    struct _Node * before = n;
    if (ind > LinkLength(n)) {
        fprintf(stderr, "Error: Insufficient list contents for operation: insert\n");
        exit(1);
    } else if (ind < 0) {
        fprintf(stderr, "Error: Index must be greater than or equal to zero\n");
        exit(1);
    }
    for (int i = 0; i < ind; i++) {
        before = before->ptr;
    }

    struct _Node * after = before->ptr;

    struct _Node * new_node = malloc(sizeof(struct _Node));
    new_node->data = d;
    new_node->type = link;
    new_node->ptr = after;

    before->ptr = new_node;
}

void PrintNodes(struct _Node * n) {
    // Print attributes of all nodes in list
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
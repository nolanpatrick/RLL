/* RLL Singly-Linked List library (Complicated version)
 * 
 * Program internal structure mechanism
 * 
 * 
 * by Nolan Adams
 * 
 * Last updated 27 January 2022
*/

#ifndef RLL_PTR_H_INCLUDED
#define RLL_PTR_H_INCLUDED

typedef enum {
    op_null,
    op_return,

    // Arithmetic
    op_add,
    op_subtract,
    op_multiply,
    op_divide,

    // Boolean
    op_gt,
    op_lt,
    op_eq,
    op_and,
    op_or,

    // Stack manipulation
    op_swap,
    op_dup,
    op_over,
    op_rot,
    op_drop,

    // Integers & Strings
    op_ipush,
    op_spush,
    op_iprint,
    op_sprint,

    // Misc.
    op_cr,
    op_nbsp,
    op_anchor,
    op_goto,

    // Loops
    op_do,
    op_while,

    // Conditionals
    op_if,
    op_fi,

    // Debugging
    op_dstack
} Operations;

typedef enum {
    head = 0,
    link
} Type;

struct _OpNode {
    // Used for sequence of program operations
    Type type;
    Operations op;
    int data;
    struct _OpNode * ptr;
};

struct _FuncNode {
    // 'head' of functions, stores handle and pointer to function
    Type type;
    char handle[32];
    struct _OpNode * ptr;    // Store pointer to beginning of function
    struct _FuncNode * func; // Store pointer to next function head
};

struct _RetNode {
    // Used for return stack (stores return addresses on each function call)
    Type type;
    struct _OpNode * ret;
    struct _RetNode * ptr;
};

extern struct _FuncNode FuncInitialize(void);               // Initialize the 'head' node of function stack (beginning of program execution)
extern struct _RetNode  RetInitialize(void);                // Initialize the 'head' node of return stack
extern struct _OpNode   OpInitialize(void);                 // Initialize the 'head' node of operation stack

extern struct _FuncNode * _FuncWalk(struct _FuncNode * n);  // Returns a pointer to the end node of function stack
extern struct _RetNode  * _RetWalk(struct _RetNode * n);    // Returns a pointer to the end node of return stack
extern struct _OpNode   * _OpWalk(struct _FuncNode * n);    // Returns a pointer to the end node of operation stack

extern struct _FuncNode *  FuncPush(struct _FuncNode * n, char h[]);          // Push (function stack)
extern struct _RetNode  *  RetPush(struct _RetNode * n, struct _OpNode * r);  // Push (return stack)
extern struct _OpNode   *  OpPush(struct _FuncNode * n, Operations o, int i); // Push (operation stack)

extern void Cleanup(struct _FuncNode * n); // Pop whole function and all nodes from main program
extern void RetPop(struct _RetNode * n);   // Pop return stack entry

extern void FuncPrint(struct _FuncNode * n);

//extern void PrintPtrNodes(struct _PtrNode * n); // Print attributes of all nodes

#endif // RLL_PTR_H_INCLUDED

#ifndef RLL_PTR_C_INCLUDED
#define RLL_PTR_C_INCLUDED
#include <string.h>

struct _FuncNode FuncInitialize(void) {
    // Initialize empty program
    struct _FuncNode to_init;
    strcpy(to_init.handle, "root");
    to_init.type = head;
    to_init.ptr = NULL;
    to_init.func = NULL;

    return(to_init);
}

struct _RetNode RetInitialize(void) {
    // Initialize empty return stack
    struct _RetNode to_init;
    to_init.type = head;
    to_init.ptr = NULL;
    return(to_init);
}

struct _FuncNode * _FuncWalk(struct _FuncNode * n) {
    // Returns pointer to last node in function stack
    struct _FuncNode * curr = n;
    while (curr->func != NULL) {
        curr = curr->func;
    }
    return(curr);
}

struct _RetNode * _RetWalk(struct _RetNode * n) {
    // Returns pointer to last node in return stack
    struct _RetNode * curr = n;
    while (curr->ptr != NULL) {
        curr = curr->ptr;
    }
    return(curr);
}

struct _OpNode * _OpWalk(struct _FuncNode * n) {
    // Returns pointer to last node in operation chain
    struct _OpNode * curr = n->ptr;
    while (curr->ptr != NULL) {
        curr = curr->ptr;
    }
    return(curr);
}

struct _FuncNode * FuncPush(struct _FuncNode * n, char h[]) {
    // Push operation for function stack
    struct _FuncNode * new_FuncNode = malloc(sizeof(struct _FuncNode));
    strcpy(new_FuncNode->handle, h);
    new_FuncNode->type = link;
    new_FuncNode->func = NULL;

    struct _FuncNode * last_FuncNode = _FuncWalk(n);

    last_FuncNode->func = new_FuncNode;

    // Initialize head _OpNode beside _FuncNode
    struct _OpNode * op_head = malloc(sizeof(struct _OpNode));
    op_head->op = op_null;
    op_head->data = 0;
    op_head->ptr = NULL;
    op_head->type = head;

    new_FuncNode->ptr = op_head;

    return(new_FuncNode);
}

struct _RetNode * RetPush(struct _RetNode * n, struct _OpNode * r) {
    // Push operation for return stack
    struct _RetNode * new_RetNode = malloc(sizeof(struct _RetNode));
    new_RetNode->ptr = NULL;
    new_RetNode->type = link;

    struct _RetNode * last_RetNode = _RetWalk(n);

    last_RetNode->ptr = new_RetNode;

    return(new_RetNode);
}

struct _OpNode * OpPush(struct _FuncNode * n, Operations o, int i) {
    // Push operation for operation chain within function
    struct _OpNode * new_OpNode = malloc(sizeof(struct _OpNode));
    new_OpNode->data = i;
    new_OpNode->op = o;
    new_OpNode->ptr = NULL;

    struct _OpNode * last_OpNode = _OpWalk(n);
}

void Cleanup(struct _FuncNode * n) {
    struct _FuncNode * curr_FuncNode = n->func;
    while (curr_FuncNode->func != NULL) {
        struct _OpNode * curr_OpNode = curr_FuncNode->ptr;
        while (curr_OpNode->ptr != NULL) {
            struct _OpNode * next_OpNode = curr_OpNode->ptr;
            free(curr_OpNode);
            curr_OpNode = next_OpNode;
        }
        struct _FuncNode * next_FuncNode = curr_FuncNode->func;
        free(curr_FuncNode);
        curr_FuncNode = next_FuncNode;
    }
    n->func = NULL;
}

void RetPop(struct _RetNode * n) {
    // Pop last node from return stack
    struct _RetNode * curr = n;
    struct _RetNode * next;

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
    curr->ptr = NULL;

    free(next); // After storing the contents of the last link, we can free its memory.
}

void FuncPrint(struct _FuncNode * n) {
    struct _FuncNode * curr = n;
    while (curr->func != NULL) {
        printf("_FuncNode:\n");
        printf("  handle: %s\n", curr->handle);
        printf("    type: %d\n", curr->type);
        printf("    func: %p\n", curr->func);
        printf("     ptr: %p\n", curr->ptr);
        curr = curr->func;
    }
    return(curr);
}

#endif // RLL_PTR_C_INCLUDED
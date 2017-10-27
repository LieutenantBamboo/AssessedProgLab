// Created by Matthew Connor Day
// Login: 2266981d
// Title: APH Exercise 1

#include <string.h>
#include <stdlib.h>
#include "date.h"
#include "tldlist.h"



/* struct tldlist
 * Contains:
 * root - root of the list
 * begin - date to start considering entries
 * end - date to stop considering entries
 * size - size of the list
 */
struct tldlist {
    TLDNode *root;
    Date *begin;
    Date *end;
    int size;

};

/* struct tldnode
 * Contains:
 * tld - string representing the top-level-domain
 * right - right child of the node
 * left - left child of the node
 * parent - parent of the node
 */

struct tldnode {
    char *tld;
    TLDNode *right;
    TLDNode *left;
    TLDNode *parent;
    int height, count;
};

/* struct tlditerator
 * Contains:
 * node - current node in iteration
 * last - last node visited
 */

struct tlditerator {
    TLDNode *node;
    TLDNode *last;
};

/*
 * tldlist_create generates a list structure for storing counts against
 * top level domains (TLDs)
 *
 * creates a TLDList that is constrained to the `begin' and `end' Date's
 * returns a pointer to the list if successful, NULL if not
 */
TLDList *tldlist_create(Date *begin, Date *end) {
    TLDList *t;

    if ((t = (TLDList *) malloc(sizeof(TLDList))) != NULL) {
        t->root = NULL;
        t->begin = date_duplicate(begin);
        t->end = date_duplicate(end);
    }
    return t;
}

/*
 * tldlist_destroy destroys the list structure in `tld'
 *
 * all heap allocated storage associated with the list is returned to the heap
 */
void tldlist_destroy(TLDList *tld) {
    // Iterate through all nodes and free, finally freeing the list itself
    // Memory-Leak-Proof Stuff Here
    TLDIterator *iter = tldlist_iter_create(tld);
    TLDNode *node;

    while ((node = tldlist_iter_next(iter)) != NULL) {
        free(node->tld);
        free(node);
    }

    date_destroy(tld->begin);
    date_destroy(tld->end);
    free(tld);
    tldlist_iter_destroy(iter);
    // TODO: Confirm no memory leakage
}

int max(int a, int b) { return a > b ? a : b; }

int get_height(TLDNode *node) {
    if (node == NULL) return 0;

    return node->height;
}

/**
 * right_rotate - will rotate to the right, reassigning pointers and parents
 * @param node - the root of the subtree to be rotated
 * @return the new node at the root of the subtree
 */

TLDNode *right_rotate(TLDNode *node) {
    TLDNode *parent = node->parent;
    TLDNode *l = node->left;
    TLDNode *mid_tree = l->right;

    // Rotate given nodes while updating parents
    l->right = node;
    node->left = mid_tree;
    if(mid_tree != NULL) mid_tree->parent = node;
    l->parent = parent;
    node->parent = l;


    // Update heights of given nodes
    node->height = 1 + max(get_height(node->left), get_height(node->right));
    l->height = 1 + max(get_height(l->left), get_height(l->right));

    return l;
}

/**
 * left_rotate - will rotate to the right, reassigning pointers and parents
 * @param node - the root of the subtree to be rotated
 * @return the new node at the root of the subtree
 */

TLDNode *left_rotate(TLDNode *node) {
    TLDNode *parent = node->parent;
    TLDNode *r = node->right;
    TLDNode *mid_tree = r->left;

    // Rotate given nodes
    r->left = node;
    node->right = mid_tree;
    if(mid_tree != NULL) mid_tree->parent = node;
    r->parent = parent;
    node->parent = r;

    // Update heights of given nodes
    node->height = 1 + max(get_height(node->left), get_height(node->right));
    r->height = 1 + max(get_height(r->left), get_height(r->right));

    return r;
}

/**
 * create_node - Utility function designed to create and return a default new node
 * @param parent - parent of the new node
 * @param tld - top level domain key string
 * @return the newly created node
 */

TLDNode *create_node(TLDNode *parent, char *tld) {
    // Allocate required memory for the node
    TLDNode *new_node = (TLDNode *) malloc(sizeof(TLDNode));
    // Setup Node properties
    new_node->tld = tld;
    new_node->left = NULL;
    new_node->right = NULL;
    new_node->parent = parent;
    new_node->count = 1;
    new_node->height = 1;
    // Return node
    return new_node;
}

/**
 * gettld - Utility function designed to extract the tld from the hostname string
 * @param hostname - The string to be converted into a tld
 * @return
 */

char *gettld(char *hostname) {
    char *tld = malloc(sizeof(hostname) + 1);
    char *token = malloc(sizeof(hostname) + 1);
    // Define delimiter
    const char delim[2] = ".";

    // Copy hostname to token
    strcpy(token, hostname);

    // Get the first token
    token = strtok(token, delim);

    // Loop through to the last
    while (token != NULL) {
        strcpy(tld, token);
        token = strtok(NULL, delim);
    }
    free(token);
    return tld;
}

// Get Balance factor of node N
int get_balance(TLDNode *node) {
    if (node == NULL) return 0;

    return (get_height(node->left) - get_height(node->right));
}

/*
 * tldlist_add adds the TLD contained in `hostname' to the tldlist if
 * `d' falls in the begin and end dates associated with the list;
 * returns 1 if the entry was counted, 0 if not
 */
int tldlist_add(TLDList *tld, char *hostname, Date *d) {
    // Allocate memory and define variables
    char *tldvar = gettld(hostname);

    // If the date to be added is within the bounds of the tldlist
    if (date_compare(d, tld->begin) >= 0 && date_compare(d, tld->end) <= 0) {
        // Call the recursive insert function
        if((tld->root = tldlist_insert(tld, NULL, tld->root, tldvar)) != NULL){
            tld->size++;
            return 1;
        }

    }
    return 0;
}

/*
 * tldlist_insert acts as a recursive function to insert a node and balance the tree on the way back up
 */
TLDNode *tldlist_insert(TLDList *list, TLDNode *parent, TLDNode *node, char *tld) {
    // BST recursive insert function with AVL
    if (node == NULL) return create_node(parent, tld);

    // Keeping parent pointers where they should be (mini-hack)
    node->parent = parent;

    if (strcmp(tld, node->tld) > 0) {
        // Assign the node's left to a recursive call and assign the childs parent as the node
        node->right = tldlist_insert(list, node, node->right, tld);
    } else if (strcmp(tld, node->tld) < 0) {
        // Identical as above, but for the left
        node->left = tldlist_insert(list, node, node->left, tld);
    } else {
        // If node strings are equal, increase the count of the already present node
        node->count++;
        free(tld);
        return node;
    }

    // Update height of current node
    node->height = 1 + max(get_height(node->left), get_height(node->right));

    // Check balance value
    int balance = get_balance(node);

    // LL
    if (balance > 1 && strcmp(tld, node->left->tld) < 0) return right_rotate(node);

    // RR
    if (balance < -1 && strcmp(tld, node->right->tld) > 0) return left_rotate(node);

    // LR
    if (balance > 1 && strcmp(tld, node->left->tld) > 0) {
        node->left = left_rotate(node->left);
        return right_rotate(node);
    }

    // RL
    if (balance < -1 && strcmp(tld, node->right->tld) < 0) {
        node->right = right_rotate(node->right);
        return left_rotate(node);
    }

    /* return the (unchanged) node pointer */
    return node;
}

/*
 * tldlist_count returns the number of successful tldlist_add() calls since
 * the creation of the TLDList
 */
long tldlist_count(TLDList *tld) {
    return tld->size;
}

TLDNode *get_least_leaf(TLDNode *node){
    TLDNode *least;

    // Error Checking
    if (node == NULL){ return node; }
    // Checks both the left and right recursively in-order
    else if(((least = get_least_leaf(node->left)) != NULL) || ((least = get_least_leaf(node->right)) != NULL)){ return least; }
    // If at a leaf node, returns the node as is
    else { return node; }
}


/*
 * get_next_inorder - does in-order iteration over the list as is
 */
TLDNode *get_next_inorder(TLDNode *node, TLDIterator *iter){
    // If the nodes parent right node is itself
    // and:
    // If the node on the right of the parent exists
    // then:
    // Get the next node recursively from the parents right node
    // else:
    // Next node is the parent of the current node
    //if(node->left == NULL && node->right != NULL && node != iter->last) node = node->right;
    if(iter->last == NULL) return node;
    else if((node->parent->right != node) && (node->parent->right != NULL))
        node = get_least_leaf(node->parent->right);
    else
        node = node->parent;

    return node;
}


/*
 * tldlist_iter_create creates an iterator over the TLDList; returns a pointer
 * to the iterator if successful, NULL if not
 */
TLDIterator *tldlist_iter_create(TLDList *tld) {
    TLDIterator *iter = (TLDIterator *) malloc(sizeof(TLDIterator));
    // Error checking
    if (iter == NULL) return NULL;

    // Assigns the first created value to be the least in the list
    iter->node = get_least_leaf(tld->root);
    iter->last = NULL;

    return iter;
}

/*
 * tldlist_iter_next returns the next element in the list; returns a pointer
 * to the TLDNode if successful, NULL if no more elements to return
 */
TLDNode *tldlist_iter_next(TLDIterator *iter) {
    TLDNode *current = iter->node;
    TLDNode *next;

    // If there are no more elements, return null
    if(current == NULL) return NULL;

    // If the current node is the root
    if(current->parent == NULL) {
        current = NULL;
        return current;
    }

    // Assign the next pointer to the returned node
    next = get_next_inorder(current, iter);
    iter->last = current;

    if (next == NULL) return NULL;
    iter->node = next;
    return next;
}

/*
 * tldlist_iter_destroy destroys the iterator specified by `iter'
 */
void tldlist_iter_destroy(TLDIterator *iter) {
    free(iter);
}

/*
 * tldnode_tldname returns the tld associated with the TLDNode
 */
char *tldnode_tldname(TLDNode *node) {
    return node->tld;
}

/*
 * tldnode_count returns the number of times that a log entry for the
 * corresponding tld was added to the list
 */
long tldnode_count(TLDNode *node) {
    return node->count;
}
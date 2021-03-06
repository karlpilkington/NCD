/**
 * @file LinkedList2.h
 * @author Ambroz Bizjak <ambrop7@gmail.com>
 * 
 * @section LICENSE
 * 
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions are met:
 * 1. Redistributions of source code must retain the above copyright
 *    notice, this list of conditions and the following disclaimer.
 * 2. Redistributions in binary form must reproduce the above copyright
 *    notice, this list of conditions and the following disclaimer in the
 *    documentation and/or other materials provided with the distribution.
 * 3. Neither the name of the author nor the
 *    names of its contributors may be used to endorse or promote products
 *    derived from this software without specific prior written permission.
 * 
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND
 * ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
 * WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
 * DISCLAIMED. IN NO EVENT SHALL THE AUTHOR BE LIABLE FOR ANY
 * DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
 * (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
 * LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND
 * ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
 * (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
 * SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 * 
 * @section DESCRIPTION
 * 
 * Doubly linked list that support multiple iterations and removing
 * aritrary elements during iteration.
 */

#ifndef BADVPN_STRUCTURE_LINKEDLIST2_H
#define BADVPN_STRUCTURE_LINKEDLIST2_H

#include <stddef.h>
#include <stdint.h>

#include <misc/debug.h>

struct LinkedList2Iterator_t;

/**
 * Linked list node.
 */
typedef struct LinkedList2Node_t
{
    struct LinkedList2Node_t *p;
    struct LinkedList2Node_t *n;
    struct LinkedList2Iterator_t *it;
} LinkedList2Node;

/**
 * Doubly linked list that support multiple iterations and removing
 * aritrary elements during iteration.
 */
typedef struct
{
    LinkedList2Node *first;
    LinkedList2Node *last;
} LinkedList2;

/**
 * Linked list iterator.
 */
typedef struct LinkedList2Iterator_t
{
    LinkedList2 *list;
    int8_t dir;
    struct LinkedList2Node_t *e;
    struct LinkedList2Iterator_t *pi;
    struct LinkedList2Iterator_t *ni;
} LinkedList2Iterator;

/**
 * Initializes the linked list.
 * 
 * @param list list to initialize
 */
static void LinkedList2_Init (LinkedList2 *list);

/**
 * Determines if the list is empty.
 * 
 * @param list the list
 * @return 1 if empty, 0 if not
 */
static int LinkedList2_IsEmpty (LinkedList2 *list);

/**
 * Returns the first node of the list.
 * 
 * @param list the list
 * @return first node of the list, or NULL if the list is empty
 */
static LinkedList2Node * LinkedList2_GetFirst (LinkedList2 *list);

/**
 * Returns the last node of the list.
 * 
 * @param list the list
 * @return last node of the list, or NULL if the list is empty
 */
static LinkedList2Node * LinkedList2_GetLast (LinkedList2 *list);

/**
 * Inserts a node to the beginning of the list.
 * 
 * @param list the list
 * @param node uninitialized node to insert
 */
static void LinkedList2_Prepend (LinkedList2 *list, LinkedList2Node *node);

/**
 * Inserts a node to the end of the list.
 * 
 * @param list the list
 * @param node uninitialized node to insert
 */
static void LinkedList2_Append (LinkedList2 *list, LinkedList2Node *node);

/**
 * Inserts a node before a given node.
 * 
 * @param list the list
 * @param node uninitialized node to insert
 * @param target node in the list to insert before
 */
static void LinkedList2_InsertBefore (LinkedList2 *list, LinkedList2Node *node, LinkedList2Node *target);

/**
 * Inserts a node after a given node.
 * 
 * @param list the list
 * @param node uninitialized node to insert
 * @param target node in the list to insert after
 */
static void LinkedList2_InsertAfter (LinkedList2 *list, LinkedList2Node *node, LinkedList2Node *target);

/**
 * Removes a node from the list.
 * 
 * @param list the list
 * @param node node to remove
 */
static void LinkedList2_Remove (LinkedList2 *list, LinkedList2Node *node);

/**
 * Returns the next node of a given node.
 * 
 * @param node reference node
 * @return next node, or NULL if none
 */
static LinkedList2Node * LinkedList2Node_Next (LinkedList2Node *node);

/**
 * Returns the previous node of a given node.
 * 
 * @param node reference node
 * @return previous node, or NULL if none
 */
static LinkedList2Node * LinkedList2Node_Prev (LinkedList2Node *node);

/**
 * Initializes a linked list iterator.
 * The iterator memory must remain available until either of these occurs:
 *   - the list is no longer needed, or
 *   - the iterator is freed with {@link LinkedList2Iterator_Free}, or
 *   - the iterator reaches the end of iteration.
 * 
 * @param it uninitialized iterator to initialize
 * @param list the list
 * @param dir direction of iteration. Must be 1 (forward) or -1 (backward).
 * @param node current position of the iterator. Can be a node in the list
 *             or NULL for end of iteration.
 */
static void LinkedList2Iterator_Init (LinkedList2Iterator *it, LinkedList2 *list, int dir, LinkedList2Node *node);

/**
 * Frees a linked list iterator.
 * 
 * @param it iterator to free
 */
static void LinkedList2Iterator_Free (LinkedList2Iterator *it);

/**
 * Initializes a linked list iterator with forward direction, with the current
 * position at the first node (or at the end of iteration if there are no nodes).
 * The iterator memory must remain available until either of these occurs:
 *   - the list is no longer needed, or
 *   - the iterator is freed with {@link LinkedList2Iterator_Free}, or
 *   - the iterator reaches the end of iteration.
 * 
 * @param it uninitialized iterator to initialize
 * @param list the list
 */
static void LinkedList2Iterator_InitForward (LinkedList2Iterator *it, LinkedList2 *list);

/**
 * Initializes a linked list iterator with backward direction, with the current
 * position at the last node (or at the end of iteration if there are no nodes).
 * The iterator memory must remain available until either of these occurs:
 *   - the list is no longer needed, or
 *   - the iterator is freed with {@link LinkedList2Iterator_Free}, or
 *   - the iterator reaches the end of iteration.
 * 
 * @param it uninitialized iterator to initialize
 * @param list the list
 */
static void LinkedList2Iterator_InitBackward (LinkedList2Iterator *it, LinkedList2 *list);

/**
 * Moves the iterator one node forward or backward (depending on its direction), or,
 * if it's at the last or first node (depending on the direction), it reaches
 * the end of iteration, or, if it's at the end of iteration, it remains there.
 * Returns the the previous position.
 * 
 * @param it the iterator
 * @return node on the position of iterator before it was (possibly) moved, or NULL
 *         if it was at the end of iteration
 */
static LinkedList2Node * LinkedList2Iterator_Next (LinkedList2Iterator *it);

void LinkedList2_Init (LinkedList2 *list)
{
    list->first = NULL;
    list->last = NULL;
}

int LinkedList2_IsEmpty (LinkedList2 *list)
{
    return (!list->first);
}

LinkedList2Node * LinkedList2_GetFirst (LinkedList2 *list)
{
    return (list->first);
}

LinkedList2Node * LinkedList2_GetLast (LinkedList2 *list)
{
    return (list->last);
}

void LinkedList2_Prepend (LinkedList2 *list, LinkedList2Node *node)
{
    node->p = NULL;
    node->n = list->first;
    if (list->first) {
        list->first->p = node;
    } else {
        list->last = node;
    }
    list->first = node;
    
    node->it = NULL;
}

void LinkedList2_Append (LinkedList2 *list, LinkedList2Node *node)
{
    node->p = list->last;
    node->n = NULL;
    if (list->last) {
        list->last->n = node;
    } else {
        list->first = node;
    }
    list->last = node;
    
    node->it = NULL;
}

void LinkedList2_InsertBefore (LinkedList2 *list, LinkedList2Node *node, LinkedList2Node *target)
{
    node->p = target->p;
    node->n = target;
    if (target->p) {
        target->p->n = node;
    } else {
        list->first = node;
    }
    target->p = node;
    
    node->it = NULL;
}

void LinkedList2_InsertAfter (LinkedList2 *list, LinkedList2Node *node, LinkedList2Node *target)
{
    node->p = target;
    node->n = target->n;
    if (target->n) {
        target->n->p = node;
    } else {
        list->last = node;
    }
    target->n = node;
    
    node->it = NULL;
}

void LinkedList2_Remove (LinkedList2 *list, LinkedList2Node *node)
{
    // jump iterators
    while (node->it) {
        LinkedList2Iterator_Next(node->it);
    }
    
    // remove from list
    if (node->p) {
        node->p->n = node->n;
    } else {
        list->first = node->n;
    }
    if (node->n) {
        node->n->p = node->p;
    } else {
        list->last = node->p;
    }
}

LinkedList2Node * LinkedList2Node_Next (LinkedList2Node *node)
{
    return node->n;
}

LinkedList2Node * LinkedList2Node_Prev (LinkedList2Node *node)
{
    return node->p;
}

void LinkedList2Iterator_Init (LinkedList2Iterator *it, LinkedList2 *list, int dir, LinkedList2Node *node)
{
    ASSERT(dir == 1 || dir == -1)
    
    it->list = list;
    it->dir = dir;
    it->e = node;
    
    if (it->e) {
        // link into node's iterator list
        it->pi = NULL;
        it->ni = it->e->it;
        if (it->e->it) {
            it->e->it->pi = it;
        }
        it->e->it = it;
    }
}

void LinkedList2Iterator_Free (LinkedList2Iterator *it)
{
    if (it->e) {
        // remove from node's iterator list
        if (it->ni) {
            it->ni->pi = it->pi;
        }
        if (it->pi) {
            it->pi->ni = it->ni;
        } else {
            it->e->it = it->ni;
        }
    }
}

void LinkedList2Iterator_InitForward (LinkedList2Iterator *it, LinkedList2 *list)
{
    LinkedList2Iterator_Init(it, list, 1, list->first);
}

void LinkedList2Iterator_InitBackward (LinkedList2Iterator *it, LinkedList2 *list)
{
    LinkedList2Iterator_Init(it, list, -1, list->last);
}

LinkedList2Node * LinkedList2Iterator_Next (LinkedList2Iterator *it)
{
    // remember original entry
    LinkedList2Node *orig = it->e;
    
    // jump to next entry
    if (it->e) {
        // get next entry
        LinkedList2Node *next;
        switch (it->dir) {
            case 1:
                next = it->e->n;
                break;
            case -1:
                next = it->e->p;
                break;
            default:
                ASSERT(0);
        }
        // destroy interator
        LinkedList2Iterator_Free(it);
        // re-initialize at next entry
        LinkedList2Iterator_Init(it, it->list, it->dir, next);
    }
    
    // return original entry
    return orig;
}

#endif

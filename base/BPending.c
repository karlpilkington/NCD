/**
 * @file BPending.c
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
 */

#include <stddef.h>

#include <misc/debug.h>
#include <misc/offset.h>

#include "BPending.h"

void BPendingGroup_Init (BPendingGroup *g)
{
    // init jobs list
    LinkedList1_Init(&g->jobs);
    
    // init pending counter
    DebugCounter_Init(&g->pending_ctr);
    
    // init debug object
    DebugObject_Init(&g->d_obj);
}

void BPendingGroup_Free (BPendingGroup *g)
{
    DebugCounter_Free(&g->pending_ctr);
    ASSERT(LinkedList1_IsEmpty(&g->jobs))
    DebugObject_Free(&g->d_obj);
}

int BPendingGroup_HasJobs (BPendingGroup *g)
{
    DebugObject_Access(&g->d_obj);
    
    return !LinkedList1_IsEmpty(&g->jobs);
}

void BPendingGroup_ExecuteJob (BPendingGroup *g)
{
    ASSERT(!LinkedList1_IsEmpty(&g->jobs))
    DebugObject_Access(&g->d_obj);
    
    // get a job
    LinkedList1Node *node = LinkedList1_GetLast(&g->jobs);
    BPending *p = UPPER_OBJECT(node, BPending, pending_node);
    ASSERT(p->pending)
    
    // remove from jobs list
    LinkedList1_Remove(&g->jobs, &p->pending_node);
    
    // set not pending
    p->pending = 0;
    
    // execute job
    p->handler(p->user);
    return;
}

BPending * BPendingGroup_PeekJob (BPendingGroup *g)
{
    DebugObject_Access(&g->d_obj);
    
    LinkedList1Node *node = LinkedList1_GetLast(&g->jobs);
    if (!node) {
        return NULL;
    }
    
    return UPPER_OBJECT(node, BPending, pending_node);
}

void BPending_Init (BPending *o, BPendingGroup *g, BPending_handler handler, void *user)
{
    // init arguments
    o->g = g;
    o->handler = handler;
    o->user = user;
    
    // set not pending
    o->pending = 0;
    
    // increment pending counter
    DebugCounter_Increment(&o->g->pending_ctr);
    
    // init debug object
    DebugObject_Init(&o->d_obj);
}

void BPending_Free (BPending *o)
{
    DebugCounter_Decrement(&o->g->pending_ctr);
    DebugObject_Free(&o->d_obj);
    
    // remove from jobs list
    if (o->pending) {
        LinkedList1_Remove(&o->g->jobs, &o->pending_node);
    }
}

void BPending_Set (BPending *o)
{
    DebugObject_Access(&o->d_obj);
    
    // remove from jobs list
    if (o->pending) {
        LinkedList1_Remove(&o->g->jobs, &o->pending_node);
    }
    
    // insert to jobs list
    LinkedList1_Append(&o->g->jobs, &o->pending_node);
    
    // set pending
    o->pending = 1;
}

void BPending_Unset (BPending *o)
{
    DebugObject_Access(&o->d_obj);
    
    if (o->pending) {
        // remove from jobs list
        LinkedList1_Remove(&o->g->jobs, &o->pending_node);
        
        // set not pending
        o->pending = 0;
    }
}

int BPending_IsSet (BPending *o)
{
    DebugObject_Access(&o->d_obj);
    
    return o->pending;
}

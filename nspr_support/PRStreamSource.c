/**
 * @file PRStreamSource.c
 * @author Ambroz Bizjak <ambrop7@gmail.com>
 * 
 * @section LICENSE
 * 
 * This file is part of BadVPN.
 * 
 * BadVPN is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License version 2
 * as published by the Free Software Foundation.
 * 
 * BadVPN is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 * 
 * You should have received a copy of the GNU General Public License along
 * with this program; if not, write to the Free Software Foundation, Inc.,
 * 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301 USA.
 */

#include <prerror.h>

#include <misc/debug.h>

#include <nspr_support/PRStreamSource.h>

static void report_error (PRStreamSource *s, int error)
{
    DEAD_ENTER(s->dead)
    FlowErrorReporter_ReportError(&s->rep, &error);
    ASSERT(DEAD_KILLED)
    DEAD_LEAVE(s->dead);
}

static void try_recv (PRStreamSource *s)
{
    ASSERT(s->out_avail > 0)
    
    int res = PR_Read(s->bprfd->prfd, s->out, s->out_avail);
    if (res < 0 && PR_GetError() == PR_WOULD_BLOCK_ERROR) {
        // wait for socket in prfd_handler
        BPRFileDesc_EnableEvent(s->bprfd, PR_POLL_READ);
        return;
    }
    
    if (res < 0) {
        report_error(s, PRSTREAMSOURCE_ERROR_NSPR);
        return;
    }
    
    if (res == 0) {
        report_error(s, PRSTREAMSOURCE_ERROR_CLOSED);
        return;
    }
    
    ASSERT(res > 0)
    ASSERT(res <= s->out_avail)
    
    // finish packet
    s->out_avail = -1;
    StreamRecvInterface_Done(&s->output, res);
}

static void output_handler_recv (PRStreamSource *s, uint8_t *data, int data_avail)
{
    ASSERT(data_avail > 0)
    ASSERT(s->out_avail == -1)
    DebugObject_Access(&s->d_obj);
    
    // set packet
    s->out_avail = data_avail;
    s->out = data;
    
    try_recv(s);
    return;
}

static void prfd_handler (PRStreamSource *s, PRInt16 event)
{
    ASSERT(s->out_avail > 0)
    ASSERT(event == PR_POLL_READ)
    DebugObject_Access(&s->d_obj);
    
    try_recv(s);
    return;
}

void PRStreamSource_Init (PRStreamSource *s, FlowErrorReporter rep, BPRFileDesc *bprfd, BPendingGroup *pg)
{
    // init arguments
    s->rep = rep;
    s->bprfd = bprfd;
    
    // init dead var
    DEAD_INIT(s->dead);
    
    // add socket event handler
    BPRFileDesc_AddEventHandler(s->bprfd, PR_POLL_READ, (BPRFileDesc_handler)prfd_handler, s);
    
    // init output
    StreamRecvInterface_Init(&s->output, (StreamRecvInterface_handler_recv)output_handler_recv, s, pg);
    
    // have no output packet
    s->out_avail = -1;
    
    DebugObject_Init(&s->d_obj);
}

void PRStreamSource_Free (PRStreamSource *s)
{
    DebugObject_Free(&s->d_obj);
    
    // free output
    StreamRecvInterface_Free(&s->output);
    
    // remove socket event handler
    BPRFileDesc_RemoveEventHandler(s->bprfd, PR_POLL_READ);
    
    // free dead var
    DEAD_KILL(s->dead);
}

StreamRecvInterface * PRStreamSource_GetOutput (PRStreamSource *s)
{
    DebugObject_Access(&s->d_obj);
    
    return &s->output;
}

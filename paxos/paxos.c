//
// Created by roni on 3/9/20.
//
#include "paxos.h"
#include "common.h"
#include <linux/slab.h>


void
paxos_value_free(paxos_value* v)
{
    pfree(v->paxos_value_val);
    pfree(v);
}

static void
paxos_value_destroy(paxos_value* v)
{
    if (v->paxos_value_len > 0)
        pfree(v->paxos_value_val);
}

void
paxos_accepted_free(paxos_accepted* a)
{
    paxos_accepted_destroy(a);
    pfree(a);
}

void
paxos_promise_destroy(paxos_promise* p)
{
    paxos_value_destroy(&p->value);
}

void
paxos_accept_destroy(paxos_accept* p)
{
    paxos_value_destroy(&p->value);
}

void
paxos_accepted_destroy(paxos_accepted* p)
{
    paxos_value_destroy(&p->value);
}

void
clearPaxosAccepted(paxos_accepted* accepted) {
  accepted -> iid = 0;
  accepted -> promise_iid = 0;
  accepted -> ballot = 0;
  accepted -> value.paxos_value_len = 0;
  memset(accepted -> value.paxos_value_val, 0, MAX_PAXOS_VALUE_SIZE);
}
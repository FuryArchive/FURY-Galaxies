# Settlement transaction isolation

Before acquiring the final settlement lock chain and before any market
mutation, FURY calls `commitLocalTransaction()`.

Engine3 stores pending database writes in a worker-thread-local
`CurrentTransaction`. Normally Task completion clears this queue, but the
explicit pre-settlement flush establishes a stronger invariant for the NPC
market path:

> every write/delete queued after this point belongs to this settlement.

No FURY purchase mutation has occurred at the flush point. The later explicit
seller-credit, city, demand, AuctionItem and sold-object writes therefore form
an isolated local update vector before the single durable commit.

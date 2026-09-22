# Post-commit side effects are outside settlement locks

The durable settlement critical section now contains only:

1. live revalidation;
2. in-memory market mutations;
3. persistent object serialization / deletes;
4. one synchronous Berkeley commit.

After commit, the function copies immutable receipt fields while the locks are
still held, exits the city-lock scope, then explicitly releases the remaining
AuctionItem, sold-object, CreditObject and demand-state locks.

Only after those releases does FURY perform:

- TransactionLog filesystem audit;
- informational logging;
- online seller lookup;
- CREO bank/cash delta packet construction;
- seller system message.

This avoids lock inversion through TransactionLog/player lookup and avoids
network I/O while settlement state locks are held. Client deltas use the
captured committed bank/cash values, not a later unlocked reread.

# Post-commit seller notification

NPC settlement mutates seller credits with `notifyClient=false`. This prevents
a client from observing money that has not reached the durable Berkeley
boundary yet.

After `commitLocalTransaction()` returns successfully, an online seller
receives the same CREO delta fields used by Core3's CreditObject:

- baseline 1, field 0x00: bank credits;
- baseline 1, field 0x01: cash credits.

Both are sent because bank overflow may legitimately spill into cash.

The system-message receipt is also emitted only after the durable commit.

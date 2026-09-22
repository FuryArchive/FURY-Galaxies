# Settlement capacity guards

FURY does not allow an NPC purchase to silently destroy credits at storage
caps.

Seller credit capacity is computed under the CreditObject lock as:

`(CREDITCAP - bank) + (CREDITCAP - cash)`

If the planned seller net payout exceeds that capacity, the purchase is
rejected.

For player cities, a settlement is also rejected when applying the planned tax
would exceed Core3's 100,000,000-credit city treasury clamp. This preserves the
conservation invariant for the simulated purchase instead of burning the
overflow.

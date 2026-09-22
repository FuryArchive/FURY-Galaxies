# Seller credit-state integrity gate

Before an NPC purchase can create credits, FURY requires:

- the loaded CreditObject owner OID to match the listing seller OID;
- bank balance in `[0, CREDITCAP]`;
- cash balance in `[0, CREDITCAP]`;
- enough combined remaining bank+cash capacity for the complete net payout.

Corrupt out-of-range balances are rejected instead of relying on
`addBankCredits()` overflow behavior to normalize them during a market sale.

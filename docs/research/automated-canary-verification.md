# Automated market crash-state verification

The Docker harness includes two companion commands.

`fury-market-probe` launches `core3 shutdown` with execution disabled and
the read-only C++ canary probe enabled. `capture` creates the baseline JSON;
`inspect` loads that baseline as fallback metadata for post-crash state.

`fury-market-verify` compares the baseline and observed JSON:

- stages 1-5 require the listing/AuctionItem/root graph, seller balances, city
  treasury and demand to match baseline;
- stages 6 and 0 require listing/AuctionItem/root graph deletion, exact expected
  seller balances, expected treasury and expected demand;
- every persistent child OID is checked individually;
- floating demand/treasury comparisons use a small numerical tolerance.

This turns the crash-recovery proof from a visual log inspection into a
machine-verifiable gate.

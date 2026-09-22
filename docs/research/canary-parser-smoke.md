# Canary selector parser smoke coverage

The decimal 64-bit canary parser is part of the portable execution-scope
helper, not AuctionManager glue.

Its smoke test covers:

- empty selector -> zero/unset;
- explicit zero;
- exact `UINT64_MAX`;
- uint64 overflow;
- non-decimal characters;
- negative text.

Malformed selectors mark the scope invalid and therefore reject every
candidate.

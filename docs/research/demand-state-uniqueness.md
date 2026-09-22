# Persistent demand state uniqueness

`furyeconomy` is a singleton-state database for the current economy slice.

Startup previously tolerated multiple valid `FuryEconomyState` objects and
silently selected the first iterator result. That makes restart behavior depend
on database iteration order.

FURY now fails closed when more than one valid state object exists:

- no arbitrary state is selected;
- `furyEconomyState` remains null;
- purchase execution is therefore suppressed by the existing runtime gate.

A duplicate-state database must be repaired explicitly before real settlement
can resume.

# Integrated canary crash verification

`fury-market-canary` now accepts `--verify <baseline-probe>` for failure
stages 1-6.

After the intentional crash marker is validated, the runner invokes
`fury-market-probe inspect` on the crashed database and then
`fury-market-verify`. Combined with `--restore baseline`, one command now
performs restore -> crash -> graceful probe -> invariant verification.

The real canary config writer also removes any stale `FURY MARKET PROBE`
override block before enabling execution. This prevents a previous read-only
probe from silently leaving `ExecutePurchases=0` after a later canary block.

Stage 0 intentionally does not support inline `--verify`: a normal successful
server run remains active for observation and should be stopped deliberately
before post-success probing.

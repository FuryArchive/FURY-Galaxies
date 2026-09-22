# Automatic graceful shutdown for stage 0 canary

Failure-injection stages 1-6 already terminate themselves. The normal success
canary previously required a person to stop Core3 after the target purchase.

FURY now has a fail-closed `CanaryAutoShutdown` option, disabled by default.
The Docker canary runner enables it only for stage 0.

After the exact canary listing reaches the durable commit and all post-commit
audit / online-seller notification work has completed, the settlement queues
Core3's native `shutdown 0` console command. The command follows the normal
graceful shutdown path and final database save.

The runner wraps stage 0 in the same timeout used by crash stages and accepts
success only when:

- Core3 exits cleanly;
- the exact listing has a durable-settlement log marker;
- the exact listing has the canary auto-shutdown marker.

`--verify baseline` is now supported for stage 0, so all seven settlement
proof runs can restore, execute, restart-probe and verify without manual server
interaction.

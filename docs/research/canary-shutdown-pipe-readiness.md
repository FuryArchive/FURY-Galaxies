# Canary shutdown command-pipe readiness

Core3 creates its console command pipe only when `ServerCore::run()` enters
`handleCommands()`. The FURY market tick is scheduled from AuctionManager
initialization, so on an unusually slow startup a successful canary settlement
can theoretically occur before that pipe exists.

Writing to an uncreated engine3 `Pipe` throws an `IOException`. A direct
post-settlement shutdown command would therefore make the stage-0 harness
timing-dependent.

FURY now schedules `FuryCanaryShutdownTask` one second after a committed stage
0 settlement. The task retries the native `shutdown 0` command once per second
for up to 30 attempts. Only a successful queue operation emits the
`queued graceful shutdown` marker accepted by the Docker runner.

If the pipe never becomes available, the task logs an error and the outer
canary timeout fails the run rather than reporting a false success.

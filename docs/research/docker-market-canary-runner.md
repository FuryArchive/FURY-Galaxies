# One-command Docker market canary runner

The Docker image installs `/usr/local/bin/fury-market-canary`. Keeping the
runner in the image instead of the persistent `/home/swgemu` volume means a
rebuilt image receives harness fixes even when an existing developer volume is
reused.

The runner accepts failure stage 0-6 plus exact listing and seller OIDs,
validates both selectors as non-zero uint64 decimal strings, idempotently
replaces a marked FURY canary block, verifies TRE data and MariaDB, and rebuilds
Core3 when the checked-out git SHA differs from the last canary build.

Stages 1-6 launch Core3 directly without gdb and accept the crash only when the
expected failure-injection marker is present. Stage 0 is the normal-success
foreground canary.

The direct crash-stage launch is required because the inherited gdb command
file catches SIGABRT and stops at the signal instead of allowing the deliberate
fail-stop to terminate.

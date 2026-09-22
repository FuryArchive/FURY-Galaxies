# FURY market baseline snapshot/restore

The Docker image installs `fury-market-snapshot` to make crash testing
repeatable without manually copying database files.

A baseline snapshot contains a compressed `mysqldump` of the configured SWG
application database, a compressed copy of the complete Core3 Berkeley
`databases/` directory, and metadata with timestamp and Core3 git SHA.

Save and restore operations refuse to run while Core3 is active. Archives are
validated before installation. Berkeley data is staged before replacement, and
the previous directory is retained until the MariaDB restore succeeds.

The intended flow is:

```
fury-market-snapshot save baseline
fury-market-canary 1 LISTING SELLER --restore baseline
...
fury-market-canary 6 LISTING SELLER --restore baseline
fury-market-canary 0 LISTING SELLER --restore baseline
```

Create the baseline only while the server is stopped and after the deterministic
seller/vendor/listing fixture has been prepared.

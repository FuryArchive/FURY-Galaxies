# Atomic rollback for canary snapshot restore

The crash-test harness treats the MariaDB application database and Core3's
Berkeley database directory as one logical test state.

Before installing a saved snapshot, `fury-market-snapshot restore` now captures
a temporary rollback dump of the current MariaDB database. The existing
Berkeley directory is retained by rename until the SQL import succeeds.

If the saved MariaDB import fails after the Berkeley directory has already been
installed, the helper now:

1. removes the newly installed Berkeley directory;
2. restores the previous Berkeley directory;
3. reimports the temporary MariaDB rollback image;
4. fails the restore operation.

If the MariaDB rollback itself also fails, the helper reports that explicitly
instead of claiming the old state was restored.

Docker smoke coverage injects a failing first MariaDB import and verifies that
the old Berkeley marker and rollback SQL are both restored. This prevents a
failed harness restore from silently contaminating the next crash stage with a
mixed database state.

# v0.32.0 Better Monitors

(May 12, 2022)

## What's Changed?

You must upgrade your Go version to 1.18. Instructions:


This migration moves a significant part of our code (the creation and updating of address monitors) to native GoLang code. Go's ability to run concurrently allows a significant performance improvement (more than 25x) of the initial scan for an address's history.

This migration is mandatory, although you need not do so all at once. TrueBlocks will warn you to migrate the first time you try to access an old-style monitor file.

### What do you need to do?

The first time you run `chifra list`, `chifra export`, or certain other commands after re-building, you will be presented with a message saying a migration is needed and directing you to this page.

The time required for this migration is anywhere from a few seconds to a few minutes depending on how long the history of your existing monitored addresses are.

This is a simple proceedure.

### Before you start

Stop any long running TrueBlocks processes (such as the `chifra scrape` or `chifra serve`). Do not restart them until the migration is complete.

You may wish to make a backup copy of your cache (although, being a cache, any cached files can be re-created, so this is optional). Find your `cachePath` with `chifra status --terse`.

## Instructions

### Assuming `chifra` is in your $PATH...

Run this command:

```
chifra status --migrate test
```

This will not modify any files. It will only tell you if any files need to be migrated.

If the above command reports that no files need to be migrated, you are finished.

If the above command reports that files need to be migrated (likely), run the following command (after making a backup if you wish):

```
chifra status --migrate all
```

This will migrate all of your existing monitors. It should complete with no problems. If it does, you're finished.

You may wish to complete the above command for any other chains you're working with (by adding `--chain <chainname>`), although, you may do this later as well.

### What should I do if there's a problem

Because this migration involves upgrading the cache, any damaged or incompletely migrated files can be removed. They will be re-created during normal operation. In the unlikely event of a problem, you may remove the contents of the `monitors` subfolder in your `cachePath` (find `cachePath` with `chifra status --terse`).

After deleting the contents of the `monitors` folder (do not delete the folder itself), run `chifra status --migrate all` again. It should report a sucessful migration. Your old monitors will be re-created the next time you query your addresses.

## You're finished!

You may now restart any long-running processes you stopped earlier: `chifra scrape` or `chifra serve`.

Please report any problems by creating an issue.

## Previous Migration

[Click here](./README-v0.27.0.md) for the previous migration.

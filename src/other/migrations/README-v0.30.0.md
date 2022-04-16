# v0.30.0 Much Better Monitors

(Apr 16, 2022)

## Why the Change?

This migration moves a significant part of our code (the creation and updating of address Monitors) to native GoLang code.

Go's ability to run concurrently allowed a significant speed up (more than 25x) in our initial scan of the index for new addresses.

This migration is mandatory. You must migrate all chains.

### What do you need to do?

Whenever you run `chifra list`, `chifra export`, or certain options on other commands, you may be told a migration is needed and directed to this page.

This migration will take anywhere from a few seconds to a few minutes depending on how many and how big your monitored addresses are.

This is a very simple proceedure. You'll only need to run a single command.

### Before you start

Stop any long running TrueBlocks processes (such as the `chifra scrape` or `chifra serve`). Do not restart them until the migration is complete.

You may wish to make a backup copy of your cache (although, being a cache, any damaged files can be re-created. Find your `cachePath` with `chifra status --terse`.

## Instructions

### Assuming `chifra` is in your $PATH...

Run this command:

```
chifra status --migrate test
```

This will not modify any files. It will only tell you if you need to do the migration.

If the above command reports that no files need to be migrated, you are finished.

If the above command reports that files need to be migrated, run this command (after making a backup if you choose to do so):

```
chifra status --migrate all
```

This will migrate all monitors and complete with no problems. If it does, you're finished.

### What should I do if there's a problem

Being a cache, any damaged or incompletely migrated files can be removed. They will be re-created during normal operation. In the unlikely event of a problem, you may remove the `monitors` subfolder in your `cachePath` (find `cachePath` with `chifra status --terse`).

Run `chifra status --migrate test` again. It should report that no migration is necessary. Your old monitors will need to be re-created, but it's not a problem as the new Monitoring code is so much faster it won't matter. The tx and reconciliation caches will remain intact.

## You're finished!

You may now restart any processes you stopped earlier: `chifra scrape` or `chifra serve`.

Please report any problems by creating an issue.

## Previous Migration

[Click here](./README-v0.27.0.md) for the previous migration.

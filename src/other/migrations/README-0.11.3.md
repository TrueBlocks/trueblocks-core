# v0.11.3 London Hard Fork Migration

**Warning:** If you're not confident using the command line, please contact us prior to proceeding. If you make a mistake and delete the wrong files, you may damage your computer.

**DO NOT SAY WE DIDN'T WARN YOU.**

---

(v0.11.3)

## Why the Change?

We changed code relative to the London Hard Fork. While it is technically possible to run without doing a migration, it may be easier or sometime necessary to complete the following tasks to ensure a valid cache.

These instructions should work, but if they don't you may always simply remove the TrueBlocks cache. As is true of all caches, you can delete it and it will be re-created.

## Where is the cache

If you're on a Mac, your cache is at `~/Library/Application Support/TrueBlocks/cache`. If you're on Linux, your cache is at `~/.local/share/trueblocks/cache`. If you're on Windows, you have no cache because TrueBlocks does not work on Windows.

## Migrating to v0.11.3

Complete the following steps from the `~/build` folder in the TrueBlocks repo:

```[bash]
git pull
cmake ../src
make clean
make -j2
```

Once that's done, you may complete the migration with:

```
chifra status --migrate test
```

which will just check to see if the cache needs to be upgraded. If it reports everythign is up to date, you are finished.

If the above command reports that some parts of the cache need to be updated, then do:

```
chifra status --migrate all
```

## What Should I Do If that Doesn't Work?

You may always remove the cache entirely. There are no files in the cache that cannot be re-created. Be careful, however, that you do not remove the wrong files. Do not say we didn't warn you.

## You're Done

TrueBlocks version 0.11.3 should now work. Please report any problems by creating an issue.

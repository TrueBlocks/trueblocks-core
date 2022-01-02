# v0.11.3 London Hard Fork Migration

**Warning:** If you're not confident using the command line, please contact us prior to proceeding. If you make a mistake and delete the wrong files, you may damage your computer.

**DO NOT SAY WE DIDN'T WARN YOU.**

---

(v0.11.3)

## Why the Change?

In version 0.11.3, we first supported the changes needed for the London Hard Fork.

While it is technically possible to run the code without doing the following migration, it may be easier or sometime necessary to complete the following tasks to ensure a valid cache.

In most cases, these instructions will work, but if they don't, you may simply remove the entire cache. As is true of all caches, if the cache is removed, it will be re-created.

## Where is the cache

On a Mac, your cache is at `~/Library/Application Support/TrueBlocks/cache`. On Linux, your cache is at `~/.local/share/trueblocks/cache`. On Windows, there is no cache, because TrueBlocks does not work on Windows.

## Migrating to v0.11.3

As a pre-requisite, complete the following steps from the `~/build` folder in the TrueBlocks repo:

```[bash]
git pull
cmake ../src
make clean
make -j2
```

Once that's done, complete the migration by first doing this:

```
chifra status --migrate test
```

which checks to see if the cache needs to be migrated. If the above command reports that everything is up to date, you are finished.

If the above command reports that some parts of the cache need to be migrated, complete the following command:

```
chifra status --migrate all
```

## What To Do if the Above Doesn't Work

It is alway possible to remove the cache. Note, however, that the previously cached files will have to be re-created. This will have a negative effect on the performance of TrueBlocks until the cache is re-created. (Speeding up reponse times is the whole purpose of the cache, after all.)

Be careful. Make sure you know what you're doing. Do not remove the wrong files. Do not say we didn't warn you.

## You're Finished

TrueBlocks version 0.11.3 should now work. Please report any problems by creating an issue.

## Previous Migration

[Click here](./README-v0.09.0.md) for the previous migration.

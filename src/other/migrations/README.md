# v0.9.0 Configuration File Migration

**Warning:** If you're not confident using the command line, please contact us prior to proceeding. If you make a mistake and delete the wrong files, you may damage your computer.

**DO NOT SAY WE DIDN'T WARN YOU.**

---

(v0.9.0)

## Why the Change?

With version 0.9.0, we made two primary changes to the way we store configuration and cached data on your computer. Those changes are:

1. We now support the [BaseDir Spec](https://specifications.freedesktop.org/basedir-spec/basedir-spec-latest.html) which describes where to store local configuration files, and

2. We separated our primary cache from our index of appearances (that is, the Unchained Index). This allows you to remove the primary cache without removing the much larger and longer to create Unchained Index.

## Migrating Configuration Files

First, we must move the existing configuration folder which exists at `$HOME/.quickBlocks`. Note that If the first `mv` command fails because the destination folder exists, you may add `-f` to the `mv` command or remove the destination folder.

On Apple:

```[bash]
rm -fR "~/Library/Application Support/TrueBlocks/*"               # be careful here!
mv ~/.quickBlocks/* "~/Library/Application Support/TrueBlocks/"
rmdir ~/.quickBlocks
cd "~/Library/Application Support/TrueBlocks/"
```

On Linux:

```[bash]
rm -fR ~/.local/share/trueblocks/*                               # be careful here!
mv ~/.quickBlocks/* ~/.local/share/trueblocks/
rmdir ~/.quickBlocks
cd ~/.local/share/trueblocks
```

On Windows:

```[bash]
You're out of luck. TrueBlocks does not support Windows.
```

## Moving the Configuration File

After moving the folder, we need to move the configuration file. The following assumes you are in the new configuration folder created above.

```[bash]
mv quickBlocks.toml trueBlocks.toml
```

## Moving the Unchained Index

In the new configuration folder, check to see if a folder called `./cache/addr_index` exits (`ls -l ./cache`). If it does, move it to its new location:

```[bash]
# Ignore if the folder does not exist or is configured elsewhere.
mv ./cache/addr_index ./unchained
```

## Edit the Configuration File (important)

There should be a file called `./trueBlocks.toml` at the top of this folder.

Edit that file and clean up any dangling paths. Change `~/.quickBlocks` to the new configuration folder throughout.

## You're Migrated

The new configuration folder should look something like this (the `cache` and `unchained` folders may be slightly different):

<img alt="Folders" src="https://github.com/TrueBlocks/trueblocks-core/blob/new-default-dir/src/other/migrations/folders.png" width="250px" />

TrueBlocks version 0.9.0 should now work. Please report any problems by creating an issue.

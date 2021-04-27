# Migrating from versions prior to 0.9.0

**Warning:** If you're not familiar with using the command line, we suggest that you contact us via our chat group prior to proceeding. If you make a mistake, for example by incorrectly removing configuration files other than TrueBlocks', you may damage your computer.

If you simply start over with the new default config folders, TrueBlocks will rebuild any files or folders it needs to operate.(Although the rebuilding does take extra time.)

**DO NOT SAY WE DIDN'T WARN YOU. THIS IS US WARNING YOU.**

## Why the Change?

As part of version 0.9.0, we made two primary changes to the way we store data on your hard drive. Those changes are:

1. We now support [this spec](https://specifications.freedesktop.org/basedir-spec/basedir-spec-latest.html) which describes where to store local configuration files on various operating systems, and
2. We separated the general TrueBlocks cache from the TrueBlocks index of appearances cache (the unchained index). This allows you to remove the general cache without removing the unchained index.

Both of these changes make it easier for us to move forward.

## Changing the Location of the TrueBlocks Configuration Files

We begin by moving the existing configuration folder which previously existed at `$HOME/.quickBlocks`. If the first `mv` command fail because the destination folder exists, add `-f` to the `mv` command or remove the destination folder.

On Apple computers:

```[bash]
mkdir -p "~/Library/Application Support/TrueBlocks/"
mv ~/.quickBlocks/* "~/Library/Application Support/TrueBlocks/"
rmdir ~/.quickBlocks
cd "~/Library/Application Support/TrueBlocks/"
```

On Linux:

```[bash]
mkdir -p ~/.local/share/trueblocks/
mv ~/.quickBlocks/* ~/.local/share/trueblocks/
rmdir ~/.quickBlocks
cd ~/.local/share/trueblocks
```

After moving the folder, rename the default configuration file. This assumes you are in the new default config folder created above.

```[bash]
mv quickBlocks.toml trueBlocks.toml
```

In the new configuration folder, check to see if a folder called `./cache/addr_index` exitss (`ls -l ./cache`). If it is present, move it to its new location:

```[bash]
mv ./cache/addr_index ./unchained
```

# Editing the Configuration TOML file

You should now be in the destination folder created above. The folders should look like this:

<img src="https://github.com/TrueBlocks/trueblocks-core/blob/new-default-dir/src/other/migrations/folders.png" width="640" />

We must now edit the configuration file to make sure no dangling paths exist there. Open an editor and look at all the entries. If any reference old paths (containing `$HOME/.quickBlocks`), change those paths to reference the new config folder.


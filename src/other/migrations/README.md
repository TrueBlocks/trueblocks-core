# Configuration File Migration

**Warning:** If you're not confident using the command line, please contact us prior to proceeding. If you make a mistake and delete the wrong files, you may damage your computer.

**DO NOT SAY WE DIDN'T WARN YOU.**

---

(v0.9.0)

## Why the Change?

With version 0.9.0, we made two primary changes to the way we store configuration and cached data on your computer. Those changes are:

1. We now support the [BaseDir Spec](https://specifications.freedesktop.org/basedir-spec/basedir-spec-latest.html) which describes where to store local configuration files on various operating systems, and
2. We chose to separate the primary TrueBlocks cache from the TrueBlocks index of appearances cache (the Unchained Index). This allows our users to remove the primary cache without removing the Unchained Index. Because the Unchained Index is immutable, there's no real reason to ever remove it.

## Migrating TrueBlocks Configuration Files

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

![Folders](https://github.com/TrueBlocks/trueblocks-core/blob/new-default-dir/src/other/migrations/folders.png?s=200)

There should be a file called `./trueBlocks.toml` at the top of the new configuration folder.

Edit the file to clean up any dangling paths. If any settings reference the old configuration paths (`~/.quickBlocks`), change those paths to reference the new folder.

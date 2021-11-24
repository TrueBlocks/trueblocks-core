# v0.9.0 Configuration File Migration

**Warning:** If you're not confident using the command line, please contact us prior to proceeding. If you make a mistake and delete the wrong files, you may damage your computer.

**DO NOT SAY WE DIDN'T WARN YOU.**

---

(v0.9.0)

## Why the Change?

With version 0.9.0, we made two changes to the way we store configuration and cache data. Those changes are:

1. We now support the [BaseDir Spec](https://specifications.freedesktop.org/basedir-spec/basedir-spec-latest.html) which describes where to store local configuration files, and

2. We separated the primary cache from the index of appearances (also called the Unchained Index). This allows you to remove the primary cache without removing the Unchained Index (which is immutable anyway and takes longer to create).

## Preliminaries

If you haven't already done so, make sure to pull the latest code and re-build it. From the `./trueblocks-core` folder (see the main README for more information):

```[bash]
git checkout develop
git pull
cd build
cmake ../src
make clean         # this is optional, but more explicit
make -j <nprocs>
```

where **nprocs** is the number of processors on your machine (or '1' if you're unsure).

## Migrating Configuration Files

After rebuilding, the first thing to do is move the existing configuration folder which previously existed at `$HOME/.quickBlocks`.

On Apple:

```[bash]
rm -fR "~/Library/Application Support/TrueBlocks/*"               # clean up in case it exists. Be careful here!
mkdir -p "~/Library/Application Support/TrueBlocks/"              # re-establish the folder
mv ~/.quickBlocks/* "~/Library/Application Support/TrueBlocks/"
rmdir ~/.quickBlocks
cd "~/Library/Application Support/TrueBlocks/"
```

On Linux:

```[bash]
rm -fR ~/.local/share/trueblocks/*                                # clean up in case it exists. Be careful here!
mkdir -p "~/.local/share/trueblocks/"                             # re-establish the folder
mv ~/.quickBlocks/* ~/.local/share/trueblocks/
rmdir ~/.quickBlocks
cd ~/.local/share/trueblocks
```

On Windows:

```[bash]
You're out of luck. TrueBlocks does not support Windows.
```

Make sure to remove the old `~/.quickBlocks` folder when you're done. `chifra` will continue to complain about needing a migration until that folder no long exists.

## Moving the Unchained Index

In the new configuration folder, check to see if a folder called `./cache/addr_index` exits (`ls -l ./cache/addr_index/`). If that folder exists, move it to its new location:

```[bash]
# Ignore if the folder does not exist or is configured elsewhere.
mv ./cache/addr_index ./unchained
```

## Moving the Configuration File

After moving the folders, the next thing to do is move the configuration file. The following assumes you are in the new configuration folder.

```[bash]
mv quickBlocks.toml trueBlocks.toml
```

## Edit the Configuration File (important!)

The final step is to edit the configuration file (now called `./trueBlocks.toml`).

Open it with any editor and change any references to the old path (`~/.quickBlocks`) to the new path. While you're at it, verify that any other settings in this file are correct.

## You're Done

The new configuration folder should look something like this (the `cache` and `unchained` folders may be slightly different):

<img alt="Folders" src="https://github.com/TrueBlocks/trueblocks-core/blob/new-default-dir/src/other/migrations/folders.png" width="250px" />

TrueBlocks version 0.9.0 should now work. Please report any problems by creating an issue.

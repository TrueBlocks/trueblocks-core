# Migrating from versions prior to 0.9.0

On Apple:

```[bash]
mv ~/.quickBlocks "~/Library/Application Support/TrueBlocks"
cd "~/Library/Application Support/TrueBlocks"
```

On Linux:

```[bash]
mv ~/.quickBlocks ~/.local/share/trueblocks
cd ~/.local/share/trueblocks
```

Then...

```[bash]
mv quickBlocks.toml trueBlocks.toml
```

If ./cache/addr_index is present on your machine

```[bash]
mv ./cache/addr_index ./unchained
```

Now, edit ./trueBlocks.toml and adjust any paths in this file to point to the new locations.

Prior to your change, the configuration folder is called `~/.quickBlocks` and has this structure:

<img src="https://github.com/TrueBlocks/trueblocks-core/blob/new-default-dir/src/other/migrations/folders.jpg" width="640" />

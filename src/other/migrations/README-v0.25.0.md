# v0.25.0 Multi-chain

---

(February 8, 2022)

## Why the Change?

With this migration we enter into the world of multi-chain blockchains.

Under the covers, this was a massive change to our code base. On the surface, it's quite simple: we added a `--chain` option to all commands. We've pre-configured a few chains for you, but you can add support for any EVM-based chain yourself.

This migration require you to edit a configuration files and move some existing folders to new locations. Be careful, if you make a mistake, you could damage not only your installation of TrueBlocks, but your computer.

Please don't say we didn't warn you.

### What's changed

This migration consists of three major changes:

- the location of the two folders (`cache` and `unchained` index) have changed,  
- two configuration items in `trueBlocks.toml` pointing to those folders have changed,  
- a number of other configuration files have been removed or moved.

### The migration in a single image

![Folders](./migration.25.png)

The above image shows the migration visually. All of the files and folders you will be moving are in TrueBlocks' root `$CONFIG` folder (`~/.local/share/trueblocks/` on Linux, `~/Library/Application Support/TrueBlocks/` on Mac).

## Instructions

In the following, we assume you're working on a Linux installation. If you're on a Mac, adjust the paths as appropriate.

### Before you start

- Stop any long running TrueBlocks processes (such as the `chifra scrape` or `chifra serve`). Do not restart them until the migration is completed.

- Preserve the old `trueBlocks.toml` configuration file:

```
mv $HOME/.local/share/trueblocks/trueBlocks.toml ./trueBlocks.save
```

### Rebuilding

- Download and rebuild the latest version (`multi-chain` branch) of TrueBlocks. Do this from the source code folder:


```
cd ./build
git pull
git checkout multi-chain   # once we merge this change, you will use the 'develop' branch
cmake ../src
make -j 4
```

This will create the new folder structure and install the newly formatted `trueBlocks.toml` file in the root config folder. Change to that folder:

```
cd $HOME/.local/share/trueblocks
pwd
```

On Linux, you should be in `$HOME/.local/share/trueblocks`. On Mac, `$HOME/Library/Application Support/TrueBlocks`.

From now on, until you've completed the migration, all `chifra` commands will refuse to run and direct you to this page.

### Moving existing cache and unchained index folders

**Important Note:** If you've customized the following paths, adjust these instructions accordingly.
 
**Note:** You may get a warnings with some of the following commands saying you're trying to move a folder into itself. It's okay.

Complete the following steps:

- Move existing cache folders into the new chain-specific `mainnet` cache folder:

```
cd $HOME/.local/share/trueblocks/cache
mv * mainnet
```

- Move existing unchained index folders into the new chain-specific `mainnet` unchained index folder:

```
cd $HOME/.local/share/trueblocks/unchained
mv * mainnet
```

`chifra` commands should continue to complain and refuse to run until you complete the following steps.

### Replace config values in the new config file

The goal of this final step is to replace a few old configuration items in the new configuration file (`trueBlocks.toml`).

We won't instruct you on how to edit files on the command line, but find these values (`cachePath`, `indexPath`, and `etherscan_key`) in the saved `trueBlocks.toml` file and replace them into the new file.

After this step, our configuration looks like this:

```
[settings]
cachePath = "/home/tb/.local/share/trueblocks/cache/"
indexPath = "/home/tb/.local/share/trueblocks/unchained/"
etherscan_key = "6---obscured---8E1B---obscured---1"
defaultChain = "mainnet"

... [ a bunch of chain specific configurations ] ....

```

The `chifra` commands should still complain and refuse to run. There's one more step...

### Removing old configuration files

Most users may remove any files or folders in the root configuration folder other than `trueBlocks.toml` file and these folders:

<img width="200" src="./migration.25.2.png">

Once you remove those other files and folders, `chifra` should stop complaining and you'll be finished with the migration.


## Are you finished?

If you've completed the above steps fully, you should be able to run any `chifra` commands as before. If `chifra` continues to complain, you've not completed something properly. Please review the above steps.

If you run `chifra status --terse` you should be able to see the folders you've modified. On our Mac, we get:

```
2022/02/08 08:44:45 Client:       erigon/2021.11.3/linux-amd64/go1.16.3 (archive, tracing)
2022/02/08 08:44:45 TrueBlocks:   GHC-TrueBlocks//0.23.7-alpha-32ec1498f-20220208 (eskey, no pinkey)
2022/02/08 08:44:45 Config Path:  /Users/tb/Library/Application Support/TrueBlocks/
2022/02/08 08:44:45 Chain (ids):  mainnet (1,1)
2022/02/08 08:44:45 Cache Path:   /Users/tb/Development/trueblocks-core/build/cache/mainnet/
2022/02/08 08:44:45 Index Path:   /Users/tb/Development/trueblocks-index/wild.0/unchained/mainnet/
2022/02/08 08:44:45 RPC Provider: http://localhost:23456/
2022/02/08 08:44:45 Progress:     14165759, 14143890, 14146478, 14146478

```

## Using Multi-Chain

Please see [this page](./) for information on using the new `--chain` option to `chifra`.

## You're Finished

Please report any problems by creating an issue.

## Previous Migration

[Click here](./README-v0.18.0.md) for the previous migration.

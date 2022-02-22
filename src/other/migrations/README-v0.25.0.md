# v0.25.0 Multi-chain

---

(February 22, 2022)

## Why the Change?

With this migration, TrueBlocks enters into the world of multi-chain blockchain scraping / indexing.

Under the covers, this was a massive change to our code base. On the surface, it's quite simple: we added the `--chain` option to all commands. We've also pre-configured a few chains for you, but you may add support for any EVM-based chain yourself.

This migration requires you to edit a configuration files and move existing folders to new locations. Be careful, if you make a mistake, you may damage not only your installation of TrueBlocks, but your computer.

Please do not say we didn't warn you.

### What do you need to change?

This migration involves you doing three things. You must

- change the location of two folders (`cache` and `unchained` index),  
- edit a configuration file to point to these new folders,  
- remove or move old configuration files and folders.

## Instructions

In the following instructions, we assume you are working on a Linux installation. If you're on a Mac, adjust the paths as needed.

### Before you start

- Stop any long running TrueBlocks processes (such as the `chifra scrape` or `chifra serve`). Do not restart them until the migration is complete.

- Move -- do not copy -- the old `trueBlocks.toml` configuration file. We will need it later.

```
mv $HOME/.local/share/trueblocks/trueBlocks.toml ./trueBlocks.save
```

### Pull and rebuild TrueBlocks

- Pull the latest copy of TrueBlocks, switch to the `develop` branch, and rebuild. If you've previously put the executable files to a non-default location (DavidFS!), remove those old executables.

Complete the following instructions from the root of the `trueblocks-core` repo:

```
cd ./build
git pull
git checkout develop
make clean
cmake ../src
make -j 4
```

The above commands will rebuild the TrueBlocks executables and create the new multi-chain folder structures. These commands will also create a newly formatted `trueBlocks.toml` file (thus the need to move the old one).

### Did the build succeed?

Let's make sure the build worked and that you have the latest version. Run

```
chifra status --terse
```

This should return a warning message and the following version (or later):

```
chifra version GHC-TrueBlocks//0.25.0-alpha
```

Until you've completed the migration, the warning message will continue to display.

### Editing configuration files

**Important:** Before doing anything else, you must edit a configuration file. Don't run any other `chifra` commands until we're done.

Display the values in the old configuration file you saved earlier:

```
cat ./trueBlocks.save
```

Make note of three values:

```
cachePath = "<cache_path>"
indexPath = "<index_path>"
etherscan_key = "<etherscan_key>"
rpcProvider = "<rpc_provider>"
```

Change the current directory to the TrueBlocks configuration folder:

```
cd $HOME/.local/share/trueblocks
pwd
```

(on Mac, `pwd` should say `$HOME/Library/Application Support/TrueBlocks`).

Edit the `trueBlocks.toml` file you find there and replace the above three values with the values you noted above. Note, we are not yet replacing the `rpcProvider` in this section. We will do that next. Your settings should now look something like this:

```
[settings]
cachePath = "<cache_path>"
indexPath = "<index_path>"
etherscan_key = "<etherscan_key>"
defaultChain = "mainnet"
```

Find the `rpcProvider` value in the configuration section called `[chains.mainnet]`. Replace that value with the value from your previous installation.

The `[chains.mainnet]` section of your file should look something like this:

```
[chains.mainnet]
apiProvider = "http://localhost:8080"
chainId = "1"
localExplorer = "http://localhost:1234"
pinGateway = "https://ipfs.unchainedindex.io/ipfs/"
remoteExplorer = "https://etherscan.io"
rpcProvider = "<your_rpc_provider>"
symbol = "ETH"
```

If you have `rpcProvider` values for other chains, you may set them now if you wish.

Run `chifra status --terse` again. You should get the same warning message as earlier.

### Moving existing cache and unchained index folders

**Important:** If you've customized the location of your `cache` or `unchained` folders, adjust these instructions accordingly to account for those differing locations.
 
Complete the following steps:

- Move the existing cache folders into the chain-specific `mainnet` cache folder:

```
cd $HOME/.local/share/trueblocks/cache
mkdir -p mainnet
mv * mainnet
```

*Note: You will get a warning saying you've tried to move a folder into itself. That's okay.*

- Move existing unchained index folders into the new chain-specific `mainnet` unchained index folder:

```
cd $HOME/.local/share/trueblocks/unchained
mkdir -p mainnet
mv * mainnet
```

*Note: You may get a warning saying you've tried to move a folder into itself. That's okay.*

Run `chifra status --terse` again. You will continue to get the warning message.

### Removing old configuration files

Almost done!

Change back into the root configuration folder:

```
cd $HOME/.local/share/trueblocks/
ls -l
```

You'll notice a few files and folders different than the image below. The last step is to remove these old files and folders. When you're finished, the root of the TrueBlocks configuration folder should look like this:

<img width="200" src="./migration.25.2.png">

When you've completed this step, the root configuration folder should contain a single `.toml` file (`trueBlocks.toml`) and four subfolders: `abis`, `config`, `cache`, `unchained`. The `config`, `cache`, and `unchained` subfolders should contain one or more chain-specific sub-folders (no files). There must be at least the `mainnet` folder and may be one or two others.

**Important:** If you've customized any of TrueBlocks' configuration files other than `trueBlocks.toml`, you should preserve those values. You will find (or you can create) fresh copies of the existing configuration files in `$HOME/.local/share/trueblocks/config/mainnet`. Move any settings you've customized to those files.

**Note:**  You may remove the `[requires]` section from the `blockScrape.toml` file as it is no longer needed.

Continue running `chifra status --terse` until it stops complaining. When it does stop complaining, you will be finished. It should tell you what files you need to remove until its satisfied.

## Are you finished?

If you've completed the above steps, you should be able to run any of the `chifra` commands. If `chifra` continues to complain, review the above steps and/or contact us in discord for help.

Run `chifra status --terse`.

If the migration is finished, it will return something similar to this:

```
<date-time> Client:       erigon/2021.11.3/linux-amd64/go1.16.3 (archive, tracing)
<date-time> TrueBlocks:   GHC-TrueBlocks//0.25.0-alpha (eskey, no pinkey)
<date-time> Config Path:  $HOME/.local/share/trueblocks/
<date-time> Chain (ids):  mainnet (1,1)
<date-time> Cache Path:   $HOME/.local/share/trueblocks/cache/mainnet/
<date-time> Index Path:   $HOME/.local/share/trueblocks/unchained/mainnet/
<date-time> RPC Provider: http://localhost:8545/
```

## Are we finished?

There are a few things that we have not yet completed for the multi-chain work. We are working on these actively, but they do not materially harm any functions. We did not want to delay release.

- `chifra init` and `chifra init -all` routines work only for the Mainnet Ethereum. For all other chains, in order to get the unchained index, you must scrape it yourself. Do so with `chifra scrape indexer`.

- On Mainnet Ethereum, TrueBlocks uses the Uniswap and Maker smart contracts to create a US dollar spot price for Ether and other tokens for all transactions. As these tools are not available on other chains (because they don't exist), we cannot create spot prices on other chains. This applies only to the commands `chifra export --accounting <address>` and `chifra transactions --reconcile...`. Instead of a `spotPrice` TrueBlocks returns a value of 1.0 for all assets on non-Mainnet chains. We welcome any ideas how to work around this limitation.

- For the same reason, support for ENS only works on the Mainnet

- For all chains other than Mainnet Ethereum, `chifra slurp` does not work. As this tools will be deprecated in the future, this is a permanent state of affairs.

## You're finished!

You may restart the processes you stopped earlier: `chifra scrape` or `chifra serve`.

Please see [the help file](https://trueblocks.io/docs/prologue/multi-chain/) for information on adding your own chains and using the new `--chain` option.

Report any problems by creating an issue.

## Previous Migration

[Click here](./README-v0.18.0.md) for the previous migration.

# v0.12.1 Fix for Index Bug / New Executable Folders

**Warning:** If you're not confident using the command line, please contact us prior to proceeding. If you make a mistake and delete the wrong files, you may damage your computer.

**DO NOT SAY WE DIDN'T WARN YOU.**

---

(v0.12.1 - 2021/09/04)

## Why the Change?

These migrations cover two recent issues with TrueBlocks. The first migration fixes a bug in the generated
index data (if you're creating it) caused by using Erigon as the RPC provider. This is further explained below. The second
migration addresses issues presented by several users who report installation problems.

### Fix to Index Data when Using Erigon as Data Source

Prior to last week, TrueBlocks only supported OpenEthereum. One of the change we made last week enabled support for the Erigon client. A single line in [this commit](https://github.com/TrueBlocks/trueblocks-core/commit/af24fe5db5af8b28a30453a77b67ef15b75d08bf) incorrectly causes a call to a routine that only OpenEthereum supports (`parity_getBlockHeaderByNumber`). Obviously, Erigon does  not support Parity only endpoints. Our testing did not find this error and, as a result, any indexes created after this commit that used Erigon contain invalid data. This same error caused various issues with timestamp data files. These timestamp related issues are also corrected with this migration.

This first migration, detailed below, is required for all users.

### Fix to Use Different Folders to Store Chifra Subcommands

Numerous users have reported difficulty installing TrueBlocks. Many times this difficulty can be
traced back to the user's $PATH. (Come on, people, it's not that hard!) Because this was causing so many
problems, we decided to fix it once and for all by removing `chifra's` subcommands from the $PATH. The `chifra` executable, however, remains in your $PATH.

This migration allows us to begin the process of committing to a public (version 1.0) command line interface as
we prepare to leave beta status. It also allows us greater flexibility in changing underlying non-public interfaces and forces users who may be using the underlying non-public interfaces to switch to the corresponding `chifra` options. 

While this second migration, details of which are below, is technical optional, we advise you to complete it anyway.

## Migrations

This first part of the migration is required for all users.

### Migration to Fix the Index and Timestamp Data

Complete the following tasks:

- If you're running either `chifra serve` or `chifra scrape`, stop them.
- Run `chifra when --timestamps --fix`
  - This command may take a while depending on your installation.
  - The process should report numerous corrections (but don't worry if it doesn't).

**Note:** If you have never run `chifra scrape`, you are finished with this migration. See below.

**Note:** If you have run `chifra scrape`, but you have never run it against Erigon, you are finished with this migration. See below.

If you have run `chifra scrape` against Erigon, complete these commands:

- Change directory into your `indexPath`. (Look inside the `trueBlocks.toml` file to find `indexPath`.)
- From the `indexPath` folder, remove the `staging`, `unripe`, and `ripe` folders.
- Next, run the following commands to remove damaged index files and Bloom filters:
  - rm -f blooms/0132*
  - rm -f finalized/0132*

**Note:** The next time you run `chifra scrape`, it will recognize that the indexer has been reset and
pick up where it should, recreating the removed data.

**Important Note:** The above instructions apply to *mainnet Ethereum* only. They remove index files created
after the bug was introduced. For other chains (such as, Rinkeby) the block numbers where the problem occurred will be different. To find the block number after which you must remove the incorrect data, run this command: `chifra when 2021-09-01:12`. This will return a block number on that chain. Delete files in both the `blooms` folder and the `finalized` folder whose second digit (`endBlock`) is after the returned block number. (Filenames in those folders have
`startBlock-endBlock.bin` structure.)

An alternative method to the above migration is to remove all of the files and folders in the `indexPath`, but this
would require a full re-scrape of the entire index.

### Migration for Change to Subcommand Paths

Complete the following steps from the `./trueblocks-core/build` folder:

```[bash]
# remove previously built executables from the local bin
rm -fR ../bin/*

# reset the bin to its default configuration
git checkout ../bin

# clean out previously created cmake build scripts
make clean

# re-create the build scripts
cmake ../src

# build the trueblocks executables as normal
make -j2
```

That's it!

You should now be able to use `chifra` identically as you did before.

## You're Finished

Please report any problems by creating an issue.

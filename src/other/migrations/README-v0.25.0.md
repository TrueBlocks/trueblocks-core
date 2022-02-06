# v0.25.0 Multi-chain

---

(February 6, 2022)

## Why the Change?

With this migration we enter into the world of multi-chain.

While the changes here are relatively simple, they are delicate in the sense that you must edit configuration files and move folders on your hard drive. It is possible to damage not only your installation of TrueBlocks, but your computer if you are not careful.

Please don't say we didn't warn you.

### What's Changed

In a certain sense, not much has changed. We added a new command line option to all tools (`--chain`). This had two basic effects: (1) we had to move the location of the `cache` and `unchained` index folders, and (2) we needed to edit one configuration file and move a few others.

The image below shows which existing folders have to be moved (by you). When you rebuild TrueBlocks to version 0.25.0 or later, the correct folders will be created during the build. Also, the new versions of the configuration files will be installed.

The existing config files and folders from the previous version will be left in place.

`chifra` will refuse to run, warning you to complete this migration, until you complete the following steps.

### The Migration in a Single Image

![Instructions](https://github.com/TrueBlocks/trueblocks-core/blob/master/src/other/migrations/migration.25.png)

The above image shows the completed migration you should see after following the step-by-step process below. This is a four step process. The folders you will be moving are in TrueBlocks' $CONFIG folder (`~/.local/share/trueblocks/` on Linux, `~/Library/Application Support/TrueBlocks/` on Mac). If you've already customized these locations in your installation, see the Already Customized section below.

### Instructions

In the following instructions `$CONFIG` stands for the root configuration folder of TrueBlocks on your computer. On Linux, this means `~/.local/share/trublocks`. On Mac, it's `~/Library/Application Support/TrueBlocks`. (On Windows it doesn't mean anything, as we don't support Windows.)

(0) Stop any long running processes that involved TrueBlocks (such as the scraper). Don't start them again until the migration is completed.

(1) Move the existing installation to a safe place. (Don't copy -- move.)

```
mv $CONFIG <safe_place>
```

This will leave your home folder as if TrueBlocks has never been installed before. Next, we install TrueBlocks fresh.

From the `./build` folder:

```
git pull
git checkout multi-chain-12
cmake ../src
make -j 4
```

Confirm that this creates folders in your $CONFIG folder called `unchained` and `cache`.


(2) Move all the files and folders currently in the $CONFIG folder into the new `mainnet` folder (except the `abis` folder and the file `trueBlocks.toml`.

```
mv cache unchained manifest mocked names mainnet
mv *.gz mainnet
mv *.toml mainnet
mv mainnet/trueBlocks.toml .
```

After this step, your folder structure should look like the middle image above.

(3) To add other chains:

- Edit the `trueBlocks.toml` file
- Either add or edit a value under `[settings]chain = mainnet` to reference one of these supported chains:
  - `mainnet`, `gnosis`, `polygon`, `rinkeby`, `georli`
- You may add your own chain, but if you do, you must supply your own genesis files. Please contact us in our discord for more information.

### Other slight changes

The `chifra export --neighbors` option now no longer include the `traceId` field. Additionally, the `reason` field is slightly less informative that it was previously. All of this in service of a significant increase in the speed of the option.

## You're Finished

Please report any problems by creating an issue.

## Previous Migration

[Click here](./README-v0.18.0.md) for the previous migration.

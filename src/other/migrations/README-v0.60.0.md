# v0.60.0 Better Docker Support

(March 11, 2023)

## Optional only migrations

For this migration there are two optional things you may wish to do:

### Clean up an Erigon bug

During the month of January (and possibly February) 2023, the Erigon node software had a bug in its `snapshot sync`. If you were running Erigon during that time and concurrently using the TrueBlocks to scrape the index yourself, your index may be invalid. Oddly, this effects only a few very specific blocks, so the chance of it effecting you are small. It's easy to repair.

To repair this problem run this command:

```
chifra init --all
```

The `chifra init` routine downloads the canonical manifest from the Unchained Index smart contract and compares your index to the correct one (according to us). If there are any descrepencies, the error files on your disc are replaced with freshly downloaded files.

Note: If you've not yet downloaded all of the index chunks (for example to save on disc space usage) remove the `--all` from the above command. This will only download the bloom filters and refresh any full chunks you may have.

### Clean up old ABI files

We corrected a number of problems related to ABIs and the `--articulation` option for numerous tools. In some cases, there may be invalid ABI-related data in your cache. It's optional, and you may not see any problems, but if you wish you may correct this by clearing the ABI cache. To do so, follow these instructions:

1. Find your `$CACHE_PATH` with `chifra config --chain mainnet --paths`.
2. Remove all the contents of `$CACHE_PATH/abis`.

Note that you may do this for all the chains your working with.

## You're finished

You're finished.

## Previous Migration

[Click here](./README-v0.45.0.md) for the previous migration.

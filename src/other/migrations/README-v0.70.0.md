# v0.70.0 Correction to Unchained Index

(May 26, 2023)

### Clean up an Erigon bug

During the month of April / May 2023, the interaction between Erigon and TrueBlocks had a bug that caused incorrect data to be published to the Unchained Index smart contract. If you were running Erigon during that time and concurrently using the TrueBlocks to scrape the index yourself, your index may be invalid. The error may have caused certain transactions to fail to reconconcile, but had not other known detrimental effects.

The problem is easy to repair. To repair this problem run this command:

```
chifra init --all
```

The `chifra init` routine downloads the canonical manifest from the Unchained Index smart contract and compares your index to the correct one (correct according to TrueBlocks). If there are any descrepencies, the errored files on your disc are replaced with freshly downloaded files.

Note: If you've not yet downloaded all of the index chunks (for example to save on disc space usage) remove the `--all` from the above command. This will only download the bloom filters and refresh any full chunks you may have.

Rerun this command as often as is necessary to complete the process without errors. Pinata sometimes times out on heavy requests.

## You're finished

You're finished.

## Previous Migration

[Click here](./README-v0.60.0.md) for the previous migration.

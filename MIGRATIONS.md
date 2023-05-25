# Migrations

| Tag     | Migration                                                                                                                      | Date       | Summary                                                                                           |
| ------- | ------------------------------------------------------------------------------------------------------------------------------ | ---------- | ------------------------------------------------------------------------------------------------- |
| v0.70.0 | [Update to Unchained Index](https://github.com/TrueBlocks/trueblocks-core/blob/develop/src/other/migrations/README-v0.70.0.md) | 2023/05/26 | Corrects certain previously incorrect chunks in the unchained index.                              |
| v0.64.0 |                                                                                                                                | 2023/05/07 | No migrations.                                                                                    |
| v0.62.0 |                                                                                                                                | 2023/04/12 | No migrations.                                                                                    |
| v0.60.0 | [Many GoLang Ports](https://github.com/TrueBlocks/trueblocks-core/blob/develop/src/other/migrations/README-v0.60.0.md)         | 2023/03/11 | Two options migrations for two very specific, small edge cases.                                   |
| v0.45.0 | [Better Docker](https://github.com/TrueBlocks/trueblocks-core/blob/develop/src/other/migrations/README-v0.45.0.md)             | 2022/12/16 | Much better support for use in the docker version (more coming soon).                             |
| v0.44.0 | [Scraper Bug Fix](https://github.com/TrueBlocks/trueblocks-core/blob/develop/src/other/migrations/README-v0.44.0.md)           | 2022/11/25 | Fixes a bug related to scraping at the head of the chain.                                         |
| v0.43.0 | [Better Accounting](https://github.com/TrueBlocks/trueblocks-core/blob/master/CHANGES.md#v0430)                                | 2022/11/16 | No migrations, but a lot of improvements to the accounting modules.                               |
| v0.41.0 | [GoLang Port for Receipts](https://github.com/TrueBlocks/trueblocks-core/blob/master/CHANGES.md#v0410)                         | 2022/10/20 | No migrations, but there were some breaking changes.                                              |
| v0.40.0 | [Better Scraper](https://github.com/TrueBlocks/trueblocks-core/blob/develop/src/other/migrations/README-v0.40.0.md)            | 2022/06/20 | Updates file format of index and Bloom filters to include version, fixes incorrect data in index. |


Note that migrations prior to version v0.40.0 are no longer supported. If your data is this old, you must start afresh.
# Unsupported Migration

| Tag     | Migration                                                                                                                         | Date       | Summary                                                                                                 |
| ------- | --------------------------------------------------------------------------------------------------------------------------------- | ---------- | ------------------------------------------------------------------------------------------------------- |
| v0.30.0 | [Much Better Monitors](https://github.com/TrueBlocks/trueblocks-core/blob/develop/src/other/migrations/README-v0.30.0.md)         | 2022/04/16 | Changes monitor file format. Ports scan of index to GoLang. Improves speed of scan by more than 25x     |
| v0.25.0 | [Multi Chain](https://github.com/TrueBlocks/trueblocks-core/blob/develop/src/other/migrations/README-v0.25.0.md)                  | 2022/02/22 | Adds `--chain` option to all commands. Now supports any EVM-based chain.                                |
| v0.18.0 | [Better Neighbors](https://github.com/TrueBlocks/trueblocks-core/blob/develop/src/other/migrations/README-v0.18.0.md)             | 2021/12/10 | Cleans up `chifra export --neighbors` output<br />- Corrects incorrect headers in .txt and .csv export. |
| v0.17.0 | [Updated API and SDK](https://github.com/TrueBlocks/trueblocks-core/blob/develop/src/other/migrations/README-v0.17.0.md)          | 2021/11/23 | Creates TypeScript SDK and forces all API endpoint options to camelCase.                                |
| v0.14.0 | [Go Version of Chifra](https://github.com/TrueBlocks/trueblocks-core/blob/develop/src/other/migrations/README-v0.14.0.md)         | 2021/10/03 | Ports chifra command line to golang<br />- Breaking change to --verbose flag.                           |
| v0.12.1 | [New Folder Structure](https://github.com/TrueBlocks/trueblocks-core/blob/develop/src/other/migrations/README-v0.12.1.md)         | 2021/09/01 | Adds support for Erigon<br />- Moves most executables to private path.                                  |
| v0.11.3 | [London Hard Fork Migration](https://github.com/TrueBlocks/trueblocks-core/blob/develop/src/other/migrations/README-v0.11.3.md)   | 2021/08/20 | Support for cache file format change due to London hard fork.                                           |
| v0.9.0  | [Configuration File Migration](https://github.com/TrueBlocks/trueblocks-core/blob/develop/src/other/migrations/README-v0.09.0.md) | 2021/06/02 | Moves configuration file locations.                                                                     |

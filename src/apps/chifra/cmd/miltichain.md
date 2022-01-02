### Open Questions

1. Add a column to the performance testing data called chain (set to mainnet for all records)
2. We should use XDG
3. Rules for checking proper migration
   - If the cachePath contains cache, tell user to remove it
   - If the old configPath contains any toml other than trueBlocks.toml, fail
   - If there is no configPath/mainnet/config, file
   - If there is no mainnet folder, fail
   - If there is no Abis folder, fail
   - If there is no mainnet/config/names.tab, fail
   - If there is no mainnet/config/allocs.tab, fail
4. What about bytzantium and the 'status' value for non-mainnet chains?
5. What about xDai?

### Config Files

$BASE_PATH = `~/local/share/trueblocks` or `~/Library/Application Support/TrueBlocks/`

```
[settings]
chain = ""              # defaults to mainnet (must be one of mainnet, ropstem, rinkeby, goerli, kovan, xdai)
basePath = ""           # defaults to ${BASE_PATH}

[mainnet]
rpcProvider = ""
remoteExplorer = ""

[rinkeby]
rpcProvider = ""
remoteExplorer = ""
```

### Calculated Paths

We want the only configuration to be ${BASE_PATH}. All other paths are calculated based on that value and ${CHAIN}

| Name            | Calculated Value                |
| --------------- | ------------------------------- |
| chainConfigPath | {BASE_PATH}/config/{CHAIN}/     |
| cachePath       | {BASE_CACHE}/cache/{CHAIN}/     |
| indexPath       | {BASE_CACHE}/unchained/{CHAIN}/ |

### Chain Data

| Chain Id | Chain     | Remote Explorer              | Erigon      | Genesis Hash (from Erigon)                                         |
| -------- | --------- | ---------------------------- | ----------- | ------------------------------------------------------------------ |
| 1        | mainnet   | https://etherscan.io         | mainnet     | 0xd4e56740f876aef8c010b86a40d5f56745a118d0906a34e69aec8c0db1cb8fa3 |
| 3        | ropsten   | https://ropsten.etherscan.io | reposten    | 0x41941023680923e0fe4d74a34bdac8141f2540e3ae90623718e47d66d1ca4a2d |
| 4        | rinkeby   | https://rinkeby.etherscan.io | rinkeby     | 0x6341fd3daf94b748c72ced5a5b26028f2474f5f00d824504e4fa37a75767e177 |
| 5        | goerli    | https://goerli.etherscan.io  | goerli      | 0xbf7e331f7f7c1dd2e05159666b3bf8bc7a8a3a9eb1d518969eab529dd9b88c1a |
| 42       | kovan     | https://kovan.etherscan.io   | kovan       | 0xa3c565fc15c7478862d50ccd6561e3c06b24cc509bf388941c25ea985ce32cb9 |
|          |           | https://polygonscan.com      |             |                                                                    |
| 56       |           | https://bscscan.com          | bsc-mainnet | 0x0d21840abff46b96c84b2ac9e10e4f5cdaeb5693cb665db62a2f3b02d2d57b5b |
|          |           | https://hecoinfo.com         |             |                                                                    |
|          |           |                              | erigonmine  | 0xfecd5c85712e36f30f09ba3a42386b42c46b5ba5395a4246b952e655f9aa0f58 |
| 1212120  |           |                              | fermion     | 0x0658360d8680ead416900a552b67b84e6d575c7f0ecab3dbe42406f9f8c34c35 |
| 77       |           |                              | sokol       | 0x5b28c1bfd3a15230c9a46b399cd0f9a6920d432e85381cc6a140b06e8410112f |
|          |           |                              | yolov3      |                                                                    |
| 97       |           |                              | chapel?     | 0x6d3c66c5357ec91d5c43af47e234a939b22557cbb552dc45bebbceeed90fbe34 |
| 1417     |           |                              | rialto?     | 0x005dc005bddd1967de6187c1c23be801eb7abdd80cebcc24f341b727b70311d6 |
|          | xdai      |                              |             | 0x4f1dd23188aab3a76b463e4af801b52b1248ef073c648cbdc4c9333d3da79756 |
|          | polkadot? |                              |             | ???                                                                |
|          | filecoin? |                              |             | ???                                                                |
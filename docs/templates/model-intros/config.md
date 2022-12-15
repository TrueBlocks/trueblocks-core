<!-- markdownlint-disable MD033 MD036 MD041 -->
The [chifra config](/docs/chifra/admin/#chifra-config) tool reports on the state (and size) of the various TrueBlocks local binary caches. TrueBlocks produces nine difference caches: `abis`, `blocks`, `monitors`, `names`, `objs`, `recons`, `slurps`, `traces`, `txs`. In general practice, these caches may take up a few GB of hard drive space, however, for very popular smart contract the size of the caches may grow rather large. Keep an eye on it.

The following commands produce and manage status:

| Tools                                              |                                               |
| -------------------------------------------------- | --------------------------------------------- |
| [chifra config](/docs/chifra/admin/#chifra-config) | report on the status of the TrueBlocks system |

Status data is made of the following data fields:

[{FIELDS}]

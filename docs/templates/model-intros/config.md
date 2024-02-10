The [chifra config](/chifra/admin/#chifra-config) tool reports on the state (and size) of the
various TrueBlocks local binary caches. TrueBlocks produces nine difference caches: `abis`, `blocks`,
`monitors`, `names`, `objs`, `recons`, `slurps`, `traces`, `transactions`. In general practice, these caches
may take up a few GB of hard drive space, however, for very popular smart contract the size of the
caches may grow rather large. Keep an eye on it.

The `status` data model is a complex beast. It contains various information including a list of
registered chains, information about many of the internal binary caches maintained by `chifra`
as well as current status information about the system including version information for both
`chifra` and the node it's running against.

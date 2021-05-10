## chifra status

The `chifra status` program allows you to manage the various TrueBlock caches. You may list all of the caches, some of the cache, or even individual caches either in terse or full detail. The cache of interest is specified with the `modes` option.

TrueBlocks maintains caches for the index of address appearances, named addresses, abi files, as well as other data including blockchain data, address monitors, and groups of address monitors called entities.

### usage

`Usage:`    chifra status [-d|-t|-v|-h] &lt;mode&gt; [mode...]  
`Purpose:`  Report on status of one or more TrueBlocks caches.

`Where:`  

| | Option | Description |
| :----- | :----- | :---------- |
|  | modes | the type of status info to retrieve, one or more of *[ index \| monitors \| entities \| names \| abis \| caches \| some\* \| all ]* |
| -d | --details | include details about items found in monitors, slurps, abis, or price caches |
| -t | --types &lt;val&gt; | for caches mode only, which type(s) of cache to report, one or more of *[ blocks \| transactions \| traces \| slurps \| prices \| all\* ]* |
| -v | --verbose | set verbose level (optional level defaults to 1) |
| -h | --help | display this help screen |

**Source code**: [`apps/cacheStatus`](https://github.com/TrueBlocks/trueblocks-core/tree/master/src/apps/cacheStatus)


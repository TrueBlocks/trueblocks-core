### further information

The `--traces` option requires your node to enable the `trace_block` (and related) RPC endpoints. Many remote RPC providers do not enable these endpoints due to the additional load they can place on the node. If you are running your own node, you can enable these endpoints by adding `trace` to your node's startup.

The test for tracing assumes your node provides tracing starting at block 1. If your is partially synced, you may export the following environment variable before running the command to instruct `chifra` where to test.

```[bash]
export TB_<chain>_FIRSTTRACE=<bn>
```

where `<chain>` is the chain you are running and `<bn>` is the block number at which tracing starts. For example, to start tracing at block 1000 on the mainnet, you would export `TB_MAINNET_FIRSTTRACE=1000`.

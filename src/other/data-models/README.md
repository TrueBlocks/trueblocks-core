List of types that support the Modeler interface

| Path                | File                         | Type                  | Doc             | Where                                                              |
| ------------------- | ---------------------------- | --------------------- | --------------- | ------------------------------------------------------------------ |
| ./internal/chunks   | handle_addresses_belongs.go: | simpleAppearanceTable | `missing`       | chunks index --belongs                                             |
| ./internal/blocks   | handle_count.go:             | simpleBlockCount      | blockCount      | blocks --count                                                     |
| ./internal/list     | handle_bounds.go:            | simpleBounds          | `missing`       | list --bounds                                                      |
| ./internal/chunks   | handle_addresses.go:         | simpleChunkAddress    | chunkAddress    | chunks addresses                                                   |
| ./internal/chunks   | handle_blooms.go:            | simpleChunkBloom      | chunkBlooms     | chunks blooms                                                      |
| ./internal/chunks   | handle_index.go:             | simpleChunkIndex      | chunkIndex      | chunks index                                                       |
| ./internal/chunks   | chunk_stats.go:              | simpleChunkStats      | chunkStats      | chunks stats                                                       |
| ./internal/chunks   | handle_status.go:            | simpleChunkStatus     | `missing`       | chunks status                                                      |
| ./internal/monitors | handle_clean.go:             | simpleMonitorClean    | monitorClean    | monitors --clean                                                   |
| ./internal/chunks   | handle_check.go:             | simpleReportCheck     | reportCheck     | chunks index --check                                               |
| ./internal/when     | handle_ts_count.go:          | simpleWhenCount       | `missing`       | when --timestamps --count                                          |
|                     |                              |                       |                 |                                                                    |
| ./pkg/types/        | types_appearance.go:         | SimpleAppearance      | appearance      | chunks appearances, list, slurp --appearances, transactions --uniq |
| ./pkg/types/        | types_block.go:              | SimpleBlock           | block           |                                                                    |
| ./pkg/types/        | types_chunkrecord.go:        | SimpleChunkRecord     | `missing`       | chunks manifest, scrape --pin                                      |
| ./pkg/types/        | types_etherscan.go:          | simpleEtherscan       | `missing`       | slurp                                                              |
| ./pkg/types/        | types_ethstate.go:           | SimpleEthState        | ethState        | state                                                              |
| ./pkg/types/        | types_function.go:           | SimpleFunction        | function        | abis, all --articulate uses                                        |
| ./pkg/types/        | types_indexappearance.go:    | SimpleIndexAppearance | `missing`       | currently unused                                                   |
| ./pkg/types/        | types_log.go:                | SimpleLog             | log             | logs                                                               |
| ./pkg/types/        | types_manifest.go:           | SimpleManifest        | manifest        | chunks manifest                                                    |
| ./pkg/types/        | types_monitor.go:            | SimpleMonitor         | monitor         | list --count, monitors --list                                      |
| ./pkg/types/        | types_name.go:               | SimpleName            | name            | names                                                              |
| ./pkg/types/        | types_namedblock.go:         | SimpleNamedBlock      | namedBlock      | when                                                               |
| ./pkg/types/        | types_parameter.go:          | SimpleParameter       | parameter       | abis, all --articulate uses                                        |
| ./pkg/types/        | types_receipt.go:            | SimpleReceipt         | receipt         | blocks, transactions                                               |
| ./pkg/types/        | types_reconciliation.go:     | SimpleReconciliation  | reconciliation  | chifra export --accounting                                         |
| ./pkg/types/        | types_timestamp.go:          | SimpleTimestamp       | `missing`       | when --timestamps                                                  |
| ./pkg/types/        | types_tokenbalance.go:       | SimpleTokenBalance    | tokenBalance    | currently unused                                                   |
| ./pkg/types/        | types_trace.go:              | SimpleTrace           | trace           | blocks --trace, transactions --trace, trace                        |
| ./pkg/types/        | types_traceaction.go:        | SimpleTraceAction     | traceAction     | --trace uses                                                       |
| ./internal/         | traces/handle_counts.go:     | simpleTraceCount      | traceCount      | traces --count                                                     |
| ./pkg/types/        | types_tracefilter.go:        | SimpleTraceFilter     | traceFilter     | traces --filter                                                    |
| ./pkg/types/        | types_traceresult.go:        | SimpleTraceResult     | traceResult     | --trace uses                                                       |
| ./pkg/types/        | types_transaction.go:        | SimpleTransaction     | transaction     | blocks, transactions                                               |
| ./pkg/types/        | types_transfer.go:           | SimpleTransfer        | transfer        | currently unused                                                   |
|                     |                              |                       |                 |                                                                    |
| `missing`           | `missing`                    | `missing`             | abi             |                                                                    |
| `missing`           | `missing`                    | `missing`             | appearanceCount |                                                                    |
| `missing`           | `missing`                    | `missing`             | cache           |                                                                    |
| `missing`           | `missing`                    | `missing`             | cacheEntry      |                                                                    |
| `missing`           | `missing`                    | `missing`             | chain           |                                                                    |
| `missing`           | `missing`                    | `missing`             | config          |                                                                    |
| `missing`           | `missing`                    | `missing`             | ethCall         |                                                                    |
| `missing`           | `missing`                    | `missing`             | pinnedChunk     |                                                                    |

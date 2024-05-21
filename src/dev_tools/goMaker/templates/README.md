List of types that support the Modeler interface

| Path                | File                     | Type                  | Documented        | gen-c++ | gen-go |               |
| ------------------- | ------------------------ | --------------------- | ----------------- | ------- | ------ | ------------- |
| ./internal/blocks   | types_blockcount.go      | simpleBlockCount      | blockCount        |         | x      |               |
| ./internal/chunks   | types_appearanceTable.go | simpleAppearanceTable | <--- missing ---> |         |        |               |
| ./internal/chunks   | types_chunkaddress.go    | simpleChunkAddress    | chunkAddress      |         | x      |               |
| ./internal/chunks   | types_chunkbloom.go      | simpleChunkBloom      | chunkBloom        |         | x      |               |
| ./internal/chunks   | types_chunkindex.go      | simpleChunkIndex      | chunkIndex        |         | x      |               |
| ./internal/chunks   | types_chunkstats.go      | simpleChunkStats      | chunkStats        |         | x      |               |
| ./internal/chunks   | types_reportcheck.go     | simpleReportCheck     | reportCheck       |         | x      |               |
| ./internal/list     | types_bounds.go          | simpleBounds          | bounds            |         | x      |               |
| ./internal/monitors | types_monitorclean.go    | simpleMonitorClean    | monitorClean      |         | x      |               |
| ./internal/status   | types_cache.go           | simpleCache           | cache             |         | x      | not turned on |
| ./internal/status   | types_cacheitem.go       | simpleCacheItem       | cacheItem         |         |        |               |
| ./internal/status   | types_status.go          | simpleStatus          | <--- missing ---> |         | x      |               |
| ./internal/traces   | types_tracecount.go      | simpleTraceCount      | traceCount        |         | x      |               |
| ./internal/when     | types_timestamp.go       | simpleTimestamp       | timestamp         |         |        |               |
| ./internal/when     | types_timestampCount.go  | simpleTimestampCount  | timestampCount    |         |        |               |
|                     |                          |                       |                   |         |        |               |
| ./pkg/types         | types_appearance.go      | Appearance            | appearance        | x       |        | not turned on |
| ./pkg/types         | types_block.go           | Block                 | block             | x       | x      |               |
| ./pkg/types         | types_chain.go           | Chain                 | chain             | x       | x      |               |
| ./pkg/types         | types_chunkrecord.go     | ChunkRecord           | chunkRecord       |         | x      |               |
| ./pkg/types         | types_ethcall.go         | EthCall               | ethCall           | x       | x      |               |
| ./pkg/types         | types_slurp.go           | Slurp                 | slurp             |         | x      |               |
| ./pkg/types         | types_ethstate.go        | State                 | ethState          | x       | x      |               |
| ./pkg/types         | types_function.go        | Function              | function          | x       | x      |               |
| ./pkg/types         | types_log.go             | Log                   | log               | x       | x      |               |
| ./pkg/types         | types_manifest.go        | Manifest              | manifest          |         | x      |               |
| ./pkg/types         | types_monitor.go         | Monitor               | monitor           | x       |        | not turned on |
| ./pkg/types         | types_name.go            | Name                  | name              | x       | x      |               |
| ./pkg/types         | types_namedblock.go      | NamedBlock            | namedBlock        |         | x      |               |
| ./pkg/types         | types_parameter.go       | Parameter             | parameter         | x       | x      |               |
| ./pkg/types         | types_receipt.go         | Receipt               | receipt           | x       | x      |               |
| ./pkg/types         | types_reconciliation.go  | Reconciliation        | reconciliation    | x       | x      |               |
| ./pkg/types         | types_tokenbalance.go    | TokenBalance          | tokenBalance      | x       | x      |               |
| ./pkg/types         | types_trace.go           | Trace                 | trace             | x       | x      |               |
| ./pkg/types         | types_traceaction.go     | TraceAction           | traceAction       | x       | x      |               |
| ./pkg/types         | types_tracefilter.go     | TraceFilter           | traceFilter       |         | x      |               |
| ./pkg/types         | types_traceresult.go     | TraceResult           | traceResult       | x       | x      |               |
| ./pkg/types         | types_transaction.go     | Transaction           | transaction       | x       | x      |               |
|                     |                          |                       |                   |         |        |               |
| ./api               | openapi.yaml             | <--- missing --->     | abi               |         |        |               |
| ./api               | openapi.yaml             | <--- missing --->     | appearanceCount   |         |        |               |
|                     |                          |                       |                   |         |        |               |
|                     |                          |                       | logfilter         |         |        |               |
|                     |                          |                       | chain             |         |        |               |
|                     |                          |                       | key               |         |        |               |

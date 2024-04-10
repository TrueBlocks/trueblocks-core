List of types that support the Modeler interface

| Path                | File                     | Type                  | Documented        | gen-c++ | gen-go |
| ------------------- | ------------------------ | --------------------- | ----------------- | ------- | ------ |
| ./internal/blocks   | types_blockcount.go      | simpleBlockCount      | blockCount        |         | x      |
| ./internal/chunks   | types_appearanceTable.go | simpleAppearanceTable | <--- missing ---> |         |        |
| ./internal/chunks   | types_chunkaddress.go    | simpleChunkAddress    | chunkAddress      |         | x      |
| ./internal/chunks   | types_chunkbloom.go      | simpleChunkBloom      | chunkBloom        |         | x      |
| ./internal/chunks   | types_chunkindex.go      | simpleChunkIndex      | chunkIndex        |         | x      |
| ./internal/chunks   | types_chunkstats.go      | simpleChunkStats      | chunkStats        |         | x      |
| ./internal/chunks   | types_reportcheck.go     | simpleReportCheck     | reportCheck       |         | x      |
| ./internal/list     | types_bounds.go          | simpleBounds          | bounds            |         | x      |
| ./internal/monitors | types_monitorclean.go    | simpleMonitorClean    | monitorClean      |         | x      |
| ./internal/status   | types_cache.go           | simpleCache           | cache             |         | x      | not turned on |
| ./internal/status   | types_cacheitem.go       | simpleCacheItem       | cacheItem         |         |        |
| ./internal/status   | types_status.go          | simpleStatus          | <--- missing ---> |         | x      |
| ./internal/traces   | types_tracecount.go      | simpleTraceCount      | traceCount        |         | x      |
| ./internal/when     | types_timestamp.go       | simpleTimestamp       | timestamp         |         |        |
| ./internal/when     | types_timestampCount.go  | simpleTimestampCount  | timestampCount    |         |        |
|                     |                          |                       |                   |         |        |
| ./pkg/types         | types_appearance.go      | SimpleAppearance      | appearance        | x       |        | not turned on |
| ./pkg/types         | types_block.go           | SimpleBlock           | block             | x       | x      |
| ./pkg/types         | types_chain.go           | SimpleChain           | chain             | x       | x      |
| ./pkg/types         | types_chunkrecord.go     | SimpleChunkRecord     | chunkRecord       |         | x      |
| ./pkg/types         | types_ethcall.go         | SimpleEthCall         | ethCall           | x       | x      |
| ./pkg/types         | types_slurp.go           | SimpleSlurp           | slurp             |         | x      |
| ./pkg/types         | types_ethstate.go        | SimpleState           | ethState          | x       | x      |
| ./pkg/types         | types_function.go        | SimpleFunction        | function          | x       | x      |
| ./pkg/types         | types_log.go             | SimpleLog             | log               | x       | x      |
| ./pkg/types         | types_manifest.go        | SimpleManifest        | manifest          |         | x      |
| ./pkg/types         | types_monitor.go         | SimpleMonitor         | monitor           | x       |        | not turned on |
| ./pkg/types         | types_name.go            | SimpleName            | name              | x       | x      |
| ./pkg/types         | types_namedblock.go      | SimpleNamedBlock      | namedBlock        |         | x      |
| ./pkg/types         | types_parameter.go       | SimpleParameter       | parameter         | x       | x      |
| ./pkg/types         | types_receipt.go         | SimpleReceipt         | receipt           | x       | x      |
| ./pkg/types         | types_reconciliation.go  | SimpleReconciliation  | reconciliation    | x       | x      |
| ./pkg/types         | types_tokenbalance.go    | SimpleTokenBalance    | tokenBalance      | x       | x      |
| ./pkg/types         | types_trace.go           | SimpleTrace           | trace             | x       | x      |
| ./pkg/types         | types_traceaction.go     | SimpleTraceAction     | traceAction       | x       | x      |
| ./pkg/types         | types_tracefilter.go     | SimpleTraceFilter     | traceFilter       |         | x      |
| ./pkg/types         | types_traceresult.go     | SimpleTraceResult     | traceResult       | x       | x      |
| ./pkg/types         | types_transaction.go     | SimpleTransaction     | transaction       | x       | x      |
|                     |                          |                       |                   |         |        |
| ./api               | openapi.yaml             | <--- missing --->     | abi               |         |        |
| ./api               | openapi.yaml             | <--- missing --->     | appearanceCount   |         |        |
|                     |                          |                       |                   |         |        |
|                     |                          |                       | logfilter         |         |        |
|                     |                          |                       | chain             |         |        |
|                     |                          |                       | key               |         |        |

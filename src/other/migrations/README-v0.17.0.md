# v0.17.0 Updated API and SDK

---

(November 23, 2021)

## Why the Change?

Version 0.17.0 is another major upgrade to our code. We added a Typescript SDK and as a result, in order to make the SDK more consistent, some of the API endpoints changed.

Here's a table showing which endpoints changed. Previously the API endpoints were identical to the command line options. Now, API options are `camelCase` and command line options are `under_bar_cased`.

### Command line / API options

*The api option is the same as the command line flag unless otherwise noted.*

| sub/route    | positional   | cmd flag       | api option* |
| ------------ | ------------ | -------------- | ----------- |
| export       | addrs        |                |             |
|              | topics       |                |             |
|              | fourbytes    |                |             |
|              |              | --appearances  |             |
|              |              | --receipts     |             |
|              |              | --logs         |             |
|              |              | --traces       |             |
|              |              | --statements   |             |
|              |              | --neighbors    |             |
|              |              | --accounting   |             |
|              |              | --articulate   |             |
|              |              | --cache        |             |
|              |              | --cache_traces | cacheTraces |
|              |              | --factory      |             |
|              |              | --count        |             |
|              |              | --first_record | firstRecord |
|              |              | --max_records  | maxRecords  |
|              |              | --relevant     |             |
|              |              | --emitter      |             |
|              |              | --topic        |             |
|              |              | --clean        |             |
|              |              | --freshen      |             |
|              |              | --staging      |             |
|              |              | --unripe       |             |
|              |              | --load         |             |
|              |              | --reversed     |             |
|              |              | --by_date      | byDate      |
|              |              | --skip_ddos    | skipDdos    |
|              |              | --max_traces   | maxTraces   |
|              |              | --first_block  | firstBlock  |
|              |              | --last_block   | lastBlock   |
|              |              |                |             |
| monitors     | addrs        |                |             |
|              |              | --appearances  |             |
|              |              | --count        |             |
|              |              | --clean        |             |
|              |              | --first_block  | firstBlock  |
|              |              | --last_block   | lastBlock   |
|              |              |                |             |
| list         | addrs        |                |             |
|              |              | --count        |             |
|              |              | --appearances  |             |
|              |              | --first_block  | firstBlock  |
|              |              | --last_block   | lastBlock   |
|              |              |                |             |
| names        | terms        |                |             |
|              |              | --expand       |             |
|              |              | --match_case   | matchCase   |
|              |              | --all          |             |
|              |              | --custom       |             |
|              |              | --prefund      |             |
|              |              | --named        |             |
|              |              | --addr         |             |
|              |              | --collections  |             |
|              |              | --tags         |             |
|              |              | --to_custom    | toCustom    |
|              |              | --clean        |             |
|              |              | --autoname     |             |
|              |              |                |             |
| abis         | addrs        |                |             |
|              |              | --known        |             |
|              |              | --sol          |             |
|              |              | --find         |             |
|              |              | --source       |             |
|              |              | --classes      |             |
|              |              |                |             |
| blocks       | blocks       |                |             |
|              |              | --hashes       |             |
|              |              | --uncles       |             |
|              |              | --trace        |             |
|              |              | --apps         |             |
|              |              | --uniq         |             |
|              |              | --logs         |             |
|              |              | --emitter      |             |
|              |              | --topic        |             |
|              |              | --articulate   |             |
|              |              | --big_range    | bigRange    |
|              |              | --count        |             |
|              |              | --cache        |             |
|              |              | --list         |             |
|              |              | --list_count   | listCount   |
|              |              |                |             |
| transactions | transactions |                |             |
|              |              | --articulate   |             |
|              |              | --trace        |             |
|              |              | --uniq         |             |
|              |              | --reconcile    |             |
|              |              | --cache        |             |
|              |              |                |             |
| receipts     | transactions |                |             |
|              |              | --articulate   |             |
|              |              |                |             |
| logs         | transactions |                |             |
|              |              | --articulate   |             |
|              |              |                |             |
| traces       | transactions |                |             |
|              |              | --articulate   |             |
|              |              | --filter       |             |
|              |              | --statediff    |             |
|              |              | --count        |             |
|              |              | --skip_ddos    | skipDdos    |
|              |              | --max          |             |
|              |              |                |             |
| when         | blocks       |                |             |
|              |              | --list         |             |
|              |              | --timestamps   |             |
|              |              | --check        |             |
|              |              | --fix          |             |
|              |              | --count        |             |
|              |              |                |             |
| state        | addrs        |                |             |
|              | blocks       |                |             |
|              |              | --parts        |             |
| state        |              | --changes      |             |
|              |              | --no_zero      | noZero      |
|              |              | --call         |             |
|              |              | --proxy_for    | proxyFor    |
|              |              |                |             |
| tokens       | addrs2       |                |             |
|              | blocks       |                |             |
|              |              | --parts        |             |
|              |              | --by_acct      | byAcct      |
|              |              | --no_zero      | noZero      |
|              |              |                |             |
| scrape       | modes        |                |             |
|              |              | --action       |             |
|              |              | --sleep        |             |
|              |              | --pin          |             |
|              |              | --publish      |             |
|              |              | --block_cnt    | blockCnt    |
|              |              |                |             |
| serve        |              | --port         |             |
|              |              |                |             |
| status       | modes        |                |             |
|              |              | --details      |             |
|              |              | --types        |             |
|              |              | --depth        |             |
|              |              | --report       |             |
|              |              | --terse        |             |
|              |              | --migrate      |             |
|              |              | --first_block  | firstBlock  |
|              |              | --last_block   | lastBlock   |
|              |              |                |             |
| chunks       | blocks       |                |             |
|              |              | --check        |             |
|              |              | --extract      |             |
|              |              | --stats        |             |
|              |              | --save         |             |
|              |              |                |             |
| pins         |              | --list         |             |
|              |              | --init         |             |
|              |              | --all          |             |
|              |              | --sleep        |             |
|              |              | --freshen      |             |
|              |              |                |             |
| init         |              | --all          |             |
|              |              |                |             |
| quotes       |              | --freshen      |             |
|              |              | --period       |             |
|              |              | --pair         |             |
|              |              | --feed         |             |
|              |              |                |             |
| explore      | terms        |                |             |
|              |              | --local        |             |
|              |              | --google       |             |
|              |              |                |             |
| slurp        | addrs        |                |             |
|              | blocks       |                |             |
|              |              | --types        |             |
|              |              | --appearances  |             |
|              |              |                |             |
| globals      |              | --log_level    | logLevel    |
|              |              | --no_header    | noHeader    |
|              |              | --to_file      | toFile      |



### Breaking Changes in Addition to Above

Changed options for `export` route:

```
	n_blocks		--> block_cnt
	n_block_procs	--> block_chan_cnt
	n_addr_procs	--> addr_chan_cnt
	cache_txs       --> cache

	# hotkeys changed
	the `-a` hotkey is now assigned to the `--articulate` option
	the `-s` hotkey is now assigned to the `--apps` option

```

Changed options for `blocks` route:

```
	Added options `--logs`, `--emitter`, and `--topics`
	Previous `--uniq` option removed as redundant
	Previously named `--uniq_txs` option renamed to `--uniq`
```

Deprecated these options for the `pins` route:

```
	`--freshen`, `--remote`, and `--init_all`
```

### Other slight changes
The field names for CSV and TXT export for some commands was switch to `camelCase` from previously `lowercase`.

For every API call, the `types` data items were removed as they were never used and very incomplete.

## You're Finished

Please report any problems by creating an issue.

## Previous Migration

[Click here](./README-v0.14.0.md) for the previous migration.

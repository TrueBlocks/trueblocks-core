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
|              |              | --summarize_by | summarizeBy |
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
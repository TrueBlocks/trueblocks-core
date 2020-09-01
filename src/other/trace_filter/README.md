## Test cases for `trace_filter`:

The results were created against Parity v2.6.7-beta.

#### 1. Starting with the example from RPC documentation (see second comment on [this issue](https://github.com/openethereum/wiki/issues/369)):

Get `100` traces from `1000th` trace in block range `[3068100-3068200]` (inclusive) for address `0x8b...`

```[bash]
curl -s --data '{"method":"trace_filter","params":[{"fromBlock":"0x2ed0c4","toBlock":"0x2ed128","toAddress":["0x8bbb73bcb5d553b5a556358d27625323fd781d37"],"after":1000,"count":100}],"id":1,"jsonrpc":"2.0"}' -H "Content-Type: application/json" -X POST localhost:8545
```

[View Result 1](./result1.json)

Because there are only two traces for that address between those blocks, the result is empty:

```[json]
{"jsonrpc":"2.0","result":[],"id":1}
```

#### 2. Same as previous test with `after` and `count` removed:

```[bash]
curl -s --data '{"method":"trace_filter","params":[{"fromBlock":"0x2ed0c4","toBlock":"0x2ed128","toAddress":["0x8bbb73bcb5d553b5a556358d27625323fd781d37"]}],"id":2,"jsonrpc":"2.0"}' -H "Content-Type: application/json" -X POST localhost:8545
```

[View Result 2](./result2.json)

Shows two traces. Note that block range is inclusive:

```[json]
{"jsonrpc":"2.0","result":[{"action":{"callType":"call","from":"0x32be343b94f860124dc4fee278fdcbd38c102d88","gas":"0x4c40d","input":"0x","to":"0x8bbb73bcb5d553b5a556358d27625323fd781d37","value":"0x3f0650ec47fd240000"},"blockHash":"0x86df301bcdd8248d982dbf039f09faf792684e1aeee99d5b58b77d620008b80f","blockNumber":3068183,"result":{"gasUsed":"0x0","output":"0x"},"subtraces":0,"traceAddress":[],"transactionHash":"0x3321a7708b1083130bd78da0d62ead9f6683033231617c9d268e2c7e3fa6c104","transactionPosition":3,"type":"call"},{"action":{"callType":"call","from":"0x267be1c1d684f78cb4f6a176c4911b741e4ffdc0","gas":"0x2328","input":"0x","to":"0x8bbb73bcb5d553b5a556358d27625323fd781d37","value":"0x3a0db91420edd20000"},"blockHash":"0xd41acd1affdddd519190f912bed89e3db49d015b269e50e901c64e816f2ebd74","blockNumber":3068200,"result":{"gasUsed":"0x0","output":"0x"},"subtraces":0,"traceAddress":[],"transactionHash":"0x512934968f8c24e95fe571a2cc84f79a082c994ca8a9fd263c3486ba56f4b77f","transactionPosition":1,"type":"call"}],"id":2}
```

#### 3. Same as previous with `count` set to `1`:

```[bash]
curl -s --data '{"method":"trace_filter","params":[{"fromBlock":"0x2ed0c4","toBlock":"0x2ed128","toAddress":["0x8bbb73bcb5d553b5a556358d27625323fd781d37"],"count":1}],"id":2,"jsonrpc":"2.0"}' -H "Content-Type: application/json" -X POST localhost:8545
```

[View Result 3](./result3.json)

Shows only the first trace:

```[json]
{"jsonrpc":"2.0","result":[{"action":{"callType":"call","from":"0x32be343b94f860124dc4fee278fdcbd38c102d88","gas":"0x4c40d","input":"0x","to":"0x8bbb73bcb5d553b5a556358d27625323fd781d37","value":"0x3f0650ec47fd240000"},"blockHash":"0x86df301bcdd8248d982dbf039f09faf792684e1aeee99d5b58b77d620008b80f","blockNumber":3068183,"result":{"gasUsed":"0x0","output":"0x"},"subtraces":0,"traceAddress":[],"transactionHash":"0x3321a7708b1083130bd78da0d62ead9f6683033231617c9d268e2c7e3fa6c104","transactionPosition":3,"type":"call"}],"id":2}
```

#### 4. Same as previous with `after` set to `1`:

```[bash]
curl -s --data '{"method":"trace_filter","params":[{"fromBlock":"0x2ed0c4","toBlock":"0x2ed128","toAddress":["0x8bbb73bcb5d553b5a556358d27625323fd781d37"],"after":1,"count":1}],"id":2,"jsonrpc":"2.0"}' -H "Content-Type: application/json" -X POST localhost:8545
```

[View Result 4](./result4.json)

Shows only the second trace:

```[json]
{"jsonrpc":"2.0","result":[{"action":{"callType":"call","from":"0x267be1c1d684f78cb4f6a176c4911b741e4ffdc0","gas":"0x2328","input":"0x","to":"0x8bbb73bcb5d553b5a556358d27625323fd781d37","value":"0x3a0db91420edd20000"},"blockHash":"0xd41acd1affdddd519190f912bed89e3db49d015b269e50e901c64e816f2ebd74","blockNumber":3068200,"result":{"gasUsed":"0x0","output":"0x"},"subtraces":0,"traceAddress":[],"transactionHash":"0x512934968f8c24e95fe571a2cc84f79a082c994ca8a9fd263c3486ba56f4b77f","transactionPosition":1,"type":"call"}],"id":2}
```

#### 5. All traces for block `10700000`:

**IMPORTANT NOTE:** Be careful. If you remove the `toBlock` parameter, Parity hangs until the OS kills it trying to extract every trace from the `fromBlock` to `latest`. Even if you kill the command, Parity stops responding and eventually gets killed...

```[bash]
curl -s --data '{"method":"trace_filter","params":[{"fromBlock":"0xa344e0","toBlock":"0xa344e0"}],"id":3,"jsonrpc":"2.0"}' -H "Content-Type: application/json" -X POST localhost:8545
```

[View Result 5](./result5.json)

Results in `794` traces.

#### 6. Same as previous limited to `toAddress` of `0xc02aa....`:

```[bash]
curl -s --data '{"method":"trace_filter","params":[{"fromBlock":"0xa344e0","toBlock":"0xa344e0","toAddress":["0xc02aaa39b223fe8d0a0e5c4f27ead9083c756cc2"]}],"id":3,"jsonrpc":"2.0"}' -H "Content-Type: application/json" -X POST localhost:8545
```

[View Result 6](./result6.json)

Results in 85 traces.

#### 7. Same as twice previous limited to `fromAddress` of `0xc3c....`:

```[bash]
curl -s --data '{"method":"trace_filter","params":[{"fromBlock":"0xa344e0","toBlock":"0xa344e0","fromAddress":["0xc3ca90684fd7b8c7e4be88c329269fc32111c4bd"]}],"id":3,"jsonrpc":"2.0"}' -H "Content-Type: application/json" -X POST localhost:8545
```

[View Result 7](./result7.json)

Results in `16` traces

#### 8. Same as three times previous limited to `fromAddress` of `0xc3c....` AND `toAddress` of `0xc02aa....`:

```[bash]
curl -s --data '{"method":"trace_filter","params":[{"fromBlock":"0xa344e0","toBlock":"0xa344e0","fromAddress":["0xc3ca90684fd7b8c7e4be88c329269fc32111c4bd"],"toAddress":["0xc02aaa39b223fe8d0a0e5c4f27ead9083c756cc2"]}],"id":3,"jsonrpc":"2.0"}' -H "Content-Type: application/json" -X POST localhost:8545
```

[View Result 8](./result8.json)

Results in `4` traces

**Note:** Specifying both `fromAddress` and `toAddress` selects records using AND.

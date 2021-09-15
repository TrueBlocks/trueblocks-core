The deepest layer of the Ethereum data is the trace. Every transaction has at least one trace which is itself a record of the transaction. If the `to` address of the transaction is a smart contract, other traces may appear, if, for example, that smart contract calls other smart contracts.

Traces may be arbitrarily deep (up to the gasLimit) and ultimately represent a tree of function calls. Some transactions have 100s of traces. The format of the trace is similar to the transaction itself have a trace `action` (which contains `from`, `to`, `value` like the transaction) and the trace `result` (containing `gasUsed` like the receipt).

### How to get traces

- **CLI**:
  - run `chifra traces <txn_id>`.
  - [See the command's documentation](/docs/chifra/chaindata/#chifra-traces)
- **API**:
  - [Calls to `/traces`](/api#operation/chaindata-traces)

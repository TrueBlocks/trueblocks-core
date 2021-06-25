# Ethereum Trace Testing

### Postman

The easiest way to use these tests is to import the `./Trace_Testing.json` into Postman and run the tests against your local node. Instructions for doing this are here: https://github.com/ledgerwatch/erigon/wiki/Using-Postman-to-test-RPC (replace the `RPC_Testing.json` file the `./Trace_Testing.json`).

---

Open Ethereum presents seven different trace routines:

- trace_call
- trace_callMany
- trace_replayTransaction
- trace_replayBlockTransaction
- trace_rawTransaction
- trace_get
- trace_transaction
- trace_block
- trace_filter

The Postman file (and these folders) contains one or more test for each routine.

The first five routines above have five tests each corresponding to the _trace_type_ (some combination of `trace`, `vmTrace`, and `stateDiff`) being requested. The `_all` and `_none` tests request all three _trace_types_ or no _trace_types_ respectively. There are 32 tests in all.

---

### Running Individual Traces

Each `.json` file in the `./tests` folder holds a single `curl` command.

Run the tests with `source <filename> <RPC_URL>` from a command line, where `RPC_URL` points to the endpoint of your RPC server.

For example, to run the first test, do:

```[bash]
source ./tests/trace_call_none http://localhost:8545
```

should produce this result:

```[json]
{
  "jsonrpc": "2.0",
  "result": {
    "output": "0x",
    "stateDiff": null,
    "trace": [],
    "vmTrace": null
  },
  "id": "1"
}
```

---

### Comparing to Expected

One simple way to test your trace implementation is to redirect the results of the call into the corresponding results file.

. If you redirect the result into the corresponding file in `./expected_results` you can then do `git diff` on that file to see what's changed.

## testRunner

The `testRunner` tool runs the TrueBlocks tests. The test cases are stored in CSV files a folder called `./testCases`.

Test cases contain the following fields:

| field   | meaning                                                                                  |
| ------- | ---------------------------------------------------------------------------------------- |
| onOff   | either `on` or `off` to enable/disable the test                                          |
| mode    | one of `cmd`, `api` or `both` (runs against command line, API server or both)            |
| speed   | one of `slow`, `medium` or `fast` allows test runner to skip tests that take a long time |
| route   | the API route of the tool                                                                |
| tool    | the command line location of the tool                                                    |
| name    | the name of the resulting test case file                                                 |
| post    | one of `y` or `n` determines if the resulting output is run through `jq` JSON formatter  |
| options | the command line / API url options (in API format)                                       |
| extra   | extra data needed for some tests                                                         |

You may run all tests from the `./build` folder by entering `make test-all` (or `make tests` to run only the `cmd/fast` cases).

### usage

`Usage:`    testRunner [-m|-f|-c|-r|-v|-h]
`Purpose:`  Run TrueBlocks' test cases with options.

`Where:`

{{<td>}}
|     | Option               | Description                                                                  |
| --- | -------------------- | ---------------------------------------------------------------------------- |
| -m  | --mode &lt;val&gt;   | determine which set of tests to run, one of [cmd, api,<br/>both]             |
| -f  | --filter &lt;val&gt; | determine how long it takes to run tests, one of [fast,<br/>medi, slow, all] |
| -c  | --clean              | clean working folder before running tests                                    |
| -r  | --report             | display performance report to screen                                         |
| -x  | --fmt &lt;val&gt;    | export format, one of [none, json, txt, csv, api]                            |
| -v  | --verbose            | set verbose level (optional level defaults to 1)                             |
| -h  | --help               | display this help screen                                                     |
{{</td>}}

**Source code**: [`dev_tools/testRunner`](https://github.com/TrueBlocks/trueblocks-core/tree/master/src/dev_tools/testRunner)


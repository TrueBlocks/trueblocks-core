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

You may run all tests from the `./build` folder by entering `make test-all`.

Purpose:
  Run TrueBlocks' test cases with options.

Usage:
  testRunner [flags]

Flags:
  -m, --mode <val>     determine which set of tests to run, one of [cmd*|api|both]
  -f, --filter <val>   determine how long it takes to run tests, one of [fast*|medi|slow|all]
  -c, --clean          clean working folder before running tests
  -s, --skip <num>     run only every 'skip' test (faster) (hidden)
  -r, --report         display performance report to screen

Global Flags:
  -x, --fmt string     export format, one of [none|json*|txt|csv]
  -v, --verbose uint   set verbose level (optional level defaults to 1)
  -h, --help           display this help screen

**Source code**: [`dev_tools/testRunner`](https://github.com/TrueBlocks/trueblocks-core/tree/master/src/dev_tools/testRunner)

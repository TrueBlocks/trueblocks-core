## testRunner

The `testRunner` tool runs the TrueBlocks tests. The test cases are stored in CSV files a folder called `./testCases`.

### Usage

`Usage:`    testRunner [-m|-f|-c|-r|-v|-h]  
`Purpose:`  Run TrueBlocks' test cases with options.

`Where:`

|          | Option                           | Description                                                                       |
| -------- | -------------------------------- | --------------------------------------------------------------------------------- |
| &#8208;m | &#8208;&#8208;mode &lt;val&gt;   | determine which set of tests to run  (default 'both')<br/>one of [cmd, api, both] |
| &#8208;f | &#8208;&#8208;filter &lt;val&gt; | determine how long it takes to run tests<br/>one of [fast, medi, slow, all]       |
| &#8208;c | &#8208;&#8208;clean              | clean working folder before running tests                                         |
| &#8208;r | &#8208;&#8208;report             | display performance report to screen                                              |
| &#8208;x | &#8208;&#8208;fmt &lt;val&gt;    | export format, one of [none, json, txt, csv, api]                                 |
| &#8208;v | &#8208;&#8208;verbose            | set verbose level (optional level defaults to 1)                                  |
| &#8208;h | &#8208;&#8208;help               | display this help screen                                                          |

#### Test Cases

Test cases, which are stored in individual files for each tool in the folder `./src/dev_tools/testRunner/testCases` contain the
following fields:

| field   | meaning                                                                                     |
| ------- | ------------------------------------------------------------------------------------------- |
| onOff   | either `on`, `off`, or `local` to enable/disable the test (`local` is enabled only locally) |
| mode    | one of `cmd`, `api` or `both` (runs against command line, API server or both)               |
| speed   | one of `slow`, `medium` or `fast` allows test runner to skip tests that take a long time    |
| route   | the API route of the tool                                                                   |
| tool    | the command line location of the tool                                                       |
| name    | the name of the resulting test case file                                                    |
| post    | one of `y` or `n` determines if the resulting output is run through `jq` JSON formatter     |
| options | the command line / API url options (in API format)                                          |
| extra   | extra data needed for some tests                                                            |

Run the tests from the `./build` folder with `make test-all` (or `make tests` to run only the `cmd/fast` test cases).

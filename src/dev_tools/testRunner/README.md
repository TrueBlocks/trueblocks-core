## testRunner

The `testRunner` tool runs the TrueBlocks tests. The test cases are stored in CSV files a folder called `./testCases`.

Test cases contain the following fields:

| field | meaning |
|-------|---------|
| onOff | either `on` or `off` to enable/disable the test |
| mode | one of `cmd`, `api` or `both` (runs against command line, API server or both) |
| speed | one of `slow`, `medium` or `fast` allows test runner to skip tests that take a long time |
| route | the API route of the tool |
| tool | the command line location of the tool |
| name | the name of the resulting test case file |
| post | one of `y` or `n` determines if the resulting output is run through `jq` JSON formatter |
| options | the command line / API url options (in API format) |
| extra | extra data needed for some tests |

You may run all tests from the `./build` folder by entering `make test-all` (or `make tests` to run only the `cmd/fast` cases).

### Usage

`Usage:`    testRunner [-m|-f|-c|-n|-o|-r|-v|-h]  
`Purpose:`  Run TrueBlocks' test cases with options.

`Where:`  

| Hotkey | Option | Description |
| :----- | :----- | :---------- |
| -m | --mode <val> | determine which set of tests to run, one of *[ cmd\* \| api \| both ]* |
| -f | --filter <val> | determine how long it takes to run tests, one of *[ fast\* \| medi \| slow \| all ]* |
| -c | --clean | clean working folder before running tests |
| -n | --no_quit | do not quit testing on first error |
| -o | --no_post | do not complete the post processing step |
| -r | --report | display performance report to screen |
| -v | --verbose | set verbose level (optional level defaults to 1) |
| -h | --help | display this help screen |

#### Other Options

All **TrueBlocks** command-line tools support the following commands (although in some case, they have no meaning):

| Command     | Description                                                                                     |
| ----------- | ----------------------------------------------------------------------------------------------- |
| --version   | display the current version of the tool                                                         |
| --nocolor   | turn off colored display                                                                        |
| --wei       | specify value in wei (the default)                                                              |
| --ether     | specify value in ether                                                                          |
| --dollars   | specify value in US dollars                                                                     |
| --raw       | report JSON data from the node with minimal processing                                          |
| --very_raw  | report JSON data from node with zero processing                                                 |
| --fmt       | export format (where appropriate). One of [ none &#124; txt &#124; csv &#124; json &#124; api ] |
| --to_file   | write the results to a temporary file and return the filename                                   |
| --output:fn | write the results to file 'fn' and return the filename                                          |
| --file:fn   | specify multiple sets of command line options in a file.                                        |

<small>*For the `--file:fn` option, place a series of valid command lines in a file and use the above options. In some cases, this option may significantly improve performance. A semi-colon at the start of a line makes that line a comment.*</small>

**Source code**: [`dev_tools/testRunner`](https://github.com/TrueBlocks/trueblocks-core/tree/master/src/dev_tools/testRunner)


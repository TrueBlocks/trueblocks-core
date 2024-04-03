### Test cases

Test cases contain the following fields:

| field   | meaning                                                                                 |
| ------- | --------------------------------------------------------------------------------------- |
| onOff   | either [`on` | `off`] to enable/disable the test                                        |
| mode    | one of [`cmd` | `api` | `both`] (runs against command line, API server or both)         |
| speed   | one of [`slow` | `medium` | `fast`] allows test runner to skip long-running tests       |
| route   | the API route of the tool                                                               |
| tool    | the command line location of the tool                                                   |
| name    | the name of the resulting test case file                                                |
| post    | one of `y` or `n` determines if the resulting output is run through `jq` JSON formatter |
| options | the command line / API url options (in API format)                                      |
| extra   | extra data needed for some tests                                                        |

You may run all tests from the `./build` folder by entering `make test-all` (or `make tests` to run
only the `cmd/fast` cases).

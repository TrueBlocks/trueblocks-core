# testRunner - new GoLang test runner

This tool runs the core's test cases of which there are nearly 1,500.

All tests will be run (in the order shown below) unless you provide a filter.

Provide a filter by running `testRunner` with the environment variable `TB_TEST_FILTER` exported, thus:

```[bash]
TB_TEST_FILTER=routeList:modeList testRunner
```

If you provide a filter, the tests will be run in the order you provide. Either of the two lists may be empty in which case all tests of that type will run. Seperate the two lists with a colon (:).

`routeList` may be empty or one or more (comma seperated) items from:

```[bash]
- tools (runs all tools routes)
    - slurp (runs only if you also set TEST_SLURP=true)
    - names
    - blocks
    - logs
    - receipts
    - state
    - tokens
    - traces
    - transactions
    - abis
    - when
- apps (runs all apps routes)
    - list
    - monitors
    - export
    - scrape
    - status
    - chifra
    - chunks
    - config
    - daemon
    - explore
    - init
```

`modeList` may be empty or one or more (comma seperated) items from:

```[bash]
- api
- cmd
- sdk
```

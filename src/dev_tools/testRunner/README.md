# testRunner

The `testRunner` tool runs the core's test cases of which there are nearly 1,500.

All tests are run (in the order shown below) unless you provide a filter. You may provide a filter in two ways: (1) export an environment variable with the name `TB_TEST_FILTER`, or (2) provide as the first argument on the tool's command line.

The filter takes the following format:

```[bash]
TB_TEST_FILTER=<routeList>:<modeList> testRunner
```

or

```[bash]
testRunner <routeList>:<modeList>
```

## Format of the filter

The filter consists of two lists `<routeList>` and `<modeList>` separated by a colon (:). If either of the lists is empty, the given filter is disabled. The tests proceed in the following order, unless you provide a filter, in which case they run in the order you provide. The special `tools` and `apps` routes are used as shorthand for all tests of that type.

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

## Examples

```[bash]
TB_TEST_FILTER=tools:cmd testRunner
```

runs the `cmd` tests for all the `tools`.

```[bash]
testRunner export:api
```

runs the `api` tests for the `export` command.

```[bash]
testRunner blocks,traces,export:sdk,cmd
```

runs the `sdk` and `cmd` tests for the `blocks`, `traces`, and `export` commands as does

```[bash]
TB_TEST_FILTER=blocks,traces,export:sdk,cmd testRunner
```

Finally,

```[bash]
testRunner
```

runs all tests.

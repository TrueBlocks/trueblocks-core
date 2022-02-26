# v0.18.0 Better Neighbors

---

(December 10, 2021)

## Why the Change?

We corrected a previous mistake in the headers of .txt and .csv output. Previously all the field names were lower case. In this release we now export field names in these two formats to agree with the JSON output. The following field names changed, but only in the casing.

### Changes

    | old header name  | new header name  |
    | ---------------- | ---------------- |
    | blocknumber      | blockNumber      |
    | gaslimit         | gasLimit         |
    | parenthash       | parentHash       |
    | addrcount        | addrCount        |
    | basefeepergas    | baseFeePerGas    |
    | transactionindex | transactionIndex |
    | transactionhash  | transactionHash  |
    | transactionscnt  | transactionsCnt  |
    | ethergasprice    | ethGasPrice      |
    | gasused          | gasUsed          |
    | iserror          | isError          |
    | logindex         | logIndex         |
    | traceaddress     | traceAddress     |
    | traceid          | traceId          |
    | compressedtx     | compressedTx     |
    | compressedlog    | compressedLog    |
    | compressedtrace  | compressedTrace  |
    | compressedresult | compressedResult |

### Other slight changes

The `chifra export --neighbors` option now no longer include the `traceId` field. Additionally, the `reason` field is slightly less informative than it was previously. All of this in service of a significant increase in the speed of the option.

## You're Finished

Please report any problems by creating an issue.

## Previous Migration

[Click here](./README-v0.17.0.md) for the previous migration.

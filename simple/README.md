# README

## Overview

This is a purposefully simple example intended to get your started.

The example application retrieves blocks at a monthly interval using the TrueBlocks SDK.
The application then prints the block numbers and their corresponding dates and quits. Simple.

## Prerequisites

Ensure you have the following installed and running:

- Go Version 1.23 or higher
- The latest version of TrueBlocks Core

## Installation

Clone the repository:

```bash
git clone https://github.com/TrueBlocks/trueblocks-core.git
```

Build TrueBlocks Core by following the ([installation instructions](https://trueblocks.io/docs/install/install-core/)), and then navigate to the example directory:

```bash
cd ../examples/simple
```

## Usage

Run the example using the following command:

```bash
go run .
```

There are no command line options.

### Output

The above will render data similar to the following. Note that each record represents the last block in the given month which would be useful, for example, in an accounting app.

```bash
...
17382265    2023-05-31 23:59:59 UTC
17595509    2023-06-30 23:59:59 UTC
17816433    2023-07-31 23:59:59 UTC
18037987    2023-08-31 23:59:59 UTC
18251964    2023-09-30 23:59:59 UTC
18473542    2023-10-31 23:59:59 UTC
18687850    2023-11-30 23:59:59 UTC
...
```

## Troubleshooting

If you encounter issues, check the following:

- Ensure at least Go Version 1.23.
- Make sure you have a valid Ethereum mainnet RPC configured.
- Ensure TrueBlocks Core is properly installed and configured.
- Verify the command-line arguments are correct and within valid ranges.

For further assistance, refer to the TrueBlocks Documentation.

## License

See the LICENSE file at the root of this repo for details.

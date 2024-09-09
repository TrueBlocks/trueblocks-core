
# NameManager CLI Tool

`nameManager` is an example command-line tool built with the [TrueBlocks SDK](https://pkg.go.dev/github.com/TrueBlocks/trueblocks-sdk/v3) designed to manage name entries associated with addresses. It offers various operations such as adding, editing, deleting, and cleaning name entries in the names database.

## Building

```bash
cd ./nameManager
go build -o nameManager main.go actions.go help.go parseArgs.go
./nameManager --help
```

## Usage

```bash
nameManager [options] address name [tags [source [symbol [decimals]]]]
nameManager --autoname address
nameManager --delete address
nameManager --undelete address
nameManager --remove address
nameManager --clean
```

## Commands and Options

### Required Arguments

- **address**: The address for all operations except `--clean` (type: `string`).
- **name**: The name to associate with the address for name editing (type: `string`).

### Optional Arguments

- **tags**: Tags associated with the name entry (type: `string`, default: `"99-User-Defined"`).
- **source**: The source of the name entry (type: `string`, default: `"TrueBlocks"`).
- **symbol**: A symbol related to the name entry (type: `string`, default: `""`).
- **decimals**: The number of decimals associated with the entry (type: `uint64`, default: `0`).

### Options

- **--help**: Displays the help message and exits.
- **--autoname**: Automatically queries the blockchain for ERC20 values associated with the given address and updates the name entry.
- **--delete**: Deletes the name entry for the specified address.
- **--undelete**: Restores a previously deleted name entry for the given address.
- **--remove**: Completely removes the node associated with the given address.
- **--clean**: Cleans the names database, sorting entries and removing any duplicates.

## Examples

### Add or Update a Name Entry

To add or update a name entry for an address:

```bash
nameManager 0x1234...ABCD "MyToken" "99-User-Defined" "TrueBlocks" "MYT" 18
```

- This command will add or update the entry with the address `0x1234...ABCD` and associate it with the name `MyToken`, the tag `99-User-Defined`, the source `TrueBlocks`, the symbol `MYT`, and `18` decimals.

### Autoname a Given Address

To automatically name an address by querying the blockchain:

```bash
nameManager --autoname 0x1234...ABCD
```

- This will query the chain for ERC20 values related to the address and update the name entry accordingly.

### Delete a Name Entry

To delete a name entry for a specific address:

```bash
nameManager --delete 0x1234...ABCD
```

- This will remove the name entry associated with the address `0x1234...ABCD`.

### Undelete a Name Entry

To restore a previously deleted name entry:

```bash
nameManager --undelete 0x1234...ABCD
```

- This will undelete the name entry associated with the address `0x1234...ABCD`.

### Remove a Node

To remove a node associated with a specific address:

```bash
nameManager --remove 0x1234...ABCD
```

- This will remove the node linked to the address `0x1234...ABCD` from the database.

### Clean the Names Database

To clean the entire names database, sorting entries and removing duplicates:

```bash
nameManager --clean
```

- This will clean up the names database, ensuring it's sorted and free of duplicates.

## Documentation

Please see our website for the [best available documentation](https://trueblocks.io/).

## Linting

Please makes sure to lint your code before submitting a pull request. See our primary repo for more information on our [linting process](https://github.com/TrueBlocks/trueblocks-core#linting).

## Contributing

We love contributors. Please see information about our [workflow](https://github.com/TrueBlocks/trueblocks-core/blob/develop/docs/BRANCHING.md) before proceeding.

1. Fork this repository into your own repo.
2. Create a branch: `git checkout -b <branch_name>`.
3. Make changes to your local branch and commit them to your forked repo: `git commit -m '<commit_message>'`
4. Push back to the original branch: `git push origin TrueBlocks/trueblocks-core`
5. Create the pull request.

## Contact

If you have questions, comments, or complaints, please join the discussion on our discord server which is [linked from our website](https://trueblocks.io).

## List of Contributors

Thanks to the following people who have contributed to this project:

- [@tjayrush](https://github.com/tjayrush)
- many others

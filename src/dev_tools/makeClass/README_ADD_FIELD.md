# Instructions for Adding a Field to a C++ Class

We use a home grown tool called `makeClass` to generate a large amount of boilerplate code for many of the C++ classes in this repo.

Each class is described with a class definition file. You can find these classes by running this command from the `./src` folder:

```shell
find . | grep classDefinitions
```

This should list a large number of files, one for each of the automatically generated C++ classes in the repo. The files are `TOML` format. The corresponding `.cpp` and `.h` are in the parent folder of the class definition.

Here's the class definition file for the CBlock class:

```
[settings]
class = CBlock
fields = gas gasLimit|\
        gas gasUsed|\
        hash hash|\
        blknum blockNumber|\
        hash parentHash|\
        address miner|\
        uint64 difficulty|\
        double price|\
        bool finalized|\
        timestamp timestamp|\
        CTransactionArray transactions|\
        CStringArray tx_hashes (nowrite)|\
        string name (nowrite)|\
        bool light (nowrite,omitempty)

[many other items removed for clarity]
```

## Running makeClass

Run `makeClass --help` to see all the available options for the tool.

In most cases, you will not use this tool directly as it gets run as part of the build process, but if you want to change a class, you must first edit the classDefinition and then run the `makeClass`.

```shell
make generate finish
```

This should re-generate the `.cpp` and the `.h` file corresponding to the file you've edited, but before you do that read on...

## Important

_Read the notes at the end of this documente about backwards compatibility before proceeding..._

## Editing the classDefinition File

To add a field to a class edit the classDefinition file. For the above `blocks.txt` file, you would add a field thus:

```
fields = gas gasLimit|\
        gas gasUsed|\
        hash hash|\
        blknum blockNumber|\
        hash parentHash|\
        address miner|\
        uint64 difficulty|\
        uint256 newField|\  # the new field
        double price|\
        bool finalized|\
        timestamp timestamp|\
        CTransactionArray transactions|\
        CStringArray tx_hashes (nowrite)|\
        string name (nowrite)|\
        bool light (nowrite,omitempty)
```

The format of each line is `fieldType fieldName|\`

For a list of valid `fieldTypes` see here: https://github.com/TrueBlocks/trueblocks-core/blob/master/src/dev_tools/makeClass/handle_generate.cpp#L82).

The `fieldName` may be any valid `c++` variable name.

The continuation mark (|\\) is needed on all lines but the last.

## Backwards Compatibility

You may notice above that some of the fields have `(nowrite)` next to them. This tell `makeClass` to skip that field when writing to the binary cache.

In most cases, however, you do want to write the field to the cache. The trouble is, if you do this, you will create a backwards incomaptible file.

In order to ensure backwards compatibility you must edit the generated `.ccp` file before running the above command.

In the `.ccp` file, find a function called `readBackLevel`. For example, for the `CBlock` class that function looks like this: https://github.com/TrueBlocks/trueblocks-core/blob/master/src/libs/etherlib/block.cpp#L500.

Next find the a function called `Serialize`. Make sure to find the version of this function that reads the data. (In this case: https://github.com/TrueBlocks/trueblocks-core/blob/master/src/libs/etherlib/block.cpp#L313)

---

Before running the above commands do this:

1. Bump the version (see the file `./src/libs/utillib/version.cpp`),

2. Copy the code from the existing `Serialize` function into the `readBackLevel` function inside of a section protected by an `isVersionNum` test corresponding to the new version number.

3. Save your work.

Now return above and run `make generate finish`.

---

The next time TrueBlocks runs, the version stored in any existing files will be less than the new version and the `readBackLevel` code will process the read.

Notice that we do not the write the upgraded data to disc. We let the calling code decide if it wants to re-write.

Also notice that the function `SerializeC` (which writes the data) always writes the latest version of the class, so we get automatic updating simply by re-writing.

## One Final Note

There's a huge chance this won't work...don't worry. `git checkout ..` erases all problems and any corrupted data in the cache can be removed and re-created.

Have fun.

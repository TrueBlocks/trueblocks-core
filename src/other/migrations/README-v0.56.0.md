Raw routines used to return CHainId 0x0. Now they correctly return 0x1. If this value was in the cache, it will be wrong and we do not update it. TO correct, remove the transasction, block, and traces casche.

Some error messages in the traces cache say Bad response, but they used to say invalid response. It causes no problem, but you can remove the traces cache if you wanted to clean this up

Remove empty abi files

Must instruct user to remove known.bin and/or remove it ourselves if it doesn't have a version number

When articulating, previously we included escaped characters such as \t and \n in the output when decoding strings.
    This caused problems when using the data in some CSV accepting files (such as MS Excel). We now produce
    [t] for `\t` and [n] for `\n` and change `\r` into a blank


Remove empty abi files

Must instruct user to remove known.bin and/or remove it ourselves if it doesn't have a version number

When articulating, previously we included escaped characters such as \t and \n in the output when decoding strings.
    This caused problems when using the data in some CSV accepting files (such as MS Excel). We now produce
    [t] for `\t` and [n] for `\n` and change `\r` into a blank


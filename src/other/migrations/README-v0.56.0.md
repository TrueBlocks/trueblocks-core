Hack for erigon bug

Better Unchained index usage


Remove AbiSource
Removes input_names and output_names
Remove empty abi files
Other changes to types in makeClass 

Must instruct user to remove known.bin and/or remove it ourselves if it doesn't have a version number

When articulating, previously we included escaped characters such as \t and \n in the output when decoding strings.
    This caused problems when using the data in some CSV accepting files (such as MS Excel). We now produce
    [t] for `\t` and [n] for `\n` and change `\r` into a blank

Breaking change -- we renamed a field in the TraceResult model to reflect the field's actual name in the Etheruem data structures returned
by the RPC.

Breaking change -- when a trace routine both creates a smart contract and self destructs, we used to report the newly createds
smart contract in the from field. As this was not canoncial -- the address is not the from address -- we stopped
doing that -- this is only from CSV and TXT. If you want to get newly created smart contract addresses, use --fmt json 
instead.


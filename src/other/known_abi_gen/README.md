Known ABI File Generator (WIP)
=============================

This tool downloads repositories containing well-known smart contracts, unzips them,
compiles `.sol` files into combined ASTs and parses them to generate slices of `types.Function`.
Finally, it marshals the slices into JSON.

TO DO
-----

We switched the tool to use `forge` (part of Foundry) instead of `solc`, as `forge` can automatically detect and fetch
different `solc` versions. However, `forge` doesn't seem to produce combined AST (just separate AST files for each `.sol` input).
This means that methods defined in contract's dependencies will be missing.

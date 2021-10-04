## chifra names

`chifra names` is a surprisingly useful tool. It allows one to associate textual names with Ethereum addresses. One may ask why this is necessary given that ENS exists. The answer is a single word: "privacy". ENS names are public. In many cases, users desire to keep personal addresses private. Try to do this on a website.

Like `chifra abis`, this tool is useful from the command line but is primarily used in support of other tools, especially `chifra export` where naming addresses becomes the single best way to turn unintelligible blockchain data into understandable information.

The various options allow you to search and filter the results. The `collections` and `tags` options are used primarily by the TrueBlocks explorer.

You may use the TrueBlocks explorer to manage (add, edit, delete) address-name associations.

```[plaintext]
Purpose:
  Query addresses or names of well known accounts.

Usage:
  chifra names [flags] <term> [term...]

Arguments:
  terms - a space separated list of one or more search terms (required)

Flags:
  -e, --expand        expand search to include all fields (search name, address, and symbol otherwise)
  -m, --match_case    do case-sensitive search
  -l, --all           include all accounts in the search
  -c, --custom        include your custom named accounts
  -p, --prefund       include prefund accounts
  -n, --named         include well know token and airdrop addresses in the search
  -a, --addr          display only addresses in the results (useful for scripting)
  -s, --collections   display collections data
  -g, --tags          export the list of tags and subtags only

Global Flags:
  -x, --fmt string   export format, one of [none|json*|txt|csv|api]
  -h, --help         display this help screen
  -v, --verbose      enable verbose (increase detail with --log_level)

Notes:
  - The tool will accept up to three terms, each of which must match against any field in the database.
  - The --match_case option enables case sensitive matching.
```

**Source code**: [`tools/ethNames`](https://github.com/TrueBlocks/trueblocks-core/tree/master/src/tools/ethNames)


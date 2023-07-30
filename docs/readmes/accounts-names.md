## chifra names

<!-- markdownlint-disable MD041 -->
`chifra names` is a surprisingly useful tool. It allows one to associate textual names with Ethereum
addresses. One may ask why this is necessary given that ENS exists. The answer is a single
word: "privacy". ENS names are public. In many cases, users desire to keep personal addresses
private. Try to do this on a website.

Like `chifra abis`, this tool is useful from the command line but is primarily used in support of
other tools, especially `chifra export` where naming addresses becomes the single best way to
turn unintelligible blockchain data into understandable information.

The various options allow you to search and filter the results. The `tags` option is used primarily
by the TrueBlocks explorer.

You may use the TrueBlocks explorer to manage (add, edit, delete) address-name associations.

```[plaintext]
Purpose:
  Query addresses or names of well known accounts.

Usage:
  chifra names [flags] <term> [term...]

Arguments:
  terms - a space separated list of one or more search terms (required)

Flags:
  -e, --expand            expand search to include all fields (search name, address, and symbol otherwise)
  -m, --match_case        do case-sensitive search
  -a, --all               include all (including custom) names in the search
  -c, --custom            include only custom named accounts in the search
  -p, --prefund           include prefund accounts in the search
  -s, --addr              display only addresses in the results (useful for scripting, assumes --no_header)
  -g, --tags              export the list of tags and subtags only
  -C, --clean             clean the data (addrs to lower case, sort by addr)
  -r, --regular           only available with --clean, cleans regular names database
  -d, --dry_run           only available with --clean or --autoname, outputs changes to stdout instead of updating databases
  -A, --autoname string   an address assumed to be a token, added automatically to names database if true
  -x, --fmt string        export format, one of [none|json*|txt|csv]
  -v, --verbose           enable verbose output
  -h, --help              display this help screen

Notes:
  - The tool will accept up to three terms, each of which must match against any field in the database.
  - The --match_case option enables case sensitive matching.
```

Data models produced by this tool:

- [name](/data-model/accounts/#name)

Links:

- [api docs](/api/#operation/accounts-names)
- [source code](https://github.com/TrueBlocks/trueblocks-core/tree/master/src/apps/chifra/internal/names)


## chifra serve

`chifra serve` delivers a JSON API for each of the `chifra` commands along with each of its options.
It does this through `flame` server, which is written in Go.

Another way to get help to run `chifra --help` or `chifra <cmd> --help` on your command line. See below for an example of converting command line options to a call to the API. There's a one-to-one correspondence between the command line tools and options and the API routes and their options.

```[plaintext]
Purpose:
  Serve the TrueBlocks API using the flame server.

Usage:
  chifra serve [flags]

Flags:
  -p, --port string   specify the server's port (:8080 default) (default ":8080")

Global Flags:
  -x, --fmt string   export format, one of [none|json*|txt|csv|api]
  -h, --help         display this help screen
  -v, --verbose      enable verbose (increase detail with --log_level)

Notes:
  - To start API open terminal window and run chifra serve.
  - See the API documentation for more information.
```

**Source code**: [`serve.go`](https://github.com/TrueBlocks/trueblocks-core/tree/develop/src/apps/chifra/cmd/serve.go)


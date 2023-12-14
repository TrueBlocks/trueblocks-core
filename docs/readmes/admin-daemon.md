<!-- markdownlint-disable MD041 -->
## chifra daemon

`chifra daemon` manages chifra's API server. Each of the `chifra` commands along with all of its options,
are provided not only by the command line, but also the API server. We call this process the
`flame` server, which is written in Go. `chifra serve` is an alias for the `chifra daemon` command.

In the future, this daemon may also manage other long-running processes such as `chifra scrape`
and `chifra monitors`, but for now, it's only managing the API server.

The `--grpc` option turns on a GRPC server that may speed up certain command such as `chifra names`,
although this option is experimental and therefore not recommended for production use.

If the default port for the API server is in use, you may change it with the `--port` option.

To get help for any command, please see the API documentation on our website. But, you may
also run `chifra --help` or `chifra <cmd> --help` on your command line to get help.

See below for an example of converting command line options to a call to the API. There's a
one-to-one correspondence between the command line tools and options and the API routes and
their options.

```[plaintext]
Purpose:
  Initialize and control long-running processes such as the API and the scrapers.

Usage:
  chifra daemon [flags]

Aliases:
  daemon, serve

Flags:
  -u, --url string   specify the API server's url and optionally its port (default "localhost:8080")
  -g, --grpc         run gRPC server to serve names
  -x, --fmt string   export format, one of [none|json*|txt|csv]
  -v, --verbose      enable verbose output
  -h, --help         display this help screen

Notes:
  - To start API open terminal window and run chifra daemon.
  - See the API documentation (https://trueblocks.io/api) for more information.
```

Data models produced by this tool:

- none

Links:

- no api for this command
- [source code](https://github.com/TrueBlocks/trueblocks-core/tree/master/src/apps/chifra/internal/daemon)

### notes

To convert the options for a command line tool to an API call, do the following:

1. Any `--snake_case` argument to the command line should be converted to `camelCase`. For example, `--no_header` on the command line should be sent as `&noHeader` to the API server.
2. Any `switch` on the command line, (i.e., options whose presence indicates `true` and whose absence indicates `false`) should be sent as a `boolean` to the API server. For example, `--no_header` on the command line should be sent as `&noHeader=true` to the API server. If the option is `fales`, you do not need to send it to the API server.
3. Positionals such as the addresses, topics, and four-bytes for `chifra export`, must be prepended with their positional name. For example, `chifra export <address> <topic>` should be sent as `&addrs=<address>&topics=<topic>` to the API server. For some commands (experiment) you may send more than one value for a positional with `%20` seperating the entries or by sending multiple positionals (i.e., `&addrs=<address1>&addrs=<address2>`).


<!-- markdownlint-disable MD041 -->
## chifra daemon

`chifra daemon` manages chifra's long-running processes include its JSON API server. Each of the `chifra` commands along with all of its options, are provided not only by the command line, but also the API server. We call this process the `flame` server, which is written in Go.

In the future, this daemon may also manage other long-running processes.

Another way to get help to run `chifra --help` or `chifra <cmd> --help` on your command line. See below for an example of converting command line options to a call to the API. There's a one-to-one correspondence between the command line tools and options and the API routes and their options.

```[plaintext]
Purpose:
  Initalize and control long-running processes such as the API and the scrapers.

Usage:
  chifra daemon [flags]

Flags:
  -p, --port string     specify the server's port (default ":8080")
  -s, --scrape string   start the scraper, initialize it with either just blooms or entire index, generate for new blocks
                        One of [ off | blooms | full-index ]
  -m, --monitor         instruct the node to start the monitors tool
  -a, --api string      instruct the node to start the API server
                        One of [ off | on ] (default "on")
  -x, --fmt string      export format, one of [none|json*|txt|csv]
  -v, --verbose         enable verbose (increase detail with --log_level)
  -h, --help            display this help screen

Notes:
  - To start API open terminal window and run chifra daemon.
  - See the API documentation (https://trueblocks.io/api) for more information.
```

**Source code**: [`internal/daemon`](https://github.com/TrueBlocks/trueblocks-core/tree/master/src/apps/chifra/internal/daemon)


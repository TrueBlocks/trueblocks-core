## chifra node

`chifra node` controls long running processes such as the scrapers, monitors, and data apis. Generally, it's used only by the docker version.

```[plaintext]
Purpose:
  Initalize and control long-running tools such as the API, scrapers, and monitors.

Usage:
  chifra node [flags]

Flags:
  -s, --scrape string   start the scraper, initialize it with either just blooms or entire index, generate for new blocks
                        One of [ off | blooms | full-index ]
  -m, --monitors        instruct the node to start the monitors tool
  -a, --api string      instruct the node to start the API server
                        One of [ off | on ] (default "on")
  -x, --fmt string      export format, one of [none|json*|txt|csv]
  -v, --verbose         enable verbose (increase detail with --log_level)
  -h, --help            display this help screen
```

**Source code**: [`internal/node`](https://github.com/TrueBlocks/trueblocks-core/tree/master/src/apps/chifra/internal/node)


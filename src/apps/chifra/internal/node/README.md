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
  -m, --monitor         instruct the node to start the monitors tool
  -a, --api string      instruct the node to start the API server
                        One of [ off | on ] (default "on")
  -x, --fmt string      export format, one of [none|json*|txt|csv]
  -v, --verbose         enable verbose (increase detail with --log_level)
  -h, --help            display this help screen
```

#### Other Options

All tools accept the following additional flags, although in some cases, they have no meaning.

```[plaintext]
  -v, --version         display the current version of the tool
      --wei             export values in wei (the default)
      --ether           export values in ether
      --raw             pass raw RPC data directly from the node with no processing
      --to_file         write the results to a temporary file and return the filename
      --output string   write the results to file 'fn' and return the filename
      --append          for --output command only append to instead of replace contents of file
      --file string     specify multiple sets of command line options in a file
```

*For the `--file string` option, you may place a series of valid command lines in a file using any valid flags. In some cases, this may significantly improve performance. A semi-colon at the start of any line makes it a comment.*

## chifra serve

`chifra serve` delivers a JSON API for each of the `chifra` commands along with each of its options. It does this through `flame` server which is written in Go.

[Get help with the API here](https://www.tokenomics.io/api.html
).

Another way to get help to run `chifra --help` or `chifra <cmd> --help` on your command line. See below for an example of converting command line options to a call to the API. There's a one-to-one correspondence between the command line tools and options and the API routes and their options.

### usage

`Usage:`    chifra serve  
`Purpose:`  Present each chifra command along with all of its options as a JSON api.

`Where:`

|          | Option                | Description                                      |
| -------- | --------------------- | ------------------------------------------------ |
|          | &#8208;&#8208;port    | specify the server's port (default ":8080")      |
| &#8208;v | &#8208;&#8208;verbose | set verbose level (optional level defaults to 1) |
| &#8208;h | &#8208;&#8208;help    | display this help screen                         |

`Notes:`

- To use the API, first open a new terminal window and run `chifra serve`
- Instead of the command line `chifra status index --details`, run `curl "http://localhost:8080/status?modes=index&details"`.
- Treat other commands similarly.

**Source code**: [`go-apps/flame`](https://github.com/TrueBlocks/trueblocks-core/tree/master/src/go-apps/flame)

## chifra serve

`chifra serve` delivers all of the `chifra` commands, along with all of their options, as a JSON api using the GoLang program we call `flame`.

The easiest way to get help on the API is to run the `chifra` command help to see all the routes and specific help for a given tool to see all the options. See below for an example of converting a command line to an API url.

### usage

`Usage:`    chifra serve  
`Purpose:`  Present each chifra command along with all of its options as a JSON api.

`Where:`  

| Short Cut | Option    | Description                                 |
| --------: | :-------- | :------------------------------------------ |
|           | --port    | specify the server's port (default ":8080") |
|        -v | --verbose | verbose level (between 0 and 10 inclusive)  |
|        -h | --help    | display this help screen                    |

`Notes:`

- To use the API, first open a new terminal window and run `chifra serve`
- Instead of the command line `chifra status index --details`, run `curl "http://localhost:8080/status?modes=index&details"`.
- Treat other commands similarly.

**Source code**: [`go-apps/flame`](https://github.com/TrueBlocks/trueblocks-core/tree/master/src/go-apps/flame)

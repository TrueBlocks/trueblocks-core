### notes

To convert the options for a command line tool to an API call, do the following:

1. Any `--snake_case` argument to the command line should be converted to `camelCase`. For example, `--no_header` on the command line should be sent as `&noHeader` to the API server.
2. Any `switch` on the command line, (i.e., options whose presence indicates `true` and whose absence indicates `false`) should be sent as a `boolean` to the API server. For example, `--no_header` on the command line should be sent as `&noHeader=true` to the API server. If the option is `fales`, you do not need to send it to the API server.
3. Positionals such as the addresses, topics, and four-bytes for `chifra export`, must be prepended with their positional name. For example, `chifra export <address> <topic>` should be sent as `&addrs=<address>&topics=<topic>` to the API server. For some commands (experiment) you may send more than one value for a positional with `%20` separating the entries or by sending multiple positionals (i.e., `&addrs=<address1>&addrs=<address2>`).

<hr />
<span style="size: -2; background-color: #febfc1; color: black; display: block; padding: 4px">
Chifra was built for the command line, a fact we purposefully take advantage of to ensure continued operation on small machines. As such, this tool is not intended to serve multiple end users in a cloud-based server environment. This is by design. Be forewarned.
</span>
<hr />

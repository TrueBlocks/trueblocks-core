## Example: forEveryIndex

This example show how to traverse every index chunk in the index. It requires you to have the index (see `chifra init --all`).

### Building

See the instructions in the file [./src/examples/README.md](../README.md) for more information.

### What this example shows

This example shows how to scan all the bloom filters and index chunks. It also reads the addresses in the full index chunk
and then tests each address for inclusion in the bloom filter. It then alters the address and tests for inclusion again as
a way to test the false positive rate of the bloom filter.
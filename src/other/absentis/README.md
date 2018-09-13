# Absentis
A Project for Quickblocks bounty that finds missing transactions



An Example Configuration file:

```toml

[[nodes]]
identifier = 'ParityLocal'
transport = 'Http'

[nodes.http]
url = 'http://localhost'
port = 8545

[nodes.ipc]
path = '/home/me/parity/ipc'

[[nodes]]
identifier = 'GethRemoteNode'
transport = 'Http'

[nodes.http]
url = 'http://32.0.1.32'
port = 8545

```


Example Command w/o config file: 


`./target/release/absentis -V txs2.csv --to 6000000 --address fb6916095ca1df60bb79ce92ce3ea74c37c5d359 -n http://localhost:8545 --transport http`

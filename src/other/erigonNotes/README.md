## Running Sepolia Testnet with Erigon

Run this command in one terminal window:

```[bash]
./build/bin/erigon \
    --datadir /mnt/md0/erigon-sepolia \
    --private.api.addr=localhost:9093 \
    --port 30305 \
    --chain sepolia \
    --http.port 8548 \
    --http.api=eth,debug,net,trace,web3,erigon
```

Run this command in a different terminal window:

```
./build/bin/rpcdaemon \
    --datadir /mnt/md0/erigon-sepolia \
    --private.api.addr=localhost:9093 \
    --http.port 8548 \
    --http.api=eth,debug,net,trace,web3,erigon
```

Make an entry in ~/.local/share/trueblocks/trueBlocks.toml, thus:

```
[chains]

*... leave other entries as they are *

[chains.sepolia]
chainId = "11155111"
rpcProvider = "http://localhost:8548"
```

and in ~/.local/share/trueblocks/config/sepolia/blockScrape.toml, add this (create the file if it doesn't exist):

```
[settings]
block_cnt = 200
allow_missing = true
first_snap = 0
apps_per_chunk = 1500
```
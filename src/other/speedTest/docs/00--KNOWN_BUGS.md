Various differences returned by 'geth' vs. 'parity' RPC

I ran RPC requests against both 'geth 1.5.2-stable' and 'parity 1.4.4-beta' for about 1,000 randomly selected blocks.  I found the following differences in the returned JSON data between the two:

**1) 'geth' returns `nonce` field from `eth_getBlockByNumber(x,true)`, Parity does not**

According to the documentation (https://github.com/ethereum/wiki/blob/master/JSON-RPC.md#eth_getblockbyhash), `nonce` should be returned by getBlockNumber.

**2) 'geth' returns the fields `from`, `to`, and `root` from `eth_getTransactionReceipt(addr)`, Parity does not**

This time, according to the documentation (https://github.com/ethereum/wiki/blob/master/JSON-RPC.md#eth_gettransactionreceipt), 'geth' is wrong. These fields are not
part of the receipt.

**2) Both clients return 'r', 's', and 'v' fields in the transaction from eth_getNumberByBlock(x,true) call**

According to the documentation (https://github.com/ethereum/wiki/blob/master/JSON-RPC.md#eth_gettransactionbyhash), these fields are not part of the transaction data and are therefore extraneous. This may be a bug in the documentation. I'm not sure. Please advise.

**4) 'geth' removes leading zeros from a transaction's `r` and `s` fields, Parity does not.**

Parity inserts zeros after the '0x' to pad the length of the value for these fields to 32 bytes. Since these fields are not documented, I don't know if they should be considered `QUANTITY` or `DATA`. If `DATA`, then Parity is right, if `QUANTITY` then 'geth' is right. I suspect Parity is right.

**6) 'geth' and Parity disagree on the value of the transaction's `v` field.**

For every transaction returned by `eth_getBlockByNumber`, 'geth' returns values of either `0x1b` or `0x1c`. Parity returns either `0` or `1` respectively. Since it's not documented, I don't know the correct value, but they do differ.

**7) Parity numbers the `logIndex` field of the receipts `logs` per transaction. 'geth' numbers the logs per block. The documents say it should be per block, but this may change. Both Parity and 'geth' start their numbering from '0' which may conflict with `null` value in C++ code.**

This is either a bug in Parity (https://github.com/ethcore/parity/issues/3537) or the documentation. There is an issue reported in `geth` discussing this as a bug in the docs (https://github.com/ethereum/go-ethereum/issues/2028).

/* eslint object-curly-newline: ["error", "never"] */
/* eslint max-len: ["error", 160] */
/*
 * This file was generated with makeClass --sdk. Do not edit it.
 */
import { address, blknum, bytes, datetime, Function, gas, hash, timestamp, uint64, wei } from '.';

export type Slurp = {
  hash: hash
  blockHash: hash
  blockNumber: blknum
  transactionIndex: blknum
  nonce: uint64
  timestamp: timestamp
  date: datetime
  from: address
  to: address
  value: wei
  gas: gas
  gasPrice: gas
  input: bytes
  hasToken: boolean
  articulatedTx: Function
  compressedTx: string
  isError: boolean
  functionName: string
  methodId: string
  gasUsed: gas
  contractAddress: address
  cumulativeGasUsed: string
  txReceiptStatus: string
  ether: string
}

/* eslint object-curly-newline: ["error", "never"] */
/* eslint max-len: ["error", 160] */
/*
 * This file was generated with makeClass --sdk. Do not edit it.
 */
import { address, blknum, bytes, datetime, Function, gas, hash, Name, Receipt, Reconciliation, timestamp, Trace, uint64, wei } from '.';

export type Transaction = {
  chainId: string
  type: string
  hash: hash
  blockHash: hash
  blockNumber: blknum
  transactionIndex: blknum
  nonce: uint64
  timestamp: timestamp
  from: address
  to: address
  value: wei
  extraValue1: wei
  extraValue2: wei
  gas: gas
  gasPrice: gas
  maxFeePerGas: gas
  maxPriorityFeePerGas: gas
  input: bytes
  isError: boolean
  hasToken: boolean
  cachebits: boolean
  reserved2: boolean
  receipt: Receipt
  traces: Trace[]
  articulatedTx: Function
  compressedTx: string
  statements: Reconciliation[]
  finalized?: boolean
  date: datetime
  fromName?: Name
  toName?: Name
  extraData: string
  ether: string
  etherGasPrice: gas
  gasUsed: gas
  encoding: string
}

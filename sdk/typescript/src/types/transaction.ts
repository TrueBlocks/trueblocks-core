/* eslint object-curly-newline: ["error", "never"] */
/* eslint max-len: ["error", 160] */
/*
 * This file was generated with makeClass --sdk. Do not edit it.
 */
import { address, blknum, bytes, datetime, Function, gas, hash, Receipt, Statement, timestamp, Trace, uint64, wei } from '.';

export type Transaction = {
  chainId: string
  blockNumber: blknum
  transactionIndex: blknum
  timestamp: timestamp
  date: datetime
  hash: hash
  blockHash: hash
  from: address
  to: address
  nonce: uint64
  value: wei
  gas: gas
  gasPrice: gas
  maxFeePerGas: gas
  maxPriorityFeePerGas: gas
  input: bytes
  isError: boolean
  hasToken: boolean
  receipt: Receipt
  traces: Trace[]
  articulatedTx: Function
  compressedTx: string
  statements: Statement[]
  gasUsed: gas
  type: string
}

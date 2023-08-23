/* eslint object-curly-newline: ["error", "never"] */
/* eslint max-len: ["error", 160] */
/*
 * This file was generated with makeClass --sdk. Do not edit it.
 */
import { address, blknum, datetime, gas, hash, timestamp, Transaction, uint64, wei } from '.';

export type Block = {
  author: address
  gasLimit: gas
  gasUsed: gas
  hash: hash
  blockNumber: blknum
  parentHash: hash
  receiptsRoot: hash
  sha3Uncles: hash
  size: uint64
  stateRoot: hash
  totalDifficulty: wei
  miner: address
  difficulty: uint64
  extraData: string
  logsBloom: string
  mixHash: string
  nonce: string
  timestamp: timestamp
  date: datetime
  baseFeePerGas: wei
  transactions: Transaction[]
  transactionsRoot: hash
  uncles?: hash[]
}

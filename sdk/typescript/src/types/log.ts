/* eslint object-curly-newline: ["error", "never"] */
/* eslint max-len: ["error", 160] */
/*
 * This file was generated with makeClass --sdk. Do not edit it.
 */
import { address, blknum, bytes, datetime, Function, hash, timestamp, topic, uint64 } from '.';

export type Log = {
  address: address
  blockHash: hash
  blockNumber: blknum
  logIndex: uint64
  topics?: topic[]
  data?: bytes
  articulatedLog?: Function
  compressedLog?: string
  transactionHash: hash
  transactionIndex: uint64
  timestamp?: timestamp
  type: string
  date?: datetime
}

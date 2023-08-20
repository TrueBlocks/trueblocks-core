/* eslint object-curly-newline: ["error", "never"] */
/* eslint max-len: ["error", 160] */
/*
 * This file was generated with makeClass --sdk. Do not edit it.
 */
import { address, blknum, bytes, datetime, Function, hash, timestamp, topic, uint64 } from '.';

export type Log = {
  blockNumber: blknum
  transactionIndex: uint64
  logIndex: uint64
  timestamp?: timestamp
  date: datetime
  address: address
  topics?: topic[]
  data?: bytes
  transactionHash: hash
  blockHash: hash
  articulatedLog?: Function
  compressedLog?: string
}

/* eslint object-curly-newline: ["error", "never"] */
/* eslint max-len: ["error", 160] */
/*
 * This file was generated with makeClass --sdk. Do not edit it.
 */
import { blknum, hash, timestamp, uint64 } from '.';

export type TraceCount = {
  blockNumber: blknum
  transactionIndex: blknum
  transactionHash: hash
  timestamp: timestamp
  tracesCnt: uint64
}

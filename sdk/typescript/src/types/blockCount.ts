/* eslint object-curly-newline: ["error", "never"] */
/* eslint max-len: ["error", 160] */
/*
 * This file was generated with makeClass --sdk. Do not edit it.
 */
import { blknum, timestamp, uint64 } from '.';

export type BlockCount = {
  blockNumber: blknum
  timestamp: timestamp
  transactionsCnt: uint64
  unclesCnt?: uint64
  logsCnt?: uint64
  tracesCnt?: uint64
  appsCnt?: uint64
  uniqsCnt?: uint64
}

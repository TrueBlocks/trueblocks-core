/* eslint object-curly-newline: ["error", "never"] */
/* eslint max-len: ["error", 160] */
/*
 * This file was generated with makeClass --sdk. Do not edit it.
 */
import { blknum, datetime, timestamp, uint64 } from '.';

export type BlockCount = {
  blockNumber: blknum
  timestamp: timestamp
  date: datetime
  transactionsCnt: uint64
  unclesCnt?: uint64
  logsCnt?: uint64
  tracesCnt?: uint64
  addressCnt?: uint64
}

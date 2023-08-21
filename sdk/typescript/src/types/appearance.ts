/* eslint object-curly-newline: ["error", "never"] */
/* eslint max-len: ["error", 160] */
/*
 * This file was generated with makeClass --sdk. Do not edit it.
 */
import { address, blknum, datetime, timestamp } from '.';

export type Appearance = {
  address: address
  blockNumber: blknum
  transactionIndex: blknum
  traceIndex: blknum
  reason: string
  name: string
  timestamp: timestamp
  date: datetime
}

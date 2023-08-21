/* eslint object-curly-newline: ["error", "never"] */
/* eslint max-len: ["error", 160] */
/*
 * This file was generated with makeClass --sdk. Do not edit it.
 */
import { blknum, datetime, timestamp } from '.';

export type NamedBlock = {
  blockNumber: blknum
  timestamp: timestamp
  date: datetime
  name?: string
}

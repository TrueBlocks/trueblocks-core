/* eslint object-curly-newline: ["error", "never"] */
/* eslint max-len: ["error", 160] */
/*
 * This file was generated with makeClass --sdk. Do not edit it.
 */
import { address, blknum, datetime, timestamp, uint64, wei } from '.';

export type Withdrawal = {
  address: address
  amount: wei
  blockNumber: blknum
  index: uint64
  timestamp: timestamp
  date: datetime
  validatorIndex: uint64
}

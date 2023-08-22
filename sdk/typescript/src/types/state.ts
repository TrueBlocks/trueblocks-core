/* eslint object-curly-newline: ["error", "never"] */
/* eslint max-len: ["error", 160] */
/*
 * This file was generated with makeClass --sdk. Do not edit it.
 */
import { address, blknum, datetime, timestamp, uint64, wei } from '.';

export type State = {
  blockNumber: blknum
  timestamp: timestamp
  date: datetime
  address: address
  accountType: string
  balance: wei
  code: string
  deployed: blknum
  nonce: uint64
  proxy: address
}

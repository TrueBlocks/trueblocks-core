/* eslint object-curly-newline: ["error", "never"] */
/* eslint max-len: ["error", 160] */
/*
 * This file was generated with makeClass --sdk. Do not edit it.
 */
import { address, blknum, datetime, Function, timestamp } from '.';

export type Result = {
  blockNumber: blknum
  timestamp: timestamp
  date: datetime
  address: address
  name: string
  encoding: string
  signature: string
  encodedArguments: string
  articulatedOut: Function
}

/* eslint object-curly-newline: ["error", "never"] */
/* eslint max-len: ["error", 160] */
/*
 * This file was generated with makeClass --sdk. Do not edit it.
 */
import { address, blknum, hash, topic } from '.';

export type LogFilter = {
  fromBlock: blknum
  toBlock: blknum
  blockHash: hash
  emitters: address[]
  topics: topic[]
}

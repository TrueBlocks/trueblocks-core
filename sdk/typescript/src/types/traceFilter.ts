/* eslint object-curly-newline: ["error", "never"] */
/* eslint max-len: ["error", 160] */
/*
 * This file was generated with makeClass --sdk. Do not edit it.
 */
import { address, uint64 } from '.';

export type TraceFilter = {
  fromBlock?: string
  toBlock?: string
  fromAddress?: address[]
  toAddress?: address[]
  after?: uint64
  count?: uint64
}

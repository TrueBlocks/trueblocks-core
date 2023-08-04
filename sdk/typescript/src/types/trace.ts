/* eslint object-curly-newline: ["error", "never"] */
/* eslint max-len: ["error", 160] */
/*
 * This file was generated with makeClass --sdk. Do not edit it.
 */
import { blknum, Function, hash, timestamp, TraceAction, TraceResult, uint64 } from '.';

export type Trace = {
  blockHash: hash
  blockNumber: blknum
  subtraces: uint64
  traceAddress: string[]
  transactionHash: hash
  transactionIndex: blknum
  type?: string
  error?: string
  action: TraceAction
  result: TraceResult
  articulatedTrace?: Function
  compressedTrace?: string
  timestamp: timestamp
}

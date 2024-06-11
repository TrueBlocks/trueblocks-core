/* eslint object-curly-newline: ["error", "never"] */
/* eslint max-len: ["error", 160] */
/*
 * Copyright 2016, 2024 The TrueBlocks Authors. All rights reserved.
 * Use of this source code is governed by a license that can
 * be found in the LICENSE file.
 *
 * This file was auto generated. DO NOT EDIT.
 */

import { address, blknum, gas, hash, Log, txnum, uint64 } from '.';

export type Receipt = {
  blockHash?: hash
  blockNumber: blknum
  contractAddress?: address
  cumulativeGasUsed?: gas
  effectiveGasPrice?: gas
  from?: address
  gasUsed: gas
  isError?: boolean
  logs: Log[]
  logsBloom?: string
  status: uint64
  to?: address
  transactionHash: hash
  transactionIndex: txnum
}

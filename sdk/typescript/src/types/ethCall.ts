/* eslint object-curly-newline: ["error", "never"] */
/* eslint max-len: ["error", 160] */
/*
 * Copyright 2016, 2024 The TrueBlocks Authors. All rights reserved.
 * Use of this source code is governed by a license that can
 * be found in the LICENSE file.
 *
 * This file was auto generated. DO NOT EDIT.
 */

import { address, blknum } from '.';

export type EthCall = {
  blockNumber: blknum
  address: address
  signature: string
  encoding: string
  bytes: string
  compressedResult: string
  deployed: blknum
}

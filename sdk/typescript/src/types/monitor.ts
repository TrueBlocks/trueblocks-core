/* eslint object-curly-newline: ["error", "never"] */
/* eslint max-len: ["error", 160] */
/*
 * Copyright 2016, 2024 The TrueBlocks Authors. All rights reserved.
 * Use of this source code is governed by a license that can
 * be found in the LICENSE file.
 *
 * This file was auto generated. DO NOT EDIT.
 */

import { address, int64, uint64 } from '.';

export type Monitor = {
  address: address
  deleted: boolean
  fileSize: int64
  isEmpty: boolean
  isStaged: boolean
  lastScanned: uint64
  nRecords: int64
  name: string
}

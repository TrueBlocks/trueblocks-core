/* eslint object-curly-newline: ["error", "never"] */
/* eslint max-len: ["error", 160] */
/*
 * Copyright 2016, 2024 The TrueBlocks Authors. All rights reserved.
 * Use of this source code is governed by a license that can
 * be found in the LICENSE file.
 *
 * This file was auto generated. DO NOT EDIT.
 */

import { address, blkrange, RangeDates, uint64 } from '.';

export type ChunkAddress = {
  address: address
  count: uint64
  offset: uint64
  range: blkrange
  rangeDates: RangeDates
}

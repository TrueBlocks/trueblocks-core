/* eslint object-curly-newline: ["error", "never"] */
/* eslint max-len: ["error", 160] */
/*
 * Copyright 2016, 2024 The TrueBlocks Authors. All rights reserved.
 * Use of this source code is governed by a license that can
 * be found in the LICENSE file.
 *
 * This file was auto generated. DO NOT EDIT.
 */

import { blkrange, float64, RangeDates, uint64 } from '.';

export type ChunkStats = {
  addrsPerBlock: float64
  appsPerAddr: float64
  appsPerBlock: float64
  bloomSz: uint64
  chunkSz: uint64
  nAddrs: uint64
  nApps: uint64
  nBlocks: uint64
  nBlooms: uint64
  range: blkrange
  rangeDates: RangeDates
  ratio: float64
  recWid: uint64
}

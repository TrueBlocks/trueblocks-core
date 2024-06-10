/* eslint object-curly-newline: ["error", "never"] */
/* eslint max-len: ["error", 160] */
// Copyright 2016, 2024 The TrueBlocks Authors. All rights reserved.
// Use of this source code is governed by a license that can
// be found in the LICENSE file.
/*
 * This file was auto generated. DO NOT EDIT.
 */
import { blkrange, hash, uint64 } from '.';

export type ChunkIndex = {
  range: blkrange
  magic: string
  hash: hash
  nAddresses: uint64
  nAppearances: uint64
  size: uint64
}


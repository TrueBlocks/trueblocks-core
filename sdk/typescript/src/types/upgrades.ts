/* eslint object-curly-newline: ["error", "never"] */
/* eslint max-len: ["error", 160] */
/*
 * Copyright 2016, 2024 The TrueBlocks Authors. All rights reserved.
 * Use of this source code is governed by a license that can
 * be found in the LICENSE file.
 *
 * This file was auto generated. DO NOT EDIT.
 */

import { ipfshash, uint64 } from '.';

export type CacheRecordUp = {
  range: string
  bloomHash:   ipfshash
  indexHash:   ipfshash
  bloomSize:   uint64
  indexSize:   uint64
  firstApp:    uint64
  latestApp:   uint64
  filename:    string
  nApps:       uint64
  nAppearance: uint64
}

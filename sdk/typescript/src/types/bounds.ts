/* eslint object-curly-newline: ["error", "never"] */
/* eslint max-len: ["error", 160] */
/*
 * Copyright 2016, 2024 The TrueBlocks Authors. All rights reserved.
 * Use of this source code is governed by a license that can
 * be found in the LICENSE file.
 *
 * This file was auto generated. DO NOT EDIT.
 */

import { Appearance, datetime, timestamp, uint64 } from '.';

export type Bounds = {
  count: uint64
  firstApp: Appearance
  firstDate?: datetime
  firstTs: timestamp
  latestApp: Appearance
  latestDate?: datetime
  latestTs: timestamp
}

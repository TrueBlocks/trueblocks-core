/* eslint object-curly-newline: ["error", "never"] */
/* eslint max-len: ["error", 160] */
// Copyright 2016, 2024 The TrueBlocks Authors. All rights reserved.
// Use of this source code is governed by a license that can
// be found in the LICENSE file.
/*
 * This file was auto generated. DO NOT EDIT.
 */
import { datetime, Appearance, timestamp, uint64 } from '.';

export type Bounds = {
  count: uint64
  firstApp: Appearance
  firstTs: timestamp
  firstDate: datetime
  latestApp: Appearance
  latestTs: timestamp
  latestDate: datetime
}

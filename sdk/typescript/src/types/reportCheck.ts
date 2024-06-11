/* eslint object-curly-newline: ["error", "never"] */
/* eslint max-len: ["error", 160] */
/*
 * Copyright 2016, 2024 The TrueBlocks Authors. All rights reserved.
 * Use of this source code is governed by a license that can
 * be found in the LICENSE file.
 *
 * This file was auto generated. DO NOT EDIT.
 */

import { uint64 } from '.';

export type ReportCheck = {
  checkedCnt: uint64
  failedCnt: uint64
  msgStrings: string[]
  passedCnt: uint64
  reason: string
  result: string
  skippedCnt: uint64
  visitedCnt: uint64
}

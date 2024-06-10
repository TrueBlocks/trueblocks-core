/* eslint object-curly-newline: ["error", "never"] */
/* eslint max-len: ["error", 160] */
// Copyright 2016, 2024 The TrueBlocks Authors. All rights reserved.
// Use of this source code is governed by a license that can
// be found in the LICENSE file.
/*
 * This file was auto generated. DO NOT EDIT.
 */
import { uint32 } from '.';

export type ReportCheck = {
  result: string
  visitedCnt: uint32
  checkedCnt: uint32
  skippedCnt: uint32
  passedCnt: uint32
  failedCnt: uint32
  msgStrings: string[]
  reason: string
}


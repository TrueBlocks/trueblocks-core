/* eslint object-curly-newline: ["error", "never"] */
/* eslint max-len: ["error", 160] */
/*
 * This file was generated with makeClass --sdk. Do not edit it.
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

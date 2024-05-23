/* eslint object-curly-newline: ["error", "never"] */
/* eslint max-len: ["error", 160] */
/*
 * This file was generated with makeClass --sdk. Do not edit it.
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

/* eslint object-curly-newline: ["error", "never"] */
/* eslint max-len: ["error", 160] */
/*
 * Copyright 2016, 2024 The TrueBlocks Authors. All rights reserved.
 * Use of this source code is governed by a license that can
 * be found in the LICENSE file.
 *
 * This file was auto generated. DO NOT EDIT.
 */

import { int64, ipfshash } from '.';

export type IpfsPin = {
  cid: ipfshash
  datePinned: string
  fileName: string
  size: int64
  status: string
}

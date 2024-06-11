/* eslint object-curly-newline: ["error", "never"] */
/* eslint max-len: ["error", 160] */
/*
 * Copyright 2016, 2024 The TrueBlocks Authors. All rights reserved.
 * Use of this source code is governed by a license that can
 * be found in the LICENSE file.
 *
 * This file was auto generated. DO NOT EDIT.
 */

import { address, uint64 } from '.';

export type Name = {
  address: address
  decimals: uint64
  deleted?: boolean
  isContract?: boolean
  isCustom?: boolean
  isErc20?: boolean
  isErc721?: boolean
  isPrefund?: boolean
  name: string
  source: string
  symbol: string
  tags: string
}

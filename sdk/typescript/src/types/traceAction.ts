/* eslint object-curly-newline: ["error", "never"] */
/* eslint max-len: ["error", 160] */
/*
 * Copyright 2016, 2024 The TrueBlocks Authors. All rights reserved.
 * Use of this source code is governed by a license that can
 * be found in the LICENSE file.
 *
 * This file was auto generated. DO NOT EDIT.
 */

import { address, bytes, float64, gas, wei } from '.';

export type TraceAction = {
  address?: address
  author?: address
  balance?: wei
  balanceEth?: float64
  callType: string
  ether?: float64
  from: address
  gas: gas
  init?: string
  input?: bytes
  refundAddress?: address
  rewardType?: string
  selfDestructed?: address
  to: address
  value: wei
}

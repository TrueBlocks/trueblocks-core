/* eslint object-curly-newline: ["error", "never"] */
/* eslint max-len: ["error", 160] */
// Copyright 2016, 2024 The TrueBlocks Authors. All rights reserved.
// Use of this source code is governed by a license that can
// be found in the LICENSE file.
/*
 * This file was auto generated. DO NOT EDIT.
 */
import { address, bytes, gas, wei } from '.';

export type TraceAction = {
  selfDestructed?: address
  balance?: wei
  callType: string
  from: address
  gas: gas
  init?: string
  input?: bytes
  refundAddress?: address
  rewardType?: string
  to: address
  value: wei
  address?: address
  author?: address
}


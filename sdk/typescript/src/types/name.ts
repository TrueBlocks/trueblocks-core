/* eslint object-curly-newline: ["error", "never"] */
/* eslint max-len: ["error", 160] */
// Copyright 2016, 2024 The TrueBlocks Authors. All rights reserved.
// Use of this source code is governed by a license that can
// be found in the LICENSE file.
/*
 * This file was auto generated. DO NOT EDIT.
 */
import { address, uint64 } from ".";

export type Name = {
  tags: string;
  address: address;
  name: string;
  symbol: string;
  source: string;
  decimals: uint64;
  isCustom?: boolean;
  isPrefund?: boolean;
  isContract?: boolean;
  isErc20?: boolean;
  isErc721?: boolean;
  deleted?: boolean;
};

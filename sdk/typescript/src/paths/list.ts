/* eslint object-curly-newline: ["error", "never"] */
/* eslint max-len: ["error", 160] */
// Copyright 2016, 2024 The TrueBlocks Authors. All rights reserved.
// Use of this source code is governed by a license that can
// be found in the LICENSE file.
/*
 * This file was auto generated. DO NOT EDIT.
 */

import * as ApiCallers from '../lib/api_callers';
import { address, Appearance, AppearanceCount, blknum, Bounds, uint64 } from '../types';

export function getList(
  parameters?: {
    addrs: address[],
    count?: boolean,
    noZero?: boolean,
    bounds?: boolean,
    unripe?: boolean,
    silent?: boolean,
    firstRecord?: uint64,
    maxRecords?: uint64,
    reversed?: boolean,
    firstBlock?: blknum,
    lastBlock?: blknum,
    fmt?: string,
    chain: string,
    noHeader?: boolean,
  },
  options?: RequestInit,
) {
  return ApiCallers.fetch<AppearanceCount[] | Appearance[] | Bounds[]>(
    { endpoint: '/list', method: 'get', parameters, options },
  );
}

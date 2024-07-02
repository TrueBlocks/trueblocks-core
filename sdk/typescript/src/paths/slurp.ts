/* eslint object-curly-newline: ["error", "never"] */
/* eslint max-len: ["error", 160] */
/*
 * Copyright 2016, 2024 The TrueBlocks Authors. All rights reserved.
 * Use of this source code is governed by a license that can
 * be found in the LICENSE file.
 *
 * This file was auto generated. DO NOT EDIT.
 */

import * as ApiCallers from '../lib/api_callers';
import { address, Appearance, blknum, float64, Monitor, Slurp, uint64 } from '../types';

export function getSlurp(
  parameters?: {
    addrs: address[],
    blocks?: blknum[],
    parts?: string[],
    appearances?: boolean,
    articulate?: boolean,
    source?: 'etherscan' | 'key' | 'covalent' | 'alchemy',
    count?: boolean,
    page?: uint64,
    pageId?: string,
    perPage?: uint64,
    sleep?: float64,
    fmt?: string,
    chain: string,
    noHeader?: boolean,
    cache?: boolean,
    decache?: boolean,
    ether?: boolean,
  },
  options?: RequestInit,
) {
  return ApiCallers.fetch<Appearance[] | Monitor[] | Slurp[]>(
    { endpoint: '/slurp', method: 'get', parameters, options },
  );
}

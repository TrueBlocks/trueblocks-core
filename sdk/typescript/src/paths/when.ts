/* eslint object-curly-newline: ["error", "never"] */
/* eslint max-len: ["error", 160] */
// Copyright 2016, 2024 The TrueBlocks Authors. All rights reserved.
// Use of this source code is governed by a license that can
// be found in the LICENSE file.
/*
 * This file was auto generated. DO NOT EDIT.
 */
import * as ApiCallers from '../lib/api_callers';
import { Block, NamedBlock, Timestamp, TimestampCount } from '../types';

export function getWhen(
  parameters?: {
    blocks?: string[],
    list?: boolean,
    timestamps?: boolean,
    count?: boolean,
    repair?: boolean,
    check?: boolean,
    update?: boolean,
    deep?: boolean,
    fmt?: string,
    chain: string,
    noHeader?: boolean,
    cache?: boolean,
    decache?: boolean,
  },
  options?: RequestInit,
) {
  return ApiCallers.fetch<Block[] | NamedBlock[] | Timestamp[] | TimestampCount[]>(
    { endpoint: '/when', method: 'get', parameters, options },
  );
}

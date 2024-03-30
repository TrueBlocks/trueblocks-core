/* eslint object-curly-newline: ["error", "never"] */
/* eslint max-len: ["error", 160] */
// Copyright 2016, 2024 The TrueBlocks Authors. All rights reserved.
// Use of this source code is governed by a license that can
// be found in the LICENSE file.
/*
 * This file was auto generated. DO NOT EDIT.
 */
import * as ApiCallers from '../lib/api_callers';
import { address, Appearance, blknum, Block, BlockCount, Log, LogFilter, topic, Trace, TraceAction, TraceResult, uint64, Withdrawal } from '../types';

export function getBlocks(
  parameters?: {
    blocks: blknum[],
    hashes?: boolean,
    uncles?: boolean,
    traces?: boolean,
    uniq?: boolean,
    flow?: 'from' | 'to' | 'reward',
    logs?: boolean,
    emitter?: address[],
    topic?: topic[],
    withdrawals?: boolean,
    articulate?: boolean,
    bigRange?: uint64,
    count?: boolean,
    fmt?: string,
    chain: string,
    noHeader?: boolean,
    cache?: boolean,
    decache?: boolean,
    ether?: boolean,
    raw?: boolean,
  },
  options?: RequestInit,
) {
  return ApiCallers.fetch<Appearance[] | Block[] | BlockCount[] | Log[] | LogFilter[] | Trace[] | TraceAction[] | TraceResult[] | Withdrawal[]>(
    { endpoint: '/blocks', method: 'get', parameters, options },
  );
}

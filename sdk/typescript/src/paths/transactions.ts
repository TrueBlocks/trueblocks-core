/* eslint object-curly-newline: ["error", "never"] */
/* eslint max-len: ["error", 160] */
// Copyright 2016, 2024 The TrueBlocks Authors. All rights reserved.
// Use of this source code is governed by a license that can
// be found in the LICENSE file.
/*
 * This file was auto generated. DO NOT EDIT.
 */

import * as ApiCallers from '../lib/api_callers';
import { address, Appearance, Log, topic, Trace, Transaction, txId } from '../types';

export function getTransactions(
  parameters?: {
    transactions: txId[],
    articulate?: boolean,
    traces?: boolean,
    uniq?: boolean,
    flow?: 'from' | 'to',
    logs?: boolean,
    emitter?: address[],
    topic?: topic[],
    fmt?: string,
    chain: string,
    noHeader?: boolean,
    cache?: boolean,
    decache?: boolean,
    ether?: boolean,
  },
  options?: RequestInit,
) {
  return ApiCallers.fetch<Appearance[] | Log[] | Trace[] | Transaction[] | address[]>(
    { endpoint: '/transactions', method: 'get', parameters, options },
  );
}

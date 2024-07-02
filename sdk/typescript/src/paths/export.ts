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
import { address, Appearance, blknum, fourbyte, Log, Message, Monitor, Receipt, State, Statement, topic, Trace, Transaction, uint64, Withdrawal } from '../types';

export function getExport(
  parameters?: {
    addrs: address[],
    topics?: topic[],
    fourbytes?: fourbyte[],
    appearances?: boolean,
    receipts?: boolean,
    logs?: boolean,
    traces?: boolean,
    neighbors?: boolean,
    accounting?: boolean,
    statements?: boolean,
    balances?: boolean,
    withdrawals?: boolean,
    articulate?: boolean,
    cacheTraces?: boolean,
    count?: boolean,
    firstRecord?: uint64,
    maxRecords?: uint64,
    relevant?: boolean,
    emitter?: address[],
    topic?: topic[],
    reverted?: boolean,
    asset?: address[],
    flow?: 'in' | 'out' | 'zero',
    factory?: boolean,
    unripe?: boolean,
    reversed?: boolean,
    noZero?: boolean,
    firstBlock?: blknum,
    lastBlock?: blknum,
    fmt?: string,
    chain: string,
    noHeader?: boolean,
    cache?: boolean,
    decache?: boolean,
    ether?: boolean,
  },
  options?: RequestInit,
) {
  return ApiCallers.fetch<Appearance[] | Log[] | Message[] | Monitor[] | Receipt[] | State[] | Statement[] | Trace[] | Transaction[] | Withdrawal[]>(
    { endpoint: '/export', method: 'get', parameters, options },
  );
}

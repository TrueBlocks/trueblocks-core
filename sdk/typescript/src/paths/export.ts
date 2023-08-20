/* eslint object-curly-newline: ["error", "never"] */
/* eslint max-len: ["error", 160] */
/*
 * This file was generated with makeClass --sdk. Do not edit it.
 */
import * as ApiCallers from '../lib/api_callers';
import { address, Appearance, AppearanceCount, blknum, fourbyte, Function, Log, Monitor, Parameter, Receipt, Statement, Token, topic, Trace, TraceAction, TraceResult, Transaction, Transfer, uint64 } from '../types';

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
    articulate?: boolean,
    cacheTraces?: boolean,
    count?: boolean,
    firstRecord?: uint64,
    maxRecords?: uint64,
    relevant?: boolean,
    emitter?: address[],
    topic?: topic[],
    asset?: address[],
    flow?: 'in' | 'out' | 'zero',
    factory?: boolean,
    unripe?: boolean,
    reversed?: boolean,
    noZero?: boolean,
    firstBlock?: blknum,
    lastBlock?: blknum,
    chain: string,
    noHeader?: boolean,
    fmt?: string,
    verbose?: boolean,
    ether?: boolean,
    raw?: boolean,
    cache?: boolean,
  },
  options?: RequestInit,
) {
  return ApiCallers.fetch<Appearance[] | AppearanceCount[] | Function[] | Log[] | Monitor[] | Parameter[] | Receipt[] | Statement[] | Token[] | Trace[] | TraceAction[] | TraceResult[] | Transaction[] | Transfer[]>(
    { endpoint: '/export', method: 'get', parameters, options },
  );
}

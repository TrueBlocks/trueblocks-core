/* eslint object-curly-newline: ["error", "never"] */
/* eslint max-len: ["error", 160] */
/*
 * This file was generated with makeClass --sdk. Do not edit it.
 */
import * as ApiCallers from '../lib/api_callers';
import { address, topic, Transaction, txId } from '../types';

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
    raw?: boolean,
  },
  options?: RequestInit,
) {
  return ApiCallers.fetch<Transaction[]>(
    { endpoint: '/transactions', method: 'get', parameters, options },
  );
}

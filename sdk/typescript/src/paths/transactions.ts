/* eslint object-curly-newline: ["error", "never"] */
/* eslint max-len: ["error", 160] */
/*
 * This file was generated with makeClass --sdk. Do not edit it.
 */
import * as ApiCallers from '../lib/api_callers';
import { address, Statement, topic, Transaction, Transfer, txId } from '../types';

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
    accountFor?: address,
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
  return ApiCallers.fetch<Statement[] | Transaction[] | Transfer[]>(
    { endpoint: '/transactions', method: 'get', parameters, options },
  );
}

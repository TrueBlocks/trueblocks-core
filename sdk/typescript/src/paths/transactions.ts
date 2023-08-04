/* eslint object-curly-newline: ["error", "never"] */
/* eslint max-len: ["error", 160] */
/*
 * This file was generated with makeClass --sdk. Do not edit it.
 */
import * as ApiCallers from '../lib/api_callers';
import { address, Transaction, Transfer, txId } from '../types';

export function getTransactions(
  parameters?: {
    transactions: txId[],
    articulate?: boolean,
    traces?: boolean,
    uniq?: boolean,
    flow?: 'from' | 'to',
    accountFor?: address,
    cache?: boolean,
    chain: string,
    noHeader?: boolean,
    fmt?: string,
    verbose?: boolean,
    logLevel?: number,
    wei?: boolean,
    ether?: boolean,
    dollars?: boolean,
    raw?: boolean,
  },
  options?: RequestInit,
) {
  return ApiCallers.fetch<Transaction[] | Transfer[]>(
    { endpoint: '/transactions', method: 'get', parameters, options },
  );
}

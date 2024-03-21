/* eslint object-curly-newline: ["error", "never"] */
/* eslint max-len: ["error", 160] */
/*
 * This file was generated with makeClass --sdk. Do not edit it.
 */
import * as ApiCallers from '../lib/api_callers';
import { Receipt, txId } from '../types';

export function getReceipts(
  parameters?: {
    transactions: txId[],
    articulate?: boolean,
    fmt?: string,
    chain: string,
    noHeader?: boolean,
    cache?: boolean,
    decache?: boolean,
    raw?: boolean,
  },
  options?: RequestInit,
) {
  return ApiCallers.fetch<Receipt[]>(
    { endpoint: '/receipts', method: 'get', parameters, options },
  );
}

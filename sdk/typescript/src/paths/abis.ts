/* eslint object-curly-newline: ["error", "never"] */
/* eslint max-len: ["error", 160] */
/*
 * This file was generated with makeClass --sdk. Do not edit it.
 */
import * as ApiCallers from '../lib/api_callers';
import { Abi, address, Function, Parameter } from '../types';

export function getAbis(
  parameters?: {
    addrs: address[],
    known?: boolean,
    proxyFor?: address,
    find?: string[],
    hint?: string[],
    encode?: string,
    fmt?: string,
    chain: string,
    noHeader?: boolean,
    cache?: boolean,
    decache?: boolean,
  },
  options?: RequestInit,
) {
  return ApiCallers.fetch<Abi[] | Function[] | Parameter[]>(
    { endpoint: '/abis', method: 'get', parameters, options },
  );
}

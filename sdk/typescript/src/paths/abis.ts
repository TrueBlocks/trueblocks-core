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
    find?: string[],
    hint?: string[],
    encode?: string,
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
  return ApiCallers.fetch<Abi[] | Function[] | Parameter[]>(
    { endpoint: '/abis', method: 'get', parameters, options },
  );
}

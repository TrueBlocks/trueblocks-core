/* eslint object-curly-newline: ["error", "never"] */
/* eslint max-len: ["error", 160] */
/*
 * This file was generated with makeClass --sdk. Do not edit it.
 */
import * as ApiCallers from '../lib/api_callers';
import { Chain } from '../types';

export function getConfig(
  parameters?: {
    mode?: 'show*' | 'edit',
    paths?: boolean,
    fmt?: string,
    chain: string,
    noHeader?: boolean,
  },
  options?: RequestInit,
) {
  return ApiCallers.fetch<Chain[]>(
    { endpoint: '/config', method: 'get', parameters, options },
  );
}

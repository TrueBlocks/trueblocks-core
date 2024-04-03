/* eslint object-curly-newline: ["error", "never"] */
/* eslint max-len: ["error", 160] */
// Copyright 2016, 2024 The TrueBlocks Authors. All rights reserved.
// Use of this source code is governed by a license that can
// be found in the LICENSE file.
/*
 * This file was auto generated. DO NOT EDIT.
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

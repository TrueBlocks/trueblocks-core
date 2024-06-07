/* eslint object-curly-newline: ["error", "never"] */
/* eslint max-len: ["error", 160] */
/*
 * This file was generated with makeClass --sdk. Do not edit it.
 */
import { address, uint64 } from ".";

export type Name = {
  tags: string;
  address: address;
  name: string;
  symbol: string;
  source: string;
  decimals: uint64;
  isCustom?: boolean;
  isPrefund?: boolean;
  isContract?: boolean;
  isErc20?: boolean;
  isErc721?: boolean;
  deleted?: boolean;
};

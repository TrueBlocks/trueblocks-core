/*-------------------------------------------------------------------------
 * This source code is confidential proprietary information which is
 * copyright (c) 2018, 2021 TrueBlocks, LLC (http://trueblocks.io)
 * All Rights Reserved
 *------------------------------------------------------------------------*/
package swagger

type Response struct {
	Data *interface{} `json:"data,omitempty"`

	Error_ []string `json:"error,omitempty"`
}

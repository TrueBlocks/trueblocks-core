/*-------------------------------------------------------------------------------------------
 * qblocks - fast, easily-accessible, fully-decentralized data from blockchains
 * copyright (c) 2016, 2021 TrueBlocks, LLC (http://trueblocks.io)
 *
 * This program is free software: you may redistribute it and/or modify it under the terms
 * of the GNU General Public License as published by the Free Software Foundation, either
 * version 3 of the License, or (at your option) any later version. This program is
 * distributed in the hope that it will be useful, but WITHOUT ANY WARRANTY; without even
 * the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU
 * General Public License for more details. You should have received a copy of the GNU General
 * Public License along with this program. If not, see http://www.gnu.org/licenses/.
 *-------------------------------------------------------------------------------------------*/
#include <algorithm>
#include "abi.h"
#include "node.h"

//-----------------------------------------------------------------------
namespace qblocks {

extern bool toPrintable(const string_q& inHex, string_q& result);
//-----------------------------------------------------------------------
bool CAbi::articulateTransaction(CTransaction* p) const {
    if (!p)
        return false;

    // articulate the events, so we can return with a fully articulated object
    for (size_t i = 0; i < p->receipt.logs.size(); i++) {
        ((CAbi*)this)->loadAbiFromEtherscan(p->receipt.logs[i].address);  // NOLINT
        articulateLog(&p->receipt.logs[i]);
    }

    // articulate the traces, so we can return with a fully articulated object
    bool hasTraces = false;
    for (auto& trace : p->traces) {
        hasTraces = true;
        trace.articulatedTrace.m_showing = false;
        ((CAbi*)this)->loadAbiFromEtherscan(trace.action.to);  // NOLINT
        if (articulateTrace(&trace))
            trace.articulatedTrace.m_showing = true;
    }

    string_q encoding = extract(p->input, 0, 10);
    string_q input = extract(p->input, 10);
    ((CAbi*)this)->loadAbiFromEtherscan(p->to);  // NOLINT
    if (findInterface(encoding, p->articulatedTx)) {
        bool ret1 = decodeRLP(p->articulatedTx.inputs, "", input);
        bool ret2 = (hasTraces ? decodeRLP(p->articulatedTx.outputs, "", p->traces[0].result.output) : false);
        return (ret1 || ret2);
    }
    if (!toPrintable(p->input, p->articulatedTx.message))
        p->articulatedTx.message = "";

    return false;
}

// Significant speed improvement if we handle these items without regular processing
static const topic_t transferTopic = str_2_Topic("0xddf252ad1be2c89b69c2b068fc378daa952ba7f163c4a11628f55a4df523b3ef");
static const topic_t ensTransferTopic =
    str_2_Topic("0xd4735d920b0f87494915f556dd9b54c8f309026070caea5c737245152564d266");
static const topic_t approvalTopic = str_2_Topic("0x8c5be1e5ebec7d5bd14f71427d1e84f3dd0314c0f7b2291e5b200ac8c7c3b925");
//-----------------------------------------------------------------------
bool parseTransferEvent(CLogEntry* p) {
    static CFunction* evt = NULL;
    if (evt == NULL) {
        evt = new CFunction;
        evt->name = "Transfer";
        evt->type = "event";
        evt->encoding = transferTopic;
        evt->inputs.push_back(CParameter("_from", "address"));
        evt->inputs.push_back(CParameter("_to", "address"));
        evt->inputs.push_back(CParameter("_amount", "uint256"));
    }
    p->articulatedLog = *evt;
    p->articulatedLog.inputs[0].value = str_2_Addr(topic_2_Str(p->topics[1]));
    p->articulatedLog.inputs[1].value = str_2_Addr(topic_2_Str(p->topics[2]));
    p->articulatedLog.inputs[2].value = bnu_2_Str(str_2_BigUint(p->data));
    return true;
}
//-----------------------------------------------------------------------
bool parseENSTransferEvent(CLogEntry* p) {
    static CFunction* evt = NULL;
    if (evt == NULL) {
        evt = new CFunction;
        evt->name = "Transfer";
        evt->type = "event";
        evt->encoding = ensTransferTopic;
        evt->inputs.push_back(CParameter("_node", "bytes32"));
        evt->inputs.push_back(CParameter("_owner", "address"));
    }
    p->articulatedLog = *evt;
    p->articulatedLog.inputs[0].value = topic_2_Str(p->topics[1]);
    p->articulatedLog.inputs[1].value = str_2_Addr(topic_2_Str(p->data));
    return true;
}
//-----------------------------------------------------------------------
bool parseApprovalEvent(CLogEntry* p) {
    static CFunction* evt = NULL;
    if (evt == NULL) {
        evt = new CFunction;
        evt->name = "Approval";
        evt->type = "event";
        evt->encoding = approvalTopic;
        evt->inputs.push_back(CParameter("_owner", "address"));
        evt->inputs.push_back(CParameter("_spender", "address"));
        evt->inputs.push_back(CParameter("_amount", "uint256"));
    }
    p->articulatedLog = *evt;
    p->articulatedLog.inputs[0].value = str_2_Addr(topic_2_Str(p->topics[1]));
    p->articulatedLog.inputs[1].value = str_2_Addr(topic_2_Str(p->topics[2]));
    p->articulatedLog.inputs[2].value = bnu_2_Str(str_2_BigUint(p->data));
    return true;
}

extern string_q parse_str(const string_q& input, const void* data);
extern string_q parse_by32(const string_q& input, const void* data = NULL);
//-----------------------------------------------------------------------
bool CAbi::articulateLog(CLogEntry* p) const {
    if (!p || p->topics.size() == 0)
        return false;

    // Hacky shortcuts are way faster since these three events are about 90% of all events
    if (p->topics[0] == transferTopic && p->topics.size() > 1)
        return parseTransferEvent(p);
    if (p->topics[0] == ensTransferTopic && p->topics.size() > 1)
        return parseENSTransferEvent(p);
    if (p->topics[0] == approvalTopic && p->topics.size() > 1)
        return parseApprovalEvent(p);

    // First, we spin looking for our event. When we find it, we copy it so we can fill it in. The topics are
    // all fixed length, so we can process them in place as we go. While we're spinning, we accumulate the
    // non-indexed parameters so we can handle them as we do a regular input field. We then weave the processed
    // data params back into the copied array and from there into the event's inputs array. See the note below
    // from the Solidity documentation.

    if (findInterface(topic_2_Str(p->topics[0]), p->articulatedLog)) {
        bool ret1 = true, ret2 = true;
        size_t which = 1;
        CParameterArray dataParams;
        for (auto& param : p->articulatedLog.inputs) {
            if (param.indexed && p->topics.size() > which) {
                string_q top = substitute(topic_2_Str(p->topics[which++]), "0x", "");
                if (param.type == "string" || param.type == "bytes") {
                    param.value = parse_by32(top);

                } else if (contains(param.type, "[")) {
                    param.value = "0x" + top;

                } else {
                    CParameterArray tmp;
                    tmp.push_back(param);
                    if (!decodeRLP(tmp, param.type, "0x" + top))
                        ret1 = false;
                    else
                        param = tmp[0];
                }
            } else {
                dataParams.push_back(param);
            }
        }
        ret2 = decodeRLP(dataParams, "", p->data);
        for (auto d : dataParams) {
            for (auto& param : p->articulatedLog.inputs) {
                if (d.name == param.name)
                    param.value = d.value;
            }
        }
        return (ret1 && ret2);
    }

    return false;

    // From solidity documentation:
    // For all fixed-length Solidity types, the EVENT_INDEXED_ARGS array contains the 32-byte encoded value directly.
    // However, for types of dynamic length, which include string, bytes, and arrays, EVENT_INDEXED_ARGS will contain
    // the Keccak hash of the packed encoded value (see Strict Encoding Mode), rather than the encoded value directly.
    // This allows applications to efficiently query for values of dynamic-length types (by setting the hash of the
    // encoded value as the topic), but leaves applications unable to decode indexed values they have not queried for.
    // For dynamic-length types, application developers face a trade-off between fast search for predetermined values
    // (if the argument is indexed) and legibility of arbitrary values (which requires that the arguments not be
    // indexed). Developers may overcome this tradeoff and achieve both efficient search and arbitrary legibility
    // by defining events with two arguments — one indexed, one not — intended to hold the same value.
    //
    // Upshot: Topics are always fixed length. They can be parsed as fixed length. The data field needs to be parsed
    // as potentially variable length RLP encoded. Since the indexed event args and the non-indexed event args can
    // be interleaved in the function interface, we need to decode them separately
    // TODO(jayrush): SEARCH FOR ISSUE #1013
}

//-----------------------------------------------------------------------
bool CAbi::articulateTrace(CTrace* p) const {
    if (!p)
        return false;

    string_q encoding = extract(p->action.input, 0, 10);
    string_q input = extract(p->action.input, 10);
    if (findInterface(encoding, p->articulatedTrace)) {
        bool ret1 = decodeRLP(p->articulatedTrace.inputs, "", input);
        bool ret2 = decodeRLP(p->articulatedTrace.outputs, "", p->result.output);
        return (ret1 || ret2);
    }
    return false;
}

//-----------------------------------------------------------------------
bool CAbi::articulateOutputs(const string_q& encoding, const string_q& output, CFunction& ret) const {
    if (findInterface(encoding, ret))
        return decodeRLP(ret.outputs, "", output);
    return true;
}

//----------------------------------------------------------------------------
// If we can reasonably convert this byte input into a string, do so, otherwise bail out
bool toPrintable(const string_q& inHex, string_q& result) {
    string_q cleaned;
    string_q nibbles = substitute(inHex, "0x", "");
    while (nibbles.size() >= 2) {
        string_q nibble = extract(nibbles, 0, 2);
        nibbles = extract(nibbles, 2);
        char ch = (char)hex_2_Ascii(nibble[0], nibble[1]);  // NOLINT
        if (ch == '\\') {
            cleaned += "\\\\";  // we are only exporting printable characters, so escape any escapes
        } else if (ch == '\"') {
            cleaned += "\\\"";  // we are only exporting printable characters, so escape any quotes
        } else if (isalpha(ch) || isdigit(ch) || ispunct(ch) || isblank(ch)) {
            cleaned += ch;
        } else if (ch == 0x19) {
            // do nothing
        } else {
            // give up
            result = inHex;
            return false;
        }
    }
    result = cleaned;
    return true;
}

}  // namespace qblocks

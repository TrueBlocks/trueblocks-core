/*-------------------------------------------------------------------------------------------
 * qblocks - fast, easily-accessible, fully-decentralized data from blockchains
 * copyright (c) 2018 Great Hill Corporation (http://greathill.com)
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
    bool CAbi::articulateTransaction(CTransaction *p) const {

        ENTER("articulateTransaction");
        if (!p)
            EXIT_NOMSG(false);

        // articulate the events, so we can return with a fully articulated object
        for (size_t i = 0 ; i < p->receipt.logs.size() ; i++)
            articulateLog(&p->receipt.logs[i]);

        // articulate the traces, so we can return with a fully articulated object
        bool hasTraces = false;
        for (auto& trace : p->traces) {
            hasTraces = true;
            trace.articulatedTrace.m_showing = false;
            ((CAbi*)this)->loadAbiByAddress(trace.action.to);
            if (articulateTrace(&trace))
                trace.articulatedTrace.m_showing = true;
        }

        if (p->input.length() >= 10 || p->input == "0x") {
            string_q encoding = extract(p->input, 0, 10);
            string_q input    = extract(p->input, 10);
            for (auto interface : interfaces) {
                if (encoding % interface.encoding) {
                    p->articulatedTx = CFunction(interface);
                    p->articulatedTx.showOutput = false;
                    bool ret1 = decodeRLP(p->articulatedTx.inputs, "", input);
                    bool ret2 = (hasTraces ? decodeRLP(p->articulatedTx.outputs, "", p->traces[0].result.output) : false);
                    EXIT_NOMSG(ret1 || ret2);
                }
            }

            if (!toPrintable(p->input, p->articulatedTx.message))
                p->articulatedTx.message = "";
        }

        EXIT_NOMSG(false);
    }

    // Significant speed improvement if we handle these items without regular processing
    static const biguint_t transferTopic = str_2_BigUint("0xddf252ad1be2c89b69c2b068fc378daa952ba7f163c4a11628f55a4df523b3ef");
    //-----------------------------------------------------------------------
    bool parseTransferEvent(CLogEntry *p) {
        static CFunction *transDef = NULL;
        if (transDef == NULL) {
            transDef = new CFunction;
            transDef->name = "Transfer";
            transDef->type = "event";
            transDef->inputs.push_back(CParameter("_from", "address"));
            transDef->inputs.push_back(CParameter("_to", "address"));
            transDef->inputs.push_back(CParameter("_amount", "uint256"));
        }
        p->articulatedLog = *transDef;
        p->articulatedLog.inputs[0].value = str_2_Addr(topic_2_Str(p->topics[1]));
        p->articulatedLog.inputs[1].value = str_2_Addr(topic_2_Str(p->topics[2]));
        p->articulatedLog.inputs[2].value = bnu_2_Str(str_2_BigUint(p->data));
        return true;
    }

    extern string_q parse_str(const string_q& input, const void *data);
    extern string_q parse_by32(const string_q& input, const void *data=NULL);
    //-----------------------------------------------------------------------
    bool CAbi::articulateLog(CLogEntry *p) const {

        if (!p)
            return false;

        // Hacky shortcut is way faster since this event is about 90% of all events
        if (p->topics[0] == transferTopic && p->topics.size() > 1)
            return parseTransferEvent(p);

        // First, we spin looking for our event. When we find it, we copy it so we can fill it in. The topics are
        // all fixed length, so we can process them in place as we go. While we're spinning, we accumulate the
        // non-indexed parameters so we can handle them as we do a regular input field. We then weave the processed
        // data params back into the copied array and from there into the event's inputs array. See the note below
        // from the Solidity documentation.

        CAbi *ncABI = (CAbi*)this;
        for (size_t i = 0 ; i < ncABI->interfaces.size() ; i++) {
            CFunction *funcPtr = &ncABI->interfaces[i];
#if 0
//            // TODO(tjayrush): ISSUE #1160
//            string_q padded = padRight(funcPtr->encoding, 66, '0');
//            cout << padded << endl;
//            cout << topic_2_Str(p->topics[0]) << endl;
#else
            string_q padded = funcPtr->encoding;
#endif
            if (topic_2_Str(p->topics[0]) % padded) {

                // We found the topic we're looking for...work on a copy...
                p->articulatedLog = CFunction(*funcPtr);
                p->articulatedLog.showOutput = false;

                bool ret1 = true, ret2 = true;
                size_t which = 1;
                CParameterArray dataParams;
                for (auto& param : p->articulatedLog.inputs) {
                    if (param.indexed && p->topics.size() > which) {
                        string_q top = substitute(topic_2_Str(p->topics[which++]), "0x", "");
                        if (param.type == "string" || param.type == "bytes") {
                            if (!toPrintable(top, param.value))
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

#if 0
                p->articulatedLog = CFunction(*intf);
                p->articulatedLog.showOutput = false;

                // separate out the topic params from the data params
                string_q topicStr; size_t which = 1, pos = 0;
                CParameterArray topics, data;
                for (auto in : p->articulatedLog.inputs) {
                    if (in.indexed && which < p->topics.size()) {
                        in.pos = pos++;
                        topics.push_back(in);
                        topicStr += topic_2_Str(p->topics[which++]);
                    } else {
                        in.pos = pos++;
                        data.push_back(in);
                    }
                }
                bool ret1 = decodeRLP(topics, "", topicStr);
                bool ret2 = decodeRLP(data, "", p->data);
                p->articulatedLog.inputs.clear();
                for (auto a : topics)
                    p->articulatedLog.inputs.push_back(a);
                for (auto a : data)
                    p->articulatedLog.inputs.push_back(a);
                sort(p->articulatedLog.inputs.begin(), p->articulatedLog.inputs.end(), sortByPosition);
                return (ret1 || ret2);
#endif
            }
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
        // TODO: SEARCH FOR ISSUE #1013
    }

    //-----------------------------------------------------------------------
    bool CAbi::articulateTrace(CTrace *p) const {

        ENTER("articulateTrace");
        if (!p)
            EXIT_NOMSG(false);

        if (p->action.input.length() >= 10 || p->action.input == "0x") {
            string_q encoding = extract(p->action.input, 0, 10);
            string_q input    = extract(p->action.input, 10);
            for (auto interface : interfaces) {
                if (encoding % interface.encoding) {
                    p->articulatedTrace = CFunction(interface);
                    p->articulatedTrace.showOutput = false;
                    bool ret1 = decodeRLP(p->articulatedTrace.inputs, "", input);
                    bool ret2 = decodeRLP(p->articulatedTrace.outputs, "", p->result.output);
                    EXIT_NOMSG(ret1 || ret2);
                }
            }
        }
        EXIT_NOMSG(false);
    }

    //-----------------------------------------------------------------------
    bool CAbi::articulateOutputs(const string_q& encoding, const string_q& output, CFunction& ret) const {
        ENTER("articulateOutputs");
        for (auto interface : interfaces) {
            if (encoding % interface.encoding) {
                ret = CFunction(interface);
                ret.showOutput = false;
                bool bRet = decodeRLP(ret.outputs, "", output);
                EXIT_NOMSG(bRet);
            }
        }
        EXIT_NOMSG(true);
    }

    //----------------------------------------------------------------------------
    // If we can reasonably convert this byte input into a string, do so, otherwise bail out
    bool toPrintable(const string_q& inHex, string_q& result) {

        bool hasControlChar = false;
        string_q in = substitute(inHex, "0x", "");
        while (!in.empty() && in.size() >= 2) {
            string_q nibble = extract(in, 0, 2);
            in = extract(in, 2);
            char ch = (char)hex_2_Ascii(nibble[0], nibble[1]);  // NOLINT
            if (ch == 0x19)
                hasControlChar = true;
            if (!isprint(ch) && !isspace(ch) && ch != 0x19)
                return false;
        }
        result = hex_2_Str(inHex);
        if (hasControlChar) {
            // TODO(tjayrush): hack alert
            string s;
            s += (char)0x20;
            s += (char)0x19;
            result = substitute(result, s, "'");
        }
        return true;
    }

}  // namespace qblocks

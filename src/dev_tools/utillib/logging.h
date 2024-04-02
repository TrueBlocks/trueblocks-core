#pragma once
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
#include "basetypes.h"

namespace qblocks {

//----------------------------------------------------------------
typedef enum {
    sev_na = 0,
    sev_info = 1,
    sev_prog = 2,
    sev_warning,
    sev_error,
    sev_fatal,
    sev_debug0 = 10,
    sev_debug1,
    sev_debug2,
    sev_debug3,
    sev_debug4,
    sev_debug8 = 18,
} severity_t;

//----------------------------------------------------------------
extern uint64_t verbose;
inline bool isLevelOn(severity_t test) {
    if (test < sev_debug0)
        return true;
    return (((severity_t)verbose) >= (test - sev_debug0));
}

//----------------------------------------------------------------
class log_policy_i {
  public:
    char end_line;
    log_policy_i(void) : end_line('\n') {
    }
    virtual void open_ostream(const string_q& name) = 0;
    virtual void close_ostream() = 0;
    virtual void write(const string_q& msg) = 0;
    virtual ~log_policy_i(void) {
    }
};

//----------------------------------------------------------------
class file_log : public log_policy_i {
    unique_ptr<ofstream> os;

  public:
    file_log() : os(new ofstream) {
    }
    void open_ostream(const string_q& name) {
        os->open(name.c_str(), ios_base::binary | ios_base::out);
        if (!os->is_open())
            throw(runtime_error("LOGGER: Unable to open an output stream"));
    }
    void close_ostream() {
        if (os)
            os->close();
    }
    void write(const string_q& msg) {
        if (os) {
            (*os) << msg << end_line;
            os->flush();
        }
    }
    ~file_log() {
        close_ostream();
    }
};

//----------------------------------------------------------------
class err_log : public log_policy_i {
  public:
    err_log() {
    }
    void open_ostream(const string_q& unused) {
    }
    void close_ostream() {
    }
    void write(const string_q& msg) {
        cerr << msg << end_line;
        cerr.flush();
    }
    ~err_log() {
    }
};

//----------------------------------------------------------------
template <typename log_policy>
class logger {
    //----------------------------------------------------------------
    stringstream log_stream;
    log_policy* policy;
    mutex write_mutex;

    //----------------------------------------------------------------
    string_q get_logline_header(void);

    //----------------------------------------------------------------
    void print_impl() {
        if (!log_stream.str().empty() && log_stream.str()[log_stream.str().length() - 1] == '\r')
            setEndline('\r');
        policy->write(get_logline_header() + log_stream.str());
        log_stream.str("");
        setEndline('\n');
    }

    //----------------------------------------------------------------
    template <typename First, typename... Rest>
    void print_impl(First parm1, Rest... parm) {
        log_stream << parm1;
        print_impl(parm...);
    }

  public:
    //----------------------------------------------------------------
    void setEndline(char ch) {
        if (policy)
            policy->end_line = ch;
    }

    //----------------------------------------------------------------
    explicit logger(const string_q& name) {
        policy = new log_policy;
        if (!policy) {
            throw std::runtime_error("LOGGER: Unable to create the logger instance");
        }
        if (!name.empty())
            policy->open_ostream(name);
    }

    //----------------------------------------------------------------
    template <severity_t severity, typename... Args>
    void print(Args... args) {
        if (!isLevelOn(severity))
            return;
        write_mutex.lock();
        switch (severity) {
            case sev_debug0:
                log_stream << ": " << "0" << "-";
                break;
            case sev_debug1:
                log_stream << ": " << "1" << "--";
                break;
            case sev_debug2:
                log_stream << ": " << "2" << "---";
                break;
            case sev_debug3:
                log_stream << ": " << "3" << "----";
                break;
            case sev_debug4:
                log_stream << ": " << "4" << "-----";
                break;
            case sev_debug8:
                log_stream << ": " << "8" << "------";
                break;
            case sev_prog:
                log_stream << "<PROG>  " << ": ";
                break;
            case sev_info:
                log_stream << "<INFO>  " << ": ";
                break;
            case sev_warning:
                log_stream << "<WARN> " << ": ";
                break;
            case sev_error:
                log_stream << "<EROR> " << ": ";
                break;
            case sev_fatal:
                log_stream << "<FATL> " << ": ";
                break;
        }
        print_impl(args...);
        write_mutex.unlock();
    }

    //----------------------------------------------------------------
    virtual ~logger() {
        if (policy) {
            policy->close_ostream();
            // delete policy;
        }
    }
};

extern logger<log_policy_i>* dLogger;
extern logger<log_policy_i>* eLogger;
}  // namespace qblocks

#define LOG_INFO eLogger->print<sev_info>
#define LOG_WARN eLogger->print<sev_warning>
#define LOG_ERR eLogger->print<sev_error>

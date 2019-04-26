#pragma once
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
#include "basetypes.h"

namespace qblocks {

    //----------------------------------------------------------------
    typedef enum { sev_info = 1, sev_warning, sev_error, sev_fatal, sev_debug0, sev_debug1, sev_debug2 } severity_t;

    //----------------------------------------------------------------
    inline bool isLevelOn(severity_t test) { if (test < sev_debug0) return true; return (verbose > (test - sev_debug0)); }

    //----------------------------------------------------------------
    class log_policy_i {
    public:
        virtual void open_ostream(const string_q& name) = 0;
        virtual void close_ostream() = 0;
        virtual void write(const string_q& msg) = 0;
        virtual ~log_policy_i(void) { };
    };

    //----------------------------------------------------------------
    class file_log : public log_policy_i {
        unique_ptr<ofstream> os;
    public:
        file_log() : os(new ofstream) { }
        void open_ostream(const string_q& name) {
            os->open(name.c_str(), ios_base::binary | ios_base::out);
            if (!os->is_open())
                throw(runtime_error("LOGGER: Unable to open an output stream"));
        }
        void close_ostream() { if (os) os->close(); }
        void write(const string_q &msg) { if (os) (*os) << msg << endl; }
        ~file_log() { close_ostream(); }
    };

    //----------------------------------------------------------------
    class err_log : public log_policy_i {
    public:
        err_log() { }
        void open_ostream(const string_q& unused) { }
        void close_ostream() { }
        void write(const string_q& msg) { cerr << msg << endl; }
        ~err_log() { }
    };

    //----------------------------------------------------------------
    template< typename log_policy >
    class logger {

        //----------------------------------------------------------------
        unsigned log_line_number;

        //----------------------------------------------------------------
        string_q get_logline_header() {

            stringstream header;
            header.fill('0');header.width(7);
            header << bBlack << log_line_number++ << " ";
            if (isTestMode()) {
                header << "TIME ~ CLOCK - ";
            } else {
                header << Now().Format(FMT_EXPORT) << " ~ ";
#define LOG_TIMING true
#define LOG_THREAD false
                if (LOG_TIMING) {
                    header.fill('0');header.width(7);
                    header << clock() << " - ";
                }
                if (LOG_THREAD) {
                    header.fill('0');header.width(7);
                    header << this_thread::get_id() << " + ";
                }
            }
            header << cOff;

            return header.str();
        }

        //----------------------------------------------------------------
        stringstream log_stream;

        //----------------------------------------------------------------
        log_policy* policy;

        //----------------------------------------------------------------
        mutex write_mutex;

        //----------------------------------------------------------------
        void print_impl() {
            policy->write( get_logline_header() + log_stream.str() );
            log_stream.str("");
        }

        //----------------------------------------------------------------
        template<typename First, typename...Rest >
        void print_impl(First parm1, Rest...parm) {
            log_stream<<parm1;
            print_impl(parm...);
        }


    public:
        //----------------------------------------------------------------
        logger( const string_q& name ) {
            log_line_number = 0;
            policy = new log_policy;
            if( !policy ) {
                throw std::runtime_error("LOGGER: Unable to create the logger instance");
            }
            if (!name.empty())
                policy->open_ostream( name );
        }

        //----------------------------------------------------------------
        template< severity_t severity , typename...Args >
        void print( Args...args ) {
            if (!isLevelOn(severity))
                return;
            write_mutex.lock();
            switch( severity ) {
                case sev_debug0:  log_stream << cWhite  << "<DEBUG>" << cOff << " : "; break;
                case sev_debug1:  log_stream << cWhite  << "<DEBUG>" << cOff << " : |-"; break;
                case sev_debug2:  log_stream << cWhite  << "<DEBUG>" << cOff << " : |--"; break;
                case sev_info:    log_stream << bGreen  << "<INFO> " << cOff << " : "; break;
                case sev_warning: log_stream << bYellow << "<WARNG>" << cOff << " : "; break;
                case sev_error:   log_stream << bRed    << "<ERROR>" << cOff << " : "; break;
                case sev_fatal:   log_stream << bTeal   << "<FATAL>" << cOff << " : "; break;
            };
            print_impl( args... );
            write_mutex.unlock();
        }

        //----------------------------------------------------------------
        virtual ~logger() {
            if( policy ) {
                policy->close_ostream();
                //delete policy;
            }
        }
    };

    extern logger<log_policy_i> *dLogger;
    extern logger<log_policy_i> *eLogger;
}  // namespace qblocks

#define LOGGING_LEVEL
#ifdef LOGGING_LEVEL
#define LOG0      qblocks::dLogger->print<sev_debug0>
#define LOG1      qblocks::dLogger->print<sev_debug1>
#define LOG2      qblocks::dLogger->print<sev_debug2>
#define LOG_INFO  qblocks::eLogger->print<sev_info>
#define LOG_WARN  qblocks::eLogger->print<sev_warning>
#define LOG_ERR   qblocks::eLogger->print<sev_error>
#define LOG_FATAL qblocks::eLogger->print<sev_fatal>
#else
#define LOG0(...)
#define LOG1(...)
#define LOG2(...)
#define LOG_INFO(...)
#define LOG_WARN(...)
#define LOG_ERR(...)
#define LOG_FATAL(...)
#endif

#define ENTER(a)       { LOG2(string_q("Enter(") + uint_2_Str(__LINE__) + ") " + a); }

// The LOG parts of these routines disappear if turned off, but they still do their work because of the returns
#define EXIT_USAGE(a)  { LOG_ERR(string_q("Exit(")  + uint_2_Str(__LINE__) + ") " + a); return usage((a)); }
#define EXIT_FAIL(a)   { LOG_WARN(string_q("Exit(")  + uint_2_Str(__LINE__) + ") " + a); return false; }
#define EXIT_OK(a)     { LOG2(string_q("Exit(")  + uint_2_Str(__LINE__) + ") " + a); return true; }

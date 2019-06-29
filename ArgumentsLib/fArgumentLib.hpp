#ifndef ARGUMENT_LIB_HPP_
#define ARGUMENT_LIB_HPP_ "1.0.0@fArgumentLib.hpp"
/**
 * DESCRIPTION:
 *     Module contains implementation of argument parser and helper structures.
 * AUTHOR:
 *     Mikhail Demchenko
 *     mailto:dev.echo.mike@gmail.com
 *     https://github.com/echo-Mike
 */

/**
 * MIT License
 *
 * Copyright (c) 2017-2019 Mikhail Demchenko dev.echo.mike@gmail.com
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 * 
 * The above copyright notice and this permission notice shall be included in all
 * copies or substantial portions of the Software.
 * 
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
 * SOFTWARE.
 */

// STD
#include <cstring>

#ifdef TEST
    #include <cstdio>
    struct FArgumentLibLoggerSingleton
    {
        static std::FILE* getInstance(const char* path)
        {
            static FArgumentLibLoggerSingleton s(path);
            return s.h;
        }
      private:
        std::FILE* h;
        FArgumentLibLoggerSingleton(const char* path) : h(std::fopen(path,"w+")) {}
        ~FArgumentLibLoggerSingleton()
        {
            std::fflush(h);
            std::fclose(h);
        }
    };
#endif

#if defined(TEST) && !defined(LOG)
#define LOG(format, ...) \
do { \
    std::fprintf(FArgumentLibLoggerSingleton::getInstance(".\\fArgumentLibTestLog.txt"), "[%-35s: %-25s: line:%-4d] " format "\n", __FILE__, __FUNCTION__, __LINE__, ## __VA_ARGS__); \
} while(0);
#endif

/**
 * @namespace opt
 * @brief Main namespace of argument parser library
 */
namespace opt
{

/**
 * @brief Represents types of options by the presence of it's argument
 * Used for long options array
 */
enum ArgType
{
    END = 0,  // Used as end marker for option array
    FLAG,     // Identifies an option that MUST NOT have any arguments
    REQ_ARG,  // An argument MUST be provided for an option
    OPT_ARG   // An argument MAY be provided for an option
};

/**
 * Represents option classification based on presence in optstring or longopt array
 * Negatives are errors
 */
enum BroadOptionType
{
    NOT_AN_OPTION  = 0,  // Examined string is not an option
    FLAG,                // Option found is a flag and it is known from input
    UNKNOWN_FLAG,        // Option found is a flag but it is unknown
    VALUE,               // Option found is a value pair and it is known from input
    UNKNOWN_VALUE,       // Option found is a value pair but it is unknown
    NOT_CLASSIFIED = -1, // There was no possibility to classify option due to lack of buffer
    FLAG_WITH_ARGO = -2, // Flag option defined by opt_string was provided with argument
    VALUE_MISSINGO = -3, // Value option defined by opt_string missing argument
    FLAG_WITH_ARGL = -4, // Flag option defined by longopts was provided with argument
    VALUE_MISSINGL = -5, // Value option defined by longopts missing argument
    MISSED_EXACT   = -6, // One of exact option type is not handled properly
};

/**
 * Represents option classification based on appearance in option entry list
 * Negatives are errors
 */
enum ExactOptionType
{   // Syntax: ABNF
    NOT_OPTION          = 0,    // <not an option string>
    SHORT_NO_VALUE,             // "-" <opt-char>
    SHORT_OPTION,               // "-" <opt-char> 1*(" ") <long-opt-value>
    PAIR_SHORT,                 // "-" <opt-char> "=" <short-opt-value>
    PAIR_NO_VALUE_SHORT,        // "-" <opt-char> "="
    SHORT_MERGED,               // "-" <opt-char> <short-opt-value>
    PAIR_INVALID_SHORT,         // "-" <opt_string> "=" <short-opt-value>
    LONG_NO_VALUE,              // "--" <opt-string>
    LONG_OPTION,                // "--" <opt-string> 1*(" ") <long-opt-value>
    PAIR_LONG,                  // "--" <opt-string> "=" <short-opt-value>
    PAIR_NO_VALUE_LONG,         // "--" <opt-string> "="
    // long-opt-value  = *(<short-opt-value> 1*(" ")) <short-opt-value>
    // short-opt-value = <opt-arg> / <quoted-opt-arg>
    // opt-arg         = 1*<any char except space chars>
    // quoted-opt-arg  = """ 1*<any char> """
    // opt-char        = <any char except space chars>
    // opt-string      = 1*<any char except space chars>
    ERROR_LENGTH        = -1,   // Option entry provided is NULL pointer
    ERROR_POSITION      = -2,   // Position provided for parsing is argc
    NOT_PAIR_OPTION     = -3,   // Internal NOT AN ERROR. Option is not a pair type
    PAIR_OPTION         = -4,   // Internal NOT AN ERROR. Option is a pair type
    PAIR_NO_VALUE       = -5,   // Internal NOT AN ERROR. Option is a pair type and has no value component
};

/**
 * Struct to be used in long option array
 */
struct Option
{
    const char* long_option; // Long option representation
    int arg_type;            // Type of provided argument
    int short_option;        // Short option representation
};

/**
 * Represents option parsion context
 */
struct ParseCtx
{
    const int argc;     // argc from main
    char* const* argv;  // argv from main
    int option_index;   // index of option to parse
    int do_scan;        // flag: if up all option list entries after option
                        // that are not an option itself are the argument of
                        // option at index option_index otherwise
                        // only the first is counted as an argument
};

/**
 * Represents information about option
 */
struct OptionInfo
{
    int long_index; // If option is found in longopts array: index of option in array
    int exact_type; // One of ExactOptionType excluding negative errors
    int broad_type; // One of BroadOptionType
};

namespace detail
{

/**
 * @brief Determines if provided string is an option
 * Option string must start with:
 *  * "-" - short options
 *  * "--" - long options
 * But not by exactly "-" or "--"
 * @warning "---" is a valid option with body "-"
 * @param[in] str String to decide on
 * @return NOT_OPTION - string is not an option
 *         SHORT_OPTION - string that starts with "-" but not exactly "-"
 *         LONG_OPTION - string that starts with "--" but not exactly "--"
 *         ERROR_LENGTH - if provided option string is too short or nullptr
 */
inline int is_option(const char* str)
{
    if (!str)
        return ERROR_LENGTH;
    if (str[0] == '\0')
        return NOT_OPTION;
    if (str[0] == '-')
    {
        if (str[1] == '\0')
            return NOT_OPTION;
        if (str[1] == '-')
        {
            return str[2] == '\0' ? NOT_OPTION : LONG_OPTION;
        }
        return SHORT_OPTION;
    }
    return NOT_OPTION;
}

/**
 * @brief Returns option body from option string
 * Option body is an option part after initial "-" or "--"
 * but before any option value or other syntax parts.
 * @param[in] str Option as string
 * @param[out] opt The pointer to which to return option body
 * @return NOT_OPTION - string is not an option
 *         SHORT_OPTION - string that starts with "-" but not exactly "-"
 *         LONG_OPTION - string that starts with "--" but not exactly "--"
 */
inline int get_option_body(const char* str, const char* *opt)
{
    int opt_type = is_option(str);
    if (!opt)
        return opt_type;
    switch (opt_type)
    {
        case ERROR_LENGTH:
            /* fall through */
        case NOT_OPTION:
            *opt = NULL;
            break;
        case SHORT_OPTION:
            *opt = str + 1;
            break;
        case LONG_OPTION:
            *opt = str + 2;
            break;
    }
    return opt_type;
}

/**
 * @brief Checks whether the option is a pair option
 * If it is returns pointer to beginning of the value part
 * The option have a pair type if it's structure:
 *  <opt_body> "=" [<opt_value>]
 * @param[in] opt_body Body of option (see: get_option_body)
 * @param[out] val The pointer to which to return option value
 * @return NOT_PAIR_OPTION - string is not a pair option
 *         PAIR_NO_VALUE - string is pair option with no value component
 *         PAIR_OPTION - string is pair option with value component
 */
inline int is_pair_option(const char* opt_body, const char* *val)
{
    if (!opt_body)
        return ERROR_LENGTH;
    const char* value_part = std::strchr(opt_body, '=');
    if (!value_part)
        return NOT_PAIR_OPTION;
    if (val)
        *val = value_part + 1;
    return value_part[1] == '\0' ? PAIR_NO_VALUE : PAIR_OPTION;
}

/**
 * @brief Scans option list until next option or end in found
 * @param[in] ctx Parsing context
 * @return position of next option in option list or end position
 */
inline int scan_until_next_option(ParseCtx* ctx)
{
    int i = ctx->option_index + 1;
    while (i != ctx->argc && is_option(ctx->argv[i]) == NOT_OPTION)
        ++i;
    return i;
}

/**
 * @brief Classifies one option list entry
 * The internal option index of context is adjusted automatically
 * @param[in] ctx Parsing context (see: ParseCtx)
 * @param[out] out_body Buffer for option body output
 * @param[out] out_val Buffer for option value output
 * @return NOT_OPTION          - entry is not an option
 *         SHORT_NO_VALUE      - flag option that starts with "-"
 *         LONG_NO_VALUE       - flag option that starts with "--"
 *         SHORT_OPTION        - value option that starts with "-"
 *         LONG_OPTION         - value option that starts with "--"
 *         PAIR_NO_VALUE_SHORT - pair option without value that starts with "-"
 *         PAIR_NO_VALUE_LONG  - pair option without value that starts with "--"
 *         PAIR_SHORT          - pair option with value that starts with "-"
 *         PAIR_LONG           - pair option with value that starts with "--"
 *         PAIR_INVALID_SHORT  - pair option that starts with "-" but have
 *                               other body symbols
 *         SHORT_MERGED        - value option with value that starts with "-"
 *                               and value is merged with body
 */
inline int parse_option(
    ParseCtx* ctx,
    char* out_body,
    char* out_val
)
{
    const char *option_body = NULL, *option_value = NULL;

    if (out_body)
        out_body[0] = '\0';
    if (out_val)
        out_val[0] = '\0';

    if (ctx->option_index >= ctx->argc)
        return ERROR_POSITION;

    int option_type = get_option_body(ctx->argv[ctx->option_index],
                                      &option_body);
    if (!(option_type == SHORT_OPTION || option_type == LONG_OPTION))
    {   // out_body == ""
        // out_val == ""
        ++ctx->option_index;
        return option_type;
    }

    if (out_body)
    {
        out_body[0] = option_body[0];
        out_body[1] = '\0';
    }
    const int next_option_index = scan_until_next_option(ctx);
    if (option_body[1] == '\0')
    {   // Option with no body except one symbol "-O" or "--O"
        if (ctx->option_index + 1 == next_option_index)
        {   // Option has no value: in then end
            // of option list or next entry is an option
            // out_body == "O"
            // out_val == ""
            ++ctx->option_index;
            return option_type == SHORT_OPTION ?
                    SHORT_NO_VALUE :
                    LONG_NO_VALUE;
        }
        // Some of next entries in option list are not
        // options -> next entry/es are an option value:
        // out_body == "O"
        // If "-O 123456" || "--O 123456"
        //  out_val == "123456"
        // If "-O "123 456"" || "--O "123 456""
        //  out_val == "123 456"
        // If "-O arg1 arg2 "arg 3" -a" && do_scan == 1
        //  out_val == "arg1 arg2 arg 3"
        // If "-O arg1 arg2 "arg 3" -a" && do_scan == 0
        //  out_val == "arg1"
        if (!out_val)
        {
            ctx->option_index = ctx->do_scan ?
                                next_option_index :
                                ctx->option_index + 2;
            return option_type;
        }
        if (ctx->do_scan)
        {
            char* out_ptr = out_val;
            for (int i = ctx->option_index + 1; i < next_option_index; ++i)
            {
                const unsigned long str_size = std::strlen(ctx->argv[i]);
                std::memcpy(out_ptr, ctx->argv[i], str_size * sizeof(char));
                out_ptr += str_size;
                if (i + 1 != next_option_index)
                    *out_ptr++ = ' ';
            }
            *out_ptr = '\0';
            ctx->option_index = next_option_index;
        }
        else
        {
            std::strcpy(out_val, ctx->argv[ctx->option_index + 1]);
            ctx->option_index += 2;
        }
        return option_type;
    }
    // Option with some body
    if (option_type == SHORT_OPTION && option_body[1] == '=')
    {   // Short pair "-O=123" "-O="123 456""
        ++ctx->option_index;
        if (option_body[2] == '\0')
        {   // No value short pair option: "-O="
            // out_body == "O"
            // out_val == ""
            return PAIR_NO_VALUE_SHORT;
        }
        if (out_val)
            std::strcpy(out_val, option_body + 2);
        // out_body == "O"
        // If "-O=123"
        //  out_val == "123"
        // If "-O="123 456""
        //  out_val == "123 456"
        return PAIR_SHORT;
    }
    const int is_pair = is_pair_option(option_body, &option_value);
    if (option_type == SHORT_OPTION && is_pair != NOT_PAIR_OPTION)
    {   // Invalid short option in format:
        // "-Oabc=123" or "-Oabc="123 456""
        // must be only "-O=123" and "-O="123 456""
        if (out_val)
            std::strcpy(out_val, option_value);
        // out_body == "O"
        // If "-Oabc=123"
        //  out_val == "123"
        // If "-Oabc="123 456""
        //  out_val == "123 456"
        ++ctx->option_index;
        return PAIR_INVALID_SHORT;
    }
    if (is_pair == PAIR_NO_VALUE || is_pair == PAIR_OPTION)
    {   // Options in format:
        // "--O[pt]=123" "--O[pt]="123 456""
        ++ctx->option_index;
        if (out_body)
        {   // Invalid long pair option "--O[pt]="
            // option_value points to next char after "=",
            // option_value-1 points to "="
            const unsigned long opt_body_length = (option_value - 1) -
                                                  option_body;
            std::memcpy(out_body, option_body, opt_body_length * sizeof(char));
            out_body[opt_body_length] = '\0';
        }
        // out_body == "O[pt]"
        // out_val == ""
        if (is_pair == PAIR_NO_VALUE)
            return PAIR_NO_VALUE_LONG;
        // out_body == "O[pt]"
        // If "--O[pt]=123"
        //  out_val == "123"
        // If "--O[pt]="123 456""
        //  out_val == "123 456"
        if (is_pair == PAIR_OPTION && out_val)
            std::strcpy(out_val, option_value);
        return PAIR_LONG;
    }
    // Option with body that is not a pair: "-O123" or "--Option"
    if (option_type == SHORT_OPTION)
    {   // Short option with value: "-O123" or "-O"123 456""
        if (out_val)
            std::strcpy(out_val, option_body + 1);
        // out_body == "O"
        // If "-O123"
        //  out_val == "123"
        // If "-O"123 456""
        //  out_val == "123 456"
        ++ctx->option_index;
        return SHORT_MERGED;
    }
    // Long option: "--Option 123" or "--Option "123 456""
    if (out_body)
        std::strcpy(out_body, option_body);
    if (ctx->option_index + 1 == next_option_index)
    {   // Long value in the end of option list: "--Option"
        // out_body == "Option"
        // out_val == ""
        ++ctx->option_index;
        return LONG_NO_VALUE;
    }
    // Long option with value as next list entry
    // out_body == "Option"
    // If "--Option 123"
    //  out_val == "123"
    // If "--Option "123 456""
    //  out_val == "123 456"
    // If "--Option -a"
    //  out_val == ""
    // If "-O arg1 arg2 "arg 3" -a" && do_scan == 1
    //  out_val == "arg1 arg2 arg 3"
    // If "-O arg1 arg2 "arg 3" -a" && do_scan == 0
    //  out_val == "arg1"
    if (!out_val)
    {
        ctx->option_index = ctx->do_scan ?
                            next_option_index :
                            ctx->option_index + 2;
        return option_type;
    }
    if (ctx->do_scan)
    {
        char* out_ptr = out_val;
        for (int i = ctx->option_index + 1; i < next_option_index; ++i)
        {
            const unsigned long str_size = std::strlen(ctx->argv[i]);
            std::memcpy(out_ptr, ctx->argv[i], str_size * sizeof(char));
            out_ptr += str_size;
            if (i + 1 != next_option_index)
                *out_ptr++ = ' ';
        }
        *out_ptr = '\0';
        ctx->option_index = next_option_index;
    }
    else
    {
        std::strcpy(out_val, ctx->argv[ctx->option_index + 1]);
        ctx->option_index += 2;
    }
    return LONG_OPTION;
}

/**
 * @brief Search for char option in option string
 * @return -1 - on error or option not found OR
 *          index of option found in option string starting with 0
 */
inline int search(const char* opt_str, int opt)
{
    if (!opt_str)
        return -1;
    const char* const orig_opt_str = opt_str;
    if (*opt_str == '+')
        ++opt_str;
    if (*opt_str == ':')
        ++opt_str;
    const char* opt_ = std::strchr(opt_str, opt);
    if (!opt_)
        return -1;
    return opt_ - orig_opt_str;
}

/**
 * @brief Search for char option in long options array
 * @return -1 - on error or option not found OR
 *          index of option found in long option array
 */
inline int search(const Option longopts[], int opt)
{
    if (!longopts)
        return -1;
    for (int i = 0; longopts[i].arg_type != END; ++i)
    {
        if (longopts[i].short_option == opt)
            return i;
    }
    return -1;
}

/**
 * @brief Search for string option in long options array
 * @return -1 - on error or option not found OR
 *          index of option found in long option array
 */
inline int search(const Option longopts[], const char* const long_opt)
{
    if (!longopts)
        return -1;
    for (int i{0}; longopts[i].arg_type != END; ++i)
    {
        if (!longopts[i].long_option)
            continue;
        if (std::strcmp(longopts[i].long_option, long_opt) == 0)
            return i;
    }
    return -1;
}

/**
 * Returns true if option on position opt_pos is a flag in opt_str
 * (a.e. have no ":" after it)
 */
inline bool is_flag(const char* const opt_str, const int opt_pos)
{
    return opt_str[opt_pos + 1] != ':';
}

/**
 * Returns true if option on position opt_pos is a flag in longopts
 */
inline bool is_flag(const Option longopts[], const int opt_pos)
{
    return longopts[opt_pos].arg_type == FLAG;
}

/**
 * Returns true if option on position opt_pos may have optional argument
 */
inline bool is_optarg(const Option longopts[], const int opt_pos)
{
    return longopts[opt_pos].arg_type == OPT_ARG;
}

/**
 * Returns short for of option or 1
 * (meaning that option has no short form and must be checked by hand)
 */
inline int get_short_form(const Option longopts[], const int opt_pos)
{
    return longopts[opt_pos].short_option ? longopts[opt_pos].short_option : 1;
}
} // namespace detail

/**
 * @brief Parses next option entry in list
 * This function conforms in style with getopt(3) and getopt_long(3)
 * @param[in:out] ctx Parsing context
 * @param[in] opt_string Optional option string analogous to one from getopt
 *  (excluding "-" and "-W" functionality)
 *  If first char is '+': option processing stops as soon as a non-option
 *      argument is encountered
 *  If has ':' at place 0 or 1 (only after '+'): flag option with value
 *      and value option without one is not a hard parsing error. In first case
 *      ';' is returned in second ':' (conforming to getopt ':'). Only takes
 *      affect for options defined in opt_string for longopts use
 *      ArgType::OPT_ARG
 * @param[in] longopts Optional long options array. MUST end with option
 *      containing ArgType::END in arg_type field
 * @param[out] info Optional pointer to structure that describes parsed option
 * @param[out] out_body Optional output buffer for body of option
 * @param[out] out_val Optional output buffer for value of option
 * @return -1 - parsing end or some type of error returned to info
 *          0 - parsed option list entry is not an option
 *          1 - Long option known from longopts but short form is not provided
 *     <char> - char to which option corresponds to based on opt_string
 *              or on longopts short form
 *          : - If opt_string has ':' attribute - value option has not got a
 *              value
 *          ; - If opt_string has ':' attribute - flag option has an
 *              unexpected value
 *          ? - Option is not found in longopts or opt_string
 */
inline int next(
    ParseCtx* ctx,
    const char* opt_string = NULL,
    const Option* longopts = NULL,
    OptionInfo* info = NULL,
    char* out_body = NULL,
    char* out_val = NULL
)
{
    OptionInfo local_info_;
    char opt_char;
    int option_type, option_position;
    const bool plus_attribute = opt_string && opt_string[0] == '+';
    const bool colon_attribute = opt_string && opt_string[plus_attribute ? 1 : 0] == ':';
    if (!info)
        info = &local_info_;
    info->long_index = -1;
    option_type = detail::parse_option(ctx, out_body, out_val);
    info->exact_type = option_type;
    if (option_type < 0)
        return -1;
    switch (option_type)
    {
        case NOT_OPTION:
        {
            info->broad_type = NOT_AN_OPTION;
            if (plus_attribute)
                return -1;
            return 0;
        } break;
        case SHORT_NO_VALUE:
            /* fall through */
        case SHORT_OPTION:
            /* fall through */
        case PAIR_SHORT:
            /* fall through */
        case PAIR_NO_VALUE_SHORT:
            /* fall through */
        case SHORT_MERGED:
            /* fall through */
        case PAIR_INVALID_SHORT:
        {    // Pair without a value is not a flag in any case
            if (!out_body)
            {
                info->broad_type = NOT_CLASSIFIED;
                return '?';
            }
            opt_char = out_body[0];
            if (!opt_string)
                goto short_longopts;
            option_position = detail::search(opt_string, opt_char);
            if (option_position >= 0)
            {
                if (detail::is_flag(opt_string, option_position))
                {
                    if (option_type == SHORT_NO_VALUE)
                    {   // Found as flag and specified as flag
                        info->broad_type = FLAG;
                        return opt_char;
                    }
                    // Found as non flag but specified as flag
                    info->broad_type = VALUE;
                    if (colon_attribute)
                    {
                        return ';';
                    }
                    info->broad_type = FLAG_WITH_ARGO;
                    return -1;
                }
                if (option_type == SHORT_NO_VALUE)
                {   // Found as flag but specified as non flag
                    info->broad_type = FLAG;
                    if (colon_attribute)
                        return ':';
                    info->broad_type = VALUE_MISSINGO;
                    return -1;
                }
                // Found as non flag and specified as non flag
                info->broad_type = VALUE;
                return opt_char;
            }
            short_longopts:
            if (!longopts)
                goto short_exit;
            option_position = detail::search(longopts, opt_char);
            if (option_position >= 0)
            {
                info->long_index = option_position;
                if (detail::is_flag(longopts, option_position))
                {
                    if (option_type == SHORT_NO_VALUE)
                    {   // Found as flag and specified as flag
                        info->broad_type = FLAG;
                        return opt_char;
                    }
                    // Found as non flag but specified as flag
                    info->broad_type = FLAG_WITH_ARGL;
                    return -1;
                }
                if (option_type == SHORT_NO_VALUE)
                {   // Found as flag but specified as non flag
                    info->broad_type = FLAG;
                    if (detail::is_optarg(longopts, option_position))
                    {   // Found as flag and specified as optional value
                        return opt_char;
                    }
                    info->broad_type = VALUE_MISSINGL;
                    return -1;
                }
                // Found as non flag and specified as non flag
                info->broad_type = VALUE;
                return opt_char;
            }
            short_exit:
            // Unknown option found
            if (option_type == SHORT_NO_VALUE)
                info->broad_type = UNKNOWN_FLAG;
            else
                info->broad_type = UNKNOWN_VALUE;
            return '?';
        } break;
        case LONG_NO_VALUE:
            /* fall through */
        case LONG_OPTION:
            /* fall through */
        case PAIR_LONG:
            /* fall through */
        case PAIR_NO_VALUE_LONG:
        {   // Pair without a value is not a flag in any case
            if (!out_body)
            {
                info->broad_type = NOT_CLASSIFIED;
                return '?';
            }
            if (!longopts)
                goto long_exit;
            option_position = detail::search(longopts, out_body);
            if (option_position >= 0)
            {
                info->long_index = option_position;
                if (detail::is_flag(longopts, option_position))
                {
                    if (option_type == LONG_NO_VALUE)
                    {   // Found as flag and specified as flag
                        info->broad_type = FLAG;
                        return detail::get_short_form(longopts, option_position);
                    }
                    // Found as non flag but specified as flag
                    info->broad_type = FLAG_WITH_ARGL;
                    return -1;
                }
                if (option_type == LONG_NO_VALUE)
                {   // Found as flag but specified as non flag
                    info->broad_type = FLAG;
                    if (detail::is_optarg(longopts, option_position))
                    {   // Found as flag and specified as optional value
                        return opt_char;
                    }
                    info->broad_type = VALUE_MISSINGL;
                    return -1;
                }
                // Found as non flag and specified as non flag
                info->broad_type = VALUE;
                return detail::get_short_form(longopts, option_position);
            }
            long_exit:
            // Unknown option found
            if (option_type == LONG_NO_VALUE)
                info->broad_type = UNKNOWN_FLAG;
            else
                info->broad_type = UNKNOWN_VALUE;
            return '?';
        } break;
    }
    // Some exact type of option is not handleled
    info->broad_type = MISSED_EXACT;
    return -1;
}
} // namespace opt
#endif /* ARGUMENT_LIB_HPP_ */
//
// Copyright (c) 2019 Vinnie Falco (vinnie.falco@gmail.com)
// Copyright (c) 2020 Krystian Stasiowski (sdkrystian@gmail.com)
//
// Distributed under the Boost Software License, Version 1.0. (See accompanying
// file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//
// Official repository: https://github.com/cppalliance/json
//

#ifndef BOOST_JSON_IMPL_PARSE_IPP
#define BOOST_JSON_IMPL_PARSE_IPP

#include <boost/json/parse.hpp>
#include <boost/json/parser.hpp>
#include <boost/json/detail/except.hpp>

BOOST_JSON_NS_BEGIN

value
parse(
    string_view s,
    error_code& ec,
    storage_ptr sp,
    const parse_options& opt)
{
    unsigned char temp[BOOST_JSON_STACK_BUFFER_SIZE];
    parser p(
        storage_ptr(),
        opt,
        temp, sizeof(temp));
    p.reset(std::move(sp));
    p.write(
        s.data(),
        s.size(),
        ec);
    if(! ec)
        p.finish(ec);
    if(ec)
        return nullptr;
    return p.release();
}

value
parse(
    string_view s,
    storage_ptr sp,
    const parse_options& opt)
{
    error_code ec;
    auto jv = parse(
        s, ec, std::move(sp), opt);
    if(ec)
        detail::throw_system_error(ec,
            BOOST_CURRENT_LOCATION);
    return jv;
}

BOOST_JSON_NS_END

#endif

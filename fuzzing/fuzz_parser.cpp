//
// Copyright (c) 2019 Vinnie Falco (vinnie.falco@gmail.com)
//
// Distributed under the Boost Software License, Version 1.0. (See accompanying
// file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//
// Official repository: https://github.com/cppalliance/json
//

/*
    This example verifies that a file contains valid JSON.
    It is implementing by subclassing basic_parser 
*/

#include <boost/json.hpp>

// This file must be manually included when
// using basic_parser to implement a parser.
#include <boost/json/basic_parser.hpp>

using namespace boost::json;

bool fuzz_parser(string_view sv) {
parser p;
error_code ec;

// This must be called once before parsing every new JSON
p.reset();

// Write the first part of the buffer
p.write( sv.data(), sv.size(), ec);

// Write the remaining part of the character buffer,
// indicating to the parser that there is no more data.
//if(! ec )
//    p.write( "3,4,5]", 6, ec );
if(! ec)
    p.finish( ec );

// Take ownership of the resulting value.
if(! ec) {
    value jv = p.release();
    return jv.is_number();
    }
    return false;
}

extern "C" int LLVMFuzzerTestOneInput(const uint8_t* data, size_t size)
{
try {
   std::string_view view{reinterpret_cast<const char*>(data), size};
   fuzz_parser(view);
} catch (...) {
}
    return 0;
}


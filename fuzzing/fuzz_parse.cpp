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

bool fuzz_parse(string_view sv) {
error_code ec;
value jv = parse( sv, ec );
if(ec) {
return false;
} 
return jv.is_number();
}

extern "C" int LLVMFuzzerTestOneInput(const uint8_t* data, size_t size)
{
try {
   std::string_view view{reinterpret_cast<const char*>(data), size};
   fuzz_parse(view);
} catch (...) {
}
    return 0;
}


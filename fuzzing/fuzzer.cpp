// originally written by msimosson: https://github.com/vinniefalco/json/issues/13

#define BOOST_FALLTHROUGH [[fallthrough]]
#include "boost/json.hpp"

namespace
{
    bool
    validate( std::string_view s )
    {
        using namespace boost::json;

        // The null parser discards all the data

        struct null_parser : basic_parser
        {
            null_parser() {}
            ~null_parser() {}
            void on_document_begin( error_code& ) override {}
            void on_document_end( error_code& ) override {}
            void on_object_begin( error_code& ) override {}
            void on_object_end( error_code& ) override {}
            void on_array_begin( error_code& ) override {}
            void on_array_end( error_code& ) override {}
            void on_key_part( string_view, error_code& ) override {}
            void on_key( string_view, error_code& ) override {}
            void on_string_part( string_view, error_code& ) override {}
            void on_string( string_view, error_code& ) override {}
            void on_int64( std::int64_t, error_code& ) override {}
            void on_uint64( std::uint64_t, error_code& ) override {}
            void on_double( double, error_code& ) override {}
            void on_bool( bool, error_code& ) override {}
            void on_null( error_code& ) override {}
        };

        // Parse with the null parser and return false on error
        null_parser p;
        error_code ec;
        p.finish( s.data(), s.size(), ec );
        if( ec )
            return false;

        // The string is valid JSON.
        return true;
    }
}

extern "C" int LLVMFuzzerTestOneInput(const uint8_t* data, size_t size);
extern "C" int LLVMFuzzerTestOneInput(const uint8_t* data, size_t size)
{
try{
    validate(std::string_view{reinterpret_cast<const char*>(data), size});
}catch(...) {}
    return 0;
}



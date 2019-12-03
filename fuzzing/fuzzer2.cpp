// based on https://github.com/vinniefalco/json/blob/develop/example/pretty.cpp

#include "boost/json.hpp"
#include <iostream>

using namespace boost;

namespace {
void
pretty_print(std::ostream& os,
             json::value const& jv,
             std::string* indent = nullptr)
{
  std::string indent_;
  if (!indent)
    indent = &indent_;
  switch (jv.kind()) {
    case json::kind::object: {
      os << "{\n";
      indent->append(4, ' ');
      auto const& obj = jv.get_object();
      if (!obj.empty()) {
        auto it = obj.begin();
        for (;;) {
          os << *indent << json::to_string(it->key()) << " : ";
          pretty_print(os, it->value(), indent);
          if (++it == obj.end())
            break;
          os << ",\n";
        }
      }
      os << "\n";
      indent->resize(indent->size() - 4);
      os << *indent << "}";
      break;
    }

    case json::kind::array: {
      os << "[\n";
      indent->append(4, ' ');
      auto const& arr = jv.get_array();
      if (!arr.empty()) {
        auto it = arr.begin();
        for (;;) {
          os << *indent;
          pretty_print(os, *it, indent);
          if (++it == arr.end())
            break;
          os << ",\n";
        }
      }
      os << "\n";
      indent->resize(indent->size() - 4);
      os << *indent << "]";
      break;
    }

    case json::kind::string: {
      os << json::to_string(jv.get_string());
      break;
    }

    case json::kind::uint64:
      os << jv.get_uint64();
      break;

    case json::kind::int64:
      os << jv.get_int64();
      break;

    case json::kind::double_:
      os << jv.get_double();
      break;

    case json::kind::bool_:
      if (jv.get_bool())
        os << "true";
      else
        os << "false";
      break;

    case json::kind::null:
      os << "null";
      break;
  }

  if (indent->empty())
    os << "\n";
}

}

extern "C" int
LLVMFuzzerTestOneInput(const uint8_t* data, size_t size);
extern "C" int
LLVMFuzzerTestOneInput(const uint8_t* data, size_t size)
{
  try {
    json::parser p;
    p.start();
    p.write((const char*)data, size);
    p.finish();
    auto jv = p.release();
    pretty_print(std::cout, jv);
  } catch (...) {
  }
  return 0;
}

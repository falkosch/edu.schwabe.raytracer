#pragma once

#include <vectorization.h>

#include <functional>
#include <ostream>
#include <sstream>

namespace vectorization::test {
  struct MessageFormat {
    static std::wstring toString(const std::ostringstream &stringStream) {
      const auto message = stringStream.str();
      return std::wstring{message.cbegin(), message.cend()};
      // RETURN_WIDE_STRING(stringStream.str().c_str());
    }

    static std::wstring forFunction(
        const ASizeT index, const v_f32_4::ValueType value, const std::string &prefix, const v_f32_4::ValueType actual,
        const std::string &operatorText, const v_f32_4::ValueType expected
    ) {
      const auto messageStream = std::ostringstream{} << "[" << index << "] " << value << " " << prefix << ": "
                                                      << actual << " " << operatorText << " " << expected;
      return toString(messageStream);
    }

    static std::wstring
    forIndex(const ASizeT index, const std::function<std::ostream &(std::ostream &)> &writeMessageToStream) {
      auto stream = std::ostringstream() << "[" << index << "]";
      writeMessageToStream(stream);
      return toString(stream);
    }

    template <typename T>
    static std::wstring forIterationValue(
        const ASizeT index, const T testValue, const std::function<std::ostream &(std::ostream &)> &writeMessageToStream
    ) {
      return MessageFormat::forIndex(index, [&](auto &stream) -> auto & {
        return writeMessageToStream(stream << " is " << testValue);
      });
    }
  };
}

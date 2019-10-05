#pragma once

#include <vectorization.h>

#include <functional>
#include <ostream>
#include <sstream>

namespace vectorization
{
    namespace test
    {
        struct MessageFormat
        {
            static const std::wstring forFunction(
                const ASizeT index,
                const v_f32_4::ValueType value,
                const std::string & prefix,
                const v_f32_4::ValueType actual,
                const std::string & operatorText,
                const v_f32_4::ValueType expected
            ) {
                auto nmessage = (std::ostringstream() << "[" << index << "] " << value << " " << prefix << ": "
                    << actual << " " << operatorText << " " << expected).str();
                return std::wstring(nmessage.cbegin(), nmessage.cend());
            }

            static const std::wstring forIndex(
                const ASizeT index,
                std::function<std::ostream & (std::ostream &)> writeMessageToStream
            ) {
                auto stream = std::ostringstream() << "[" << index << "]";
                writeMessageToStream(stream);
                auto nmessage = stream.str();
                return std::wstring(nmessage.cbegin(), nmessage.cend());
            }

            template <typename T>
            static const std::wstring forIterationValue(
                const ASizeT index,
                const T testValue,
                std::function<std::ostream & (std::ostream &)> writeMessageToStream
            ) {
                return MessageFormat::forIndex(index, [&](auto & stream) -> auto & {
                    return writeMessageToStream(stream << " is " << testValue);
                });
            }
        };
    }
}

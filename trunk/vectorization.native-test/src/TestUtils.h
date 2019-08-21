#pragma once

#include <vectorization.h>

#include <functional>
#include <ostream>
#include <sstream>

namespace vectorization
{
	namespace test
	{
		struct TestUtils {

			static const std::wstring format(
				const ASizeT index,
				std::function<std::ostream & (std::ostream &)> StreamWriter
			) {
				auto stream = std::ostringstream();
				StreamWriter(stream << "test[index=" << index << "]");
				auto nmessage = stream.str();
				return std::wstring(nmessage.cbegin(), nmessage.cend());
			}

			template <typename T>
			static const std::wstring format(
				const ASizeT index,
				const T testValue,
				std::function<std::ostream & (std::ostream &)> StreamWriter
			) {
				return format(index, [&](auto & stream) -> auto & {
					return StreamWriter(stream << "==" << testValue);
				});
			}

			template <ASizeT exclusiveEnd>
			static void testEachComponent(
				std::function<void(const ASizeT index, std::wstring::pointer)> Tester,
				std::function<std::ostream & (std::ostream &)> MessageWriter
			) {
				staticFor<VectorIndices::X, exclusiveEnd>([&](auto i) {
					std::wstring message = TestUtils::format(i, MessageWriter);
					Tester(i, message.data());
				});
			}

			template <ASizeT exclusiveEnd>
			static void testEachComponent(
				std::function<void(const ASizeT index, std::wstring::pointer)> Tester
			) {
				testEachComponent<exclusiveEnd>(Tester, [&](auto & stream) -> auto & {
					return stream;
				});
			}

			template <ASizeT exclusiveEnd>
			static void testEachComponent(
				std::function<void(const ASizeT index, std::wstring::pointer)> Tester,
				std::string message
			) {
				testEachComponent<exclusiveEnd>(Tester, [&](auto & stream) -> auto & {
					return stream << " --- " << message;
				});
			}

		};
	}
}

#pragma once

#include <CppUnitTest.h>
#include <vectorization.h>
using namespace vectorization;

#include <vector>

namespace Microsoft
{
    namespace VisualStudio
    {
        namespace CppUnitTestFramework
        {
            template<> inline std::wstring ToString<UInt_16>(const UInt_16 & t) {
                RETURN_WIDE_STRING(t);
            }

            template<> inline std::wstring ToString<
                std::array<Float_32, VectorSizes::W>
            >(const std::array<Float_32, VectorSizes::W> & t) {
                RETURN_WIDE_STRING("["
                    << t.at(VectorIndices::X) << ","
                    << t.at(VectorIndices::Y) << ","
                    << t.at(VectorIndices::Z) << ","
                    << t.at(VectorIndices::W) << "]"
                );
            }

            template<> inline std::wstring ToString<
                std::array<BoolTypes<Float_32>::Type, VectorSizes::W>
            >(const std::array<BoolTypes<Float_32>::Type, VectorSizes::W> & t) {
                RETURN_WIDE_STRING("["
                    << t.at(VectorIndices::X) << ","
                    << t.at(VectorIndices::Y) << ","
                    << t.at(VectorIndices::Z) << ","
                    << t.at(VectorIndices::W) << "]"
                );
            }

            template<> inline std::wstring ToString<
                std::array<Float_32, VectorSizes::X8>
            >(const std::array<Float_32, VectorSizes::X8> & t) {
                RETURN_WIDE_STRING("["
                    << t.at(VectorIndices::X1) << ","
                    << t.at(VectorIndices::X2) << ","
                    << t.at(VectorIndices::X3) << ","
                    << t.at(VectorIndices::X4) << ","
                    << t.at(VectorIndices::X5) << ","
                    << t.at(VectorIndices::X6) << ","
                    << t.at(VectorIndices::X7) << ","
                    << t.at(VectorIndices::X8) << "]"
                );
            }

            template<> inline std::wstring ToString<
                std::array<BoolTypes<Float_32>::Type, VectorSizes::X8>
            >(const std::array<BoolTypes<Float_32>::Type, VectorSizes::X8> & t) {
                RETURN_WIDE_STRING("["
                    << t.at(VectorIndices::X1) << ","
                    << t.at(VectorIndices::X2) << ","
                    << t.at(VectorIndices::X3) << ","
                    << t.at(VectorIndices::X4) << ","
                    << t.at(VectorIndices::X5) << ","
                    << t.at(VectorIndices::X6) << ","
                    << t.at(VectorIndices::X7) << ","
                    << t.at(VectorIndices::X8) << "]"
                );
            }

            template<> inline std::wstring ToString<
                std::array<Float_64, VectorSizes::Y>
            >(const std::array<Float_64, VectorSizes::Y> & t) {
                RETURN_WIDE_STRING("["
                    << t.at(VectorIndices::X) << ","
                    << t.at(VectorIndices::Y) << "]"
                );
            }

            template<> inline std::wstring ToString<
                std::array<BoolTypes<Float_64>::Type, VectorSizes::Y>
            >(const std::array<BoolTypes<Float_64>::Type, VectorSizes::Y> & t) {
                RETURN_WIDE_STRING("["
                    << t.at(VectorIndices::X) << ","
                    << t.at(VectorIndices::Y) << "]"
                );
            }

            template<> inline std::wstring ToString<
                std::array<Float_64, VectorSizes::W>
            >(const std::array<Float_64, VectorSizes::W> & t) {
                RETURN_WIDE_STRING("["
                    << t.at(VectorIndices::X) << ","
                    << t.at(VectorIndices::Y) << ","
                    << t.at(VectorIndices::Z) << ","
                    << t.at(VectorIndices::W) << "]"
                );
            }

            template<> inline std::wstring ToString<
                std::array<BoolTypes<Float_64>::Type, VectorSizes::W>
            >(const std::array<BoolTypes<Float_64>::Type, VectorSizes::W> & t) {
                RETURN_WIDE_STRING("["
                    << t.at(VectorIndices::X) << ","
                    << t.at(VectorIndices::Y) << ","
                    << t.at(VectorIndices::Z) << ","
                    << t.at(VectorIndices::W) << "]"
                );
            }

            template<> inline std::wstring ToString<
                std::vector<Float_32>
            >(const std::vector<Float_32> & t) {
                auto _s = std::wstringstream{ } << "[";
                for (auto iter{ t.cbegin() }; iter < t.cend(); iter++) {
                    if (iter == t.cbegin()) {
                        _s << *iter;
                    } else {
                        _s << "," << *iter;
                    }
                }
                _s << "]";
                return _s.str();
            }

            template<> inline std::wstring ToString<
                std::vector<Float_64>
            >(const std::vector<Float_64> & t) {
                auto _s = std::wstringstream{ } << "[";
                for (auto iter{ t.cbegin() }; iter < t.cend(); iter++) {
                    if (iter == t.cbegin()) {
                        _s << *iter;
                    } else {
                        _s << "," << *iter;
                    }
                }
                _s << "]";
                return _s.str();
            }
        }
    }
}

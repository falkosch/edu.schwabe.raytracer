#pragma once

#include <CppUnitTest.h>
#include <vectorization.h>

#include <vector>

namespace Microsoft
{
    namespace VisualStudio
    {
        namespace CppUnitTestFramework
        {
            template<> inline std::wstring ToString<vectorization::UInt_16>(const vectorization::UInt_16 & t) {
                RETURN_WIDE_STRING(t);
            }

            template<> inline std::wstring ToString<
                std::array<vectorization::Float_32, vectorization::VectorSizes::W>
            >(const std::array<vectorization::Float_32, vectorization::VectorSizes::W> & t) {
                RETURN_WIDE_STRING("["
                    << t.at(vectorization::VectorIndices::X) << ","
                    << t.at(vectorization::VectorIndices::Y) << ","
                    << t.at(vectorization::VectorIndices::Z) << ","
                    << t.at(vectorization::VectorIndices::W) << "]"
                );
            }

            template<> inline std::wstring ToString<
                std::array<vectorization::Float_32, vectorization::VectorSizes::X8>
            >(const std::array<vectorization::Float_32, vectorization::VectorSizes::X8> & t) {
                RETURN_WIDE_STRING("["
                    << t.at(vectorization::VectorIndices::X1) << ","
                    << t.at(vectorization::VectorIndices::X2) << ","
                    << t.at(vectorization::VectorIndices::X3) << ","
                    << t.at(vectorization::VectorIndices::X4) << ","
                    << t.at(vectorization::VectorIndices::X5) << ","
                    << t.at(vectorization::VectorIndices::X6) << ","
                    << t.at(vectorization::VectorIndices::X7) << ","
                    << t.at(vectorization::VectorIndices::X8) << "]"
                );
            }

            template<> inline std::wstring ToString<
                std::array<vectorization::Float_64, vectorization::VectorSizes::Y>
            >(const std::array<vectorization::Float_64, vectorization::VectorSizes::Y> & t) {
                RETURN_WIDE_STRING("["
                    << t.at(vectorization::VectorIndices::X) << ","
                    << t.at(vectorization::VectorIndices::Y) << "]"
                );
            }

            template<> inline std::wstring ToString<
                std::array<vectorization::Float_64, vectorization::VectorSizes::W>
            >(const std::array<vectorization::Float_64, vectorization::VectorSizes::W> & t) {
                RETURN_WIDE_STRING("["
                    << t.at(vectorization::VectorIndices::X) << ","
                    << t.at(vectorization::VectorIndices::Y) << ","
                    << t.at(vectorization::VectorIndices::Z) << ","
                    << t.at(vectorization::VectorIndices::W) << "]"
                );
            }

            template<> inline std::wstring ToString<
                std::vector<vectorization::Float_32>
            >(const std::vector<vectorization::Float_32> & t) {
                auto _s = std::wstringstream{} << "[";
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
                std::vector<vectorization::Float_64>
            >(const std::vector<vectorization::Float_64> & t) {
                auto _s = std::wstringstream{} << "[";
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

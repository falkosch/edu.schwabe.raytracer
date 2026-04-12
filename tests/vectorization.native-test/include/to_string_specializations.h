#pragma once

#include <CppUnitTest.h>
#include <vectorization.h>
using namespace vectorization;

#include <vector>

namespace Microsoft::VisualStudio::CppUnitTestFramework {
  // template<> inline std::wstring ToString<UInt_16>(const UInt_16 & t) {
  //     RETURN_WIDE_STRING(t);
  // }

  template <>
  inline std::wstring ToString<std::array<Float_32, VectorSizes::W>>(const std::array<Float_32, VectorSizes::W> &q) {
    RETURN_WIDE_STRING(
        "[" << q.at(VectorIndices::X) << "," << q.at(VectorIndices::Y) << "," << q.at(VectorIndices::Z) << ","
            << q.at(VectorIndices::W) << "]"
    );
  }

  template <>
  inline std::wstring ToString<std::array<BoolTypes<Float_32>::Type, VectorSizes::W>>(
      const std::array<BoolTypes<Float_32>::Type, VectorSizes::W> &q
  ) {
    RETURN_WIDE_STRING(
        "[" << q.at(VectorIndices::X) << "," << q.at(VectorIndices::Y) << "," << q.at(VectorIndices::Z) << ","
            << q.at(VectorIndices::W) << "]"
    );
  }

  template <>
  inline std::wstring ToString<std::array<Float_32, VectorSizes::X8>>(const std::array<Float_32, VectorSizes::X8> &q) {
    RETURN_WIDE_STRING(
        "[" << q.at(VectorIndices::X1) << "," << q.at(VectorIndices::X2) << "," << q.at(VectorIndices::X3) << ","
            << q.at(VectorIndices::X4) << "," << q.at(VectorIndices::X5) << "," << q.at(VectorIndices::X6) << ","
            << q.at(VectorIndices::X7) << "," << q.at(VectorIndices::X8) << "]"
    );
  }

  template <>
  inline std::wstring ToString<std::array<BoolTypes<Float_32>::Type, VectorSizes::X8>>(
      const std::array<BoolTypes<Float_32>::Type, VectorSizes::X8> &q
  ) {
    RETURN_WIDE_STRING(
        "[" << q.at(VectorIndices::X1) << "," << q.at(VectorIndices::X2) << "," << q.at(VectorIndices::X3) << ","
            << q.at(VectorIndices::X4) << "," << q.at(VectorIndices::X5) << "," << q.at(VectorIndices::X6) << ","
            << q.at(VectorIndices::X7) << "," << q.at(VectorIndices::X8) << "]"
    );
  }

  template <>
  inline std::wstring ToString<std::array<Float_64, VectorSizes::Y>>(const std::array<Float_64, VectorSizes::Y> &q) {
    RETURN_WIDE_STRING("[" << q.at(VectorIndices::X) << "," << q.at(VectorIndices::Y) << "]");
  }

  template <>
  inline std::wstring ToString<std::array<BoolTypes<Float_64>::Type, VectorSizes::Y>>(
      const std::array<BoolTypes<Float_64>::Type, VectorSizes::Y> &q
  ) {
    RETURN_WIDE_STRING("[" << q.at(VectorIndices::X) << "," << q.at(VectorIndices::Y) << "]");
  }

  template <>
  inline std::wstring ToString<std::array<Float_64, VectorSizes::W>>(const std::array<Float_64, VectorSizes::W> &q) {
    RETURN_WIDE_STRING(
        "[" << q.at(VectorIndices::X) << "," << q.at(VectorIndices::Y) << "," << q.at(VectorIndices::Z) << ","
            << q.at(VectorIndices::W) << "]"
    );
  }

  template <>
  inline std::wstring ToString<std::array<BoolTypes<Float_64>::Type, VectorSizes::W>>(
      const std::array<BoolTypes<Float_64>::Type, VectorSizes::W> &q
  ) {
    RETURN_WIDE_STRING(
        "[" << q.at(VectorIndices::X) << "," << q.at(VectorIndices::Y) << "," << q.at(VectorIndices::Z) << ","
            << q.at(VectorIndices::W) << "]"
    );
  }

  template <>
  inline std::wstring ToString<std::vector<Float_32>>(const std::vector<Float_32> &q) {
    auto _s = std::wstringstream{} << "[";
    for (auto iter{q.cbegin()}; iter < q.cend(); ++iter) {
      if (iter == q.cbegin()) {
        _s << *iter;
      } else {
        _s << "," << *iter;
      }
    }
    _s << "]";
    return _s.str();
  }

  template <>
  inline std::wstring ToString<std::vector<Float_64>>(const std::vector<Float_64> &q) {
    auto _s = std::wstringstream{} << "[";
    for (auto iter{q.cbegin()}; iter < q.cend(); ++iter) {
      if (iter == q.cbegin()) {
        _s << *iter;
      } else {
        _s << "," << *iter;
      }
    }
    _s << "]";
    return _s.str();
  }
}

#pragma once

#include "raytracing_tests.h"

#include <cstdint>
#include <filesystem>
#include <string>
#include <system_error>

namespace raytracer::test {
  // A file path in the temp directory, unique per instance; the file is removed on destruction.
  struct TempFile {
    std::filesystem::path path;

    explicit TempFile(const std::string& suffix) {
      path = std::filesystem::temp_directory_path()
             / ("raytracing_test_" + std::to_string(reinterpret_cast<std::uintptr_t>(this)) + suffix);
    }
    ~TempFile() {
      std::error_code ec;
      std::filesystem::remove(path, ec);
    }
    TempFile(const TempFile&) = delete;
    TempFile& operator=(const TempFile&) = delete;

    std::string str() const { return path.string(); }
  };
}

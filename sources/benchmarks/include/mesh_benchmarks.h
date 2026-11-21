#pragma once

#include <vectorization.h>

#include <string>
#include <vector>

namespace benchmarks
{
  using namespace vectorization;

  // KD-tree traversal + leaf intersection benchmarks on real meshes. File names are relative to the working directory
  // (the data/ folder); missing files are skipped with a warning.
  void runMeshBenchmarks(const std::vector<std::string>& meshFiles, ASizeT repetitions);
}

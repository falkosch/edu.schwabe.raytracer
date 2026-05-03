#include "vectorization_tests.h"

namespace vectorization::test {
  TEST_CLASS(v_f32_8_ConversionsTest) {
  public:
    TEST_METHOD(float8AliasMatchesType) {
      Assert::IsTrue(std::is_same_v<Float8_32, v_f32_8>, L"Float8_32 alias", LINE_INFO());
      Assert::IsTrue(std::is_same_v<Float8, v_f32_8>, L"Float8 alias", LINE_INFO());
    }
  };
}

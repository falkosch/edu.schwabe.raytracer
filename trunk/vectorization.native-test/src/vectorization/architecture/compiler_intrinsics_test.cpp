#include <CppUnitTest.h>
#include <vectorization.h>

#include <array>

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace vectorization
{
	namespace test
	{

		TEST_CLASS(CompilerIntrinsicsTest)
		{
		public:

			TEST_METHOD(testGeneratesMasksForFloat4_32)
			{
				Assert::AreEqual(0 + 0 + 0 + 0,
					BLEND_MASK_FLOAT4_32(0, 0, 0, 0), L" 0 != MASK(0,0,0,0)", LINE_INFO());

				Assert::AreEqual(1 + 0 + 4 + 0,
					BLEND_MASK_FLOAT4_32(1, 0, 1, 0), L" 5 != MASK(1,0,1,0)", LINE_INFO());

				Assert::AreEqual(0 + 2 + 0 + 8,
					BLEND_MASK_FLOAT4_32(0, 1, 0, 1), L"10 != MASK(0,1,0,1)", LINE_INFO());

				Assert::AreEqual(1 + 2 + 4 + 8,
					BLEND_MASK_FLOAT4_32(1, 1, 1, 1), L"15 != MASK(1,1,1,1)", LINE_INFO());
			}

			TEST_METHOD(testGeneratesMasksForInt4_32)
			{
				Assert::AreEqual(0 + 0 + 0 + 0 + 0 + 0 + 0 + 0,
					BLEND_MASK_INT4_32(0, 0, 0, 0), L"  0 != MASK(0,0,0,0)", LINE_INFO());

				Assert::AreEqual(1 + 2 + 0 + 0 + 16 + 32 + 0 + 0,
					BLEND_MASK_INT4_32(1, 0, 1, 0), L" 51 != MASK(1,0,1,0)", LINE_INFO());

				Assert::AreEqual(0 + 0 + 4 + 8 + 0 + 0 + 64 + 128,
					BLEND_MASK_INT4_32(0, 1, 0, 1), L"204 != MASK(0,1,0,1)", LINE_INFO());

				Assert::AreEqual(1 + 2 + 4 + 8 + 16 + 32 + 64 + 128,
					BLEND_MASK_INT4_32(1, 1, 1, 1), L"255 != MASK(1,1,1,1)", LINE_INFO());
			}

			TEST_METHOD(testConvertsUnsignedIntegersToFloats)
			{
				std::array<vectorization::UInt_32, 5> specimen = { 0, 1, 2, 3, 4 };

				for (const auto & v : specimen) {
					auto test = _mm_set1_epi32(v);
					auto actual = vectorization::_mm_cvtepu32_ps(test);
					auto expected = _mm_set1_ps(static_cast<vectorization::Float_32>(v));
					Assert::AreEqual(1, _mm_comieq_ss(expected, actual), L"Conversion error in _mm_cvtepu32_ps", LINE_INFO());
				}
			}

		};

	}
}
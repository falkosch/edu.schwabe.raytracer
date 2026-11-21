#include "raytracing_tests.h"
#include "raytracing/shading/spectral/table_lookup.h"

#include <vectorization.h>

namespace raytracer::test
{
    using namespace raytracer::spectral;

    // 5 entries on the grid 400, 410, ..., 440nm
    static constexpr Float TABLE[] = {1.0f, 3.0f, 7.0f, 2.0f, 4.0f};
    static constexpr Float TABLE_MIN = 400.0f;
    static constexpr Float TABLE_STEP = 10.0f;
    static constexpr ASizeT TABLE_SIZE = 5;

    static Float8 lookup(const Float8& wavelengths)
    {
        return lookupTable(TABLE, tableCoordinates(wavelengths, TABLE_MIN, TABLE_STEP, TABLE_SIZE));
    }

    TEST_CLASS (TableLookupTest)
    {
        public
        :
        TEST_METHOD(returnsTableEntriesAtGridPoints)
        {
            const auto result = lookup(Float8{400.0f, 410.0f, 420.0f, 430.0f, 440.0f, 400.0f, 410.0f, 420.0f});
            Assert::AreEqual(1.0f, x1(result), 1e-6f, L"400nm", LINE_INFO());
            Assert::AreEqual(3.0f, x2(result), 1e-6f, L"410nm", LINE_INFO());
            Assert::AreEqual(7.0f, x3(result), 1e-6f, L"420nm", LINE_INFO());
            Assert::AreEqual(2.0f, x4(result), 1e-6f, L"430nm", LINE_INFO());
            Assert::AreEqual(4.0f, x5(result), 1e-6f, L"440nm, the last entry", LINE_INFO());
        }

        TEST_METHOD(interpolatesLinearlyBetweenGridPoints)
        {
            const auto result = lookup(Float8{405.0f, 412.5f, 437.5f, 425.0f, 405.0f, 405.0f, 405.0f, 405.0f});
            Assert::AreEqual(2.0f, x1(result), 1e-5f, L"halfway between 1 and 3", LINE_INFO());
            Assert::AreEqual(4.0f, x2(result), 1e-5f, L"a quarter from 3 to 7", LINE_INFO());
            Assert::AreEqual(3.5f, x3(result), 1e-5f, L"three quarters from 2 to 4", LINE_INFO());
            Assert::AreEqual(4.5f, x4(result), 1e-5f, L"halfway between 7 and 2", LINE_INFO());
        }

        TEST_METHOD(clampsWavelengthsOutsideTheGrid)
        {
            const auto result = lookup(Float8{380.0f, 399.0f, 441.0f, 700.0f, 400.0f, 400.0f, 400.0f, 400.0f});
            Assert::AreEqual(1.0f, x1(result), 1e-6f, L"far below: first entry", LINE_INFO());
            Assert::AreEqual(1.0f, x2(result), 1e-6f, L"just below: first entry", LINE_INFO());
            Assert::AreEqual(4.0f, x3(result), 1e-6f, L"just above: last entry", LINE_INFO());
            Assert::AreEqual(4.0f, x4(result), 1e-6f, L"far above: last entry", LINE_INFO());
        }
    };
}

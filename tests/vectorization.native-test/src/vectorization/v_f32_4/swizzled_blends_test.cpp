#include "vectorization_tests.h"

namespace vectorization::test {
  TEST_CLASS(v_f32_4_SwizzledBlendsTest) {
  public:
    TEST_METHOD(testSwizzledBlend2x2Identity) {
      const v_f32_4 a{1.0f, 2.0f, 3.0f, 4.0f};
      const v_f32_4 b{5.0f, 6.0f, 7.0f, 8.0f};

      const auto actual =
          swizzledBlend2x2<VectorIndices::X, VectorIndices::Y, VectorIndices::Z, VectorIndices::W>(a, b);

      Assert::AreEqual(1.0f, x(actual), L"X mismatch", LINE_INFO());
      Assert::AreEqual(2.0f, y(actual), L"Y mismatch", LINE_INFO());
      Assert::AreEqual(7.0f, z(actual), L"Z mismatch", LINE_INFO());
      Assert::AreEqual(8.0f, w(actual), L"W mismatch", LINE_INFO());
    }

    TEST_METHOD(testSwizzledBlend2x2Reverse) {
      const v_f32_4 a{1.0f, 2.0f, 3.0f, 4.0f};
      const v_f32_4 b{5.0f, 6.0f, 7.0f, 8.0f};

      const auto actual =
          swizzledBlend2x2<VectorIndices::W, VectorIndices::Z, VectorIndices::Y, VectorIndices::X>(a, b);

      Assert::AreEqual(4.0f, x(actual), L"X mismatch", LINE_INFO());
      Assert::AreEqual(3.0f, y(actual), L"Y mismatch", LINE_INFO());
      Assert::AreEqual(6.0f, z(actual), L"Z mismatch", LINE_INFO());
      Assert::AreEqual(5.0f, w(actual), L"W mismatch", LINE_INFO());
    }

    TEST_METHOD(testNamedSwizzledBlends) {
      const v_f32_4 a{1.0f, 2.0f, 3.0f, 4.0f};
      const v_f32_4 b{5.0f, 6.0f, 7.0f, 8.0f};

      // xx_xx: {a.x, a.x, b.x, b.x}
      {
        const auto actual = xx_xx(a, b);
        Assert::AreEqual(1.0f, x(actual), L"xx_xx X mismatch", LINE_INFO());
        Assert::AreEqual(1.0f, y(actual), L"xx_xx Y mismatch", LINE_INFO());
        Assert::AreEqual(5.0f, z(actual), L"xx_xx Z mismatch", LINE_INFO());
        Assert::AreEqual(5.0f, w(actual), L"xx_xx W mismatch", LINE_INFO());
      }

      // xx_yy: {a.x, a.x, b.y, b.y}
      {
        const auto actual = xx_yy(a, b);
        Assert::AreEqual(1.0f, x(actual), L"xx_yy X mismatch", LINE_INFO());
        Assert::AreEqual(1.0f, y(actual), L"xx_yy Y mismatch", LINE_INFO());
        Assert::AreEqual(6.0f, z(actual), L"xx_yy Z mismatch", LINE_INFO());
        Assert::AreEqual(6.0f, w(actual), L"xx_yy W mismatch", LINE_INFO());
      }

      // xy_xy: {a.x, a.y, b.x, b.y}
      {
        const auto actual = xy_xy(a, b);
        Assert::AreEqual(1.0f, x(actual), L"xy_xy X mismatch", LINE_INFO());
        Assert::AreEqual(2.0f, y(actual), L"xy_xy Y mismatch", LINE_INFO());
        Assert::AreEqual(5.0f, z(actual), L"xy_xy Z mismatch", LINE_INFO());
        Assert::AreEqual(6.0f, w(actual), L"xy_xy W mismatch", LINE_INFO());
      }

      // xy_zw: {a.x, a.y, b.z, b.w}
      {
        const auto actual = xy_zw(a, b);
        Assert::AreEqual(1.0f, x(actual), L"xy_zw X mismatch", LINE_INFO());
        Assert::AreEqual(2.0f, y(actual), L"xy_zw Y mismatch", LINE_INFO());
        Assert::AreEqual(7.0f, z(actual), L"xy_zw Z mismatch", LINE_INFO());
        Assert::AreEqual(8.0f, w(actual), L"xy_zw W mismatch", LINE_INFO());
      }

      // xz_xz: {a.x, a.z, b.x, b.z}
      {
        const auto actual = xz_xz(a, b);
        Assert::AreEqual(1.0f, x(actual), L"xz_xz X mismatch", LINE_INFO());
        Assert::AreEqual(3.0f, y(actual), L"xz_xz Y mismatch", LINE_INFO());
        Assert::AreEqual(5.0f, z(actual), L"xz_xz Z mismatch", LINE_INFO());
        Assert::AreEqual(7.0f, w(actual), L"xz_xz W mismatch", LINE_INFO());
      }

      // yx_xy: {a.y, a.x, b.x, b.y}
      {
        const auto actual = yx_xy(a, b);
        Assert::AreEqual(2.0f, x(actual), L"yx_xy X mismatch", LINE_INFO());
        Assert::AreEqual(1.0f, y(actual), L"yx_xy Y mismatch", LINE_INFO());
        Assert::AreEqual(5.0f, z(actual), L"yx_xy Z mismatch", LINE_INFO());
        Assert::AreEqual(6.0f, w(actual), L"yx_xy W mismatch", LINE_INFO());
      }

      // yy_yy: {a.y, a.y, b.y, b.y}
      {
        const auto actual = yy_yy(a, b);
        Assert::AreEqual(2.0f, x(actual), L"yy_yy X mismatch", LINE_INFO());
        Assert::AreEqual(2.0f, y(actual), L"yy_yy Y mismatch", LINE_INFO());
        Assert::AreEqual(6.0f, z(actual), L"yy_yy Z mismatch", LINE_INFO());
        Assert::AreEqual(6.0f, w(actual), L"yy_yy W mismatch", LINE_INFO());
      }

      // yw_yw: {a.y, a.w, b.y, b.w}
      {
        const auto actual = yw_yw(a, b);
        Assert::AreEqual(2.0f, x(actual), L"yw_yw X mismatch", LINE_INFO());
        Assert::AreEqual(4.0f, y(actual), L"yw_yw Y mismatch", LINE_INFO());
        Assert::AreEqual(6.0f, z(actual), L"yw_yw Z mismatch", LINE_INFO());
        Assert::AreEqual(8.0f, w(actual), L"yw_yw W mismatch", LINE_INFO());
      }

      // zz_yy: {a.z, a.z, b.y, b.y}
      {
        const auto actual = zz_yy(a, b);
        Assert::AreEqual(3.0f, x(actual), L"zz_yy X mismatch", LINE_INFO());
        Assert::AreEqual(3.0f, y(actual), L"zz_yy Y mismatch", LINE_INFO());
        Assert::AreEqual(6.0f, z(actual), L"zz_yy Z mismatch", LINE_INFO());
        Assert::AreEqual(6.0f, w(actual), L"zz_yy W mismatch", LINE_INFO());
      }

      // zz_zz: {a.z, a.z, b.z, b.z}
      {
        const auto actual = zz_zz(a, b);
        Assert::AreEqual(3.0f, x(actual), L"zz_zz X mismatch", LINE_INFO());
        Assert::AreEqual(3.0f, y(actual), L"zz_zz Y mismatch", LINE_INFO());
        Assert::AreEqual(7.0f, z(actual), L"zz_zz Z mismatch", LINE_INFO());
        Assert::AreEqual(7.0f, w(actual), L"zz_zz W mismatch", LINE_INFO());
      }

      // zw_zw: {a.z, a.w, b.z, b.w}
      {
        const auto actual = zw_zw(a, b);
        Assert::AreEqual(3.0f, x(actual), L"zw_zw X mismatch", LINE_INFO());
        Assert::AreEqual(4.0f, y(actual), L"zw_zw Y mismatch", LINE_INFO());
        Assert::AreEqual(7.0f, z(actual), L"zw_zw Z mismatch", LINE_INFO());
        Assert::AreEqual(8.0f, w(actual), L"zw_zw W mismatch", LINE_INFO());
      }

      // ww_ww: {a.w, a.w, b.w, b.w}
      {
        const auto actual = ww_ww(a, b);
        Assert::AreEqual(4.0f, x(actual), L"ww_ww X mismatch", LINE_INFO());
        Assert::AreEqual(4.0f, y(actual), L"ww_ww Y mismatch", LINE_INFO());
        Assert::AreEqual(8.0f, z(actual), L"ww_ww Z mismatch", LINE_INFO());
        Assert::AreEqual(8.0f, w(actual), L"ww_ww W mismatch", LINE_INFO());
      }
    }

    TEST_METHOD(testSwizzledBlendMasked) {
      const v_f32_4 a{1.0f, 2.0f, 3.0f, 4.0f};
      const v_f32_4 b{5.0f, 6.0f, 7.0f, 8.0f};

      // Identity swizzle with Zero mask: selects all from swizzled a
      {
        const auto actual = swizzledBlendMasked<VectorIndices::X, VectorIndices::Y, VectorIndices::Z, VectorIndices::W>(
            a, b, Zero<v_f32_4>()
        );
        Assert::AreEqual(1.0f, x(actual), L"Zero mask X mismatch", LINE_INFO());
        Assert::AreEqual(2.0f, y(actual), L"Zero mask Y mismatch", LINE_INFO());
        Assert::AreEqual(3.0f, z(actual), L"Zero mask Z mismatch", LINE_INFO());
        Assert::AreEqual(4.0f, w(actual), L"Zero mask W mismatch", LINE_INFO());
      }

      // Identity swizzle with MaskAll: selects all from swizzled b
      {
        const auto actual = swizzledBlendMasked<VectorIndices::X, VectorIndices::Y, VectorIndices::Z, VectorIndices::W>(
            a, b, MaskAll<v_f32_4>()
        );
        Assert::AreEqual(5.0f, x(actual), L"MaskAll X mismatch", LINE_INFO());
        Assert::AreEqual(6.0f, y(actual), L"MaskAll Y mismatch", LINE_INFO());
        Assert::AreEqual(7.0f, z(actual), L"MaskAll Z mismatch", LINE_INFO());
        Assert::AreEqual(8.0f, w(actual), L"MaskAll W mismatch", LINE_INFO());
      }

      // Identity swizzle with MaskX: selects X from b, rest from a
      {
        const auto actual = swizzledBlendMasked<VectorIndices::X, VectorIndices::Y, VectorIndices::Z, VectorIndices::W>(
            a, b, MaskX<v_f32_4>()
        );
        Assert::AreEqual(5.0f, x(actual), L"MaskX X mismatch", LINE_INFO());
        Assert::AreEqual(2.0f, y(actual), L"MaskX Y mismatch", LINE_INFO());
        Assert::AreEqual(3.0f, z(actual), L"MaskX Z mismatch", LINE_INFO());
        Assert::AreEqual(4.0f, w(actual), L"MaskX W mismatch", LINE_INFO());
      }

      // Reverse swizzle with MaskAll: selects all from swizzled b (reversed)
      {
        const auto actual = swizzledBlendMasked<VectorIndices::W, VectorIndices::Z, VectorIndices::Y, VectorIndices::X>(
            a, b, MaskAll<v_f32_4>()
        );
        Assert::AreEqual(8.0f, x(actual), L"Reverse MaskAll X mismatch", LINE_INFO());
        Assert::AreEqual(7.0f, y(actual), L"Reverse MaskAll Y mismatch", LINE_INFO());
        Assert::AreEqual(6.0f, z(actual), L"Reverse MaskAll Z mismatch", LINE_INFO());
        Assert::AreEqual(5.0f, w(actual), L"Reverse MaskAll W mismatch", LINE_INFO());
      }
    }
  };
}

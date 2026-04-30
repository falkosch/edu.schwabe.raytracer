#include "raytracing_tests.h"

#include <cmath>

namespace raytracer::test {
  // Schlick-Fresnel approximation reproduced from raytracing/Raytracer.cpp::schlickFresnel.
  // The production helper is file-local (no header export); these tests lock in the formula by
  // computing it from the same public primitives. Eta layout: (etaA/etaB, etaB/etaA, etaA, etaB).
  static Float schlickFresnelDenseToLessDense(const Float negNdotI, const Float etaA, const Float etaB) {
    const Float etaItoT = etaA / etaB;
    const Float cosSqrPhiT = 1.0f - (etaItoT * etaItoT - etaItoT * negNdotI * etaItoT * negNdotI);
    const Float cosPhi = std::sqrt(std::max(0.0f, cosSqrPhiT));
    const Float sqrtR0 = (etaA - etaB) / (etaA + etaB);
    const Float R0 = sqrtR0 * sqrtR0;
    const Float oneSubCosPhi = 1.0f - cosPhi;
    return R0 + (1.0f - R0) * oneSubCosPhi * oneSubCosPhi * oneSubCosPhi * oneSubCosPhi * oneSubCosPhi;
  }

  static Float schlickFresnelLessDenseToDense(const Float negNdotI, const Float etaA, const Float etaB) {
    const Float cosPhi = std::max(0.0f, negNdotI);
    const Float sqrtR0 = (etaA - etaB) / (etaA + etaB);
    const Float R0 = sqrtR0 * sqrtR0;
    const Float oneSubCosPhi = 1.0f - cosPhi;
    return R0 + (1.0f - R0) * oneSubCosPhi * oneSubCosPhi * oneSubCosPhi * oneSubCosPhi * oneSubCosPhi;
  }

  TEST_CLASS(FresnelTest) {
  public:
    TEST_METHOD(schlickAtNormalIncidenceEqualsR0AirToGlass) {
      const Float etaAir = 1.0f;
      const Float etaGlass = 1.5f;
      const Float r = schlickFresnelLessDenseToDense(1.0f, etaAir, etaGlass);
      const Float expectedR0 = ((etaAir - etaGlass) / (etaAir + etaGlass));
      const Float expectedR0Sq = expectedR0 * expectedR0;

      Assert::AreEqual(expectedR0Sq, r, 1e-6f, L"normal incidence should equal R0", LINE_INFO());
    }

    TEST_METHOD(schlickAtGrazingAngleApproachesOne) {
      // negNdotI -> 0 means grazing
      const Float r = schlickFresnelLessDenseToDense(0.0f, 1.0f, 1.5f);

      Assert::AreEqual(1.0f, r, 1e-5f, L"grazing angle should approach 1.0", LINE_INFO());
    }

    TEST_METHOD(schlickIsMonotonicAsAngleIncreases) {
      // As negNdotI decreases (angle from normal grows), reflectance must grow
      const Float r0 = schlickFresnelLessDenseToDense(1.0f, 1.0f, 1.5f);
      const Float r1 = schlickFresnelLessDenseToDense(0.7f, 1.0f, 1.5f);
      const Float r2 = schlickFresnelLessDenseToDense(0.3f, 1.0f, 1.5f);
      const Float r3 = schlickFresnelLessDenseToDense(0.05f, 1.0f, 1.5f);

      Assert::IsTrue(r0 <= r1, L"r should grow as angle increases (0 -> 0.7)", LINE_INFO());
      Assert::IsTrue(r1 <= r2, L"r should grow as angle increases (0.7 -> 0.3)", LINE_INFO());
      Assert::IsTrue(r2 <= r3, L"r should grow as angle increases (0.3 -> 0.05)", LINE_INFO());
    }

    TEST_METHOD(schlickReflectanceInUnitRange) {
      for (Float negNdotI = 0.0f; negNdotI <= 1.0f; negNdotI += 0.1f) {
        const Float r = schlickFresnelLessDenseToDense(negNdotI, 1.0f, 1.5f);
        Assert::IsTrue(r >= 0.0f && r <= 1.0f, L"reflectance must be in [0,1]", LINE_INFO());
      }
    }

    TEST_METHOD(schlickWaterAirR0) {
      // Water R0 ~ ((1.333 - 1) / (1.333 + 1))^2 ~ 0.02037
      const Float r = schlickFresnelLessDenseToDense(1.0f, 1.0f, 1.333f);
      Assert::AreEqual(0.02037f, r, 0.0001f, L"water-air R0", LINE_INFO());
    }

    TEST_METHOD(schlickGlassAirR0) {
      // Glass R0 ~ ((1.0 - 1.5) / (1.0 + 1.5))^2 = 0.04
      const Float r = schlickFresnelLessDenseToDense(1.0f, 1.0f, 1.5f);
      Assert::AreEqual(0.04f, r, 1e-6f, L"glass-air R0", LINE_INFO());
    }

    TEST_METHOD(schlickEnteringLessDenseUsesSnellsLaw) {
      // Going from glass (1.5) -> air (1.0): less dense path. Use the dense->less helper.
      // At normal incidence Snell yields cosPhi = sqrt(1 - eta^2 * (1 - negNdotI^2)).
      // For negNdotI = 1.0 (normal incidence): cosPhi = sqrt(1 - eta^2 + eta^2) = 1.
      const Float r = schlickFresnelDenseToLessDense(1.0f, 1.5f, 1.0f);
      const Float expectedR0Sq = 0.04f;
      Assert::AreEqual(expectedR0Sq, r, 1e-6f, L"glass->air at normal incidence", LINE_INFO());
    }

    TEST_METHOD(refractAirToGlassDirection) {
      // 45 degree incidence in xy-plane, normal +y, going into glass
      const Float4 incident = normalize3(Float4{1.0f, -1.0f, 0.0f, 0.0f});
      const Float4 normal{0.0f, 1.0f, 0.0f, 0.0f};
      const Float4 eta = Float4{1.0f / 1.5f, 1.5f, 1.0f, 1.5f};
      // NdotI is negative since ray goes against normal
      const auto refracted = refract(incident, normal, eta);

      // Snell: sin(theta_t) = (eta_i/eta_t) * sin(theta_i) = (1/1.5) * sin(45)
      // Refracted x component should match (eta_i/eta_t) * incident.x
      const Float expectedX = (1.0f / 1.5f) * x(incident);
      Assert::AreEqual(expectedX, x(refracted), 1e-5f, L"refracted x via Snell", LINE_INFO());
      // y should be negative (continuing through surface) and unit length
      Assert::IsTrue(y(refracted) < 0.0f, L"refracted ray should travel through surface", LINE_INFO());
      const Float lenSq = x(refracted) * x(refracted) + y(refracted) * y(refracted) + z(refracted) * z(refracted);
      Assert::AreEqual(1.0f, lenSq, 1e-5f, L"refracted ray should be unit length", LINE_INFO());
    }

    TEST_METHOD(refractTotalInternalReflectionReturnsZero) {
      // Glass to air at very shallow angle => TIR
      const Float4 incident = normalize3(Float4{1.0f, -0.05f, 0.0f, 0.0f});
      const Float4 normal{0.0f, 1.0f, 0.0f, 0.0f};
      // Going from glass(1.5) to air(1.0): eta = (1.5/1.0, 1.0/1.5, 1.5, 1.0)
      const Float4 eta = Float4{1.5f, 1.0f / 1.5f, 1.5f, 1.0f};
      const auto refracted = refract(incident, normal, eta);

      // refract chooses by sign of NdotI; ensure TIR yields zero direction
      Assert::AreEqual(0.0f, x(refracted), L"TIR x should be zero", LINE_INFO());
      Assert::AreEqual(0.0f, y(refracted), L"TIR y should be zero", LINE_INFO());
      Assert::AreEqual(0.0f, z(refracted), L"TIR z should be zero", LINE_INFO());
    }

    TEST_METHOD(refractAtNormalIncidencePassesThrough) {
      // Ray straight along -y, normal +y => no bending
      const Float4 incident{0.0f, -1.0f, 0.0f, 0.0f};
      const Float4 normal{0.0f, 1.0f, 0.0f, 0.0f};
      const Float4 eta = Float4{1.0f / 1.5f, 1.5f, 1.0f, 1.5f};
      const auto refracted = refract(incident, normal, eta);

      Assert::AreEqual(0.0f, x(refracted), 1e-6f, L"normal incidence x", LINE_INFO());
      Assert::AreEqual(-1.0f, y(refracted), 1e-6f, L"normal incidence y unchanged", LINE_INFO());
      Assert::AreEqual(0.0f, z(refracted), 1e-6f, L"normal incidence z", LINE_INFO());
    }

    TEST_METHOD(refractEqualEtaGivesIdentity) {
      // Same medium on both sides => no refraction
      const Float4 incident = normalize3(Float4{1.0f, -1.0f, 0.0f, 0.0f});
      const Float4 normal{0.0f, 1.0f, 0.0f, 0.0f};
      const Float4 eta = Float4{1.0f, 1.0f, 1.0f, 1.0f};
      const auto refracted = refract(incident, normal, eta);

      Assert::AreEqual(x(incident), x(refracted), 1e-5f, L"same eta x preserved", LINE_INFO());
      Assert::AreEqual(y(incident), y(refracted), 1e-5f, L"same eta y preserved", LINE_INFO());
    }

    TEST_METHOD(reflectFromHorizontalSurface) {
      // Ray going +x, -y bounces off ground (+y normal) => +x, +y
      const Float4 incident = normalize3(Float4{1.0f, -1.0f, 0.0f, 0.0f});
      const Float4 normal{0.0f, 1.0f, 0.0f, 0.0f};
      const auto reflected = reflect(incident, normal);

      Assert::AreEqual(x(incident), x(reflected), 1e-6f, L"reflected x preserved", LINE_INFO());
      Assert::AreEqual(-y(incident), y(reflected), 1e-6f, L"reflected y flipped", LINE_INFO());
    }

    TEST_METHOD(reflectAtNormalIncidence) {
      // Ray straight into wall comes straight back
      const Float4 incident{0.0f, -1.0f, 0.0f, 0.0f};
      const Float4 normal{0.0f, 1.0f, 0.0f, 0.0f};
      const auto reflected = reflect(incident, normal);

      Assert::AreEqual(0.0f, x(reflected), 1e-6f, L"x stays zero", LINE_INFO());
      Assert::AreEqual(1.0f, y(reflected), 1e-6f, L"y fully reversed", LINE_INFO());
    }
  };
}

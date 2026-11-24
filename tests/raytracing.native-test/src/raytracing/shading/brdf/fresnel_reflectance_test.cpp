#include "raytracing_tests.h"

#include <raytracing/shading/brdf/fresnel_reflectance.h>
#include <raytracing/shading/brdf/ggx.h>
#include <raytracing/shading/sampling.h>

namespace raytracer::test {
  // brdf::fresnelReflectance without total internal reflection (Schlick's approximation) for a ray from a medium of
  // index etaI towards one of index etaT
  static Float schlickFresnelFrom(const Float negNdotI, const Float etaI, const Float etaT) {
    const auto eta = RefractionIndices::refractionEta(etaI, etaT);
    return x(brdf::fresnelReflectance(false, Float4(negNdotI), eta));
  }

  static Float schlickFresnelDenseToLessDense(const Float negNdotI, const Float etaA, const Float etaB) {
    Assert::IsTrue(etaB < etaA, L"towards the less dense medium", LINE_INFO());
    return schlickFresnelFrom(negNdotI, etaA, etaB);
  }

  static Float schlickFresnelLessDenseToDense(const Float negNdotI, const Float etaA, const Float etaB) {
    Assert::IsTrue(etaB >= etaA, L"towards the denser medium", LINE_INFO());
    return schlickFresnelFrom(negNdotI, etaA, etaB);
  }

  // The two lobes of a rough dielectric interface (Walter et al. 2007) for a ray along the incident direction onto the
  // normal (on the incident side), as Raytracer::traceGlossyReflection and traceRoughTransmission estimate them from
  // VNDF-sampled microfacet normals: each microfacet reflects its Fresnel reflectance and refracts the rest, weighed
  // by Smith's masking of the outgoing direction; directions on the wrong side and total reflections refract nothing
  struct Lobes {
    double reflected;
    double refracted;
  };

  static Lobes roughDielectricLobes(const Float4 &incident, const Float4 &normal, const Float4 &eta, const Float alpha) {
    constexpr ASizeT Samples = 1 << 14;
    const auto V = -incident;
    auto lobes = Lobes{0.0, 0.0};
    for (ASizeT i = 0; i < Samples; ++i) {
      const auto m = brdf::sampleGGXNormal(normal, V, sequencePoint2D(3u, i), alpha);
      const auto F = static_cast<double>(x(brdf::fresnelReflectance(incident, m, eta)));
      const auto reflected = reflect(incident, m);
      const auto NoR = dot3(normal, reflected);
      if (NoR > 0.0f) {
        lobes.reflected += F * brdf::smithG1(NoR, alpha);
      }
      const auto refracted = refractEta(incident, m, dotv(m, incident), xxxx(eta));
      const auto NoT = -dot3(normal, refracted);
      if (!allTrue3(!refracted) && NoT > 0.0f) {
        lobes.refracted += (1.0 - F) * brdf::smithG1(NoT, alpha);
      }
    }
    lobes.reflected /= Samples;
    lobes.refracted /= Samples;
    return lobes;
  }

  TEST_CLASS(FresnelReflectanceTest) {
  public:
    TEST_METHOD(atANormalItIsTheReflectanceOfTheCosine) {
      // entering glass and leaving it at 30 degrees
      const Float4 normal{0.0f, 1.0f, 0.0f, 0.0f};
      const auto incident = normalize3(Float4{0.5f, -0.866f, 0.0f, 0.0f});
      const auto entering = RefractionIndices::refractionEta(1.0f, 1.5f);
      const auto leaving = RefractionIndices::refractionEta(1.5f, 1.0f);
      Assert::AreEqual(x(brdf::fresnelReflectance(false, Float4(0.866f), entering)),
                       x(brdf::fresnelReflectance(incident, normal, entering)), 1e-4f, L"entering", LINE_INFO());
      Assert::AreEqual(x(brdf::fresnelReflectance(false, Float4(0.866f), leaving)),
                       x(brdf::fresnelReflectance(incident, normal, leaving)), 1e-4f, L"leaving", LINE_INFO());
    }

    TEST_METHOD(atANormalItReflectsTotallyBeyondTheCriticalAngle) {
      // leaving glass at 60 degrees, beyond the critical angle of 41.8 degrees
      const Float4 normal{0.0f, 1.0f, 0.0f, 0.0f};
      const auto incident = normalize3(Float4{0.866f, -0.5f, 0.0f, 0.0f});
      const auto leaving = RefractionIndices::refractionEta(1.5f, 1.0f);
      Assert::AreEqual(1.0f, x(brdf::fresnelReflectance(incident, normal, leaving)), L"total", LINE_INFO());
    }

    TEST_METHOD(roughDielectricLobesKeepTheEnergy) {
      // the lobes add up to about 1 (the single-scattering microfacet model loses a few percent to masking), and a
      // smooth surface's split: the reflected lobe near its Fresnel reflectance
      const Float4 normal{0.0f, 1.0f, 0.0f, 0.0f};
      const auto incident = normalize3(Float4{0.707f, -0.707f, 0.0f, 0.0f});
      const auto entering = RefractionIndices::refractionEta(1.0f, 1.5f);
      for (const auto alpha : {0.05f, 0.3f}) {
        const auto lobes = roughDielectricLobes(incident, normal, entering, alpha);
        Assert::IsTrue(lobes.reflected + lobes.refracted <= 1.0 + 1e-3, L"no light gained", LINE_INFO());
        Assert::IsTrue(lobes.reflected + lobes.refracted > (alpha < 0.1f ? 0.98 : 0.9), L"little lost", LINE_INFO());
      }
      const auto smooth = roughDielectricLobes(incident, normal, entering, 0.05f);
      Assert::AreEqual(static_cast<double>(x(brdf::fresnelReflectance(incident, normal, entering))), smooth.reflected,
                       0.005, L"near the smooth surface's reflectance", LINE_INFO());
    }

    TEST_METHOD(totallyReflectingMicrofacetsReflectTheirLight) {
      // leaving glass at 40 degrees, just below the critical angle: the microfacets tilted beyond it reflect totally,
      // so the rough surface reflects more than the smooth one, and no light is lost to the total reflections
      const Float4 normal{0.0f, 1.0f, 0.0f, 0.0f};
      const auto incident = normalize3(Float4{0.643f, -0.766f, 0.0f, 0.0f});
      const auto leaving = RefractionIndices::refractionEta(1.5f, 1.0f);
      const auto lobes = roughDielectricLobes(incident, normal, leaving, 0.2f);
      Assert::IsTrue(lobes.reflected > x(brdf::fresnelReflectance(incident, normal, leaving)) + 0.05,
                     L"more reflection", LINE_INFO());
      Assert::IsTrue(lobes.reflected + lobes.refracted > 0.85, L"the total reflections stay", LINE_INFO());
    }

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

    TEST_METHOD(schlickEnteringLessDenseTakesTheTransmittedCosine) {
      // Glass -> air at cos 0.9: Snell gives cos 0.75664 for the transmitted ray, so the reflectance is
      // 0.04 + 0.96 * (1 - 0.75664)^5 = 0.040819, not the 0.0400096 of the incident cosine
      const Float r = schlickFresnelDenseToLessDense(0.9f, 1.5f, 1.0f);
      Assert::AreEqual(0.0408195f, r, 1e-6f, L"glass->air at cos 0.9", LINE_INFO());
    }

    TEST_METHOD(schlickFillsAllLanes) {
      const auto r = brdf::fresnelReflectance(false, Float4(0.5f), RefractionIndices::refractionEta(1.0f, 1.5f));
      Assert::AreEqual(x(r), y(r), L"lane y", LINE_INFO());
      Assert::AreEqual(x(r), z(r), L"lane z", LINE_INFO());
      Assert::AreEqual(x(r), w(r), L"lane w", LINE_INFO());
    }

    TEST_METHOD(fresnelReflectanceIsOneForTotalInternalReflection) {
      const auto r = brdf::fresnelReflectance(true, Float4(0.05f), RefractionIndices::refractionEta(1.5f, 1.0f));
      Assert::AreEqual(1.0f, x(r), L"glass->air beyond the critical angle reflects all", LINE_INFO());
      Assert::AreEqual(1.0f, w(r), L"in all lanes", LINE_INFO());
    }

    TEST_METHOD(fresnelReflectanceIgnoresTotalInternalReflectionTowardsDenserMedium) {
      // A ray into a denser medium is never totally reflected, so the flag doesn't apply
      const auto eta = RefractionIndices::refractionEta(1.0f, 1.5f);
      const auto r = brdf::fresnelReflectance(true, Float4(0.3f), eta);
      Assert::AreEqual(schlickFresnelFrom(0.3f, 1.0f, 1.5f), x(r), L"Schlick", LINE_INFO());
    }
  };

  // vectorization's refract and reflect with the eta layout fresnelReflectance takes
  TEST_CLASS(FresnelRefractionTest) {
  public:
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

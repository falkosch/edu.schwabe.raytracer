#include "stdafx.h"

#include <iostream>

namespace vectorization
{

    struct PerformanceTest
    {
        struct Parameters
        {
            // HINT: Switch between different configurations for Iterations and Repitions to see different performance
            // behaviour. If Iterations is very low (<10000 or so), then the arrays fit well into the CPU-caches and
            // the performance should go up and should peak for the Vector4s struct test. If Iterations is very high,
            // then the array won't fit well into the CPU-caches and also prefetching can't hide the memory bandwith
            // limits. The performance will still be better than for the scalar test, but should be very much higher.

            // Increasing Repetitions increases the accuracy of the tests, but keep in mind that there is an
            // upper bound for "4 * Iterations * Repitions". The product of them must fit into a 32 bit integer.

            // Count of elements to be processed
            static const ASizeT Iterations = ASizeT(100);

            // Repeat inner loop
            static const ASizeT Repetitions = ASizeT(5000 * 1000);
        };

        template <typename ArrayType, typename ValueType, ASizeT VectorSize, ASizeT Alignment>
        static ArrayType initArray()
        {
            typedef ValueType*const PCValueType;
            const ASizeT Elements = VectorSize * Parameters::Iterations;
            PCValueType data = PCValueType(_mm_malloc(Elements * sizeof(ValueType), Alignment));
            for (ASizeT i = Zero<ASizeT>(); i < Elements; ++i)
            {
                unsigned short rand;
                _rdrand16_step(&rand);
                data[i] = ValueType(rand);
            }
            return ArrayType(data);
        }

        static const double getInvTimerFrequency()
        {
            LARGE_INTEGER frequency;
            QueryPerformanceFrequency(&frequency);
            return reciprocal(double(frequency.QuadPart));
        }

        template <typename VectorType>
        static void testVectorStruct()
        {
            typedef typename VectorType::ValueType ValueType;
            typedef VectorType *const PCArrayType;
            typedef const PCArrayType CPCArrayType;

            std::cout << "Vector struct test: Iterations=" << Parameters::Iterations * Parameters::Repetitions
                << "; VectorType::SIZE=" << VectorType::SIZE
                << "; VectorType::ALIGNMENT=" << VectorType::ALIGNMENT << std::endl;

            const double invFrequency = getInvTimerFrequency();

            CPCArrayType a = initArray<CPCArrayType, ValueType, VectorType::SIZE, VectorType::ALIGNMENT>();
            CPCArrayType b = initArray<CPCArrayType, ValueType, VectorType::SIZE, VectorType::ALIGNMENT>();
            PCArrayType r = initArray<PArrayType, ValueType, VectorType::SIZE, VectorType::ALIGNMENT>();

            const VectorType f = VectorType(Two<ValueType>());

            const ASizeT UpperBound = roundDown(Parameters::Iterations, VectorSizes::W);

            LARGE_INTEGER start;
            QueryPerformanceCounter(&start);

            for (ASizeT repition = Zero<ASizeT>(); repition < Parameters::Repetitions; ++repition)
            {
                ASizeT i;
                for (i = VectorIndices::X; i < UpperBound; i += VectorSizes::W)
                {
                    const ASizeT iX = i + VectorIndices::X;
                    r[iX] = a[iX] + f * b[iX];
                    const ASizeT iY = i + VectorIndices::Y;
                    r[iY] = a[iY] + f * b[iY];
                    const ASizeT iZ = i + VectorIndices::Z;
                    r[iZ] = a[iZ] + f * b[iZ];
                    const ASizeT iW = i + VectorIndices::W;
                    r[iW] = a[iW] + f * b[iW];
                }
                for (; i < Parameters::Iterations; ++i)
                {
                    r[i] = a[i] + f * b[i];
                }
            }

            LARGE_INTEGER stop;
            QueryPerformanceCounter(&stop);

            _mm_free(a);
            _mm_free(b);
            _mm_free(r);

            const double ElementFactor = double(Parameters::Repetitions * VectorType::SIZE * Parameters::Iterations);
            const double timeDelta = double(stop.QuadPart - start.QuadPart) * invFrequency;
            std::cout << "Test duration: " << timeDelta << "s" << std::endl;
            std::cout << "Spend time per element " << timeDelta / ElementFactor << "s" << std::endl;
            std::cout << "Elements per second " << LONGLONG(ElementFactor / timeDelta) << std::endl;
        }

#if !defined(VECTORIZATION_NO_INTRINSICS)

        template<>
        static void testVectorStruct<v_f32_4>()
        {
            typedef v_f32_4 VectorType;
            typedef VectorType::ValueType ValueType;
            typedef ValueType *const PCArrayType;
            typedef const PCArrayType CPCArrayType;
            const ASizeT STEP_SIZE = VectorType::SIZE * VectorSizes::W;

            std::cout << "Vector struct test: Iterations=" << Parameters::Iterations * Parameters::Repetitions
                << "; VectorType::SIZE=" << VectorType::SIZE
                << "; VectorType::ALIGNMENT=" << __alignof(VectorType) << std::endl;

            const double invFrequency = getInvTimerFrequency();

            CPCArrayType a = initArray<CPCArrayType, ValueType, VectorType::SIZE, __alignof(VectorType)>();
            CPCArrayType b = initArray<CPCArrayType, ValueType, VectorType::SIZE, __alignof(VectorType)>();
            PCArrayType r = initArray<PCArrayType, ValueType, VectorType::SIZE, __alignof(VectorType)>();

            const VectorType f = _mm_set1_ps(Two<ValueType>());

            const ASizeT UpperBound = roundDown(Parameters::Iterations, VectorSizes::W);

            LARGE_INTEGER start, stop;
            QueryPerformanceCounter(&start);

            for (ASizeT repition = Zero<ASizeT>(); repition < Parameters::Repetitions; ++repition)
            {
                ASizeT i;
                for (i = Zero<ASizeT>(); i < UpperBound; i += STEP_SIZE)
                {
                    const ASizeT i0 = i + VectorIndices::X * VectorType::SIZE;
                    const ASizeT i1 = i + VectorIndices::Y * VectorType::SIZE;
                    const ASizeT i2 = i + VectorIndices::Z * VectorType::SIZE;
                    const ASizeT i3 = i + VectorIndices::W * VectorType::SIZE;
                    const VectorType bp0 = _mm_load_ps(&b[i0]);
                    const VectorType bp1 = _mm_load_ps(&b[i1]);
                    const VectorType bp2 = _mm_load_ps(&b[i2]);
                    const VectorType bp3 = _mm_load_ps(&b[i3]);
                    const VectorType ap0 = _mm_load_ps(&a[i0]);
                    const VectorType ap1 = _mm_load_ps(&a[i1]);
                    const VectorType ap2 = _mm_load_ps(&a[i2]);
                    const VectorType ap3 = _mm_load_ps(&a[i3]);
                    const VectorType f_mul_bp0 = _mm_mul_ps(f, bp0);
                    const VectorType f_mul_bp1 = _mm_mul_ps(f, bp1);
                    const VectorType f_mul_bp2 = _mm_mul_ps(f, bp2);
                    const VectorType f_mul_bp3 = _mm_mul_ps(f, bp3);
                    const VectorType rp0 = _mm_add_ps(f_mul_bp0, ap0);
                    const VectorType rp1 = _mm_add_ps(f_mul_bp1, ap1);
                    const VectorType rp2 = _mm_add_ps(f_mul_bp2, ap2);
                    const VectorType rp3 = _mm_add_ps(f_mul_bp3, ap3);
                    _mm_stream_ps(&r[i0], rp0);
                    _mm_stream_ps(&r[i1], rp1);
                    _mm_stream_ps(&r[i2], rp2);
                    _mm_stream_ps(&r[i3], rp3);
                }
                for (; i < UpperBound; i += VectorType::SIZE)
                {
                    const VectorType bp = _mm_load_ps(&b[i]);
                    const VectorType ap = _mm_load_ps(&a[i]);
                    const VectorType f_mul_bp = _mm_mul_ps(f, bp);
                    const VectorType rp = _mm_add_ps(f_mul_bp, ap);
                    _mm_stream_ps(&r[i], rp);
                }
            }

            QueryPerformanceCounter(&stop);

            _mm_free(a);
            _mm_free(b);
            _mm_free(r);

            const double ElementFactor = double(Parameters::Repetitions * Parameters::Iterations * VectorType::SIZE);
            const double timeDelta = double(stop.QuadPart - start.QuadPart) * invFrequency;
            std::cout << "Test duration: " << timeDelta << "s" << std::endl;
            std::cout << "Spend time per element " << timeDelta / ElementFactor << "s" << std::endl;
            std::cout << "Elements per second " << LONGLONG(ElementFactor / timeDelta) << std::endl;
        }
#endif
    };

}

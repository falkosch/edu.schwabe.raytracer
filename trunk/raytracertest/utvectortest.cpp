#include "stdafx.h"

#include <typeinfo>

#define ASSERT_MSG(msg) const wchar_t * const MESSAGE = L#msg;

namespace vectorization
{
    TEST_CLASS(UTVectorTest)
    {
        enum Specs : ASizeT { SIZE = ASizeT(11) };
        typedef Float_32 ValueType;
        typedef UTVector<SIZE, ValueType> VectorType;
        const ValueType ZERO = Zero<ValueType>();
        const ValueType A = NegOne<ValueType>();
        const ValueType B = One<ValueType>();
        const ValueType C = Two<ValueType>();
        const ValueType D = ValueType(3);
        const ValueType E = ValueType(4);
        const ValueType F = ValueType(5);
        const ValueType G = ValueType(6);

        typedef UTVector<4, ValueType> SwizzleType;
        typedef SwizzleType::VectorBoolType IntVectorType;
        typedef IntVectorType::ValueType IntType;
        const IntType iZERO = Zero<IntType>();
        const IntType iA = NegOne<IntType>();
        const IntType iB = One<IntType>();
        const IntType iC = Two<IntType>();
        const IntType iD = IntType(3);
        const IntType iE = IntType(4);
        const IntType iF = IntType(5);
        const IntType iG = IntType(6);

    public:

        TEST_METHOD(Specification)
        {
            ASSERT_MSG(Malformed vector specification);
            Assert::AreEqual(ASizeT(SIZE), ASizeT(VectorType::SIZE), MESSAGE, LINE_INFO());

            Assert::AreEqual(typeid(VectorType).hash_code(), typeid(VectorType::VectorType).hash_code(), MESSAGE, LINE_INFO());
            Assert::AreEqual(typeid(ValueType).hash_code(), typeid(VectorType::ValueType).hash_code(), MESSAGE, LINE_INFO());
            Assert::AreEqual(typeid(Float_32_Limits).hash_code(), typeid(VectorType::ValueLimits).hash_code(), MESSAGE, LINE_INFO());

            Assert::IsTrue(std::is_integral<VectorType::BoolType>::value, MESSAGE, LINE_INFO());
            Assert::AreEqual(typeid(VectorType::BoolType).hash_code(), typeid(VectorType::VectorBoolType::ValueType).hash_code(), MESSAGE, LINE_INFO());
            Assert::AreEqual(typeid(VectorType::BoolType).hash_code(), typeid(VectorType::VectorBoolType::VectorBoolType::ValueType).hash_code(), MESSAGE, LINE_INFO());

            Assert::AreEqual(VectorSizes::W, ASizeT(IntVectorType::SIZE), MESSAGE, LINE_INFO());
        }

        TEST_METHOD(Accessors)
        {
            ASSERT_MSG(Accessor returns wrong value.);
            VectorType t = VectorType(C);

            Assert::AreEqual(C, component<VectorIndices::X>(t), MESSAGE, LINE_INFO());
            Assert::AreEqual(C, x(t), MESSAGE, LINE_INFO());
            t = replaceX(t, B);
            Assert::AreEqual(B, component<VectorIndices::X>(t), MESSAGE, LINE_INFO());
            Assert::AreEqual(B, x(t), MESSAGE, LINE_INFO());
            t.setX(C);
            Assert::AreEqual(C, component<VectorIndices::X>(t), MESSAGE, LINE_INFO());
            Assert::AreEqual(C, x(t), MESSAGE, LINE_INFO());

            Assert::AreEqual(C, component<VectorIndices::Y>(t), MESSAGE, LINE_INFO());
            Assert::AreEqual(C, y(t), MESSAGE, LINE_INFO());
            t = replaceY(t, B);
            Assert::AreEqual(B, component<VectorIndices::Y>(t), MESSAGE, LINE_INFO());
            Assert::AreEqual(B, y(t), MESSAGE, LINE_INFO());
            t.setY(C);
            Assert::AreEqual(C, component<VectorIndices::Y>(t), MESSAGE, LINE_INFO());
            Assert::AreEqual(C, y(t), MESSAGE, LINE_INFO());

            Assert::AreEqual(C, component<VectorIndices::Z>(t), MESSAGE, LINE_INFO());
            Assert::AreEqual(C, z(t), MESSAGE, LINE_INFO());
            t = replaceZ(t, B);
            Assert::AreEqual(B, component<VectorIndices::Z>(t), MESSAGE, LINE_INFO());
            Assert::AreEqual(B, z(t), MESSAGE, LINE_INFO());
            t.setZ(C);
            Assert::AreEqual(C, component<VectorIndices::Z>(t), MESSAGE, LINE_INFO());
            Assert::AreEqual(C, z(t), MESSAGE, LINE_INFO());

            Assert::AreEqual(C, component<VectorIndices::W>(t), MESSAGE, LINE_INFO());
            Assert::AreEqual(C, w(t), MESSAGE, LINE_INFO());
            t = replaceW(t, B);
            Assert::AreEqual(B, component<VectorIndices::W>(t), MESSAGE, LINE_INFO());
            Assert::AreEqual(B, w(t), MESSAGE, LINE_INFO());
            t.setW(C);
            Assert::AreEqual(C, component<VectorIndices::W>(t), MESSAGE, LINE_INFO());
            Assert::AreEqual(C, w(t), MESSAGE, LINE_INFO());

            Assert::AreEqual(C, t[VectorIndices::AboveW], MESSAGE, LINE_INFO());
            t[VectorIndices::AboveW] = B;
            Assert::AreEqual(B, t[VectorIndices::AboveW], MESSAGE, LINE_INFO());
        }

        TEST_METHOD(Constructors)
        {
            ASSERT_MSG(Constructor fills wrong values.);
            VectorType t;

            // Default constructor
            t = VectorType();
            for (ASizeT i = VectorIndices::X; i < SIZE; ++i)
                Assert::AreEqual(ZERO, t.components[i], MESSAGE, LINE_INFO());

            // Constructors

            t = VectorType(D);
            for (ASizeT i = VectorIndices::X; i < SIZE; ++i)
                Assert::AreEqual(D, t.components[i], MESSAGE, LINE_INFO());

            t = VectorType(D, E);
            Assert::AreEqual(D, t.components[VectorIndices::X], MESSAGE, LINE_INFO());
            Assert::AreEqual(E, t.components[VectorIndices::Y], MESSAGE, LINE_INFO());
            for (ASizeT i = VectorIndices::Z; i < SIZE; ++i)
                Assert::AreEqual(ZERO, t.components[i], MESSAGE, LINE_INFO());

            t = VectorType(D, E, F);
            Assert::AreEqual(D, t.components[VectorIndices::X], MESSAGE, LINE_INFO());
            Assert::AreEqual(E, t.components[VectorIndices::Y], MESSAGE, LINE_INFO());
            Assert::AreEqual(F, t.components[VectorIndices::Z], MESSAGE, LINE_INFO());
            for (ASizeT i = VectorIndices::W; i < SIZE; ++i)
                Assert::AreEqual(ZERO, t.components[i], MESSAGE, LINE_INFO());

            t = VectorType(D, E, F, G);
            Assert::AreEqual(D, t.components[VectorIndices::X], MESSAGE, LINE_INFO());
            Assert::AreEqual(E, t.components[VectorIndices::Y], MESSAGE, LINE_INFO());
            Assert::AreEqual(F, t.components[VectorIndices::Z], MESSAGE, LINE_INFO());
            Assert::AreEqual(G, t.components[VectorIndices::W], MESSAGE, LINE_INFO());
            for (ASizeT i = VectorIndices::AboveW; i < SIZE; ++i)
                Assert::AreEqual(ZERO, t.components[i], MESSAGE, LINE_INFO());

            t = convert<VectorType>(UTVector<SIZE / 2, ValueType>(B));
            for (ASizeT i = VectorIndices::X; i < SIZE / 2; ++i)
                Assert::AreEqual(B, t.components[i], MESSAGE, LINE_INFO());
            for (ASizeT i = SIZE / 2; i < SIZE; ++i)
                Assert::AreEqual(ZERO, t.components[i], MESSAGE, LINE_INFO());

            // Copy constructor

            t = VectorType(VectorType(D));
            for (ASizeT i = VectorIndices::X; i < SIZE; ++i)
                Assert::AreEqual(D, t.components[i], MESSAGE, LINE_INFO());
        }

        TEST_METHOD(ExplicitConversions)
        {
            ASSERT_MSG(Constructor fills wrong values.);
            VectorType t;

            t = VectorType(3);
            for (ASizeT i = VectorIndices::X; i < SIZE; ++i)
                Assert::AreEqual(D, t.components[i], MESSAGE, LINE_INFO());

            t = VectorType(3, 4);
            Assert::AreEqual(D, t.components[VectorIndices::X], MESSAGE, LINE_INFO());
            Assert::AreEqual(E, t.components[VectorIndices::Y], MESSAGE, LINE_INFO());
            for (ASizeT i = VectorIndices::Z; i < SIZE; ++i)
                Assert::AreEqual(ZERO, t.components[i], MESSAGE, LINE_INFO());

            t = VectorType(3, 4, 5);
            Assert::AreEqual(D, t.components[VectorIndices::X], MESSAGE, LINE_INFO());
            Assert::AreEqual(E, t.components[VectorIndices::Y], MESSAGE, LINE_INFO());
            Assert::AreEqual(F, t.components[VectorIndices::Z], MESSAGE, LINE_INFO());
            for (ASizeT i = VectorIndices::W; i < SIZE; ++i)
                Assert::AreEqual(ZERO, t.components[i], MESSAGE, LINE_INFO());

            t = VectorType(3, 4, 5, 6);
            Assert::AreEqual(D, t.components[VectorIndices::X], MESSAGE, LINE_INFO());
            Assert::AreEqual(E, t.components[VectorIndices::Y], MESSAGE, LINE_INFO());
            Assert::AreEqual(F, t.components[VectorIndices::Z], MESSAGE, LINE_INFO());
            Assert::AreEqual(G, t.components[VectorIndices::W], MESSAGE, LINE_INFO());
            for (ASizeT i = VectorIndices::AboveW; i < SIZE; ++i)
                Assert::AreEqual(ZERO, t.components[i], MESSAGE, LINE_INFO());

            t = convert<VectorType>(UTVector<SIZE / 2, Int_8>(-1));
            for (ASizeT i = VectorIndices::X; i < SIZE / 2; ++i)
                Assert::AreEqual(A, t.components[i], MESSAGE, LINE_INFO());
            for (ASizeT i = SIZE / 2; i < SIZE; ++i)
                Assert::AreEqual(ZERO, t.components[i], MESSAGE, LINE_INFO());

            t = convert<VectorType>(UTVector<SIZE, Int_8>(-1));
            for (ASizeT i = VectorIndices::X; i < SIZE; ++i)
                Assert::AreEqual(A, t.components[i], MESSAGE, LINE_INFO());
        }

        TEST_METHOD(SOASwizzleAccessors)
        {
            ASSERT_MSG(Swizzle fills wrong values.);
            SwizzleType t = SwizzleType(A, B, C, D);
            SwizzleType s;

            s = xxxx(t);
            for (ASizeT i = VectorIndices::X; i < VectorSizes::W; ++i)
                Assert::AreEqual(A, s.components[i], MESSAGE, LINE_INFO());

            s = yyyy(t);
            for (ASizeT i = VectorIndices::X; i < VectorSizes::W; ++i)
                Assert::AreEqual(B, s.components[i], MESSAGE, LINE_INFO());

            s = zzzz(t);
            for (ASizeT i = VectorIndices::X; i < VectorSizes::W; ++i)
                Assert::AreEqual(C, s.components[i], MESSAGE, LINE_INFO());

            s = wwww(t);
            for (ASizeT i = VectorIndices::X; i < VectorSizes::W; ++i)
                Assert::AreEqual(D, s.components[i], MESSAGE, LINE_INFO());
        }

        TEST_METHOD(CommonlyUsedSwizzleAccessors)
        {
            ASSERT_MSG(Swizzle fills wrong values.);
            SwizzleType t = SwizzleType(ZERO, A, B, C);
            SwizzleType s;

            s = xxxz(t);
            Assert::AreEqual(ZERO, s.components[VectorIndices::X], MESSAGE, LINE_INFO());
            Assert::AreEqual(ZERO, s.components[VectorIndices::Y], MESSAGE, LINE_INFO());
            Assert::AreEqual(ZERO, s.components[VectorIndices::Z], MESSAGE, LINE_INFO());
            Assert::AreEqual(B, s.components[VectorIndices::W], MESSAGE, LINE_INFO());

            s = xyxy(t);
            Assert::AreEqual(ZERO, s.components[VectorIndices::X], MESSAGE, LINE_INFO());
            Assert::AreEqual(A, s.components[VectorIndices::Y], MESSAGE, LINE_INFO());
            Assert::AreEqual(ZERO, s.components[VectorIndices::Z], MESSAGE, LINE_INFO());
            Assert::AreEqual(A, s.components[VectorIndices::W], MESSAGE, LINE_INFO());

            s = xzzz(t);
            Assert::AreEqual(ZERO, s.components[VectorIndices::X], MESSAGE, LINE_INFO());
            Assert::AreEqual(B, s.components[VectorIndices::Y], MESSAGE, LINE_INFO());
            Assert::AreEqual(B, s.components[VectorIndices::Z], MESSAGE, LINE_INFO());
            Assert::AreEqual(B, s.components[VectorIndices::W], MESSAGE, LINE_INFO());

            s = yxxy(t);
            Assert::AreEqual(A, s.components[VectorIndices::X], MESSAGE, LINE_INFO());
            Assert::AreEqual(ZERO, s.components[VectorIndices::Y], MESSAGE, LINE_INFO());
            Assert::AreEqual(ZERO, s.components[VectorIndices::Z], MESSAGE, LINE_INFO());
            Assert::AreEqual(A, s.components[VectorIndices::W], MESSAGE, LINE_INFO());

            s = yxwz(t);
            Assert::AreEqual(A, s.components[VectorIndices::X], MESSAGE, LINE_INFO());
            Assert::AreEqual(ZERO, s.components[VectorIndices::Y], MESSAGE, LINE_INFO());
            Assert::AreEqual(C, s.components[VectorIndices::Z], MESSAGE, LINE_INFO());
            Assert::AreEqual(B, s.components[VectorIndices::W], MESSAGE, LINE_INFO());

            s = yzxw(t);
            Assert::AreEqual(A, s.components[VectorIndices::X], MESSAGE, LINE_INFO());
            Assert::AreEqual(B, s.components[VectorIndices::Y], MESSAGE, LINE_INFO());
            Assert::AreEqual(ZERO, s.components[VectorIndices::Z], MESSAGE, LINE_INFO());
            Assert::AreEqual(C, s.components[VectorIndices::W], MESSAGE, LINE_INFO());

            s = yzwx(t);
            Assert::AreEqual(A, s.components[VectorIndices::X], MESSAGE, LINE_INFO());
            Assert::AreEqual(B, s.components[VectorIndices::Y], MESSAGE, LINE_INFO());
            Assert::AreEqual(C, s.components[VectorIndices::Z], MESSAGE, LINE_INFO());
            Assert::AreEqual(ZERO, s.components[VectorIndices::W], MESSAGE, LINE_INFO());

            s = zxyw(t);
            Assert::AreEqual(B, s.components[VectorIndices::X], MESSAGE, LINE_INFO());
            Assert::AreEqual(ZERO, s.components[VectorIndices::Y], MESSAGE, LINE_INFO());
            Assert::AreEqual(A, s.components[VectorIndices::Z], MESSAGE, LINE_INFO());
            Assert::AreEqual(C, s.components[VectorIndices::W], MESSAGE, LINE_INFO());

            s = zzyy(t);
            Assert::AreEqual(B, s.components[VectorIndices::X], MESSAGE, LINE_INFO());
            Assert::AreEqual(B, s.components[VectorIndices::Y], MESSAGE, LINE_INFO());
            Assert::AreEqual(A, s.components[VectorIndices::Z], MESSAGE, LINE_INFO());
            Assert::AreEqual(A, s.components[VectorIndices::W], MESSAGE, LINE_INFO());

            s = zwxy(t);
            Assert::AreEqual(B, s.components[VectorIndices::X], MESSAGE, LINE_INFO());
            Assert::AreEqual(C, s.components[VectorIndices::Y], MESSAGE, LINE_INFO());
            Assert::AreEqual(ZERO, s.components[VectorIndices::Z], MESSAGE, LINE_INFO());
            Assert::AreEqual(A, s.components[VectorIndices::W], MESSAGE, LINE_INFO());

            s = zwzw(t);
            Assert::AreEqual(B, s.components[VectorIndices::X], MESSAGE, LINE_INFO());
            Assert::AreEqual(C, s.components[VectorIndices::Y], MESSAGE, LINE_INFO());
            Assert::AreEqual(B, s.components[VectorIndices::Z], MESSAGE, LINE_INFO());
            Assert::AreEqual(C, s.components[VectorIndices::W], MESSAGE, LINE_INFO());
        }

        TEST_METHOD(BlendSwizzleAccessors)
        {
            ASSERT_MSG(Blend swizzle fills wrong values.);
            SwizzleType t1 = SwizzleType(ZERO, A, B, C);
            SwizzleType t2 = SwizzleType(D, E, F, G);
            SwizzleType s;

            s = xx_xx(t1, t2);
            Assert::AreEqual(ZERO, s.components[VectorIndices::X], MESSAGE, LINE_INFO());
            Assert::AreEqual(ZERO, s.components[VectorIndices::Y], MESSAGE, LINE_INFO());
            Assert::AreEqual(D, s.components[VectorIndices::Z], MESSAGE, LINE_INFO());
            Assert::AreEqual(D, s.components[VectorIndices::W], MESSAGE, LINE_INFO());

            s = xy_xy(t1, t2);
            Assert::AreEqual(ZERO, s.components[VectorIndices::X], MESSAGE, LINE_INFO());
            Assert::AreEqual(A, s.components[VectorIndices::Y], MESSAGE, LINE_INFO());
            Assert::AreEqual(D, s.components[VectorIndices::Z], MESSAGE, LINE_INFO());
            Assert::AreEqual(E, s.components[VectorIndices::W], MESSAGE, LINE_INFO());

            s = xy_zw(t1, t2);
            Assert::AreEqual(ZERO, s.components[VectorIndices::X], MESSAGE, LINE_INFO());
            Assert::AreEqual(A, s.components[VectorIndices::Y], MESSAGE, LINE_INFO());
            Assert::AreEqual(F, s.components[VectorIndices::Z], MESSAGE, LINE_INFO());
            Assert::AreEqual(G, s.components[VectorIndices::W], MESSAGE, LINE_INFO());

            s = xz_xz(t1, t2);
            Assert::AreEqual(ZERO, s.components[VectorIndices::X], MESSAGE, LINE_INFO());
            Assert::AreEqual(B, s.components[VectorIndices::Y], MESSAGE, LINE_INFO());
            Assert::AreEqual(D, s.components[VectorIndices::Z], MESSAGE, LINE_INFO());
            Assert::AreEqual(F, s.components[VectorIndices::W], MESSAGE, LINE_INFO());

            s = yy_yy(t1, t2);
            Assert::AreEqual(A, s.components[VectorIndices::X], MESSAGE, LINE_INFO());
            Assert::AreEqual(A, s.components[VectorIndices::Y], MESSAGE, LINE_INFO());
            Assert::AreEqual(E, s.components[VectorIndices::Z], MESSAGE, LINE_INFO());
            Assert::AreEqual(E, s.components[VectorIndices::W], MESSAGE, LINE_INFO());

            s = yw_yw(t1, t2);
            Assert::AreEqual(A, s.components[VectorIndices::X], MESSAGE, LINE_INFO());
            Assert::AreEqual(C, s.components[VectorIndices::Y], MESSAGE, LINE_INFO());
            Assert::AreEqual(E, s.components[VectorIndices::Z], MESSAGE, LINE_INFO());
            Assert::AreEqual(G, s.components[VectorIndices::W], MESSAGE, LINE_INFO());

            s = zz_zz(t1, t2);
            Assert::AreEqual(B, s.components[VectorIndices::X], MESSAGE, LINE_INFO());
            Assert::AreEqual(B, s.components[VectorIndices::Y], MESSAGE, LINE_INFO());
            Assert::AreEqual(F, s.components[VectorIndices::Z], MESSAGE, LINE_INFO());
            Assert::AreEqual(F, s.components[VectorIndices::W], MESSAGE, LINE_INFO());

            s = zw_zw(t1, t2);
            Assert::AreEqual(B, s.components[VectorIndices::X], MESSAGE, LINE_INFO());
            Assert::AreEqual(C, s.components[VectorIndices::Y], MESSAGE, LINE_INFO());
            Assert::AreEqual(F, s.components[VectorIndices::Z], MESSAGE, LINE_INFO());
            Assert::AreEqual(G, s.components[VectorIndices::W], MESSAGE, LINE_INFO());

            s = ww_ww(t1, t2);
            Assert::AreEqual(C, s.components[VectorIndices::X], MESSAGE, LINE_INFO());
            Assert::AreEqual(C, s.components[VectorIndices::Y], MESSAGE, LINE_INFO());
            Assert::AreEqual(G, s.components[VectorIndices::Z], MESSAGE, LINE_INFO());
            Assert::AreEqual(G, s.components[VectorIndices::W], MESSAGE, LINE_INFO());
        }

        TEST_METHOD(VariadicBlendAccessors)
        {
            ASSERT_MSG(Variadic blend fills wrong values.);

            SwizzleType t1 = SwizzleType(ZERO, A, B, C);
            SwizzleType t2 = SwizzleType(D, E, F, G);
            SwizzleType s;
            SwizzleType::VectorBoolType blendMask;

            blendMask = SwizzleType::VectorBoolType(0, 0, 0, 0);
            s = blendMasked(t1, t2, blendMask);
            Assert::AreEqual(ZERO, s.components[VectorIndices::X], MESSAGE, LINE_INFO());
            Assert::AreEqual(A, s.components[VectorIndices::Y], MESSAGE, LINE_INFO());
            Assert::AreEqual(B, s.components[VectorIndices::Z], MESSAGE, LINE_INFO());
            Assert::AreEqual(C, s.components[VectorIndices::W], MESSAGE, LINE_INFO());

            blendMask = SwizzleType::VectorBoolType(1, 1, 1, 1);
            s = blendMasked(t1, t2, blendMask);
            Assert::AreEqual(D, s.components[VectorIndices::X], MESSAGE, LINE_INFO());
            Assert::AreEqual(E, s.components[VectorIndices::Y], MESSAGE, LINE_INFO());
            Assert::AreEqual(F, s.components[VectorIndices::Z], MESSAGE, LINE_INFO());
            Assert::AreEqual(G, s.components[VectorIndices::W], MESSAGE, LINE_INFO());

            blendMask = SwizzleType::VectorBoolType(2, -3, 4, -5);
            s = blendMasked(t1, t2, blendMask);
            Assert::AreEqual(D, s.components[VectorIndices::X], MESSAGE, LINE_INFO());
            Assert::AreEqual(E, s.components[VectorIndices::Y], MESSAGE, LINE_INFO());
            Assert::AreEqual(F, s.components[VectorIndices::Z], MESSAGE, LINE_INFO());
            Assert::AreEqual(G, s.components[VectorIndices::W], MESSAGE, LINE_INFO());

            blendMask = SwizzleType::VectorBoolType(0, -3, 4, 0);
            s = blendMasked(t1, t2, blendMask);
            Assert::AreEqual(ZERO, s.components[VectorIndices::X], MESSAGE, LINE_INFO());
            Assert::AreEqual(E, s.components[VectorIndices::Y], MESSAGE, LINE_INFO());
            Assert::AreEqual(F, s.components[VectorIndices::Z], MESSAGE, LINE_INFO());
            Assert::AreEqual(C, s.components[VectorIndices::W], MESSAGE, LINE_INFO());
        }

        TEST_METHOD(BinaryMathOperators)
        {
            ASSERT_MSG(Binary math op returns wrong results.);

            VectorType t1 = VectorType(ZERO, A, B, C);
            VectorType t2 = VectorType(D, E, F, G);
            VectorType s;

#define CHECK_OP1(op) \
            s = t1 op t2; \
            Assert::AreEqual(ZERO op D, s.components[VectorIndices::X], MESSAGE, LINE_INFO()); \
            Assert::AreEqual(A op E, s.components[VectorIndices::Y], MESSAGE, LINE_INFO()); \
            Assert::AreEqual(B op F, s.components[VectorIndices::Z], MESSAGE, LINE_INFO()); \
            Assert::AreEqual(C op G, s.components[VectorIndices::W], MESSAGE, LINE_INFO());
#define CHECK_OP2(op) \
            s = t1 op F; \
            Assert::AreEqual(ZERO op F, s.components[VectorIndices::X], MESSAGE, LINE_INFO()); \
            Assert::AreEqual(A op F, s.components[VectorIndices::Y], MESSAGE, LINE_INFO()); \
            Assert::AreEqual(B op F, s.components[VectorIndices::Z], MESSAGE, LINE_INFO()); \
            Assert::AreEqual(C op F, s.components[VectorIndices::W], MESSAGE, LINE_INFO());
#define CHECK_OP(op) \
            CHECK_OP1(op); \
            for (ASizeT i = VectorIndices::AboveW; i < SIZE; ++i) \
                Assert::AreEqual(ZERO op ZERO, s.components[i], MESSAGE, LINE_INFO()); \
            CHECK_OP2(op); \
            for (ASizeT i = VectorIndices::AboveW; i < SIZE; ++i) \
                Assert::AreEqual(ZERO op F, s.components[i], MESSAGE, LINE_INFO());

            CHECK_OP(+);
            CHECK_OP(-);
            CHECK_OP(*);

            CHECK_OP1(/ );
            for (ASizeT i = VectorIndices::AboveW; i < SIZE; ++i)
                // isnan because in this test all other comps>W of t1 and t2 are zero in the division
                Assert::IsTrue(std::isnan(s.components[i]), MESSAGE, LINE_INFO());

            CHECK_OP2(/ );
            for (ASizeT i = VectorIndices::AboveW; i < SIZE; ++i)
                Assert::AreEqual(ZERO / F, s.components[i], MESSAGE, LINE_INFO());

            s = t1 % t2;
            Assert::AreEqual(mod(ZERO, D), s.components[VectorIndices::X], MESSAGE, LINE_INFO());
            Assert::AreEqual(mod(A, E), s.components[VectorIndices::Y], MESSAGE, LINE_INFO());
            Assert::AreEqual(mod(B, F), s.components[VectorIndices::Z], MESSAGE, LINE_INFO());
            Assert::AreEqual(mod(C, G), s.components[VectorIndices::W], MESSAGE, LINE_INFO());
            for (ASizeT i = VectorIndices::AboveW; i < SIZE; ++i)
                // isnan because in this test all other comps>W of t1 and t2 are zero in the division
                Assert::IsTrue(std::isnan(s.components[i]), MESSAGE, LINE_INFO());

            s = t2 % F;
            Assert::AreEqual(mod(D, F), s.components[VectorIndices::X], MESSAGE, LINE_INFO());
            Assert::AreEqual(mod(E, F), s.components[VectorIndices::Y], MESSAGE, LINE_INFO());
            Assert::AreEqual(mod(F, F), s.components[VectorIndices::Z], MESSAGE, LINE_INFO());
            Assert::AreEqual(mod(G, F), s.components[VectorIndices::W], MESSAGE, LINE_INFO());
            for (ASizeT i = VectorIndices::AboveW; i < SIZE; ++i)
                Assert::AreEqual(mod(ZERO, F), s.components[i], MESSAGE, LINE_INFO());

#undef CHECK_OP
#undef CHECK_OP1
#undef CHECK_OP2
        }

        TEST_METHOD(MathOperatorsInteger)
        {
            ASSERT_MSG(Binary math op returns wrong integer results.);
            IntVectorType i1 = IntVectorType(iA, iA, iB, iC);
            IntVectorType i2 = IntVectorType(iD, iE, iF, iG);
            IntVectorType r;

#define CHECK_OP(op) \
            r = i2 op i1; \
            Assert::AreEqual(iD op iA, r.components[VectorIndices::X], MESSAGE, LINE_INFO()); \
            Assert::AreEqual(iE op iA, r.components[VectorIndices::Y], MESSAGE, LINE_INFO()); \
            Assert::AreEqual(iF op iB, r.components[VectorIndices::Z], MESSAGE, LINE_INFO()); \
            Assert::AreEqual(iG op iC, r.components[VectorIndices::W], MESSAGE, LINE_INFO()); \
            r = i2 op iC; \
            Assert::AreEqual(iD op iC, r.components[VectorIndices::X], MESSAGE, LINE_INFO()); \
            Assert::AreEqual(iE op iC, r.components[VectorIndices::Y], MESSAGE, LINE_INFO()); \
            Assert::AreEqual(iF op iC, r.components[VectorIndices::Z], MESSAGE, LINE_INFO()); \
            Assert::AreEqual(iG op iC, r.components[VectorIndices::W], MESSAGE, LINE_INFO());
#define CHECK_OP2(op) \
            CHECK_OP(op) \
            r = i2; \
            r op##= i1; \
            Assert::AreEqual(iD op iA, r.components[VectorIndices::X], MESSAGE, LINE_INFO()); \
            Assert::AreEqual(iE op iA, r.components[VectorIndices::Y], MESSAGE, LINE_INFO()); \
            Assert::AreEqual(iF op iB, r.components[VectorIndices::Z], MESSAGE, LINE_INFO()); \
            Assert::AreEqual(iG op iC, r.components[VectorIndices::W], MESSAGE, LINE_INFO()); \
            r = i2; \
            r op##= iC; \
            Assert::AreEqual(iD op iC, r.components[VectorIndices::X], MESSAGE, LINE_INFO()); \
            Assert::AreEqual(iE op iC, r.components[VectorIndices::Y], MESSAGE, LINE_INFO()); \
            Assert::AreEqual(iF op iC, r.components[VectorIndices::Z], MESSAGE, LINE_INFO()); \
            Assert::AreEqual(iG op iC, r.components[VectorIndices::W], MESSAGE, LINE_INFO());

            CHECK_OP2(/ );
            CHECK_OP2(%);
            CHECK_OP2(<< );
            CHECK_OP2(>> );
            CHECK_OP2(&);
            CHECK_OP2(| );

#undef CHECK_OP2
#undef CHECK_OP
        }

        TEST_METHOD(UnaryMathOperators)
        {
            ASSERT_MSG(Binary math op returns wrong integer results.);
            VectorType t1 = VectorType(-ZERO, A, B, C);
            VectorType s;
            VectorType::VectorBoolType bs;

            s = -t1;
            Assert::AreEqual(ZERO, s.components[VectorIndices::X], MESSAGE, LINE_INFO());
            Assert::AreEqual(-A, s.components[VectorIndices::Y], MESSAGE, LINE_INFO());
            Assert::AreEqual(-B, s.components[VectorIndices::Z], MESSAGE, LINE_INFO());
            Assert::AreEqual(-C, s.components[VectorIndices::W], MESSAGE, LINE_INFO());
            for (ASizeT i = VectorIndices::AboveW; i < SIZE; ++i)
                Assert::AreEqual(-ZERO, s.components[i], MESSAGE, LINE_INFO());

            bs = !t1;
            Assert::AreEqual(IntType(!(-ZERO)), bs.components[VectorIndices::X], MESSAGE, LINE_INFO());
            Assert::AreEqual(IntType(!A), bs.components[VectorIndices::Y], MESSAGE, LINE_INFO());
            Assert::AreEqual(IntType(!B), bs.components[VectorIndices::Z], MESSAGE, LINE_INFO());
            Assert::AreEqual(IntType(!C), bs.components[VectorIndices::W], MESSAGE, LINE_INFO());
            for (ASizeT i = VectorIndices::AboveW; i < SIZE; ++i)
                Assert::AreEqual(IntType(!ZERO), bs.components[i], MESSAGE, LINE_INFO());

            IntVectorType i1 = IntVectorType(iZERO, iA, iB, iC);
            IntVectorType is;
            is = ~i1;
            Assert::AreEqual(~iZERO, is.components[VectorIndices::X], MESSAGE, LINE_INFO());
            Assert::AreEqual(~iA, is.components[VectorIndices::Y], MESSAGE, LINE_INFO());
            Assert::AreEqual(~iB, is.components[VectorIndices::Z], MESSAGE, LINE_INFO());
            Assert::AreEqual(~iC, is.components[VectorIndices::W], MESSAGE, LINE_INFO());
        }

        TEST_METHOD(AssignmentMathOperators)
        {
            ASSERT_MSG(Binary math op returns wrong results.);

            VectorType t1 = VectorType(ZERO, A, B, C);
            VectorType t2 = VectorType(D, E, F, G);
            VectorType s;

#define CHECK_OP1(op) \
            s = t1; \
            s op##= t2; \
            Assert::AreEqual(ZERO op D, s.components[VectorIndices::X], MESSAGE, LINE_INFO()); \
            Assert::AreEqual(A op E, s.components[VectorIndices::Y], MESSAGE, LINE_INFO()); \
            Assert::AreEqual(B op F, s.components[VectorIndices::Z], MESSAGE, LINE_INFO()); \
            Assert::AreEqual(C op G, s.components[VectorIndices::W], MESSAGE, LINE_INFO());
#define CHECK_OP2(op) \
            s = t1; \
            s op##= F; \
            Assert::AreEqual(ZERO op F, s.components[VectorIndices::X], MESSAGE, LINE_INFO()); \
            Assert::AreEqual(A op F, s.components[VectorIndices::Y], MESSAGE, LINE_INFO()); \
            Assert::AreEqual(B op F, s.components[VectorIndices::Z], MESSAGE, LINE_INFO()); \
            Assert::AreEqual(C op F, s.components[VectorIndices::W], MESSAGE, LINE_INFO());
#define CHECK_OP(op) \
            CHECK_OP1(op); \
            for (ASizeT i = VectorIndices::AboveW; i < SIZE; ++i) \
                Assert::AreEqual(ZERO op ZERO, s.components[i], MESSAGE, LINE_INFO()); \
            CHECK_OP2(op); \
            for (ASizeT i = VectorIndices::AboveW; i < SIZE; ++i) \
                Assert::AreEqual(ZERO op F, s.components[i], MESSAGE, LINE_INFO());

            CHECK_OP(+);
            CHECK_OP(-);
            CHECK_OP(*);

            CHECK_OP1(/ );
            for (ASizeT i = VectorIndices::AboveW; i < SIZE; ++i)
                // isnan because in this test all other comps>W of t1 and t2 are zero in the division
                Assert::IsTrue(std::isnan(s.components[i]), MESSAGE, LINE_INFO());
            CHECK_OP2(/ );
            for (ASizeT i = VectorIndices::AboveW; i < SIZE; ++i)
                Assert::AreEqual(ZERO / F, s.components[i], MESSAGE, LINE_INFO());

            s = t1;
            s %= t2;
            Assert::AreEqual(mod(ZERO, D), s.components[VectorIndices::X], MESSAGE, LINE_INFO());
            Assert::AreEqual(mod(A, E), s.components[VectorIndices::Y], MESSAGE, LINE_INFO());
            Assert::AreEqual(mod(B, F), s.components[VectorIndices::Z], MESSAGE, LINE_INFO());
            Assert::AreEqual(mod(C, G), s.components[VectorIndices::W], MESSAGE, LINE_INFO());
            for (ASizeT i = VectorIndices::AboveW; i < SIZE; ++i)
                // isnan because in this test all other comps>W of t1 and t2 are zero in the division
                Assert::IsTrue(std::isnan(s.components[i]), MESSAGE, LINE_INFO());
            s = t1;
            s %= D;
            Assert::AreEqual(mod(ZERO, D), s.components[VectorIndices::X], MESSAGE, LINE_INFO());
            Assert::AreEqual(mod(A, D), s.components[VectorIndices::Y], MESSAGE, LINE_INFO());
            Assert::AreEqual(mod(B, D), s.components[VectorIndices::Z], MESSAGE, LINE_INFO());
            Assert::AreEqual(mod(C, D), s.components[VectorIndices::W], MESSAGE, LINE_INFO());
            for (ASizeT i = VectorIndices::AboveW; i < SIZE; ++i)
                Assert::AreEqual(mod(ZERO, D), s.components[i], MESSAGE, LINE_INFO());

#undef CHECK_OP
#undef CHECK_OP1
#undef CHECK_OP2
        }

        TEST_METHOD(SpecialMathOperations)
        {
            ASSERT_MSG(Special math operation returns wrong results.);

            VectorType t1 = VectorType(ZERO, A, B, C);
            VectorType t2 = VectorType(D, E, F, G);
            VectorType s;
            VectorType::BoolType n1, n2;
            ValueType d, r;

            n1 = isNegative(t1);
            n2 = isNegative(t2);
            Assert::AreEqual(Two<VectorType::BoolType>(), n1, MESSAGE, LINE_INFO());
            Assert::AreEqual(Zero<VectorType::BoolType>(), n2, MESSAGE, LINE_INFO());

            d = dot(t1, t2);
            s = dotv(t1, t2);
            Assert::AreEqual(d, ZERO * D + A * E + B * F + C * G, Float_Limits::epsilon(), MESSAGE, LINE_INFO());
            for (ASizeT i = VectorIndices::X; i < SIZE; ++i)
                Assert::AreEqual(d, s.components[i], MESSAGE, LINE_INFO());

            d = dot3(t1, t2);
            s = dot3v(t1, t2);
            Assert::AreEqual(d, ZERO * D + A * E + B * F, Float_Limits::epsilon(), MESSAGE, LINE_INFO());
            for (ASizeT i = VectorIndices::X; i < SIZE; ++i)
                Assert::AreEqual(d, s.components[i], MESSAGE, LINE_INFO());

            r = sqrt(ZERO * ZERO + A * A + B * B + C * C);
            d = length(t1);
            s = lengthv(t1);
            Assert::AreEqual(d, r, Float_Limits::epsilon(), MESSAGE, LINE_INFO());
            for (ASizeT i = VectorIndices::X; i < SIZE; ++i)
                Assert::AreEqual(d, s.components[i], MESSAGE, LINE_INFO());

            r = sqrt(ZERO * ZERO + A * A + B * B);
            d = length3(t1);
            s = length3v(t1);
            Assert::AreEqual(d, r, Float_Limits::epsilon(), MESSAGE, LINE_INFO());
            for (ASizeT i = VectorIndices::X; i < SIZE; ++i)
                Assert::AreEqual(d, s.components[i], MESSAGE, LINE_INFO());

            struct H
            {
                static const ValueType sqr(const ValueType v) { return v * v; }
            };
            r = sqrt(H::sqr(ZERO - D) + H::sqr(A - E) + H::sqr(B - F) + H::sqr(C - G));
            d = distance(t1, t2);
            Assert::AreEqual(d, r, Float_Limits::epsilon(), MESSAGE, LINE_INFO());

            d = distance(t1, t1);
            Assert::AreEqual(d, ZERO, Float_Limits::epsilon(), MESSAGE, LINE_INFO());

            r = sqrt(H::sqr(ZERO - D) + H::sqr(A - E) + H::sqr(B - F));
            d = distance3(t1, t2);
            Assert::AreEqual(d, r, Float_Limits::epsilon(), MESSAGE, LINE_INFO());

            d = distance3(t1, t1);
            Assert::AreEqual(d, ZERO, Float_Limits::epsilon(), MESSAGE, LINE_INFO());

            s = min(t1, t2);
            Assert::AreEqual(d, ZERO, Float_Limits::epsilon(), MESSAGE, LINE_INFO());
        }
    };
}
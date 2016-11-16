#include "stdafx.h"

#include <typeinfo>

#define ASSERT_MSG(msg) const wchar_t * const MESSAGE = L#msg;

namespace vectorization
{
    TEST_CLASS(Float44Test)
    {
        enum Specs : ASizeT
        {
            ROWS = ASizeT(4),
            COLUMNS = ASizeT(4),
            SIZE = ROWS * COLUMNS
        };

        typedef Float_32 ValueType;
        typedef Float44 MatrixType;

        const ValueType A = Zero<ValueType>();
        const ValueType B = NegOne<ValueType>();
        const ValueType C = One<ValueType>();
        const ValueType D = Two<ValueType>();
        const ValueType E = ValueType(3);
        const ValueType F = ValueType(4);
        const ValueType G = ValueType(5);
        const ValueType H = ValueType(6);

    public:

        TEST_METHOD(Specification)
        {
            ASSERT_MSG(Malformed matrix specification);
            Assert::AreEqual(ASizeT(ROWS), ASizeT(MatrixType::ROWS), MESSAGE, LINE_INFO());
            Assert::AreEqual(ASizeT(COLUMNS), ASizeT(MatrixType::COLUMNS), MESSAGE, LINE_INFO());
            Assert::AreEqual(ASizeT(SIZE), ASizeT(MatrixType::SIZE), MESSAGE, LINE_INFO());

            Assert::AreEqual(typeid(Float_32).hash_code(), typeid(MatrixType::ValueType).hash_code(), MESSAGE, LINE_INFO());
            Assert::AreEqual(typeid(Float_32_Limits).hash_code(), typeid(MatrixType::ValueLimits).hash_code(), MESSAGE, LINE_INFO());

            Assert::IsTrue(std::is_integral<MatrixType::BoolType>::value, MESSAGE, LINE_INFO());
            Assert::AreEqual(typeid(MatrixType::BoolType).hash_code(), typeid(MatrixType::MatrixBoolType::ValueType).hash_code(), MESSAGE, LINE_INFO());
            Assert::AreEqual(typeid(MatrixType::BoolType).hash_code(), typeid(MatrixType::MatrixBoolType::MatrixBoolType::ValueType).hash_code(), MESSAGE, LINE_INFO());
        }

        TEST_METHOD(Constructors)
        {
            ASSERT_MSG(Constructor sets wrong initial values);
            MatrixType t = MatrixType();
            for (ASizeT r = Zero<ASizeT>(); r < ROWS; ++r)
                for (ASizeT c = Zero<ASizeT>(); c < COLUMNS; ++c)
                    Assert::AreEqual(t[r * COLUMNS + c], (r == c) ? C : A, MESSAGE, LINE_INFO());

            t[0] = B;
            MatrixType t2 = MatrixType(t);
            Assert::AreEqual(t2[0], B, MESSAGE, LINE_INFO());
        }

        TEST_METHOD(BinaryMathOperators)
        {
            ASSERT_MSG(Binary math op returns wrong result);
            MatrixType t = MatrixType() * MatrixType();

            for (ASizeT r = Zero<ASizeT>(); r < ROWS; ++r)
                for (ASizeT c = Zero<ASizeT>(); c < COLUMNS; ++c)
                    Assert::AreEqual(t[r * COLUMNS + c], (r == c) ? C : A, MESSAGE, LINE_INFO());

            t = MatrixType({ {
                    A, C, A, A,
                    C, A, A, A,
                    A, A, A, C,
                    A, A, C, A
                } });

            t = t * t;
            for (ASizeT r = Zero<ASizeT>(); r < ROWS; ++r)
                for (ASizeT c = Zero<ASizeT>(); c < COLUMNS; ++c)
                    Assert::AreEqual(t[r * COLUMNS + c], (r == c) ? C : A, MESSAGE, LINE_INFO());

            t = MatrixType({ {
                    A, A, A, C,
                    A, A, C, A,
                    A, C, A, A,
                    C, A, A, A
                } });

            MatrixType t2 = t * MatrixType();
            for (ASizeT r = Zero<ASizeT>(); r < ROWS; ++r)
                for (ASizeT c = Zero<ASizeT>(); c < COLUMNS; ++c)
                    Assert::AreEqual(t2[r * COLUMNS + c], (r == (COLUMNS - (c + 1))) ? C : A, MESSAGE, LINE_INFO());
        }

        TEST_METHOD(SpecialMathOperations)
        {
            ASSERT_MSG(Special math operation returns wrong result);
            MatrixType t = MatrixType({ {
                    C, A, A, A,
                    A, C, A, A,
                    A, A, C, -D,
                    A, A, C, A
                } });

            MatrixType t2 = inverse(t);
        }
    };
}
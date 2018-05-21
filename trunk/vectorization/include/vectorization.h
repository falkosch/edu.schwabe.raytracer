#pragma once

#include "vectorization/compiler.h"

#include "vectorization/datatypes.h"

#include "vectorization/intrinsics.h"

#include "vectorization/constants.h"

#include "vectorization/selects.h"

#include "vectorization/functions.h"

#include "vectorization/utvector.h"

#include "vectorization/v_i32_4.h"
#include "vectorization/v_ui32_4.h"
#include "vectorization/v_ui64_2.h"

#include "vectorization/v_f32_4.h"
#include "vectorization/v_f32_4x4.h"

#include "vectorization/m_f32_4x4.h"

#include "vectorization/conversions.h"

namespace vectorization
{

	typedef UTVector<VectorSizes::X, Int_8> Int1_8;
	typedef UTVector<VectorSizes::Y, Int_8> Int2_8;
	typedef UTVector<VectorSizes::Z, Int_8> Int3_8;
	typedef UTVector<VectorSizes::W, Int_8> Int4_8;

	typedef UTVector<VectorSizes::X, UInt_8> UInt1_8;
	typedef UTVector<VectorSizes::Y, UInt_8> UInt2_8;
	typedef UTVector<VectorSizes::Z, UInt_8> UInt3_8;
	typedef UTVector<VectorSizes::W, UInt_8> UInt4_8;

	typedef UTVector<VectorSizes::X, Int_16> Int1_16;
	typedef UTVector<VectorSizes::Y, Int_16> Int2_16;
	typedef UTVector<VectorSizes::Z, Int_16> Int3_16;
	typedef UTVector<VectorSizes::W, Int_16> Int4_16;

	typedef UTVector<VectorSizes::X, UInt_16> UInt1_16;
	typedef UTVector<VectorSizes::Y, UInt_16> UInt2_16;
	typedef UTVector<VectorSizes::Z, UInt_16> UInt3_16;
	typedef UTVector<VectorSizes::W, UInt_16> UInt4_16;

	typedef UTVector<VectorSizes::X, Int_32> Int1_32;
	typedef UTVector<VectorSizes::Y, Int_32> Int2_32;
	typedef UTVector<VectorSizes::Z, Int_32> Int3_32;
	typedef v_i32_4 Int4_32;

	typedef UTVector<VectorSizes::X, UInt_32> UInt1_32;
	typedef UTVector<VectorSizes::Y, UInt_32> UInt2_32;
	typedef UTVector<VectorSizes::Z, UInt_32> UInt3_32;
	typedef v_ui32_4 UInt4_32;

	typedef UTVector<VectorSizes::X, Float_32> Float1_32;
	typedef UTVector<VectorSizes::Y, Float_32> Float2_32;
	typedef UTVector<VectorSizes::Z, Float_32> Float3_32;
	typedef v_f32_4 Float4_32;

	typedef UTVector<VectorSizes::X, Int_64> Int1_64;
	typedef UTVector<VectorSizes::Y, Int_64> Int2_64;
	//typedef v_i64_2 Int2_64;
	typedef UTVector<VectorSizes::Z, Int_64> Int3_64;
	typedef UTVector<VectorSizes::W, Int_64> Int4_64;
	//typedef v_i64_4 Int4_64;

	typedef UTVector<VectorSizes::X, UInt_64> UInt1_64;
	typedef v_ui64_2 UInt2_64;
	typedef UTVector<VectorSizes::Z, UInt_64> UInt3_64;
	typedef UTVector<VectorSizes::W, UInt_64> UInt4_64;
	//typedef v_ui64_4 UInt4_64;

	typedef UTVector<VectorSizes::X, Float_64> Float1_64;
	typedef UTVector<VectorSizes::Y, Float_64> Float2_64;
	//typedef v_f64_2 Float2_64;
	typedef UTVector<VectorSizes::Z, Float_64> Float3_64;
	typedef UTVector<VectorSizes::W, Float_64> Float4_64;
	//typedef v_f64_4 Float4_64;

#ifdef ARCH_X64
	typedef UInt1_64 Size1;
	typedef UInt2_64 Size2;
	typedef UInt3_64 Size3;
	typedef UInt4_64 Size4;
#else
	typedef UInt1_32 Size1;
	typedef UInt2_32 Size2;
	typedef UInt3_32 Size3;
	typedef UInt4_32 Size4;
#endif

	typedef Int1_32 Int1;
	typedef Int2_32 Int2;
	typedef Int3_32 Int3;
	typedef Int4_32 Int4;

	typedef UInt1_32 UInt1;
	typedef UInt2_32 UInt2;
	typedef UInt3_32 UInt3;
	typedef UInt4_32 UInt4;

	typedef Float1_32 Float1;
	typedef Float2_32 Float2;
	typedef Float3_32 Float3;
	typedef Float4_32 Float4;

	//typedef m_ui32_4x4 UInt44;
	//typedef m_i32_4x4 Int44;
	typedef m_f32_4x4 Float44;

}

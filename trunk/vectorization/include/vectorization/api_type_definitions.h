#pragma once

#include "v_i32_4.h"
#include "v_ui32_4.h"
#include "v_ui64_2.h"

#include "v_f32_4.h"
#include "v_f32_4x4.h"

#include "m_f32_4x4.h"

namespace vectorization
{

	typedef v_i32_4 Int1_8;
	typedef v_i32_4 Int2_8;
	typedef v_i32_4 Int3_8;
	typedef v_i32_4 Int4_8;

	typedef v_ui32_4 UInt1_8;
	typedef v_ui32_4 UInt2_8;
	typedef v_ui32_4 UInt3_8;
	typedef v_ui32_4 UInt4_8;

	typedef v_i32_4 Int1_16;
	typedef v_i32_4 Int2_16;
	typedef v_i32_4 Int3_16;
	typedef v_i32_4 Int4_16;

	typedef v_ui32_4 UInt1_16;
	typedef v_ui32_4 UInt2_16;
	typedef v_ui32_4 UInt3_16;
	typedef v_ui32_4 UInt4_16;

	typedef v_i32_4 Int1_32;
	typedef v_i32_4 Int2_32;
	typedef v_i32_4 Int3_32;
	typedef v_i32_4 Int4_32;

	typedef v_ui32_4 UInt1_32;
	typedef v_ui32_4 UInt2_32;
	typedef v_ui32_4 UInt3_32;
	typedef v_ui32_4 UInt4_32;

	//typedef v_i64_2 Int1_64;
	//typedef v_i64_2 Int2_64;

	typedef v_ui64_2 UInt1_64;
	typedef v_ui64_2 UInt2_64;

	typedef v_f32_4 Float1_32;
	typedef v_f32_4 Float2_32;
	typedef v_f32_4 Float3_32;
	typedef v_f32_4 Float4_32;

	//typedef v_f64_2 Float1_64;
	//typedef v_f64_2 Float2_64;


	// Architecture dependend size vector type

#ifdef ARCH_X64
	typedef UInt1_64 Size1;
	typedef UInt2_64 Size2;
	//typedef UInt3_64 Size3;
	//typedef UInt4_64 Size4;
#else
	typedef UInt1_32 Size1;
	typedef UInt2_32 Size2;
	typedef UInt3_32 Size3;
	typedef UInt4_32 Size4;
#endif


	// Shortcuts for the default precision vectors

#ifdef VECTORIZATION_HIGH_PRECISION

	typedef Int_64 Int;
	typedef UInt_64 UInt;
	typedef Bool_64 Bool;
	typedef Float_64 Float;

	typedef Int1_64 Int1;
	typedef Int2_64 Int2;
	typedef Int3_64 Int3;
	typedef Int4_64 Int4;

	typedef UInt1_64 UInt1;
	typedef UInt2_64 UInt2;
	typedef UInt3_64 UInt3;
	typedef UInt4_64 UInt4;

	typedef Float1_64 Float1;
	typedef Float2_64 Float2;
	typedef Float3_64 Float3;
	typedef Float4_64 Float4;

	typedef m_ui64_4x4 UInt44;
	typedef m_i64_4x4 Int44;
	typedef m_f64_4x4 Float44;

#else

	typedef Int_32 Int;
	typedef UInt_32 UInt;
	typedef Bool_32 Bool;
	typedef Float_32 Float;

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

#endif

}

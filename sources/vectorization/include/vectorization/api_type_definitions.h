#pragma once

#include "v_i32_4.h"
#include "v_ui32_4.h"
#include "v_ui64_2.h"

#include "v_f32_4.h"
#include "v_f32_4x4.h"

#include "m_f32_4x4.h"

namespace vectorization {
  using Int1_8 = v_i32_4;
  using Int2_8 = v_i32_4;
  using Int3_8 = v_i32_4;
  using Int4_8 = v_i32_4;

  using UInt1_8 = v_ui32_4;
  using UInt2_8 = v_ui32_4;
  using UInt3_8 = v_ui32_4;
  using UInt4_8 = v_ui32_4;

  using Int1_16 = v_i32_4;
  using Int2_16 = v_i32_4;
  using Int3_16 = v_i32_4;
  using Int4_16 = v_i32_4;

  using UInt1_16 = v_ui32_4;
  using UInt2_16 = v_ui32_4;
  using UInt3_16 = v_ui32_4;
  using UInt4_16 = v_ui32_4;

  using Int1_32 = v_i32_4;
  using Int2_32 = v_i32_4;
  using Int3_32 = v_i32_4;
  using Int4_32 = v_i32_4;

  using UInt1_32 = v_ui32_4;
  using UInt2_32 = v_ui32_4;
  using UInt3_32 = v_ui32_4;
  using UInt4_32 = v_ui32_4;

  // using Int1_64 = v_i64_2;
  // using Int2_64 = v_i64_2;

  using UInt1_64 = v_ui64_2;
  using UInt2_64 = v_ui64_2;

  using Float1_32 = v_f32_4;
  using Float2_32 = v_f32_4;
  using Float3_32 = v_f32_4;
  using Float4_32 = v_f32_4;

  // using Float1_64 = v_f64_2;
  // using Float2_64 = v_f64_2;

  // Architecture dependent size vector type

#ifdef ARCH_X64
  using Size1 = UInt1_64;
  using Size2 = UInt2_64;
  // using Size3 = UInt3_64;
  // using Size4 = UInt4_64;
#else
  using Size1 = UInt1_32;
  using Size2 = UInt2_32;
  using Size3 = UInt3_32;
  using Size4 = UInt4_32;
#endif

  // Shortcuts for the default precision vectors

#ifdef VECTORIZATION_HIGH_PRECISION

  using Int = Int_64;
  using UInt = UInt_64;
  using Bool = Bool_64;
  using Float = Float_64;

  using Int1 = Int1_64;
  using Int2 = Int2_64;
  using Int3 = Int3_64;
  using Int4 = Int4_64;

  using UInt1 = UInt1_64;
  using UInt2 = UInt2_64;
  using UInt3 = UInt3_64;
  using UInt4 = UInt4_64;

  using Float1 = Float1_64;
  using Float2 = Float2_64;
  using Float3 = Float3_64;
  using Float4 = Float4_64;

  using UInt44 = m_ui64_4x4;
  using Int44 = m_i64_4x4;
  using Float44 = m_f64_4x4;

#else

  using Int = Int_32;
  using UInt = UInt_32;
  using Bool = Bool_32;
  using Float = Float_32;

  using Int1 = Int1_32;
  using Int2 = Int2_32;
  using Int3 = Int3_32;
  using Int4 = Int4_32;

  using UInt1 = UInt1_32;
  using UInt2 = UInt2_32;
  using UInt3 = UInt3_32;
  using UInt4 = UInt4_32;

  using Float1 = Float1_32;
  using Float2 = Float2_32;
  using Float3 = Float3_32;
  using Float4 = Float4_32;

  // using UInt44 = m_ui32_4x4;
  // using Int44 = m_i32_4x4;
  using Float44 = m_f32_4x4;

#endif
}

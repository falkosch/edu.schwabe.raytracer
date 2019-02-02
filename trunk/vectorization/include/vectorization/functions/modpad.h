#pragma once

#include "../architecture/compiler_intrinsics.h"

namespace vectorization
{

	const Int_8 modpad(const Int_8 unpadded, const Int_8 modPadding) noexcept;

	const UInt_8 modpad(const UInt_8 unpadded, const UInt_8 modPadding) noexcept;

	const Int_16 modpad(const Int_16 unpadded, const Int_16 modPadding) noexcept;

	const UInt_16 modpad(const UInt_16 unpadded, const UInt_16 modPadding) noexcept;

	const Int_32 modpad(const Int_32 unpadded, const Int_32 modPadding) noexcept;

	const UInt_32 modpad(const UInt_32 unpadded, const UInt_32 modPadding) noexcept;

	const Int_64 modpad(const Int_64 unpadded, const Int_64 modPadding) noexcept;

	const UInt_64 modpad(const UInt_64 unpadded, const UInt_64 modPadding) noexcept;

}

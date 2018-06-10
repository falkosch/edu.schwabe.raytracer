#pragma once

#include "../architecture.h"

namespace vectorization
{

	template <typename U>
	const U convert(const bool) noexcept;


	template <typename U>
	const U convert(const bool, const bool) noexcept;


	template <typename U>
	const U convert(const Int_32 v) noexcept;

	template <>
	const Float_32 convert<Float_32>(const Int_32 v) noexcept;

	template <>
	const Float_64 convert<Float_64>(const Int_32 v) noexcept;


	template <typename U>
	const U convert(const Int_64 v) noexcept;

	template <>
	const Float_32 convert<Float_32>(const Int_64 v) noexcept;

	template <>
	const Float_64 convert<Float_64>(const Int_64 v) noexcept;


	template <typename U>
	const U convert(const Float_32 v) noexcept;

	template <>
	const Int_32 convert<Int_32>(const Float_32 v) noexcept;

	template <>
	const Int_64 convert<Int_64>(const Float_32 v) noexcept;


	template <typename U>
	const U convert(const Float_64 v) noexcept;

	template <>
	const Int_32 convert<Int_32>(const Float_64 v) noexcept;

	template <>
	const Int_64 convert<Int_64>(const Float_64 v) noexcept;

}

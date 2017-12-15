#pragma once

namespace box
{
	using U8 = unsigned char;
	using S8 = signed char;
	using U16 = unsigned short int;
	using S16 = signed short int;
	using U32 = unsigned int;
	using S32 = signed int;
	using U64 = unsigned long long int;
	using S64 = signed long long int;

	static_assert(sizeof(U8) == 1, "please run on real computer");
	static_assert(sizeof(U16) == 2, "please run on real computer");
	static_assert(sizeof(U32) == 4, "please run on real computer");
	static_assert(sizeof(U64) == 8, "please run on real computer");
}
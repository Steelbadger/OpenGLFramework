#pragma once
#include <mmintrin.h>
#include <xmmintrin.h>
#include <emmintrin.h>

namespace SIMD
{
	inline __m128& operator *(const __m128 &left, const __m128 &right)
	{
		return _mm_mul_ps(left, right);
	}

	inline __m128& operator *(const __m128 &left, const float &right)
	{
		return _mm_mul_ps(left, _mm_set1_ps(right));
	}

	inline __m128i operator *(const __m128i &a, const __m128i &b)
	{
		__m128i tmp1 = _mm_mul_epu32(a,b); /* mul 2,0*/
		__m128i tmp2 = _mm_mul_epu32( _mm_srli_si128(a,4), _mm_srli_si128(b,4)); /* mul 3,1 */
		return _mm_unpacklo_epi32(_mm_shuffle_epi32(tmp1, _MM_SHUFFLE (0,0,2,0)), _mm_shuffle_epi32(tmp2, _MM_SHUFFLE (0,0,2,0))); /* shuffle results to [63..0] and pack */
	}

	inline __m128i operator *(const __m128i &a, const int &b)
	{
		__m128i tmpb = _mm_set1_epi32(b);
		__m128i tmp1 = _mm_mul_epu32(a,tmpb); /* mul 2,0*/
		__m128i tmp2 = _mm_mul_epu32( _mm_srli_si128(a,4), _mm_srli_si128(tmpb,4)); /* mul 3,1 */
		return _mm_unpacklo_epi32(_mm_shuffle_epi32(tmp1, _MM_SHUFFLE (0,0,2,0)), _mm_shuffle_epi32(tmp2, _MM_SHUFFLE (0,0,2,0))); /* shuffle results to [63..0] and pack */
	}

	inline __m128& operator /(const __m128 &num, const __m128 &den)
	{
		return _mm_div_ps(num, den);
	}

	inline __m128& operator /(const __m128 &num, const float &den)
	{
		return _mm_div_ps(num, _mm_set1_ps(den));
	}

	inline __m128& operator +(const __m128 &left, const __m128 &right)
	{
		return _mm_add_ps(left, right);
	}

	inline __m128& operator -(const __m128 &left, const __m128 &right)
	{
		return _mm_sub_ps(left, right);
	}

	inline __m128& operator -(const float &left, const __m128 &right)
	{
		return _mm_sub_ps(_mm_set1_ps(left), right);
	}


	inline __m128& operator -(const __m128 &left, const float &right)
	{
		return _mm_sub_ps(left, _mm_set1_ps(right));
	}

	inline __m128i& operator +(const __m128i &left, const __m128i &right)
	{
		return _mm_add_epi32(left, right);
	}

	inline __m128i& operator +(const __m128i &left, const int &right)
	{	
		return _mm_add_epi32(left, _mm_set1_epi32(right));
	}

	inline __m128i& operator -(const __m128i &left, const int &right)
	{	
		return _mm_sub_epi32(left, _mm_set1_epi32(right));
	}

	inline __m128i& operator -(const __m128i &left, const __m128i &right)
	{
		return _mm_sub_epi32(left, right);
	}

	inline __m128& Assign(float* vals)
	{
		return _mm_load_ps(vals);
	}

	inline __m128& Assign(float val)
	{
		return _mm_set1_ps(val);
	}

	inline __m128i& Assign(int* vals)
	{
		return _mm_set_epi32(vals[0], vals[1], vals[2], vals[3]);
	}

	inline __m128i& Assign(const __m128 val)
	{
		return _mm_cvtps_epi32(val);
	}

	inline __m128& Assign(const __m128i val)
	{
		return _mm_cvtepi32_ps(val);
	}


	inline void Return(float * target, __m128 val)
	{
		return _mm_store_ps(target, val);
	}

	inline __m128i& operator <<(const __m128i val, const int shift)
	{
		return _mm_slli_epi32(val, shift);
	}

	inline __m128i& operator >>(const __m128i val, const int shift)
	{
		return _mm_srai_epi32(val, shift);
	}

	inline __m128i& operator &(const __m128i lhs, const __m128i rhs)
	{
		return _mm_and_si128(lhs, rhs);
	}

	inline __m128i& operator &(const __m128i lhs, const int rhs)
	{
		return _mm_and_si128(lhs, _mm_set1_epi32(rhs));
	}

	inline __m128i& operator ^(const __m128i lhs, const __m128i rhs)
	{
		return _mm_xor_si128(lhs, rhs);
	}

	inline __m128i& operator ^(const __m128i lhs, const int rhs)
	{
		return _mm_xor_si128(lhs, _mm_set1_epi32(rhs));
	}

};


#include <stdio.h>
#include <x86intrin.h>

enum TYPE
{
	i8, u8, i16, u16, i32, u32, i64, u64
};

typedef union {
	__m128i 			int128;
	
	unsigned char		m128_u8[16];
	signed char			m128_i8[16];

	unsigned short		m128_u16[8];
	signed short		m128_i16[8];

	u_int32_t			m128_u32[4];
	int32_t				m128_i32[4];

	u_int64_t			m128_u64[4];
	int64_t				m128_i64[4];
} intVec;

typedef union {
	__m128 				f128;

	float				f32[4];

} floatVec;

void print_u8(__m128i a)
{
	intVec tmp;
	tmp.int128 = a;
	printf ("[");
	for (int i=15; i>0; i--) {
		printf ("%X, ", tmp.m128_u8[i]);
	}
	printf ("%X]\n\n", tmp.m128_u8[0]);
}

void print_i8(__m128i a)
{
	intVec tmp;
	tmp.int128 = a;
	printf ("[");
	for (int i=15; i>0; i--) {
		printf ("%X, ", tmp.m128_i8[i]);
	}
	printf ("%X]\n\n", tmp.m128_i8[0]);
}

void print_u16(__m128i a)
{
	intVec tmp;
	tmp.int128 = a;
	printf ("[");
	for (int i=7; i>0; i--) {
		printf ("%X, ", tmp.m128_u16[i]);
	}
	printf ("%X]\n\n", tmp.m128_u16[0]);
}

void print_i16(__m128i a)
{
	intVec tmp;
	tmp.int128 = a;
	printf ("[");
	for (int i=7; i>0; i--) {
		printf ("%X, ", tmp.m128_i16[i]);
	}
	printf ("%X]\n\n", tmp.m128_i16[0]);
}

void print_u32(__m128i a)
{
	intVec tmp;
	tmp.int128 = a;
	printf ("[");
	for (int i=3; i>0; i--) {
		printf ("%X, ", tmp.m128_u32[i]);
	}
	printf ("%X]\n\n", tmp.m128_u32[0]);
}

void print_i32(__m128i a)
{
	intVec tmp;
	tmp.int128 = a;
	printf ("[");
	for (int i=3; i>0; i--) {
		printf ("%X, ", tmp.m128_i32[i]);
	}
	printf ("%X]\n\n", tmp.m128_i32[0]);
}

void print_u64(__m128i a)
{
	intVec tmp;
	tmp.int128 = a;
	printf ("[");
	for (int i=1; i>0; i--) {
		printf ("%lX, ", tmp.m128_u64[i]);
	}
	printf ("%lX]\n\n", tmp.m128_u64[0]);
}

void print_i64(__m128i a)
{
	intVec tmp;
	tmp.int128 = a;
	printf ("[");
	for (int i=1; i>0; i--) {
		printf ("%lX, ", tmp.m128_i64[i]);
	}
	printf ("%lX]\n\n", tmp.m128_i64[0]);
}

void print_int_vector(__m128i a, int type)
{
	switch (type)
	{
		case i8:
			print_i8(a);
			break;
		case u8:
			print_u8(a);
			break;
		case i16:
			print_i16(a);
			break;
		case u16:
			print_u16(a);
			break;
		case i32:
			print_i32(a);
			break;
		case u32:
			print_u32(a);
			break;
		case i64:
			print_i64(a);
			break;
		case u64:
			print_u64(a);
			break;
	}
}

void print_spfp_vector(__m128 a)
{
	floatVec tmp;
	tmp.f128 = a;
	printf ("[");
	for (int i=3; i>0; i--) {
		printf ("%f, ", tmp.f32[i]);
	}
	printf ("%f]\n\n", tmp.f32[0]);
}

int main(void)
{
	__m128i a;
	__m128 b;

	unsigned char intArray [16] = {	0X00, 0X11, 0X22, 0X33, 0X44, 0X55, 0X66, 0X77,
								0X88, 0X99, 0XAA, 0XBB, 0XCC, 0XDD, 0XEE, 0XFF};
	
	float fArray[4] = {1231231232.5, -6763.4, 5, 5};

	a = _mm_load_si128((const __m128i*)intArray);
	b = _mm_load_ps(fArray);

	print_int_vector(a, i32);
	print_int_vector(a, u32);

	print_spfp_vector(b);

	return 0;
}
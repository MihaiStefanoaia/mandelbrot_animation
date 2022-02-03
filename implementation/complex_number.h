#pragma once

struct complex_number
{
	long double r;
	long double i;
};

struct complex_number c_add(struct complex_number a, struct complex_number b)
{
	struct complex_number ret;

	ret.r = a.r + b.r;
	ret.i = a.i + b.i;

	return ret;
}

struct complex_number c_mul(struct complex_number a, struct complex_number b)
{
	struct complex_number ret;

	ret.r = a.r * b.r - a.i * b.i; //ac-bd
	ret.i = a.r * b.i + a.i * b.r; //ad+bc

	return ret;
}
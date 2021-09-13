#pragma once

struct TRGB {
	BYTE r, g, b;

	TRGB() : r(0), g(0), b(0) {}
	TRGB(BYTE _r, BYTE _g, BYTE _b) :
		r(_r), g(_g), b(_b) {}
};
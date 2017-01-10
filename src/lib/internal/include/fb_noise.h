#ifndef FB_NOISE
#define FB_NOISE

#include <noise\noise.h>

class FBNoise {

public:
	FBNoise() {

	}

	double GetValueForInput(double x, double y, double z) {
		noise::module::Perlin perlinModule;
		return perlinModule.GetValue(x, y, z);
	}

};

#endif
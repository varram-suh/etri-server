#pragma once
#include <random>


struct ks19937 {
private:
	static std::mt19937 rng;
public:
	// This is equivalent to srand().
	static void seed(std::mt19937::result_type new_seed = std::mt19937::default_seed) {
		rng.seed(new_seed);
	}

	// This is equivalent to rand().
	static uint64_t get() {
		return rng();
	}
	static int getIntValue(int minValue, int maxValue)
	{
		std::uniform_int_distribution<int> dist(minValue, maxValue);
		return dist(rng);
	}
	static float getFloatValue(float minValue, float maxValue)
	{
		std::uniform_real_distribution<float> dist(minValue, maxValue);
		return dist(rng);
	}
	static double getDoubleValue(double minValue, double maxValue)
	{
		std::uniform_real_distribution<double> dist(minValue, maxValue);
		return dist(rng);
	}
};

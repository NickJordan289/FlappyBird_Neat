#pragma once
#include <vector>
#include <cmath>
#include <algorithm>
#include <random>

#include <SFML/Graphics.hpp>

#if _WIN32 || _WIN64
	#if _WIN64
		std::string ENVIRONMENTBIT = "64-bit";
	#else
		#if _WIN32
			std::string ENVIRONMENTBIT = "32-bit";
		#endif
	#endif
#else
	std::string ENVIRONMENTBIT = "00-bit";
#endif

typedef long long int64;
typedef unsigned long long uint64;

std::string GetEnvironmentBit() {
	return ENVIRONMENTBIT;
}

// inclusive min, inclusive max
template <typename T>
T rNum(T Min, T Max) { return ((float(rand()) / float(RAND_MAX)) * (Max - Min)) + Min; }
// inclusive max
template <typename T>
T rNum(T Max) { return T((float(rand()) / float(RAND_MAX)) * (Max - 0)) + 0; }

template <typename T>
inline T lerp(T v0, T v1, T t) {
	return (1 - t)*v0 + t * v1;
}

double map(double n, double start1, double stop1, double start2, double stop2) {
	return ((n - start1) / (stop1 - start1))*(stop2 - start2) + start2;
};

float dist(sf::Vector2f a, sf::Vector2f b) {
	return std::sqrt(std::pow(b.x - a.x, 2) + std::pow(b.y - a.y, 2));
}

float magnitude(sf::Vector2f a) {
	return std::sqrt(std::pow(a.x, 2) + std::pow(a.y, 2));
}

float magnitude(sf::Vector3f a) {
	return std::sqrt(std::pow(a.x, 2) + std::pow(a.y, 2) + std::pow(a.z, 2));
}

float sqrMagnitude(sf::Vector3f a) {
	return a.x* a.x + a.y* a.y + a.z* a.z;
}

double randomDouble(double a, double b) {
	double random = ((double)rand()) / (double)RAND_MAX;
	double diff = b - a;
	double r = random * diff;
	return a + r;
}

float dot(sf::Vector3f a, sf::Vector3f b) {
	float aMag = magnitude(a);
	float bMag = magnitude(b);
	return a.x * b.x + a.y * b.y + a.z * b.z;
}

sf::Vector2f normalize(sf::Vector2f a) {
	float len = magnitude(a);
	if (len != 0.f)
		return sf::Vector2f(a.x / len, a.y / len);
	return a;
}

sf::Vector3f normalize(sf::Vector3f a) {
	float len = magnitude(a);
	if (len != 0.f)
		return sf::Vector3f(a.x / len, a.y / len, a.z / len);
	return a;
}

template <typename T>
void RemoveAt(std::vector<T> V, int index) {
	V.erase(V.begin() + index);
}

/*
Concatenates Two Vectors
*/
template <typename T>
std::vector<T> AddRange(std::vector<T> A, std::vector<T> B)
{
	std::vector<T> AB;
	AB.reserve(A.size() + B.size()); // preallocate memory
	AB.insert(AB.end(), A.begin(), A.end());
	AB.insert(AB.end(), B.begin(), B.end());
	return AB;
}

template <typename A, typename B>
A constrain(A n, B low, B high) {
	return std::max(std::min(n, high), low);
};

template <typename T>
void delete_pointed_to(T* const ptr) {
	delete ptr;
}

template <typename T>
void free_pointer_vector_memory(std::vector<T*> ptr_vector) {
	std::for_each(ptr_vector.begin(), ptr_vector.end(), delete_pointed_to<T>);
}

template <typename T>
inline T Interpolate(T x0, T x1, T alpha) {
	return x0 * (1 - alpha) + alpha * x1;
}

float TruncateRGB(float n) {
	return std::max(std::min(n, 255.f), 0.f);
}

sf::Color FloatToColour(float value) {
	return sf::Color(sf::Uint8(value*255.f), sf::Uint8(value*255.f), sf::Uint8(value*255.f));
}

int RGBToDec(sf::Color c)
{
	return (c.r << 16) | (c.g << 8) | c.b;
}

sf::Color RGBFromDec(int d)
{
	sf::Color temp;
	temp.r = (d >> 16) & 255;
	temp.g = (d >> 8) & 255;
	temp.b = d & 255;
	return temp;
}
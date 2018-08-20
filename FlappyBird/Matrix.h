/*
	Credit to Daniel Shiffman's videos on Perceptrons, Matrix Math and Neural Networks
*/

#pragma once
#include <vector>
#include <iostream>
#include "ExtraFuncs.h"

class Matrix {
private:
	std::vector<std::vector<double>> m;

public:
	Matrix() {
	}

	Matrix(std::vector<std::vector<double>> m) {
		this->m = m;
	}

	Matrix(int rows, int columns, bool random = false) {
		for (int i = 0; i < rows; i++) {
			std::vector<double> temp;
			for (int j = 0; j < columns; j++) {
				temp.push_back(0.f);
			}
			m.push_back(temp);
		}
		if (random)
			randomise();
	}

	// Matrix addition
	static Matrix add(Matrix a, Matrix b) {
		Matrix result = Matrix(a);
		for (int row = 0; row < result.m.size(); row++) {
			for (int col = 0; col < result.m[0].size(); col++) {
				result.m[row][col] = a.m[row][col] + b.m[row][col];
			}
		}
		return result;
	}

	// Matrix addition
	Matrix add(Matrix b) {
		for (int row = 0; row < m.size(); row++) {
			for (int col = 0; col < m[0].size(); col++) {
				m[row][col] += b.m[row][col];
			}
		}
		return *this;
	}

	// Scalar addition
	void add(double b) {
		for (int row = 0; row < m.size(); row++) {
			for (int col = 0; col < m[row].size(); col++) {
				m[row][col] += b;
			}
		}
	}

	inline static Matrix subtract(Matrix a, Matrix b) {
		Matrix temp = a;
		temp.sub(b);
		return temp;
	}

	void sub(Matrix b) {
		for (int row = 0; row < m.size(); row++)
			for (int col = 0; col < m[row].size(); col++)
				m[row][col] -= b.m[row][col];
	}

	inline static Matrix multiply(Matrix a, Matrix b) {
		Matrix temp = a;
		temp.multiply(b);
		return temp;
	}

	// Hadamard product
	void multiply(Matrix b) {
		for (int row = 0; row < m.size(); row++) {
			for (int col = 0; col < m[0].size(); col++) {
				m[row][col] *= b.m[row][col];
			}
		}
	}

	// Scalar Multiplcation
	void multiply(double b) {
		for (int row = 0; row < m.size(); row++) {
			for (int col = 0; col < m[0].size(); col++) {
				m[row][col] *= b;
			}
		}
	}

	// produces a a.rows * b.columns matrix
	static Matrix dot(Matrix a, Matrix b) {
		// Won't work if columns of A don't equal columns of B
		if (a.m[0].size() != b.m.size()) {
			throw std::invalid_argument("incompatible matrix sizes");
		}
		// Make a new matrix
		Matrix result = Matrix(a.m.size(), b.m[0].size(), false);
		for (int i = 0; i < a.m.size(); i++) {
			for (int j = 0; j < b.m[0].size(); j++) {
				// Sum all the rows of A times columns of B
				double sum = 0;
				for (int k = 0; k < a.m[0].size(); k++) {
					sum += a.m[i][k] * b.m[k][j];
				}
				// New value
				result.m[i][j] = sum;
			}
		}
		return result;
	}

	// runs function against every element in the matrix
	// optional param chance is the odds that the function will be ran (used for GA)
	Matrix map(double(*func)(double), double chance=1.0) {
		for (int row = 0; row < m.size(); row++) {
			for (int col = 0; col < m[row].size(); col++) {
				if (chance==1.0 || randomDouble(0, 1) < chance) {
					m[row][col] = func(m[row][col]);
				}
			}
		}
		return *this;
	}

	inline static Matrix Map(Matrix a, double(*func)(double), double chance=1.0) {
		Matrix temp = a;
		temp.map(func,chance);
		return temp;
	}

	// returns a transposed copy of the matrix
	inline Matrix T(void) {
		// copy because we dont want to manipulate this object only use it for calculation
		Matrix temp = *this;
		temp.transpose();
		return temp;
	}

	// swaps rows and columns
	Matrix transpose() {
		Matrix result = Matrix(m[0].size(), m.size(), false);
		for (int i = 0; i < result.m.size(); i++) {
			for (int j = 0; j < result.m[0].size(); j++) {
				result.m[i][j] = m[j][i];
			}
		}
		m = result.m;
		return *this;
	}

#pragma region Operators
	bool operator== (const Matrix &rhs) {
		for (int i = 0; i < m.size(); i++) {
			for (int j = 0; j < m[0].size(); j++) {
				if (m[i][j] != rhs.m[i][j])
					return false;
			}
		}
		return true;
	}

	bool operator== (const std::vector<double> &rhs) {
		Matrix temp = Matrix::fromVector(rhs);
		for (int i = 0; i < m.size(); i++) {
			for (int j = 0; j < m[0].size(); j++) {
				if (m[i][j] != temp.m[i][j])
					return false;
			}
		}
		return true;
	}

	inline Matrix operator+ (const Matrix &rhs) {
		// copy because we dont want to manipulate this object only use it for calculation
		Matrix lhs = Matrix(m);
		lhs.add(rhs);
		return lhs;
	}

	inline Matrix& operator+= (const Matrix &rhs) {
		this->add(rhs);
		return *this;
	}

	inline Matrix operator- (const Matrix &rhs) {
		// copy because we dont want to manipulate this object only use it for calculation
		Matrix lhs = Matrix(m);
		lhs.sub(rhs);
		return lhs;
	}

	inline Matrix& operator-= (const Matrix &rhs) {
		this->sub(rhs);
		return *this;
	}

	inline Matrix operator* (const Matrix &rhs) {
		// copy because we dont want to manipulate this object only use it for calculation
		Matrix lhs = Matrix(m);
		lhs.multiply(rhs);
		return lhs;
	}

	template<typename T>
	inline Matrix operator* (const T &rhs) {
		// copy because we dont want to manipulate this object only use it for calculation
		Matrix lhs = Matrix(m);
		lhs.multiply(rhs);
		return lhs;
	}

	inline Matrix& operator*= (const Matrix &rhs) {
		this->multiply(rhs);
		return *this;
	}

	template<typename T>
	inline Matrix& operator*= (const T &rhs) {
		this->multiply(rhs);
		return *this;
	}
#pragma endregion

	template<typename T>
	static Matrix fromVector(std::vector<T> a) {
		Matrix newMatrix = Matrix(a.size(), 1, false);
		for (int i = 0; i < a.size(); i++)
			newMatrix.m[i][0] = a[i];
		return newMatrix;
	}

	std::vector<double> toVector() {
		std::vector<double> temp;
		for (int row = 0; row < m.size(); row++)
			for (int col = 0; col < m[row].size(); col++)
				temp.push_back(m[row][col]);
		return temp;
	}

	void randomise() {
		for (int row = 0; row < m.size(); row++)
			for (int col = 0; col < m[row].size(); col++)
				m[row][col] = randomDouble(-1, 1);
	}

	void print() {
		for (int row = 0; row < m.size(); row++) {
			std::cout << "[";
			for (int col = 0; col < m[row].size(); col++) {
				std::cout << m[row][col];
				if (col < m[row].size() - 1) {
					std::cout << ",";
				}
			}
			std::cout << "]";
			if (row < m.size() - 1) {
				std::cout << std::endl;
			}
		}
		std::cout << "\n" << std::endl;
	}
};
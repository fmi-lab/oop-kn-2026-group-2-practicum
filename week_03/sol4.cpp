#include <iostream>
#include <iomanip>

class Matrix {
	using value_type = double;
	std::size_t rows, cols;
	value_type *data;

   public:
	Matrix(std::size_t r, std::size_t c) : rows(r), cols(c), data(new value_type[r * c]) {}

	Matrix(const Matrix &other) : Matrix(other.rows, other.cols) {
		for (std::size_t i = 0; i < rows * cols; i++)
			data[i] = other.data[i];
	}

	/// инициализация с initializer list, БОНУС ЗАДАЧА
	Matrix(std::initializer_list<std::initializer_list<value_type>> init)
		: rows(init.size()), cols(init.begin()->size()), data(nullptr) {
		// забележете, че не заделяме памет преди да сме сигурни, че извикването е валидно
		for (const auto &row : init) {
			if (row.size() != cols) {
				std::cout << "Error: Inconsistent row sizes in initializer list." << std::endl;
				std::exit(1);
			}
		}
		data		  = new value_type[rows * cols];
		std::size_t r = 0;
		for (const auto &row : init) {
			std::size_t c = 0;
			for (value_type val : row)
				at(r, c++) = val;
			r++;
		}
	}

	Matrix &operator=(const Matrix &other) {
		if (this != &other) {
			if (rows != other.rows || cols != other.cols) {
				Matrix temp(other);
				std::swap(rows, temp.rows);
				std::swap(cols, temp.cols);
				std::swap(data, temp.data);
				// често използван трик: деструктора на temp ще освободи старата памет, която
				// сме 'прехвърлили' като отговорност.
				return *this;
			}
			for (std::size_t i = 0; i < rows * cols; i++)
				data[i] = other.data[i];
		}
		return *this;
	}

	~Matrix() { delete[] data; }

	void	   setValue(std::size_t r, std::size_t c, value_type val) { data[r * cols + c] = val; }
	value_type getValue(std::size_t r, std::size_t c) const { return data[r * cols + c]; }

	// внимаваме за константността на методите 
	value_type		 &at(std::size_t r, std::size_t c) { return data[r * cols + c]; }
	const value_type &at(std::size_t r, std::size_t c) const { return data[r * cols + c]; }

	void setRow(std::size_t r, std::initializer_list<value_type> vals) {
		if (vals.size() != cols) {
			std::cout << "Error: Incorrect number of values for row." << std::endl;
			std::exit(1);
		}
		std::size_t i = 0;
		for (value_type val : vals)
			at(r, i++) = val;
	}

	/// печатаме матрицата с ширина w за всяко число
	void print(int w = 4) const {
		for (std::size_t i = 0; i < rows; i++) {
			for (std::size_t j = 0; j < cols; j++)
				std::cout << std::setw(w) << at(i, j) << " ";
			std::cout << std::endl;
		}
	}
	
	/// идентитет
	static Matrix identity(std::size_t n) {
		Matrix result(n, n);
		for (std::size_t i = 0; i < n; i++)
			for (std::size_t j = 0; j < n; j++)
				result.at(i, j) = (i == j); // 1 на диагонала, 0 навсякъде другаде
		return result;
	}

	static Matrix add(const Matrix &m1, const Matrix &m2) {
		if (m1.rows != m2.rows || m1.cols != m2.cols) {
			std::cout << "Error: Incompatible dimensions for addition." << std::endl;
			std::exit(1);
		}

		Matrix result(m1.rows, m1.cols);
		for (std::size_t i = 0; i < m1.rows * m1.cols; i++)
			result.data[i] = m1.data[i] + m2.data[i];
		return result;
	}

	static Matrix multiply(const Matrix &m1, const Matrix &m2) {
		if (m1.cols != m2.rows) {
			std::cout << "Error: Incompatible dimensions for multiplication." << std::endl;
			std::exit(1);
		}

		Matrix result(m1.rows, m2.cols);
		for (std::size_t i = 0; i < m1.rows; i++) {
			for (std::size_t j = 0; j < m2.cols; j++) {
				value_type sum = 0;
				for (std::size_t k = 0; k < m1.cols; k++)
					sum += m1.getValue(i, k) * m2.getValue(k, j);
				result.setValue(i, j, sum);
			}
		}
		return result;
	}
};

int main() {
	Matrix mat1(2, 3), mat2(2, 3);

	mat1.setValue(0, 0, 1.0);
	mat1.setValue(0, 1, 2.0);
	mat1.setValue(0, 2, 3.0);
	mat1.setValue(1, 0, 4.0);
	mat1.setValue(1, 1, 5.0);
	mat1.setValue(1, 2, 6.0);

	mat2.setValue(0, 0, 7.0);
	mat2.setValue(0, 1, 8.0);
	mat2.setValue(0, 2, 9.0);
	mat2.setValue(1, 0, 10.0);
	mat2.setValue(1, 1, 11.0);
	mat2.setValue(1, 2, 12.0);

	std::cout << "Matrix 1:" << std::endl;
	mat1.print();

	std::cout << "Matrix 2:" << std::endl;
	mat2.print();

	Matrix sum = Matrix::add(mat1, mat2);
	std::cout << "Sum of matrices:" << std::endl;
	sum.print();

	Matrix identity = Matrix::identity(3);
	std::cout << "Identity matrix of size 3:" << std::endl;
	identity.print();

	// бонуса 
	Matrix mat = {{1.0, 2.0, 3.0}, {4.0, 5.0, 6.0}, {7.0, 8.0, 9.0}};
	std::cout << "Matrix initialized with initializer list:" << std::endl;
	mat.print();

	return 0;
}

#include "stdafx.h"
#include <iostream>
#include <sstream>
#include <fstream>

using namespace std;

class matrix_t {

	int ** data;
	unsigned int rows;
	unsigned int columns;

public:
	bool success = true;

	matrix_t() {
		rows = 0;
		columns = 0;
		success = true;
		data = nullptr;
	}

	matrix_t & operator =(matrix_t const & other) {
		if (this != &other) {
			for (unsigned int i = 0; i < rows; ++i) {
				delete[] data[i];
			}
			delete[] data;

			data = new int *[other.rows];
			for (unsigned int i = 0; i < other.rows; ++i) {
				data[i] = new int[other.columns];
				for (unsigned int j = 0; j < other.columns; ++j) {
					data[i][j] = other.data[i][j];
				}
			}

			rows = other.rows;
			columns = other.columns;
		}

		return *this;
	}

	matrix_t add(matrix_t & other) const {

		matrix_t result;

		result.data = new int *[rows];
		for (int i = 0; i < rows; i++) {
			result.data[i] = new int[columns];
		}

		if (other.rows == rows && other.columns == columns) {
			for (int i = 0; i < rows; i++) {
				for (int j = 0; j < columns; j++) {
					result.data[i][j] = other.data[i][j] + data[i][j];
				}
			}
			result.rows = other.rows;
			result.columns = other.columns;

		}
		else result.success = false;

		return result;
	}


	matrix_t sub(matrix_t const & other) {

		matrix_t result;

		result.data = new int *[rows];
		for (int i = 0; i < rows; i++) {
			result.data[i] = new int[columns];
		}

		if (other.rows == rows && other.columns == columns) {
			for (int i = 0; i < rows; i++) {
				for (int j = 0; j < columns; j++) {
					data[i][j] = other.data[i][j] - data[i][j];
				}
			}
			result.data = data;
			result.rows = other.rows;
			result.columns = other.columns;

		}
		else success = false;

		return result;
	}


	matrix_t mul(matrix_t & other) {

		matrix_t result;

		if (other.columns == rows) {
			int n = other.columns;
			result.data = new int *[n];
			for (int i = 0; i < n; i++) {
				result.data[i] = new int[n];
			}

			for (int i = 0; i < n; i++) {
				for (int j = 0; j < n; j++) {
					data[i][j] = 0;
					for (int k = 0; k < n; k++)
						data[i][j] += other.data[i][k] * data[k][j];
				}
			}
			result.data = data;
			result.rows = rows;
			result.columns = other.columns;

		}
		else success = false;

		return result;
	}


	matrix_t trans(matrix_t & other) {
		matrix_t result;

		result.rows = other.columns;
		result.columns = other.rows;

		result.data = new int *[result.rows];
		for (int i = 0; i < result.rows; i++) {
			result.data[i] = new int[result.columns];
		}

		for (int i = 0; i < result.rows; i++) {
			for (int j = 0; j < result.columns; j++) {
				result.data[i][j] = other.data[j][i];
			}
		}

		return result;
	}


	std::ifstream & read(std::ifstream & file)
	{
		unsigned int rows_e = 0;
		unsigned int columns_e = 0;
		char symbol;
		int ** elements = nullptr;

		string line;

		if (getline(file, line)) {
			istringstream sstream(line);
			if (sstream >> rows_e && sstream >> symbol && symbol == ',' && sstream >> columns_e && sstream.eof()) {
				elements = new int * [rows_e];
				for (unsigned int i = 0; i < rows_e && success; i++) {
					elements[i] = new int[columns_e];
					string line;
					if (getline(file, line)) {
						istringstream sstream(line);
						for (unsigned j = 0; j < columns_e; j++) {
							sstream >> elements[i][j];
							if (!sstream) {
								success = false;
								break;
							}
						}
						success &= sstream.eof();
					}
					else {
						success = false;
					}
				}
			}
			else {
				success = false;
			}
		}
		else {
			success = false;
		}

		if (success) {
			data = elements;
			rows = rows_e;
			columns = columns_e;
		}


		return file;
	}


	std::ostream & write(std::ostream & stream)const {
		cout << endl;
		for (int i = 0; i < rows; i++) {
			for (int j = 0; j < columns; j++) {
				stream << data[i][j] << ' ';
			}
			stream << endl;
		}
		return stream;
	}

	~matrix_t() {
		for (unsigned int i = 0; i < rows; ++i) {
			delete[] data[i];
		}
		delete[] data;
	}
};




int main()
{

	matrix_t matrix1;

	string line;
	string filename1, filename2;

	char oper;
	if (getline(cin, line)) {
		istringstream stream(line);
		if (stream >> filename1 && stream >> oper) {
			if (stream >> filename2) {
				ifstream file1(filename1);
				ifstream file2(filename2);

				matrix1.read(file1);
				if (matrix1.success) {
					matrix_t matrix2;
					matrix_t result;

					switch (oper) {
					case '+':
						matrix2.read(file2);
						result = matrix1.add(matrix2);
						if (matrix2.success && result.success) {
							result.write(cout);
						}
						else cout << "An error has occured while reading input data";

						break;

					case '-':
						matrix2.read(file2);
						result = matrix1.sub(matrix2);
						
						if (matrix2.success && result.success) {
							result.write(cout);
							
						}
						else cout << "An error has occured while reading input data";

						break;

					case '*':
						matrix2.read(file2);
						result = matrix1.mul(matrix2);
						if (matrix2.success && result.success) {
							result.write(cout);
							
						}
						else cout << "An error has occured while reading input data";

						break;
					}
					
				}
				else cout << "An error has occured while reading input data";
			}
			else {
				ifstream file1(filename1);
				matrix1.read(file1);
				if (matrix1.success) {

					switch (oper) {
					case 'T':
						matrix_t result;
						result = matrix1.trans(matrix1);
						if (result.success) {
							result.write(cout);
							
						}
						break;
					}
				

				}
				else cout << "An error has occured while reading input data";
			}
		}
		else cout << "An error has occured while reading input data";
	}
	else cout << "An error has occured while reading input data";

	cin.get();
	cin.get();
	return 0;
}

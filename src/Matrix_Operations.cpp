#include<iostream>
#include<string>
#include<sstream>
#include<fstream>
#include<map>
#include<vector>

using std::cout;
using std::endl;

template<typename T>
class QSMatrix{

	std::vector<std::vector<T> > mat;
	unsigned rows;
	unsigned columns;

public:

	QSMatrix(unsigned rows, unsigned columns, const T& initial);
	QSMatrix(const QSMatrix<T>& rhs);
	QSMatrix<T>& operator=(const QSMatrix<T>& rhs);
	~QSMatrix();

	// Mathematical operations

	// Matrix-matrix operations
	QSMatrix<T> operator+(const QSMatrix<T>& rhs);
	QSMatrix<T>& operator+=(const QSMatrix<T>& rhs);
	QSMatrix<T> operator-(const QSMatrix<T>& rhs);
	QSMatrix<T>& operator-=(const QSMatrix<T>& rhs);
	QSMatrix<T> operator*(const QSMatrix<T>& rhs);
	QSMatrix<T>& operator*=(const QSMatrix<T>& rhs);

	// Matrix-scalar operations
	QSMatrix<T> operator+(const T& rhs);
	QSMatrix<T>& operator+=(const T& rhs);
	QSMatrix<T> operator-(const T& rhs);
	QSMatrix<T>& operator-=(const T& rhs);
	QSMatrix<T> operator*(const T& rhs);
	QSMatrix<T>& operator*=(const T& rhs);
	QSMatrix<T> operator/(const T& rhs);
	QSMatrix<T>& operator/=(const T& rhs);

	// Matrix-vector operations
	QSMatrix<T> operator*(const std::vector<T>& rhs);

	// Special matrix operations
	QSMatrix<T> transpose();

	unsigned get_rows() const;
	unsigned get_columns() const;

	// Indexing operator
	T& operator()(const unsigned& rows, const unsigned& columns);
	const T& operator()(const unsigned& rows, const unsigned& columns) const;

	// Overloading stream insertion operator
	friend std::ostream& operator<<(std::ostream& out, const QSMatrix<T>& matrix);

};

// method implementations
template<typename T>
QSMatrix<T>::QSMatrix(unsigned rows, unsigned columns, const T& initial){
	this->rows = rows;
	this->columns = columns;
	mat.resize(this->rows);
	for(int i = 0; i < mat.size(); i++)
		mat[i].resize(this->columns, initial);
}

template<typename T>
QSMatrix<T>::QSMatrix(const QSMatrix<T>& rhs){
	mat = rhs.mat;
	rows = rhs.rows; // rhs.get_rows()
	columns = rhs.columns; // rhs.get_columns()
}

template<typename T>
QSMatrix<T>::~QSMatrix(){}

template<typename T>
QSMatrix<T>& QSMatrix<T>::operator=(const QSMatrix<T>& rhs){

	if(&rhs == this)
		return *this;

	unsigned new_rows = rhs.get_rows();
	unsigned new_columns = rhs.get_columns();
	mat.resize(new_rows);

	for(int i = 0; i < mat.size(); i++)
		mat[i].resize(new_columns);

	for(int i =0; i < new_rows; i++){
		for(int j = 0; j < new_columns; j++)
			mat[i][j] = rhs(i,j); // using the overloaded indexing operator
	}

	this->rows = new_rows;
	this->columns = new_columns;

	return *this;

}

template<typename T>
QSMatrix<T> QSMatrix<T>::operator+(const QSMatrix<T>& rhs){

	//if(this->rows != rhs.get_rows() || this->columns!=rhs.get_columns())
	//	return;

	QSMatrix<T> result(rows, columns, 0.0);
	for(int i =0; i < rows; i++){
		for(int j = 0; j < columns; j++)
			result(i,j) = this->mat[i][j] + rhs(i,j); // using the overloaded indexing operator
	}
	return result;
}

template<typename T>
QSMatrix<T>& QSMatrix<T>::operator+=(const QSMatrix<T>& rhs){

	if(this->rows != rhs.get_rows() || this->columns!=rhs.get_columns())
		return;

	QSMatrix<T> result(rows, columns, 0.0);
	for(int i =0; i < rows; i++){
		for(int j = 0; j < columns; j++)
			this->mat[i][j] += rhs(i,j); // using the overloaded indexing operator
	}
	return *this;
}

template<typename T>
QSMatrix<T> QSMatrix<T>::operator-(const QSMatrix<T>& rhs){

	if(this->rows != rhs.get_rows() || this->columns!=rhs.get_columns())
		return;

	QSMatrix<T> result(rows, columns, 0.0);
	for(int i =0; i < rows; i++){
		for(int j = 0; j < columns; j++)
			result(i,j) = this->mat[i][j] - rhs(i,j); // using the overloaded indexing operator
	}
	return result;
}

template<typename T>
QSMatrix<T>& QSMatrix<T>::operator-=(const QSMatrix<T>& rhs){

	if(this->rows != rhs.get_rows() || this->columns!=rhs.get_columns())
		return;

	QSMatrix<T> result(rows, columns, 0.0);
	for(int i =0; i < rows; i++){
		for(int j = 0; j < columns; j++)
			this->mat[i][j] -= rhs(i,j); // using the overloaded indexing operator
	}
	return *this;
}

template<typename T>
QSMatrix<T> QSMatrix<T>::operator*(const QSMatrix<T>& rhs){

	if(this->columns != rhs.get_rows())
		return;

	QSMatrix<T> result(rows, columns, 0.0);
	for(int i =0; i < this->rows; i++){
		for(int j = 0; j < this->columns; j++){
			for(int k = 0; k < rhs.get_rows(); k++)
				result(i,j) += this->mat[i][k]*rhs(k,j); // using the overloaded indexing operator
		}
	}
	return result;
}

template<typename T>
QSMatrix<T>& QSMatrix<T>::operator*=(const QSMatrix<T>& rhs){

	if(this->columns != rhs.get_rows() )
		return;

	QSMatrix result = (*this)*rhs;
	(*this) = result;
	return *this;
}

template<typename T>
QSMatrix<T> QSMatrix<T>::operator+(const T& rhs){
	QSMatrix<T> result(rows, columns, 0.0);
	for(int i =0; i < rows; i++){
		for(int j = 0; j < columns; j++)
			result(i,j) = this->mat[i][j] + rhs; // using the overloaded indexing operator
	}
	return result;
}

template<typename T>
QSMatrix<T>& QSMatrix<T>::operator+=(const T& rhs){
	for(int i =0; i < rows; i++){
			for(int j = 0; j < columns; j++)
				this->mat[i][j] += rhs; // using the overloaded indexing operator
		}
		return *this;
}

template<typename T>
QSMatrix<T> QSMatrix<T>::operator-(const T& rhs){
	QSMatrix<T> result(rows, columns, 0.0);
	for(int i =0; i < rows; i++){
		for(int j = 0; j < columns; j++)
			result(i,j) = this->mat[i][j] - rhs; // using the overloaded indexing operator
	}
	return result;
}

template<typename T>
QSMatrix<T>& QSMatrix<T>::operator-=(const T& rhs){
	for(int i =0; i < rows; i++){
		for(int j = 0; j < columns; j++)
			this->mat[i][j] -= rhs; // using the overloaded indexing operator
	}
	return *this;
}

template<typename T>
QSMatrix<T> QSMatrix<T>::operator*(const T& rhs){
	QSMatrix<T> result(rows, columns, 0.0);
	for(int i =0; i < rows; i++){
		for(int j = 0; j < columns; j++)
			result(i,j) = this->mat[i][j] * rhs; // using the overloaded indexing operator
	}
	return result;
}

template<typename T>
QSMatrix<T>& QSMatrix<T>::operator*=(const T& rhs){
	for(int i =0; i < rows; i++){
		for(int j = 0; j < columns; j++)
			this->mat[i][j] *= rhs; // using the overloaded indexing operator
	}
	return *this;
}

template<typename T>
QSMatrix<T> QSMatrix<T>::operator/(const T& rhs){

	if(rhs == 0)
		return;

	QSMatrix<T> result(rows, columns, 0.0);
	for(int i =0; i < rows; i++){
		for(int j = 0; j < columns; j++)
			result(i,j) = this->mat[i][j]/rhs; // using the overloaded indexing operator
	}
	return result;
}

template<typename T>
QSMatrix<T>& QSMatrix<T>::operator/=(const T& rhs){

	if(rhs == 0)
		return;

	for(int i =0; i < rows; i++){
		for(int j = 0; j < columns; j++)
			this->mat[i][j] /= rhs; // using the overloaded indexing operator
	}
	return *this;
}

template<typename T>
QSMatrix<T> QSMatrix<T>::operator*(const std::vector<T>& rhs){

	if(rhs.size() != this->get_columns())
		return;

	QSMatrix result(rows, columns,0.0);
	for(int i =0; i < rows; i++){
		for(int j = 0; j < columns; j++)
			result(i,j) = this->mat[i][j]*rhs[j]; // using the overloaded indexing operator
	}
	return result;
}

template<typename T>
QSMatrix<T> QSMatrix<T>::transpose(){

	QSMatrix result(columns, rows,0.0);
	for(int i =0; i < rows; i++){
		for(int j = 0; j < columns; j++)
			result(j,i) = this->mat[i][j]; // using the overloaded indexing operator
	}
	return result;
}

template<typename T>
unsigned QSMatrix<T>::get_rows() const{
	return rows;
}

template<typename T>
unsigned QSMatrix<T>::get_columns() const{
	return columns;
}

template<typename T>
T& QSMatrix<T>::operator()(const unsigned& rows, const unsigned& columns){
	return this->mat[rows][columns];
}

template<typename T>
const T& QSMatrix<T>::operator()(const unsigned& rows, const unsigned& columns) const{
	return this->mat[rows][columns];
}

template<typename T>
std::ostream& operator<<(std::ostream& out, const QSMatrix<T>& matrix){
	for(int i = 0; i < matrix.get_rows(); i++){
		for(int j = 0; j < matrix.get_columns(); j++){
			out << matrix(i,j) << " ";
		}
		out << endl;
	}
	return out;
}

int main() {

	QSMatrix<double> mat1(4,4,1);
	QSMatrix<double> mat2(4,4,3);

	QSMatrix<double> matrix = mat1 + mat2;

	// cout << mat3 << endl;
	for(int i = 0; i < matrix.get_rows(); i++){
		for(int j = 0; j < matrix.get_columns(); j++){
			cout << matrix(i,j) << " ";
		}
		cout << endl;
	}

}

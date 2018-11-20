#ifndef CMATRIX_HPP
#define CMATRIX_HPP

#include "magma_types.h" 
#include "magma_z.h" 
#include "magma_auxiliary.h" 
#include "alglib/linalg.h"


class ComplexMatrix
{	
public:

	magmaDoubleComplex *magma_array;
	alglib::complex_2d_array alg_array;

	ComplexMatrix(uint n);
	ComplexMatrix(const ComplexMatrix &c);
	~ComplexMatrix();

	const ComplexMatrix &operator=(const ComplexMatrix &cm);


	uint m_n;
	uint N() {return m_n;}
	uint size() {return m_n * m_n;}


	
	void print_magma(std::string s) const;
	void print_alg(std::string s) const;

	void generate();
	void generate_test();
	void transpose();
	void copy(const ComplexMatrix &cm);

	void makeLMatrix();
	void makeUMatrix();

	static double m_min;
	static double m_max;
	static double m_cond;
	void set_alg_array(const alglib::complex_2d_array &array);
	magmaDoubleComplex alg_to_magma(const alglib::complex &n);
	alglib::complex magma_to_alg(const magmaDoubleComplex &n);
	void alg_to_magma_array();
	void magma_to_alg_array();
private:
	

	

	void init(uint n);
	void m_clean();
	void zero();
};

ComplexMatrix operator*(const ComplexMatrix &l, const ComplexMatrix &r);

#endif
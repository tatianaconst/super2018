#include "cmatrix.hpp"



ComplexMatrix::ComplexMatrix(uint n)
	: m_n(0), magma_array(NULL)
{
	init(n);
}

ComplexMatrix::~ComplexMatrix()
{
	m_clean();
}

ComplexMatrix::ComplexMatrix(const ComplexMatrix &m)
{
    init(m.m_n);
    alg_array = m.alg_array;
    memcpy(magma_array, m.magma_array, size() * sizeof(magmaDoubleComplex));
}


void ComplexMatrix::set_alg_array(const alglib::complex_2d_array &array)
{

    init(array.rows());

    int offset = 0;
    for (uint i = 0; i < m_n; ++i) {
        const alglib::complex *row = array[i];
        for (uint j = 0; j < m_n; ++j)
            magma_array[offset++] = alg_to_magma(row[j]);
    }
}

// ComplexMatrix ComplexMatrix::operator*(const ComplexMatrix &r)
// {
	
//     uint n = m_n;
//     alglib::complex_2d_array larray = alg_array;
//     alglib::complex_2d_array rarray = r.alg_array;
//     alglib::complex_2d_array mul_matrix;
//     mul_matrix.setlength(n, n);

//     // перемножаем матрицы с помощью функции из alglib
//     alglib::cmatrixgemm(n, n, n, 1,
//                         larray, 0, 0, 0,
//                         rarray, 0, 0, 0, 0,
//                         mul_matrix, 0,0);
//     ComplexMatrix result(n);
//     result.set_alg_array(mul_matrix);

//     return result;
// }



void ComplexMatrix::init(uint n)
{
    m_n = n;
    if (n == 0) {
        magma_array = NULL;
    }
    else {
        magma_array = new magmaDoubleComplex[size()];
        alg_array.setlength(n, n);
    }
}

void ComplexMatrix::m_clean()
{
    delete []magma_array;
}

void ComplexMatrix::generate()
{
	alglib::cmatrixrndcond(m_n, m_cond, alg_array);
	//init(alg_array.rows());
	alg_to_magma_array();
	
}	

void ComplexMatrix::generate_test()
{
	for (uint i = 0; i < m_n; ++i) {
		for (uint j = 0; j < m_n; ++j) {
			magmaDoubleComplex m;
			m.x = i * m_n + j;
			m.y = 0;
			magma_array[i * m_n + j] = m;
		}
	}
	magma_to_alg_array();
}	

void ComplexMatrix::transpose()
{
	alglib::complex_2d_array m = alg_array;
	alglib::cmatrixtranspose(m_n, m_n, m, 0, 0, alg_array, 0, 0);
	alg_to_magma_array();
}
void ComplexMatrix::print_magma(std::string s) const
{
	std::cout << "MAGMA " << s << std::endl;
	for (uint i = 0; i < m_n; ++i) {
		for (uint j = 0; j < m_n; ++j)
		{
			magmaDoubleComplex m = magma_array[i * m_n +j];
			printf("x=%f, y=%f\t\t", m.x, m.y);
		}
		printf("\n");
	}
	print_alg(s);
	printf("\n\n\n");

}

void ComplexMatrix::print_alg(std::string s) const
{
	std::cout << "ALGLIB " << s << std::endl;
	for (uint i = 0; i < m_n; ++i) {
		const alglib::complex *row = alg_array[i];
		for (uint j = 0; j < m_n; ++j)
		{
			alglib::complex m = row[j];
			printf("x=%f, y=%f\t\t", m.x, m.y);
		}
		printf("\n");
	}
	printf("\n\n\n");

}

magmaDoubleComplex ComplexMatrix::alg_to_magma(const alglib::complex &n)
{
	magmaDoubleComplex m;
	m.x = n.x;
	m.y = n.y;
	return m;
}

alglib::complex ComplexMatrix::magma_to_alg(const magmaDoubleComplex &n)
{
	return alglib::complex(n.x, n.y);
}
void ComplexMatrix::zero()
{
	alglib::complex z_array[m_n*m_n];
	for (uint i = 0; i < m_n*m_n; ++i)
		z_array[i] = 0;
	alg_array.setcontent(m_n, m_n, z_array);
	alg_to_magma_array();
}

void ComplexMatrix::makeLMatrix()
{
	for (uint i = 0; i < m_n; ++i) {
		for (uint j = i + 1; j < m_n; ++j) {
			magma_array[i * m_n + j].x = 0;
			magma_array[i * m_n + j].y = 0;
		}
		magmaDoubleComplex e;
		e.x = 1;
		e.y = 0;
		magma_array[i * m_n + i] = e;
	}
	//alg_array.setcontent(m_n, m_n, z_array);
	magma_to_alg_array();
}

void ComplexMatrix::makeUMatrix()
{
	for (uint i = 0; i < m_n; ++i) {
		for (uint j = 0; j < i; ++j) {
			magma_array[i * m_n + j].x = 0;
			magma_array[i * m_n + j].y = 0;
		}
	}
	//alg_array.setcontent(m_n, m_n, z_array);
	magma_to_alg_array();
}


void ComplexMatrix::alg_to_magma_array()
{
	//uint off = 0;
	for (uint i = 0; i < m_n; ++i) {
		const alglib::complex *row = alg_array[i];
		for (uint j = 0; j < m_n; ++j)
			magma_array[j * m_n + i] = alg_to_magma(row[j]);
	}
}

void ComplexMatrix::magma_to_alg_array()
{
	alglib::complex new_alg_array[m_n * m_n];
	uint off = 0;
	for (uint i = 0; i < m_n; ++i) {
		//const magmaDoubleComplex *row = magma_array[i];
		for (uint j = 0; j < m_n; ++j)
			new_alg_array[off++] = magma_to_alg(magma_array[j * m_n + i]);
	}
	alg_array.setcontent(m_n, m_n, new_alg_array);
}

ComplexMatrix operator*(const ComplexMatrix &l, const ComplexMatrix &r)
{
    uint n = l.m_n;

    printf("operator* left\n");
    l.print_magma("left*");
    printf("operator* right\n");
    r.print_magma("right*");
    alglib::complex_2d_array larray = l.alg_array;
    alglib::complex_2d_array rarray = r.alg_array;
    alglib::complex_2d_array mul_matrix;
    mul_matrix.setlength(n, n);

    alglib::cmatrixgemm(n, n, n, 1,
                        larray, 0, 0, 0,
                        rarray, 0, 0, 0, 0,
                        mul_matrix, 0,0);
    ComplexMatrix result(n);
    //result.set_alg_array(mul_matrix);
    result.alg_array = mul_matrix;
    result.alg_to_magma_array();

    return result;
}

// void ComplexMatrix::magma_to_alg_array()
// {
// 	uint off = 0;
// 	for (uint i = 0; i < m_n; ++i) {
// 		const magmaDoubleComplex *row = magma_array[i];
// 		for (uint j = 0; j < m_n; ++j)
// 			alg_array[off++] = magma_to_alg(row[j]);
// 	}
// }
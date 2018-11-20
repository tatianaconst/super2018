#include "cmatrix.hpp"

double ComplexMatrix::m_min = 0.1;
double ComplexMatrix::m_max = 999.9;
double ComplexMatrix::m_cond = 1.0001;

typedef std::list<uint> ListUint;
static ListUint matrix_sizes()
{
    ListUint sizes;
    for (uint i = 3; i < 4; ++i)
    	sizes.push_back(i);
    // for (uint i = 1000; i <1010; ++i)
    //     sizes.push_back(i);
    // for (uint i = 200; i <210; ++i)
    //     sizes.push_back(i);
    // for (uint i = 2000; i <2005; ++i)
    //     sizes.push_back(i);
    return sizes;
}

bool checkLU(const ComplexMatrix matrixBase, ComplexMatrix L,
	ComplexMatrix U, const int ipiv[])
{

	uint n = matrixBase.m_n;

	//ComplexMatrix P;
    //VecUint permutations = getPermutations(ipiv, n);


    //P.setPermutations(permutations.data(), n);

    ComplexMatrix LU = L * U;
    LU.print("L*U");

    //Matrix PLU = P * LU;


    //const  *array_PLU = PLU.array();
   // const ValueType *array_M = M.array();

    // for (uint i = 0; i < n * n; ++i) {
    //     static double eps = 0.01;
    //     ValueType delta = ABS(array_PLU[i] - array_M[i]);
    //     if(delta > eps) {
    //         std::cout << delta << std::endl;
    //         return false;
    //     }
    // }
    return true;
}


void func_for_n(uint num, uint gpu)
{
	ComplexMatrix matrixBase(num);
	//matrixBase.generate();
	matrixBase.generate_test();

    matrixBase.print("matrixBase");

	 ComplexMatrix matrixRes(matrixBase);
	// matrixRes.transpose();
	// matrixRes.print_magma();


	magma_int_t ngpu = gpu;
	magma_int_t m = num;
	magma_int_t n = num;
	//magmaDoubleComplex *A = matrixRes.magma_array;
	magma_int_t lda = num;
	magma_int_t ipiv[num];
	magma_int_t info; 

	


	double start_time = magma_wtime();

	magma_int_t func_result = magma_zgetrf_m (
        gpu, m, n, matrixRes.magma_array, lda, ipiv, &info);
	if (info != 0)
		printf("INFO");
	printf("INFO=%d\n", info);

	double fin_time = magma_wtime();

	matrixRes.magma_to_alg_array();


	ComplexMatrix matrixL(matrixRes);
    matrixL.print("matrixRes");
	matrixL.makeLMatrix();
    matrixL.print("L");

	ComplexMatrix matrixU(matrixRes);
	//matrixU.print_magma();
	matrixU.makeUMatrix();
    matrixU.print("U");

	checkLU(matrixBase, matrixL, matrixU, ipiv);
}

int main(int argc, char *argv[])
{
	if (magma_init() == MAGMA_SUCCESS) {
		printf("Magma_init\n");
	}
	ListUint msizes = matrix_sizes();
	for (ListUint::const_iterator it = msizes.begin();
         it != msizes.end();
         ++it)
    {
        func_for_n(*it, 1);
    }
}

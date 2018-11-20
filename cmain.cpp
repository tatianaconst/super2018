#include "cmatrix.hpp"

double ComplexMatrix::m_min = 0.1;
double ComplexMatrix::m_max = 999.9;
double ComplexMatrix::m_cond = 1.0001;



#define ABS(x) ((x) > 0 ? (x) : (-(x)))

// typedef std::list<uint> ListUint;
// static ListUint matrix_sizes()
// {
//     ListUint sizes;
//     for (uint i = 1000; i < 32000; i = i * 1.2)
//     	sizes.push_back(i);
//     return sizes;
// }


bool checkLU(const ComplexMatrix matrixBase, ComplexMatrix L,
	ComplexMatrix U, int ipiv[])
{
	uint n = matrixBase.m_n;

	//ComplexMatrix P;
    //VecUint permutations = getPermutations(ipiv, n);


    //P.setPermutations(permutations.data(), n);

    ComplexMatrix LU = L * U;
    LU.print("L*U");
    //LU.alg_array.transpose();


    magma_int_t *newpiv = new magma_int_t[n * n];


    //printf("ok3\n");
	magma_swp2pswp(MagmaNoTrans, n, ipiv, newpiv);
	//printf("ok4\n");



	for (uint i = 0; i < n; ++i) {
		//printf("%d\n", newpiv[i]);
		const alglib::complex *rowLU = LU.alg_array[i];
		const alglib::complex *rowBase = matrixBase.alg_array[newpiv[i]];
		for (uint j = 0; j < n; ++j) {
			double deltaX = ABS(rowLU[j].x - rowBase[j].x);
			double deltaY = ABS(rowLU[j].y - rowBase[j].y);
			double eps = 0.001;
			if ((deltaX > eps) || (deltaY > eps)) {
				printf("Error: %d %d : %f %f : %f %f\n",i, j, rowBase[j].x, rowLU[j].x, rowBase[j].y, rowLU[j].y);
				delete[] newpiv;
				return false;
			}
		}
	}
	delete[] newpiv;
    return true;
}


void func_for_n(uint num, uint gpu)
{
	ComplexMatrix matrixBase(num);
	matrixBase.generate_magma_only();


    //matrixBase.print("matrixBase");

    ComplexMatrix matrixRes(matrixBase);



	magma_int_t ngpu = gpu;
	magma_int_t m = num;
	magma_int_t n = num;
	magma_int_t lda = num;
	magma_int_t ipiv[num];
	magma_int_t info; 

	for (uint i = 0; i < 5; ++i) {

		double start_time = magma_wtime();

		magma_int_t func_result = magma_zgetrf_m (
	        gpu, m, n, matrixRes.magma_array, lda, ipiv, &info);


		double fin_time = magma_wtime();

		printf("###,%d,1,0,%f\n", num, fin_time - start_time);
	}

	// matrixRes.magma_to_alg_array();


	// ComplexMatrix matrixL(matrixRes);
 //    matrixL.print("matrixRes");
	// matrixL.makeLMatrix();
 //    matrixL.print("L");

	// ComplexMatrix matrixU(matrixRes);

	// matrixU.makeUMatrix();
 //    matrixU.print("U");

	// if (checkLU(matrixBase, matrixL, matrixU, ipiv))
	// 	printf("Sucsess\n");
}

int main(int argc, char *argv[])
{
	FILE *sizes;
	//sizes = fopen("./sizes", "r+");
	//if (sizes == NULL) printf("FILE\n");
	int n;
	//fscanf(sizes,"%d", &n);
	for (;;) {
		sizes = fopen("./sizes1", "r");
		fscanf(sizes,"%d", &n);
		//printf("%d\n", n);
		fclose(sizes);

		func_for_n(n, 1);

		sizes = fopen("./sizes1", "w");
		//printf("%d\n", n);
		//printf("%d\n", (n*1.2>=32000)?1000:(n*1.2));
		int k = n*1.2;
		fprintf(sizes, "%d\n", ((k>=32000)?1000:(k)));
		fclose(sizes);

	}

	// for (;n < 32000; n = n * 1.2) {
	// 	func_for_n(n, 1);
	// 	fprintf(sizes, "%d", n);
	// }



	// ListUint msizes = matrix_sizes();


	// for (ListUint::const_iterator it = msizes.begin();
 //         it != msizes.end();
 //         ++it)
 //    {
 //        func_for_n(*it, 1);
 //    }
}

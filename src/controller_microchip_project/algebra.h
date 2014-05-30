/******************************************************************************
 *  PROJECT	     	: DPI2007-61527
 *  AUTHOR	     	: Antonio Camacho Santiago (among others)
 *  FILE		 	: algebra.h
 *  DESCRIPTION  	: collection of functions
 *  CPU TYPE     	: dsPIC33FJ256MC710.
 *  SOFTWARE        : ERIKA Real-Time Kernel, version 1.4.3 (code name "Elisa")
 *  COMPANY	     	: Automatic Control Department,
 *  				  Technical University of Catalonia
 *
 *  REVISION HISTORY:
 *	VERSION			: 0.1
 *  AUTHOR			: Antonio Camacho Santiago
 * 	DATE			: 28th July 2008
 * 	COMMENTS		: Matrix operations: Transpose_Matrix, Multiply_Matrices
 * 					  Multiply_Matrix_by_Scalar, Add_Matrices, Subtract_Matrices
 *  SOURCE			: http://mymathlib.webtrellis.net/index.html
 *
 *  REVISION HISTORY:
 *	VERSION			: 0.2
 *  AUTHOR			: Antonio Camacho Santiago
 * 	DATE			: September 18th, 2008
 * 	COMMENTS		: Multiply_Matrix_by_Scalar
 *                    don't overwrite A and allocating the result in Ax
 * 	SOURCE:
 *                    																	  																				  *
 *  REVISION HISTORY:
 *	VERSION			: 0.3
 *  AUTHOR			: Antonio Camacho Santiago
 * 	DATE			: 1st November 2008
 *  COMMENTS		: Additional matrix operations: Copy_Matrix, Copy_Vector
 * 					  Set_Submatrix, Get_Submatrix, Join_Matrices_by_Row
 * 	SOURCE			: http://mymathlib.webtrellis.net/index.html
 *
 *  REVISION HISTORY:
 *	VERSION			: 0.4
 *  AUTHOR			: Antonio Camacho Santiago
 * 	DATE			: 2nd November 2008
 * 	COMMENTS		: round to nearest integer: round2nearest
 *  SOURCE			:
 *
 *  REVISION HISTORY:
 *	VERSION			: 0.5
 *  AUTHOR			: Antonio Camacho Santiago
 * 	DATE			: 3rd November 2008
 * 	COMMENTS		: exponential of a matrix, convert to discrete: expm, c2d
 * 	SOURCE			:
 *
 *  REVISION HISTORY:
 *	VERSION			: 0.6
 *  AUTHOR			: Antonio Camacho Santiago
 * 	DATE			: 4th November 2008
 * 	COMMENTS		: Delay: Delay, Delay_Us
 * 	SOURCE			: ERIKA software ...ee\pkg\board\ee_flex\src\ee_board.c
 *  TODO			: Check values with oscilloscope
 *
 *  REVISION HISTORY:
 *	VERSION			: 0.8
 *  AUTHOR			: Antonio Camacho Santiago
 * 	DATE			: 6th November 2008
 * 	COMMENTS		: pseudo random generator: rnd_floating
 * 	SOURCE			: http://www.icsi.berkeley.edu/~sather/performance/benchmarks/data/matrix.c
 *
 *  REVISION HISTORY:
 *	VERSION			: 0.9
 *  AUTHOR			: Antonio Camacho Santiago
 * 	DATE			: 8th November 2008
 * 	COMMENTS		: SoftwareReset: Reset via software
 * 	SOURCE			:
 *
 *  REVISION HISTORY:
 *	VERSION			: 1.0
 *  AUTHOR			: Antonio Camacho Santiago
 * 	DATE			: 18th June 2009
 * 	COMMENTS		: Invert_Matrix: Easier than <dsp.h> MatrixInvert
 * 	SOURCE			:
 *
 *  REVISION HISTORY:
 *	VERSION			: 1.1
 *  AUTHOR			: Antonio Camacho Santiago
 * 	DATE			: 18th June 2009
 * 	COMMENTS		: continuous2discrete_poles: Discretization of continuous
 *                    poles for a sampling period h
 *                    Only for second order systems
 * 	SOURCE			:
 *
 *  REVISION HISTORY:
 *	VERSION			: 1.2
 *  AUTHOR			: Antonio Camacho Santiago
 *  DATE			: 18th June 2009
 * 	COMMENTS		: acker: Feedback gain K obtained via pole placement
 *                    using Ackermann's formula
 *                    Only for second order systems
 * 	SOURCE			: Gene F. Franklin, David J. Powell, and Michael L. Workman.
 *                    Digital Control of Dynamic Systems (3rd Edition).
 *                    Prentice Hall, December 1997.
 *
 *  REVISION HISTORY:
 *	VERSION			: 1.3
 *  AUTHOR			: Antonio Camacho Santiago
 * 	DATE			: 1st September 2009
 * 	COMMENTS		: subs: substitute h in Phi and Gamma for the electronic
 *                    double integrator
 *                    Only for second order systems
 * 	SOURCE:
 *****************************************************************************/


#include <math.h>
#include <dsp.h>

typedef struct{
	float real;
	float imag;
}complex;


/******************************************************************************
 * File: transpose_matrix.c
 * Routine(s):
 *    Transpose_Matrix
 ******************************************************************************
 ******************************************************************************
 *  void Transpose_Matrix(float *At, float *A, int nrows, int ncols)
 *
 * Description:
 *     Take the transpose of A and store in At, i.e. At = A'.
 *     The matrix At should be declared as float At[ncols][nrows] in the
 *     calling routine, and the matrix A declared as float A[nrows[ncols].
 *     In general, At and A should be disjoint i.e. their memory locations
 *     should be distinct.
 *
 *  Arguments:
 *     float *At   Pointer to the first element of the matrix At.
 *     float *A    Pointer to the first element of the matrix A.
 *     int    nrows The number of rows of matrix A and number of columns of
 *                  the matrix At.
 *     int    ncols The number of columns of the matrix A and the number of
 *                  rows of the matrix At.
 *
 *  Return Values:
 *     void
 *
 *  Example:
 *     #define N
 *     #define M
 *     float A[M][N],  At[N][M];
 *
 *     (your code to initialize the matrix A)
 *
 *     Transpose_Matrix(&At[0][0], &A[0][0], M, N);
 *     printf("The transpose of A is the matrix At \n"); ...
 *****************************************************************************/
void Transpose_Matrix(float *At, float *A, int nrows, int ncols)
{
   float *pA;
   float *pAt;
   int i,j;

   for (i = 0; i < nrows; At += 1, A += ncols, i++) {
      pAt = At;
      pA = A;
      for (j = 0; j < ncols; pAt += nrows, j++) *pAt = pA[j];
   }
}



/******************************************************************************
 * File: multiply_matrices.c
 * Routine(s):
 *    Multiply_Matrices
 ******************************************************************************
 ******************************************************************************
 *  void Multiply_Matrices(float *C, float *A, int nrows, int ncols,
 *                                                    float *B, int mcols)
 *
 *  Description:
 *     Post multiply the nrows x ncols matrix A by the ncols x mcols matrix
 *     B to form the nrows x mcols matrix C, i.e. C = A B.
 *     The matrix C should be declared as float C[nrows][mcols] in the
 *     calling routine.  The memory allocated to C should not include any
 *     memory allocated to A or B.
 *
 *  Arguments:
 *     float *C    Pointer to the first element of the matrix C.
 *     float *A    Pointer to the first element of the matrix A.
 *     int    nrows The number of rows of the matrices A and C.
 *     int    ncols The number of columns of the matrices A and the
 *                   number of rows of the matrix B.
 *     float *B    Pointer to the first element of the matrix B.
 *     int    mcols The number of columns of the matrices B and C.
 *
 *  Return Values:
 *     void
 *
 *  Example:
 *     #define N
 *     #define M
 *     #define NB
 *     float A[M][N],  B[N][NB], C[M][NB];
 *
 *     (your code to initialize the matrices A and B)
 *
 *     Multiply_Matrices(&C[0][0], &A[0][0], M, N, &B[0][0], NB);
 *     printf("The matrix C is \n"); ...
 *****************************************************************************/
void Multiply_Matrices(float *C, float *A, int nrows, int ncols, float *B, int mcols)
{
   float *pA = A;
   float *pB;
   float *p_B;
   float *pC = C;
   int i,j,k;

   for (i = 0; i < nrows; A += ncols, i++)
      for (p_B = B, j = 0; j < mcols; C++, p_B++, j++) {
         pB = p_B;
         pA = A;
         *C = 0.0;
         for (k = 0; k < ncols; pA++, pB += mcols, k++)
            *C += *pA * *pB;
      }
}





/******************************************************************************
 * File: mul_matrix_by_scalar.c
 * Routine(s):
 *    Multiply_Matrix_by_Scalar
 ******************************************************************************
 ******************************************************************************
 *  void Multiply_Matrix_by_Scalar(float *A, float x, int nrows, int ncols)
 *
 *  Description:
 *     Multiply each element of the matrix A by the scalar x.
 *
 *  Arguments:
 *     float *A    Pointer to the first element of the matrix A.
 *     float x     Scalar to multiply each element of the matrix A.
 *     int    nrows The number of rows of matrix A.
 *     int    ncols The number of columns of the matrix A.
 *
 *  Return Values:
 *     void
 *
 *  Example:
 *     #define N
 *     #define M
 *     float A[M][N],  x;
 *
 *     (your code to initialize the matrix A and scalar x)
 *
 *     Multiply_Matrix_by_Scalar(&A[0][0], x, M, N);
 *     printf("The matrix A is \n"); ...
 *****************************************************************************/
void Multiply_Matrix_by_Scalar(float *Ax, float *A, float x, int nrows, int ncols)
{
	   register int i;
	   register int n = nrows * ncols;

	   for (i = 0; i < n; i++) Ax[i] = A[i] * x;

}


/******************************************************************************
 * File: add_matrices.c
 * Routine(s):
 *    Add_Matrices
 ******************************************************************************
 ******************************************************************************
 *  void Add_Matrices(float *C, float *A, float *B, int nrows, int ncols)
 *
 *  Description:
 *     Add matrices A and B to form the matrix C, i.e. C = A + B.
 *     All matrices should be declared as " float X[nrows][ncols] " in the
 *     calling routine, where X = A, B, C.
 *
 *  Arguments:
 *     float *C    Address of the first element of the matrix C.
 *     float *A    Address of the first element of the matrix A.
 *     float *B    Address of the first element of the matrix B.
 *     int    nrows The number of rows of matrices A, B, and C.
 *     int    ncols The number of columns of the matrices A, B, and C.
 *
 *  Return Values:
 *      void
 *
 *  Example:
 *     #define N
 *     #define M
 *     float A[M][N],  B[M][N], C[M][N];
 *
 *     (your code to initialize the matrices A and B)
 *
 *     Add_Matrices((float *) C, &A[0][0], &B[0][0], M, N);
 *     printf("The matrix C = A + B is \n"); ...
 *****************************************************************************/
void Add_Matrices(float *C, float *A, float *B, int nrows, int ncols)
{
   register int i;
   register int n = nrows * ncols;

   for (i = 0; i < n; i++) C[i] = A[i] + B[i];
}



/******************************************************************************
 * File: subtract_matrices.c
 * Routine(s):
 *    Subtract_Matrices
 ******************************************************************************
 ******************************************************************************
 *  void Subtract_Matrices(float *C, float *A, float *B, int nrows,
 *                                                               int ncols)
 *
 *  Description:
 *     Subtract the matrix B from the matrix A to form the matrix C, i.e.
 *     C = A - B.
 *     All matrices should be declared as " float X[nrows][ncols] " in the
 *     calling routine, where X = A, B, C.
 *
 *  Arguments:
 *     float *C    Pointer to the first element of the matrix C.
 *     float *A    Pointer to the first element of the matrix A.
 *     float *B    Pointer to the first element of the matrix B.
 *     int    nrows The number of rows of matrices A and B.
 *     int    ncols The number of columns of the matrix A.
 *
 *  Return Values:
 *     void
 *
 *  Example:
 *     #define N
 *     #define M
 *     float A[M][N],  B[M][N], C[M][N];
 *
 *     (your code to initialize the matrices A and B)
 *
 *     Subtract_Matrices(&C[0][0], &A[0][0], &B[0][0], M, N);
 *     printf("The matrix C = A - B  is \n"); ...
 *****************************************************************************/
void Subtract_Matrices(float *C, float *A, float *B, int nrows, int ncols)
{
   register int i;
   register int n = nrows * ncols;

   for (i = 0; i < n; i++) C[i] = A[i] - B[i];
}

/******************************************************************************
 * File: ccopy_matrix.c
 * Routine(s):
 *    Copy_Matrix
 ******************************************************************************
 ******************************************************************************
 *  void Copy_Matrix(float *A, float *B, int nrows, int ncols)
 *
 *  Description:
 *     Copy the nrows x ncols matrix B to the nrows x ncols matrix A.
 *     i.e.    A = B.
 *
 *  Arguments:
 *     float *A    Pointer to the first element of the destination matrix
 *                  A[nrows][ncols].
 *     float *B    Pointer to the first element of the source matrix
 *                  B[nrows][ncols].
 *     int    nrows The number of rows matrices A and B.
 *     int    ncols The number of columns of the matrices A and B.
 *
 *  Return Values:
 *     void
 *
 *  Example:
 *     #define N
 *     #define M
 *     float A[N][M],  B[N][M];
 *
 *     (your code to initialize the matrix B)
 *
 *     Copy_Matrix(&A[0][0], &B[0][0], N, M);
 *     printf(" Matrix A is \n");
 *****************************************************************************/

void Copy_Matrix(float *A, float *B, int nrows, int ncols)
{
   int n = nrows * ncols;

   for (; n > 0; n--) *A++ = *B++;
}



/******************************************************************************
 * File: ccopy_vector.c
 * Routine(s):
 *    Copy_Vector
 ******************************************************************************
 ******************************************************************************
 *  void Copy_Vector(float *d, float *s, int n)
 *
 *  Description:
 *     Copy the n dimensional vector s(source) to the n dimensional
 *     vector d(destination).
 *
 *  Arguments:
 *      float *d  Pointer to the first element of the destination vector d.
 *      float *s  Pointer to the first element of the source vector s.
 *      int    n   The number of elements of the source / destination vectors.
 *
 *  Return Values:
 *     void
 *
 *  Example:
 *     #define N
 *     float v[N],  vd[N];
 *
 *     (your code to initialize the vector v)
 *
 *     Copy_Vector(vd, v, N);
 *     printf(" Vector vd is \n");
 *****************************************************************************/

void Copy_Vector(float *d, float *s, int n)
{
   for (; n > 0; n--) *d++ = *s++;
}



/******************************************************************************
 * File: set_submatrix.c
 * Routine(s):
 *    Set_Submatrix
 ******************************************************************************
 ******************************************************************************
 *  void Set_Submatrix(float *A, int ncols, float *S, int mrows, int mcols,
 *                                                        int row, int col)
 *
 *  Description:
 *     Copy the mrows x mcols submatrix S into the nrows x ncols matrix A
 *     starting at the location A[row][col].
 *
 *  Arguments:
 *     float *A    Pointer to the first element of the matrix A[n][n].
 *     int    ncols The number of columns of the matrix A.
 *     float *S    Destination address of the submatrix.
 *     int    mrows The number of rows matrix S.
 *     int    mcols The number of columns of the matrix S.
 *     int    row   The row of A corresponding to the first row of S.
 *     int    col   The column of A corresponding to the first column of S.
 *
 *  Return Values:
 *     void
 *
 *  Example:
 *     #define N
 *     #define M
 *     #define NB
 *     #define MB
 *     float A[M][N],  B[MB][NB];
 *     int row, col;
 *
 *     (your code to initialize the matrix A, submatrix B, row number row,
 *      and column number col )
 *
 *     if ( (row > 0) && ( row + MB < M ) && ( col > 0 ) && (col + NB < N)
 *        Set_Submatrix(&A[0][0], N, &B[0][0], MB, NB, row, col);
 *     printf("The matrix A is \n"); ...
 *****************************************************************************/
void Set_Submatrix(float *A, int ncols, float *S, int mrows, int mcols,
                                                           int row, int col) {
   int i,j;

   for ( A += row * ncols + col, i = 0; i < mrows; A += ncols, i++)
      for (j = 0; j < mcols; j++) *(A + j) = *S++;
}



/******************************************************************************
 * File: join_by_rows.c
 * Routine(s):
 *    Join_Matrices_by_Row
 ******************************************************************************
 ******************************************************************************
 *  void Join_Matrices_by_Row(float *C, float *A, int nrows, int ncols,
 *                                                     float *B, int mcols)
 *
 *  Description:
 *     Copy the nrows x ncols matrix A into the nrows x (ncols + mcols)
 *     matrix C and then copy the nrows x mcols matrix B to starting at
 *     the location C[0][ncols], i.e. C = [A:B].
 *     The matrix C should be declared as float C[nrows][ncols + mcols] in
 *     the calling routine.
 *
 *  Arguments:
 *     float *C    Pointer to the first element of the matrix C.
 *     float *A    Pointer to the first element of the matrix A.
 *     int    nrows The number of rows of matrices A and B.
 *     int    ncols The number of columns of the matrix A.
 *     float *B    Pointer to the first element of the matrix B.
 *     int    mcols The number of columns of the matrix B.
 *
 *  Return Values:
 *     void
 *
 *  Example:
 *     #define N
 *     #define M
 *     #define NB
 *     float A[M][N],  B[M][NB], C[M][N+NB];
 *
 *     (your code to initialize the matrices A and B)
 *
 *     Join_Matrices_by_Row(&C[0][0], &A[0][0], M, N, &B[0][0], NB);
 *     printf("The matrix C is \n"); ...
 *****************************************************************************/

#include <string.h>                             // required for memcpy()

void Join_Matrices_by_Row(float *C, float *A, int nrows, int ncols,
                                                          float *B, int mcols)
{
   for (; nrows > 0; nrows--) {
      memcpy( C, A, sizeof(float) * ncols );
      C += ncols;
      A += ncols;
      memcpy( C, B, sizeof(float) * mcols );
      C += mcols;
      B += mcols;
   }
}

/******************************************************************************
 * File: get_submatrix.c
 * Routine(s):
 *    Get_Submatrix
 ******************************************************************************
 ******************************************************************************
 *  void Get_Submatrix(float *S, int mrows, int mcols,
 *                                   float *A, int ncols, int row, int col)
 *
 *  Description:
 *     Copy the mrows and mcols of the nrows x ncols matrix A starting with
 *     A[row][col] to the submatrix S.
 *     Note that S should be declared float S[mrows][mcols] in the calling
 *     routine.
 *
 *  Arguments:
 *     float *S    Destination address of the submatrix.
 *     int    mrows The number of rows of the matrix S.
 *     int    mcols The number of columns of the matrix S.
 *     float *A    Pointer to the first element of the matrix A[nrows][ncols]
 *     int    ncols The number of columns of the matrix A.
 *     int    row   The row of A corresponding to the first row of S.
 *     int    col   The column of A corresponding to the first column of S.
 *
 *  Return Values:
 *     void
 *
 *  Example:
 *     #define N
 *     #define M
 *     #define NB
 *     #define MB
 *     float A[M][N],  B[MB][NB];
 *     int row, col;
 *
 *     (your code to set the matrix A, the row number row and column number
 *      col)
 *
 *     if ( (row >= 0) && (col >= 0) && ((row + MB) < M) && ((col + NB) < N) )
 *         Get_Submatrix(&B[0][0], MB, NB, &A[0][0], N, row, col);
 *     printf("The submatrix B is \n"); ... }
 *****************************************************************************/

//#include <string.h>                                    // required for memcpy()

void Get_Submatrix(float *S, int mrows, int mcols,
                                        float *A, int ncols, int row, int col)
{
   int number_of_bytes = sizeof(float) * mcols;

   for (A += row * ncols + col; mrows > 0; A += ncols, S+= mcols, mrows--)
      memcpy(S, A, number_of_bytes);
}


/******************************************************************************
 * Routine(s):
 *    Invert_Matrix
 ******************************************************************************
 ******************************************************************************
 * A trick to make easier matrix inversion
 * MatrixInvert needs #include <dsp.h>
 *****************************************************************************/

void Invert_Matrix(float *inv_A,float *A,int size)
{
	#define DST_ELEMS 8//2
	#define SRC_ELEMS 8//2
	float pivot[DST_ELEMS]; // Definitions used with MatrixInvert
	int swappedRow[DST_ELEMS];
	int swappedCols[DST_ELEMS];

	MatrixInvert(size, inv_A, A, pivot, swappedRow, swappedCols);
}




/******************************************************************************
 * Routine(s):
 *    round2nearest
 ******************************************************************************
 ******************************************************************************
 * Round to nearest integer
 * Another simple way to implement this function
 * is adding 0.5 and casting to integer
 *****************************************************************************/
float round2nearest(float value)
{
	float max;
	float min;
	max=ceil(value);
	min=floor(value);
	if ((max-value)>(value-min))
	{
		return min;
	}else{
		return max;
	}
}

/******************************************************************************
 * Routine(s):
 *    expm exponential of a matrix
 ******************************************************************************
 ******************************************************************************
 * Implements exponential of a matrix A using Taylor expansion
 *
 * See above c2d convert to discrete for details
 *
 *****************************************************************************/
void expm(float *expmAh, float *A, float h)
{

	#define n 2 //define the system order (number of state variables)

	#define number_of_iterations 10 //Taylor number of iterations

	static float eAh[n][n]={0};//eAh[n+1][n+1]={0};
	static float At[n][n]={0};
	static float temp1[n][n]={0};
	static float temp2[n][n]={0};
	static float temp3[n][n]={0};
	static unsigned char c1=0;
	static unsigned char c2=1;
	static unsigned char c3=0;
	static float den=1;

	den=1;//initialize den to 1
	for (c1=0;c1<n;c1++)//initialize expm to zero, another simplest way can't be implemented bla, bla, bla
	{
		for (c2=0;c2<n;c2++)
		{
			eAh[c1][c2]=0.0;
			temp1[c1][c2]=0.0;
			temp2[c1][c2]=0.0;
			temp3[c1][c2]=0.0;
		}
	}

	for (c1=0;c1<n;c1++)
	{
		eAh[c1][c1]=1.0;
		temp1[c1][c1]=1.0;
	}

	Multiply_Matrix_by_Scalar(&At[0][0],A, h, n, n);//This line could be placed in main()

	for (c3=1;c3<number_of_iterations;c3++)
	{
		den=den*c3;
		Multiply_Matrices(&temp2[0][0], &At[0][0], n, n, &temp1[0][0], n);
		for (c1=0;c1<n;c1++)
		{
			for (c2=0;c2<n;c2++)
			{
				temp1[c1][c2]=temp2[c1][c2];//Remember the value for next iteration
			}
		}

		Multiply_Matrix_by_Scalar(&temp3[0][0],&temp2[0][0], 1/den, n, n);
		Add_Matrices(&eAh[0][0], &eAh[0][0], &temp3[0][0], n, n);
	}

	for (c1=0;c1<n*n;c1++)
	{
		expmAh[c1]=eAh[0][c1];//Copying data to destination. It should be fixed!!!!
	}
}





/******************************************************************************
 * Routine(s):
 *    c2d convert to discrete
 ******************************************************************************
 ******************************************************************************
 * Implements discretization of A and B using Taylor expansion
 * and returns phi and gamma according to h
 *
 * Attention:
 *  Taylor expansions may not converge!!!!
 *  And m-file is shown at the end of this function to test
 *  convergence and select proper number of iterations
 *
 * x_dot=Ax+Bu	Continuous system
 *
 * x(k+1)=phi x(k) + gamma u(k)	Discrete system with sample time h
 *
 * Adapted from matlab, edit c2d for details
 *
 * Build an extended matrix AB:
 *  phi_gamma=expm([A,B;zeros]*h)
 *  and extract phi and gamma
 *
 * By default:
 *  n=2 System order
 *  number_of_iterations=10
 *
 * Example:
 *  static float A[n][n]={{1,2},{3,4}};
 *  static float B[n]={5,6};
 *  static float h=0.01;//in seconds
 *  static float phi[n][n]={0};
 *  static float gamma[n]={0};
 *  c2d(phi,gamma,A,B,h);
 *****************************************************************************/
void c2d(float *phi,float *gamma,float *A,float *B,float h)

{
	//x_dot=Ax+Bu
	//dim(A)=n·n matrix
	//dim(B)=n·1 matrix //still not implemented dim(B)=n·m

	#define n 2 //define the system order (number of state variables)

	#define number_of_iterations 10 //Taylor number of iterations
	//expm(Ah)=I + Ah + A^2 h^2 / 2! + A^3 h^3 / 3! + ... The series may not converge!!!

	static float AB[n+1][n+1]={0};
	static float eAh[n+1][n+1]={0};//eAh[n+1][n+1]={0};
	static float ABt[n+1][n+1]={0};
	static float temp1[n+1][n+1]={0};
	static float temp2[n+1][n+1]={0};
	static float temp3[n+1][n+1]={0};
	static unsigned char c1=0;
	static unsigned char c2=1;
	static unsigned char c3=0;
	static float den=1;

	Join_Matrices_by_Row(&AB[0][0], A,n,n,B,1);//Build extended space. Edit matlab function c2d for details

	den=1;//initialize den to 1
	for (c1=0;c1<n+1;c1++)//initialize expm to zero, another simplest way can't be implemented bla, bla, bla
	{
		for (c2=0;c2<n+1;c2++)
		{
			eAh[c1][c2]=0.0;
			temp1[c1][c2]=0.0;
			temp2[c1][c2]=0.0;
			temp3[c1][c2]=0.0;
		}
	}

	for (c1=0;c1<n+1;c1++)
	{
		eAh[c1][c1]=1.0;
		temp1[c1][c1]=1.0;
	}

	Multiply_Matrix_by_Scalar(ABt,AB, h, 3, 3);//This line could be placed in main()

	for (c3=1;c3<number_of_iterations;c3++)
	{
		den=den*c3;
		Multiply_Matrices(&temp2[0][0], &ABt[0][0], n+1, n+1, &temp1[0][0], n+1);
		for (c1=0;c1<n+1;c1++)
		{
			for (c2=0;c2<n+1;c2++)
			{
				temp1[c1][c2]=temp2[c1][c2];//Remember the value for next iteration
			}
		}

		Multiply_Matrix_by_Scalar(&temp3[0][0],&temp2[0][0], 1/den, n+1, n+1);
		Add_Matrices(&eAh[0][0], &eAh[0][0], &temp3[0][0], n+1, n+1);
	}

	Get_Submatrix(phi,n,n,eAh,n+1,0,0);
	Get_Submatrix(gamma,n,1,eAh,n+1,0,2);
}
/*---------------------------------------------------------------------------//
%m-file to compute the error of the Taylor expansion
disp('Beginning...')

A=[-25.1429,25.0000;25.0000,-50.1429];%[1,2;3,4]
B=[0;25];%[5;6];
h=0.05;
[phi,gamma]=c2d(A,B,h)
errorA11=[];errorA12=[];errorA21=[];errorA22=[];errorB11=[];errorB21=[];
eAh=eye(3);
for a=1:9 %number of iterations
    AB=[A,B;0 0 0];
    eAh=eAh+(AB*h)^a/factorial(a);
    errorA11=[errorA11,eAh(1,1)-phi(1,1)];
    errorA12=[errorA12,eAh(1,2)-phi(1,2)];
    errorA21=[errorA21,eAh(2,1)-phi(2,1)];
    errorA22=[errorA22,eAh(2,2)-phi(2,2)];
    errorB11=[errorB11,eAh(1,3)-gamma(1)];
    errorB21=[errorB21,eAh(2,3)-gamma(2)];
end
figure;
eAh
subplot(2,3,1);bar(errorA11);title('\Phi_{11}');
subplot(2,3,2);bar(errorA12);title('\Phi_{12}');
subplot(2,3,4);bar(errorA21);title('\Phi_{21}');
subplot(2,3,5);bar(errorA22);title('\Phi_{22}');
subplot(2,3,3);bar(errorB11);title('\Gamma_1');
subplot(2,3,6);bar(errorB21);title('\Gamma_2');
-----------------------------------------------------------------------------//
*/


/******************************************************************************
 * Routine(s):
 *    Delay_Us
*******************************************************************************
*******************************************************************************
 * Implements a delay routine
 * Extracted from ERIKA software
 *  ...ee\pkg\board\ee_flex\src\ee_board.c
 *
 * Example:
 * Delay_Us(Delay200uS_count);
 * Delay(Delay_40mS_Cnt);
 *****************************************************************************/
unsigned int lcd_temp_count = 0;
#define Fcy  40000000
#define Delay200uS_count  (Fcy * 0.0002) / 1080
#define Delay500uS_count  (Fcy * 0.0005) / 1080

void Delay_Us( unsigned int delayUs_count )
{
 lcd_temp_count = delayUs_count +1;
 asm volatile("outer1: dec _lcd_temp_count");
 asm volatile("cp0 _lcd_temp_count");
 asm volatile("bra z, done1");
 asm volatile("do #1500, inner1" );
 asm volatile("nop");
 asm volatile("inner1: nop");
 asm volatile("bra outer1");
 asm volatile("done1:");
}

#define Delay_1mS_Cnt   (Fcy * 0.001) / 2950
#define Delay_2mS_Cnt   (Fcy * 0.002) / 2950
#define Delay_5mS_Cnt   (Fcy * 0.005) / 2950
#define Delay_10mS_Cnt   (Fcy * 0.010) / 2950
#define Delay_20mS_Cnt   (Fcy * 0.020) / 2950
#define Delay_40mS_Cnt   (Fcy * 0.040) / 2950
#define Delay_50mS_Cnt   (Fcy * 0.050) / 2950
#define Delay_100ms_Cnt  (Fcy * 0.100) / 2950
#define Delay_300ms_Cnt  (Fcy * 0.300) / 2950
#define Delay_1S_Cnt   (Fcy * 1) / 2950
void Delay( unsigned int delay_count )
{
	lcd_temp_count = delay_count +1;
	asm volatile("outer: dec _lcd_temp_count");
	asm volatile("cp0 _lcd_temp_count");
	asm volatile("bra z, done");
	asm volatile("do #3200, inner" );
	asm volatile("nop");
	asm volatile("inner: nop");
	asm volatile("bra outer");
	asm volatile("done:");
}






/******************************************************************************
 * Routine(s):
 *    rnd_floating
 ******************************************************************************
 ******************************************************************************
 * pseudo random generator
 * Extracted from
 * http://www.icsi.berkeley.edu/~sather/performance/benchmarks/data/matrix.c
 *
 * Example:
 *  static float x;
 *  x=rnd_floating();
 *****************************************************************************/
long next()
{
	static long seed=997;
	seed=seed*123457+97;
	if(seed<0) seed= -seed;
	return seed;
}

float rnd_floating() { return (float)(next()%1000000)/1000000.0; }




/******************************************************************************
 * Función:	SoftwareReset()
 * Descripción:	Software reset for the dsPIC33
 ******************************************************************************
 ******************************************************************************
 * Software reset by moving the stack pointer
 *
 * Extracted from internet forums
 *
 * Example:
 *  SoftwareReset();
 *****************************************************************************/

void SoftwareReset (void)
{
	asm ("MOV #__SP_init, W15");//; Initalize the Stack Pointer
	asm ("MOV #__SPLIM_init, W0");//; Initialize the Stack Pointer Limit Register
	asm ("MOV W0, SPLIM");
	asm ("NOP");//; Add NOP to follow SPLIM initialization
}
















/******************************************************************************
 * Routine(s):
 *    continuous2discrete_poles
 ******************************************************************************
 ******************************************************************************
 * Converts continuous poles to discrete for sampling period h
 * discrete_poles=exp(continuous_poles*h)
 * poles belong to complex set of numbers
 *
 * Example:
 *  h=0.02;
 * 	continuous_poles[0].real=-5;
 * 	continuous_poles[0].imag=-12;
 * 	continuous_poles[1].real=-5;
 * 	continuous_poles[1].imag=+12;
 * 	continuous2discrete_poles(discrete_poles,continuous_poles,h);
 *
 * Result:
 * 	discrete_poles[0].real=0.8789030;
 * 	discrete_poles[0].imag=-0.2150822;
 * 	discrete_poles[1].real=0.8789030;
 * 	discrete_poles[1].imag=0.2150822;
 *****************************************************************************/
void continuous2discrete_poles(complex *discrete_poles,complex *continuous_poles, float h)
{
	discrete_poles[0].real=exp(continuous_poles[0].real*h)*cos(continuous_poles[0].imag*h);
	discrete_poles[0].imag=exp(continuous_poles[0].real*h)*sin(continuous_poles[0].imag*h);
	discrete_poles[1].real=exp(continuous_poles[1].real*h)*cos(continuous_poles[1].imag*h);
	discrete_poles[1].imag=exp(continuous_poles[1].real*h)*sin(continuous_poles[1].imag*h);

}



/******************************************************************************
 * Routine(s):
 *    acker
 ******************************************************************************
 ******************************************************************************
 * Finds the feedback gain K for space-state control u=-K·x via pole placement
 * using Ackermann's formula
 *
 * References: Gene F. Franklin, David J. Powell, and Michael L. Workman.
 *             Digital Control of Dynamic Systems (3rd Edition).
 *             Prentice Hall, December 1997.
 *
 *             The mathworks: acker - Pole placement design for single-input systems
 *             http://www.mathworks.es/access/helpdesk/help/toolbox/control/
 *             index.html?/access/helpdesk/help/toolbox/control/ref/acker.html
 *             Note: acker is limited to single-input systems and the pair
 *             must be controllable.
 *
 * Example:
 *  acker(K,phi,gamma,discrete_poles);
 *****************************************************************************/
void acker(float *K, float *phi, float *gamma, complex *discrete_poles)
{
	static float ctrb[2][2]={{0,0},{0,0}};
	static float inv_ctrb[2][2]={{0,0},{0,0}};

	static float phi_gamma[2]={0,0};
	static float e[2]={0,1.0};
	static float phi_2[2][2]={{0,0},{0,0}};
	static float b1_phi[2][2]={{0,0},{0,0}};
	static float eye2[2][2]={{1,0},{0,1}};
	static float b2_eye2[2][2]={{0,0},{0,0}};
	static float e_invctrb[2]={0,0};
	static float phi2_b1phi[2][2]={{0,0},{0,0}};
	static float pd[2][2]={{0,0},{0,0}};
	static float b1=2;
	static float b2=3;
	Multiply_Matrices(phi_gamma, phi, 2, 2, gamma, 1);
	Join_Matrices_by_Row(ctrb, gamma,2,1,phi_gamma,2);

	Invert_Matrix(inv_ctrb,ctrb,2);


	b1=-(discrete_poles[0].real+discrete_poles[1].real);
	b2=discrete_poles[0].real*discrete_poles[1].real-discrete_poles[0].imag*discrete_poles[1].imag;


	Multiply_Matrices(phi_2, phi, 2, 2, phi, 2);
	Multiply_Matrix_by_Scalar(b1_phi,phi, b1, 2, 2);
	Multiply_Matrix_by_Scalar(b2_eye2,eye2, b2, 2, 2);
	Add_Matrices(phi2_b1phi, phi_2, b1_phi, 2, 2);
	Add_Matrices(pd, phi2_b1phi, b2_eye2, 2, 2);


	Multiply_Matrices(e_invctrb, e, 1, 2, inv_ctrb, 2);

	Multiply_Matrices(K, e_invctrb, 1, 2, pd, 2);
}


/******************************************************************************
 * Routine(s):
 *    subs
 ******************************************************************************
 ******************************************************************************
 * An easy and cost-inexpensive way to compute Phi and Gamma for the electronic
 * double integrator substituting h in the discrete state-space matrices Phi and
 * Gamma
 *
 * Continuous state-space
 * x_dot=Ax+Bu
 * x_dot=[0 -1/(R1*C1);0 0]x + [0;-1/(R2*C2)]
 *
 * Discrete state-space
 * x(k+1)=Phi(h) x(k) + Gamma(h) *u(k)
 * x(k+1)=[1 -h/(R1*C1);0 1]x(k) + [h^2/(2*R1*C1*R2*C2);-h/(R2*C2)]
 *
 * Example:
 *  static float Phi[2][2]={{0,0},{0,0}};
 *  static float Gamma[2]={0,0};
 *	h=0.05;
 *	subs(Phi,Gamma,h);
 *
 *****************************************************************************/
void subs(float *phi,float *gamma, float h)
{
	#define Res1 100000
	#define Res2 100000
	#define Cap1 0.000000470
	#define Cap2 0.000000470
	*phi=1;
	phi++;
	*phi=-h/(Res1*Cap1);
    phi++;
    *phi=0;
    phi++;
    *phi=1;

	*gamma=1/(2*Res1*Cap1*Res2*Cap2)*h*h;
	gamma++;
	*gamma=-h/(Res2*Cap2);
    gamma++;
}

/******************************************************************************
 * Delay routine
 * void delay_in_ms(unsigned int time_in_ms);
 * Example:
 *      delay_in_ms(10);  // 10ms delay
******************************************************************************/
void delay_in_ms(unsigned int time_in_ms)
{
	unsigned int i, j;
	for (i=0; i<time_in_ms; i++){
		j=0;
		do{ j++;}while(j<=9962);
	}
}




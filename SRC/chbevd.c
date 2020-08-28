/* chbevd.f -- translated by f2c (version 20061008).
   You must link the resulting object file with libf2c:
	on Microsoft Windows system, link with libf2c.lib;
	on Linux or Unix systems, link with .../path/to/libf2c.a -lm
	or, if you install libf2c.a in a standard place, with -lf2c -lm
	-- in that order, at the end of the command line, as in
		cc *.o -lf2c -lm
	Source for libf2c is in /netlib/f2c/libf2c.zip, e.g.,

		http://www.netlib.org/f2c/libf2c.zip
*/

#include "f2c.h"
#include "blaswrap.h"

/* Table of constant values */

static complex c_b1 = {0.f,0.f};
static complex c_b2 = {1.f,0.f};
static real c_b13 = 1.f;
static integer c__1 = 1;

/* Subroutine */ int chbevd_(char *jobz, char *uplo, integer *n, integer *kd, 
	complex *ab, integer *ldab, real *w, complex *z__, integer *ldz, 
	complex *work, integer *lwork, real *rwork, integer *lrwork, integer *
	iwork, integer *liwork, integer *info)
{
    /* System generated locals */
    integer ab_dim1, ab_offset, z_dim1, z_offset, i__1;
    real r__1;

    /* Builtin functions */
    double sqrt(doublereal);

    /* Local variables */
    real eps;
    integer inde;
    real anrm;
    integer imax;
    real rmin, rmax;
    integer llwk2;
    extern /* Subroutine */ int cgemm_(char *, char *, integer *, integer *, 
	    integer *, complex *, complex *, integer *, complex *, integer *, 
	    complex *, complex *, integer *);
    real sigma;
    extern logical lsame_(char *, char *);
    integer iinfo;
    extern /* Subroutine */ int sscal_(integer *, real *, real *, integer *);
    integer lwmin;
    logical lower;
    integer llrwk;
    logical wantz;
    integer indwk2;
    extern doublereal clanhb_(char *, char *, integer *, integer *, complex *, 
	     integer *, real *);
    integer iscale;
    extern /* Subroutine */ int clascl_(char *, integer *, integer *, real *, 
	    real *, integer *, integer *, complex *, integer *, integer *), cstedc_(char *, integer *, real *, real *, complex *, 
	    integer *, complex *, integer *, real *, integer *, integer *, 
	    integer *, integer *), chbtrd_(char *, char *, integer *, 
	    integer *, complex *, integer *, real *, real *, complex *, 
	    integer *, complex *, integer *);
    extern doublereal slamch_(char *);
    extern /* Subroutine */ int clacpy_(char *, integer *, integer *, complex 
	    *, integer *, complex *, integer *);
    real safmin;
    extern /* Subroutine */ int xerbla_(char *, integer *);
    real bignum;
    integer indwrk, liwmin;
    extern /* Subroutine */ int ssterf_(integer *, real *, real *, integer *);
    integer lrwmin;
    real smlnum;
    logical lquery;


/*  -- LAPACK driver routine (version 3.2) -- */
/*     Univ. of Tennessee, Univ. of California Berkeley and NAG Ltd.. */
/*     November 2006 */

/*     .. Scalar Arguments .. */
/*     .. */
/*     .. Array Arguments .. */
/*     .. */

/*  Purpose */
/*  ======= */

/*  CHBEVD computes all the eigenvalues and, optionally, eigenvectors of */
/*  a complex Hermitian band matrix A.  If eigenvectors are desired, it */
/*  uses a divide and conquer algorithm. */

/*  The divide and conquer algorithm makes very mild assumptions about */
/*  floating point arithmetic. It will work on machines with a guard */
/*  digit in add/subtract, or on those binary machines without guard */
/*  digits which subtract like the Cray X-MP, Cray Y-MP, Cray C-90, or */
/*  Cray-2. It could conceivably fail on hexadecimal or decimal machines */
/*  without guard digits, but we know of none. */

/*  Arguments */
/*  ========= */

/*  JOBZ    (input) CHARACTER*1 */
/*          = 'N':  Compute eigenvalues only; */
/*          = 'V':  Compute eigenvalues and eigenvectors. */

/*  UPLO    (input) CHARACTER*1 */
/*          = 'U':  Upper triangle of A is stored; */
/*          = 'L':  Lower triangle of A is stored. */

/*  N       (input) INTEGER */
/*          The order of the matrix A.  N >= 0. */

/*  KD      (input) INTEGER */
/*          The number of superdiagonals of the matrix A if UPLO = 'U', */
/*          or the number of subdiagonals if UPLO = 'L'.  KD >= 0. */

/*  AB      (input/output) COMPLEX array, dimension (LDAB, N) */
/*          On entry, the upper or lower triangle of the Hermitian band */
/*          matrix A, stored in the first KD+1 rows of the array.  The */
/*          j-th column of A is stored in the j-th column of the array AB */
/*          as follows: */
/*          if UPLO = 'U', AB(kd+1+i-j,j) = A(i,j) for max(1,j-kd)<=i<=j; */
/*          if UPLO = 'L', AB(1+i-j,j)    = A(i,j) for j<=i<=min(n,j+kd). */

/*          On exit, AB is overwritten by values generated during the */
/*          reduction to tridiagonal form.  If UPLO = 'U', the first */
/*          superdiagonal and the diagonal of the tridiagonal matrix T */
/*          are returned in rows KD and KD+1 of AB, and if UPLO = 'L', */
/*          the diagonal and first subdiagonal of T are returned in the */
/*          first two rows of AB. */

/*  LDAB    (input) INTEGER */
/*          The leading dimension of the array AB.  LDAB >= KD + 1. */

/*  W       (output) REAL array, dimension (N) */
/*          If INFO = 0, the eigenvalues in ascending order. */

/*  Z       (output) COMPLEX array, dimension (LDZ, N) */
/*          If JOBZ = 'V', then if INFO = 0, Z contains the orthonormal */
/*          eigenvectors of the matrix A, with the i-th column of Z */
/*          holding the eigenvector associated with W(i). */
/*          If JOBZ = 'N', then Z is not referenced. */

/*  LDZ     (input) INTEGER */
/*          The leading dimension of the array Z.  LDZ >= 1, and if */
/*          JOBZ = 'V', LDZ >= max(1,N). */

/*  WORK    (workspace/output) COMPLEX array, dimension (MAX(1,LWORK)) */
/*          On exit, if INFO = 0, WORK(1) returns the optimal LWORK. */

/*  LWORK   (input) INTEGER */
/*          The dimension of the array WORK. */
/*          If N <= 1,               LWORK must be at least 1. */
/*          If JOBZ = 'N' and N > 1, LWORK must be at least N. */
/*          If JOBZ = 'V' and N > 1, LWORK must be at least 2*N**2. */

/*          If LWORK = -1, then a workspace query is assumed; the routine */
/*          only calculates the optimal sizes of the WORK, RWORK and */
/*          IWORK arrays, returns these values as the first entries of */
/*          the WORK, RWORK and IWORK arrays, and no error message */
/*          related to LWORK or LRWORK or LIWORK is issued by XERBLA. */

/*  RWORK   (workspace/output) REAL array, */
/*                                         dimension (LRWORK) */
/*          On exit, if INFO = 0, RWORK(1) returns the optimal LRWORK. */

/*  LRWORK  (input) INTEGER */
/*          The dimension of array RWORK. */
/*          If N <= 1,               LRWORK must be at least 1. */
/*          If JOBZ = 'N' and N > 1, LRWORK must be at least N. */
/*          If JOBZ = 'V' and N > 1, LRWORK must be at least */
/*                        1 + 5*N + 2*N**2. */

/*          If LRWORK = -1, then a workspace query is assumed; the */
/*          routine only calculates the optimal sizes of the WORK, RWORK */
/*          and IWORK arrays, returns these values as the first entries */
/*          of the WORK, RWORK and IWORK arrays, and no error message */
/*          related to LWORK or LRWORK or LIWORK is issued by XERBLA. */

/*  IWORK   (workspace/output) INTEGER array, dimension (MAX(1,LIWORK)) */
/*          On exit, if INFO = 0, IWORK(1) returns the optimal LIWORK. */

/*  LIWORK  (input) INTEGER */
/*          The dimension of array IWORK. */
/*          If JOBZ = 'N' or N <= 1, LIWORK must be at least 1. */
/*          If JOBZ = 'V' and N > 1, LIWORK must be at least 3 + 5*N . */

/*          If LIWORK = -1, then a workspace query is assumed; the */
/*          routine only calculates the optimal sizes of the WORK, RWORK */
/*          and IWORK arrays, returns these values as the first entries */
/*          of the WORK, RWORK and IWORK arrays, and no error message */
/*          related to LWORK or LRWORK or LIWORK is issued by XERBLA. */

/*  INFO    (output) INTEGER */
/*          = 0:  successful exit. */
/*          < 0:  if INFO = -i, the i-th argument had an illegal value. */
/*          > 0:  if INFO = i, the algorithm failed to converge; i */
/*                off-diagonal elements of an intermediate tridiagonal */
/*                form did not converge to zero. */

/*  ===================================================================== */

/*     .. Parameters .. */
/*     .. */
/*     .. Local Scalars .. */
/*     .. */
/*     .. External Functions .. */
/*     .. */
/*     .. External Subroutines .. */
/*     .. */
/*     .. Intrinsic Functions .. */
/*     .. */
/*     .. Executable Statements .. */

/*     Test the input parameters. */

    /* Parameter adjustments */
    ab_dim1 = *ldab;
    ab_offset = 1 + ab_dim1;
    ab -= ab_offset;
    --w;
    z_dim1 = *ldz;
    z_offset = 1 + z_dim1;
    z__ -= z_offset;
    --work;
    --rwork;
    --iwork;

    /* Function Body */
    wantz = lsame_(jobz, "V");
    lower = lsame_(uplo, "L");
    lquery = *lwork == -1 || *liwork == -1 || *lrwork == -1;

    *info = 0;
    if (*n <= 1) {
	lwmin = 1;
	lrwmin = 1;
	liwmin = 1;
    } else {
	if (wantz) {
/* Computing 2nd power */
	    i__1 = *n;
	    lwmin = i__1 * i__1 << 1;
/* Computing 2nd power */
	    i__1 = *n;
	    lrwmin = *n * 5 + 1 + (i__1 * i__1 << 1);
	    liwmin = *n * 5 + 3;
	} else {
	    lwmin = *n;
	    lrwmin = *n;
	    liwmin = 1;
	}
    }
    if (! (wantz || lsame_(jobz, "N"))) {
	*info = -1;
    } else if (! (lower || lsame_(uplo, "U"))) {
	*info = -2;
    } else if (*n < 0) {
	*info = -3;
    } else if (*kd < 0) {
	*info = -4;
    } else if (*ldab < *kd + 1) {
	*info = -6;
    } else if (*ldz < 1 || wantz && *ldz < *n) {
	*info = -9;
    }

    if (*info == 0) {
	work[1].r = (real) lwmin, work[1].i = 0.f;
	rwork[1] = (real) lrwmin;
	iwork[1] = liwmin;

	if (*lwork < lwmin && ! lquery) {
	    *info = -11;
	} else if (*lrwork < lrwmin && ! lquery) {
	    *info = -13;
	} else if (*liwork < liwmin && ! lquery) {
	    *info = -15;
	}
    }

    if (*info != 0) {
	i__1 = -(*info);
	xerbla_("CHBEVD", &i__1);
	return 0;
    } else if (lquery) {
	return 0;
    }

/*     Quick return if possible */

    if (*n == 0) {
	return 0;
    }

    if (*n == 1) {
	i__1 = ab_dim1 + 1;
	w[1] = ab[i__1].r;
	if (wantz) {
	    i__1 = z_dim1 + 1;
	    z__[i__1].r = 1.f, z__[i__1].i = 0.f;
	}
	return 0;
    }

/*     Get machine constants. */

    safmin = slamch_("Safe minimum");
    eps = slamch_("Precision");
    smlnum = safmin / eps;
    bignum = 1.f / smlnum;
    rmin = sqrt(smlnum);
    rmax = sqrt(bignum);

/*     Scale matrix to allowable range, if necessary. */

    anrm = clanhb_("M", uplo, n, kd, &ab[ab_offset], ldab, &rwork[1]);
    iscale = 0;
    if (anrm > 0.f && anrm < rmin) {
	iscale = 1;
	sigma = rmin / anrm;
    } else if (anrm > rmax) {
	iscale = 1;
	sigma = rmax / anrm;
    }
    if (iscale == 1) {
	if (lower) {
	    clascl_("B", kd, kd, &c_b13, &sigma, n, n, &ab[ab_offset], ldab, 
		    info);
	} else {
	    clascl_("Q", kd, kd, &c_b13, &sigma, n, n, &ab[ab_offset], ldab, 
		    info);
	}
    }

/*     Call CHBTRD to reduce Hermitian band matrix to tridiagonal form. */

    inde = 1;
    indwrk = inde + *n;
    indwk2 = *n * *n + 1;
    llwk2 = *lwork - indwk2 + 1;
    llrwk = *lrwork - indwrk + 1;
    chbtrd_(jobz, uplo, n, kd, &ab[ab_offset], ldab, &w[1], &rwork[inde], &
	    z__[z_offset], ldz, &work[1], &iinfo);

/*     For eigenvalues only, call SSTERF.  For eigenvectors, call CSTEDC. */

    if (! wantz) {
	ssterf_(n, &w[1], &rwork[inde], info);
    } else {
	cstedc_("I", n, &w[1], &rwork[inde], &work[1], n, &work[indwk2], &
		llwk2, &rwork[indwrk], &llrwk, &iwork[1], liwork, info);
	cgemm_("N", "N", n, n, n, &c_b2, &z__[z_offset], ldz, &work[1], n, &
		c_b1, &work[indwk2], n);
	clacpy_("A", n, n, &work[indwk2], n, &z__[z_offset], ldz);
    }

/*     If matrix was scaled, then rescale eigenvalues appropriately. */

    if (iscale == 1) {
	if (*info == 0) {
	    imax = *n;
	} else {
	    imax = *info - 1;
	}
	r__1 = 1.f / sigma;
	sscal_(&imax, &r__1, &w[1], &c__1);
    }

    work[1].r = (real) lwmin, work[1].i = 0.f;
    rwork[1] = (real) lrwmin;
    iwork[1] = liwmin;
    return 0;

/*     End of CHBEVD */

} /* chbevd_ */

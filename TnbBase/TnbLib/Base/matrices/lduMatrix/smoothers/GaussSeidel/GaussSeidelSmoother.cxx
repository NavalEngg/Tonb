#include <GaussSeidelSmoother.hxx>

// * * * * * * * * * * * * * * Static Data Members * * * * * * * * * * * * * //

namespace tnbLib
{
	defineTypeNameAndDebug(GaussSeidelSmoother, 0);

	lduSmoother::addsymMatrixConstructorToTable<GaussSeidelSmoother>
		addGaussSeidelSmootherSymMatrixConstructorToTable_;

	lduSmoother::addasymMatrixConstructorToTable<GaussSeidelSmoother>
		addGaussSeidelSmootherAsymMatrixConstructorToTable_;
}


// * * * * * * * * * * * * * * * * Constructors  * * * * * * * * * * * * * * //

tnbLib::GaussSeidelSmoother::GaussSeidelSmoother
(
	const lduMatrix& matrix,
	const FieldField<Field, scalar>& coupleBouCoeffs,
	const FieldField<Field, scalar>& coupleIntCoeffs,
	const lduInterfaceFieldPtrsList& interfaces
)
	:
	lduSmoother
	(
		matrix,
		coupleBouCoeffs,
		coupleIntCoeffs,
		interfaces
	)
{}


// * * * * * * * * * * * * * * * Member Functions  * * * * * * * * * * * * * //

void tnbLib::GaussSeidelSmoother::smooth
(
	scalarField& x,
	const lduMatrix& matrix_,
	const scalarField& b,
	const FieldField<Field, scalar>& coupleBouCoeffs_,
	const lduInterfaceFieldPtrsList& interfaces_,
	const direction cmpt,
	const label nSweeps
)
{
	register scalar* __restrict xPtr = x.begin();

	register const label nCells = x.size();

	scalarField bPrime(nCells);
	register scalar* __restrict bPrimePtr = bPrime.begin();

	register const scalar* const __restrict diagPtr = matrix_.diag().begin();
	register const scalar* const __restrict upperPtr =
		matrix_.upper().begin();
	register const scalar* const __restrict lowerPtr =
		matrix_.lower().begin();

	register const label* const __restrict uPtr =
		matrix_.lduAddr().upperAddr().begin();

	register const label* const __restrict ownStartPtr =
		matrix_.lduAddr().ownerStartAddr().begin();


	// Coupled boundary initialisation.  The coupled boundary is treated
	// as an effective jacobi interface in the boundary.
	// Note: there is a change of sign in the coupled
	// interface update.  The reason for this is that the
	// internal coefficients are all located at the l.h.s. of
	// the matrix whereas the "implicit" coefficients on the
	// coupled boundaries are all created as if the
	// coefficient contribution is of a b-kind (i.e. they
	// have a sign as if they are on the r.h.s. of the matrix.
	// To compensate for this, it is necessary to turn the
	// sign of the contribution.

	// Handled by LHS switch on initMatrixInterfaces and updateMatrixInterfaces
	// HJ, 22/May/2013

	for (label sweep = 0; sweep < nSweeps; sweep++)
	{
		bPrime = b;

		// Update from lhs
		matrix_.initMatrixInterfaces
		(
			coupleBouCoeffs_,
			interfaces_,
			x,
			bPrime,
			cmpt,
			true         // switch to lhs
		);

		// Update from lhs
		matrix_.updateMatrixInterfaces
		(
			coupleBouCoeffs_,
			interfaces_,
			x,
			bPrime,
			cmpt,
			true         // switch to lhs
		);

		register scalar curX;
		register label fStart;
		register label fEnd = ownStartPtr[0];

		for (register label cellI = 0; cellI < nCells; cellI++)
		{
			// Start and end of this row
			fStart = fEnd;
			fEnd = ownStartPtr[cellI + 1];

			// Get the accumulated neighbour side
			curX = bPrimePtr[cellI];

			// Accumulate the owner product side
			for (register label curFace = fStart; curFace < fEnd; curFace++)
			{
				curX -= upperPtr[curFace] * xPtr[uPtr[curFace]];
			}

			// Finish current x
			curX /= diagPtr[cellI];

			// Distribute the neighbour side using current x
			for (register label curFace = fStart; curFace < fEnd; curFace++)
			{
				bPrimePtr[uPtr[curFace]] -= lowerPtr[curFace] * curX;
			}

			xPtr[cellI] = curX;
		}
	}
}


void tnbLib::GaussSeidelSmoother::smooth
(
	scalarField& x,
	const scalarField& b,
	const direction cmpt,
	const label nSweeps
) const
{
	smooth
	(
		x,
		matrix_,
		b,
		coupleBouCoeffs_,
		interfaces_,
		cmpt,
		nSweeps
	);
}


// ************************************************************************* //
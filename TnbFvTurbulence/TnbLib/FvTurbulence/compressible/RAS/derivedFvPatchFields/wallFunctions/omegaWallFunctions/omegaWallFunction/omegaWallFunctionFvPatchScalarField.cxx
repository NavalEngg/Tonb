#include <omegaWallFunctionFvPatchScalarField.hxx>

#include <compRASModel.hxx>
#include <fvPatchFieldMapper.hxx>
#include <volFields.hxx>
#include <addToRunTimeSelectionTable.hxx>
#include <zeroGradientFvPatchFields.hxx>

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

namespace tnbLib
{
	namespace compressible
	{
		namespace RASModels
		{

			// * * * * * * * * * * * * * Private Member Functions  * * * * * * * * * * * //

			void omegaWallFunctionFvPatchScalarField::checkType()
			{
				if (!patch().isWall())
				{
					FatalErrorIn("omegaWallFunctionFvPatchScalarField::checkType()")
						<< "Invalid wall function specification" << nl
						<< "    Patch type for patch " << patch().name()
						<< " must be wall" << nl
						<< "    Current patch type is " << patch().type() << nl << endl
						<< abort(FatalError);
				}
			}


			// * * * * * * * * * * * * * * * * Constructors  * * * * * * * * * * * * * * //

			omegaWallFunctionFvPatchScalarField::omegaWallFunctionFvPatchScalarField
			(
				const fvPatch& p,
				const DimensionedField<scalar, volMesh>& iF
			)
				:
				fixedInternalValueFvPatchField<scalar>(p, iF),
				UName_("U"),
				rhoName_("rho"),
				kName_("k"),
				GName_("RASModel::G"),
				muName_("mu"),
				mutName_("mut"),
				Cmu_(0.09),
				kappa_(0.41),
				E_(9.8),
				beta1_(0.075)
			{
				checkType();
			}


			omegaWallFunctionFvPatchScalarField::omegaWallFunctionFvPatchScalarField
			(
				const omegaWallFunctionFvPatchScalarField& ptf,
				const fvPatch& p,
				const DimensionedField<scalar, volMesh>& iF,
				const fvPatchFieldMapper& mapper
			)
				:
				fixedInternalValueFvPatchField<scalar>(ptf, p, iF, mapper),
				UName_(ptf.UName_),
				rhoName_(ptf.rhoName_),
				kName_(ptf.kName_),
				GName_(ptf.GName_),
				muName_(ptf.muName_),
				mutName_(ptf.mutName_),
				Cmu_(ptf.Cmu_),
				kappa_(ptf.kappa_),
				E_(ptf.E_),
				beta1_(ptf.beta1_)
			{
				checkType();
			}


			omegaWallFunctionFvPatchScalarField::omegaWallFunctionFvPatchScalarField
			(
				const fvPatch& p,
				const DimensionedField<scalar, volMesh>& iF,
				const dictionary& dict
			)
				:
				fixedInternalValueFvPatchField<scalar>(p, iF, dict),
				UName_(dict.lookupOrDefault<word>("U", "U")),
				rhoName_(dict.lookupOrDefault<word>("rho", "rho")),
				kName_(dict.lookupOrDefault<word>("k", "k")),
				GName_(dict.lookupOrDefault<word>("G", "RASModel::G")),
				muName_(dict.lookupOrDefault<word>("mu", "mu")),
				mutName_(dict.lookupOrDefault<word>("mut", "mut")),
				Cmu_(dict.lookupOrDefault<scalar>("Cmu", 0.09)),
				kappa_(dict.lookupOrDefault<scalar>("kappa", 0.41)),
				E_(dict.lookupOrDefault<scalar>("E", 9.8)),
				beta1_(dict.lookupOrDefault<scalar>("beta1", 0.075))
			{
				checkType();
			}


			omegaWallFunctionFvPatchScalarField::omegaWallFunctionFvPatchScalarField
			(
				const omegaWallFunctionFvPatchScalarField& owfpsf
			)
				:
				fixedInternalValueFvPatchField<scalar>(owfpsf),
				UName_(owfpsf.UName_),
				rhoName_(owfpsf.rhoName_),
				kName_(owfpsf.kName_),
				GName_(owfpsf.GName_),
				muName_(owfpsf.muName_),
				mutName_(owfpsf.mutName_),
				Cmu_(owfpsf.Cmu_),
				kappa_(owfpsf.kappa_),
				E_(owfpsf.E_),
				beta1_(owfpsf.beta1_)
			{
				checkType();
			}


			omegaWallFunctionFvPatchScalarField::omegaWallFunctionFvPatchScalarField
			(
				const omegaWallFunctionFvPatchScalarField& owfpsf,
				const DimensionedField<scalar, volMesh>& iF
			)
				:
				fixedInternalValueFvPatchField<scalar>(owfpsf, iF),
				UName_(owfpsf.UName_),
				rhoName_(owfpsf.rhoName_),
				kName_(owfpsf.kName_),
				GName_(owfpsf.GName_),
				muName_(owfpsf.muName_),
				mutName_(owfpsf.mutName_),
				Cmu_(owfpsf.Cmu_),
				kappa_(owfpsf.kappa_),
				E_(owfpsf.E_),
				beta1_(owfpsf.beta1_)
			{
				checkType();
			}


			// * * * * * * * * * * * * * * * Member Functions  * * * * * * * * * * * * * //

			void omegaWallFunctionFvPatchScalarField::updateCoeffs()
			{
				if (updated())
				{
					return;
				}

				// If G field is not present, execute zero gradient evaluation
				// HJ, 20/Mar/2011
				if (!db().foundObject<volScalarField>(GName_))
				{
					InfoIn("void omegaWallFunctionFvPatchScalarField::updateCoeffs()")
						<< "Cannot access " << GName_ << " field for patch "
						<< patch().name() << ".  Evaluating as zeroGradient"
						<< endl;

					fvPatchScalarField::updateCoeffs();
					zeroGradientFvPatchScalarField::evaluate();

					return;
				}

				const RASModel& rasModel = db().lookupObject<RASModel>("RASProperties");
				const scalar yPlusLam = rasModel.yPlusLam(kappa_, E_);
				const scalarField& y = rasModel.y()[patch().index()];

				const scalar Cmu25 = pow(Cmu_, 0.25);

				volScalarField& G = const_cast<volScalarField&>
					(db().lookupObject<volScalarField>(GName_));

				// Note: omega is now a refValue and set in fixedInternalValueFvPatchField
				// HJ, 3/Aug/2011
				scalarField& omega = refValue();

				const scalarField& k = db().lookupObject<volScalarField>(kName_);

				const scalarField& rhow =
					lookupPatchField<volScalarField, scalar>(rhoName_);

				const scalarField& muw =
					lookupPatchField<volScalarField, scalar>(muName_);

				const scalarField& mutw =
					lookupPatchField<volScalarField, scalar>(mutName_);

				const fvPatchVectorField& Uw =
					lookupPatchField<volVectorField, vector>(UName_);

				const scalarField magGradUw = mag(Uw.snGrad());

				const labelList& faceCells = patch().faceCells();

				// Set omega and G
				forAll(mutw, faceI)
				{
					label faceCellI = faceCells[faceI];

					scalar yPlus =
						Cmu25 * y[faceI] * sqrt(k[faceCellI])
						/ (muw[faceI] / rhow[faceI]);

					scalar omegaVis = 6.0*(muw[faceI] / rhow[faceI]) / (beta1_*sqr(y[faceI]));

					scalar omegaLog = sqrt(k[faceCellI]) / (Cmu25*kappa_*y[faceI]);

					// Note: omega is now a refValue and set in
					// fixedInternalValueFvPatchField
					// HJ, 3/Aug/2011
					omega[faceI] = sqrt(sqr(omegaVis) + sqr(omegaLog));

					if (yPlus > yPlusLam)
					{
						G[faceCellI] =
							(mutw[faceI] + muw[faceI])
							*magGradUw[faceI]
							* Cmu25*sqrt(k[faceCellI])
							/ (kappa_*y[faceI]);
					}
					else
					{
						G[faceCellI] = 0.0;
					}
				}

				// TODO: perform averaging for cells sharing more than one boundary face

				fixedInternalValueFvPatchField<scalar>::updateCoeffs();
			}


			void omegaWallFunctionFvPatchScalarField::write(Ostream& os) const
			{
				fixedInternalValueFvPatchField<scalar>::write(os);
				writeEntryIfDifferent<word>(os, "U", "U", UName_);
				writeEntryIfDifferent<word>(os, "rho", "rho", rhoName_);
				writeEntryIfDifferent<word>(os, "k", "k", kName_);
				writeEntryIfDifferent<word>(os, "G", "RASModel::G", GName_);
				writeEntryIfDifferent<word>(os, "mu", "mu", muName_);
				writeEntryIfDifferent<word>(os, "mut", "mut", mutName_);
				os.writeKeyword("Cmu") << Cmu_ << token::END_STATEMENT << nl;
				os.writeKeyword("kappa") << kappa_ << token::END_STATEMENT << nl;
				os.writeKeyword("E") << E_ << token::END_STATEMENT << nl;
				os.writeKeyword("beta1") << beta1_ << token::END_STATEMENT << nl;
			}


			// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

			makePatchTypeField
			(
				fvPatchScalarField,
				omegaWallFunctionFvPatchScalarField
			);

			// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

		} // End namespace RASModels
	} // End namespace compressible
} // End namespace tnbLib

// ************************************************************************* //
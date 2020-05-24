#include <compRASModel.hxx>

#include <addToRunTimeSelectionTable.hxx>
#include <tnbTime.hxx>

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

namespace tnbLib
{
	namespace compressible
	{

		// * * * * * * * * * * * * * * Static Data Members * * * * * * * * * * * * * //

		defineTypeNameAndDebug(RASModel, 0);
		defineRunTimeSelectionTable(RASModel, dictionary);
		addToRunTimeSelectionTable(turbulenceModel, RASModel, turbulenceModel);

		// * * * * * * * * * * * * * Protected Member Functions  * * * * * * * * * * //

		void RASModel::printCoeffs()
		{
			if (printCoeffs_)
			{
				Info << type() << "Coeffs" << coeffDict_ << endl;
			}
		}


		// * * * * * * * * * * * * * * * * Constructors  * * * * * * * * * * * * * * //

		RASModel::RASModel
		(
			const word& type,
			const volScalarField& rho,
			const volVectorField& U,
			const surfaceScalarField& phi,
			const basicThermo& thermophysicalModel
		)
			:
			turbulenceModel(rho, U, phi, thermophysicalModel),

			IOdictionary
			(
				IOobject
				(
					"RASProperties",
					U.time().constant(),
					U.db(),
					IOobject::MUST_READ,
					IOobject::NO_WRITE
				)
			),

			turbulence_(lookup("turbulence")),
			printCoeffs_(lookupOrDefault<Switch>("printCoeffs", false)),
			coeffDict_(subOrEmptyDict(type + "Coeffs")),

			k0_("k0", dimVelocity*dimVelocity, SMALL),
			epsilon0_("epsilon0", k0_.dimensions() / dimTime, SMALL),
			epsilonSmall_("epsilonSmall", epsilon0_.dimensions(), SMALL),
			omega0_("omega0", dimless / dimTime, SMALL),
			omegaSmall_("omegaSmall", omega0_.dimensions(), SMALL),
			muRatio_(lookupOrDefault<scalar>("muRatio", 1e6)),

			y_(mesh_)
		{
			// Force the construction of the mesh deltaCoeffs which may be needed
			// for the construction of the derived models and BCs
			mesh_.deltaCoeffs();
		}


		// * * * * * * * * * * * * * * * * Selectors * * * * * * * * * * * * * * * * //

		autoPtr<RASModel> RASModel::New
		(
			const volScalarField& rho,
			const volVectorField& U,
			const surfaceScalarField& phi,
			const basicThermo& thermophysicalModel
		)
		{
			word modelName;

			// Enclose the creation of the dictionary to ensure it is deleted
			// before the turbulenceModel is created otherwise the dictionary is
			// entered in the database twice
			{
				IOdictionary dict
				(
					IOobject
					(
						"RASProperties",
						U.time().constant(),
						U.db(),
						IOobject::MUST_READ,
						IOobject::NO_WRITE
					)
				);

				dict.lookup("RASModel") >> modelName;
			}

			Info << "Selecting RAS turbulence model " << modelName << endl;

			dictionaryConstructorTable::iterator cstrIter =
				dictionaryConstructorTablePtr_->find(modelName);

			if (cstrIter == dictionaryConstructorTablePtr_->end())
			{
				FatalErrorIn
				(
					"RASModel::New(const volScalarField&, "
					"const volVectorField&, const surfaceScalarField&, "
					"basicThermo&)"
				) << "Unknown RASModel type " << modelName
					<< endl << endl
					<< "Valid RASModel types are :" << endl
					<< dictionaryConstructorTablePtr_->sortedToc()
					<< exit(FatalError);
			}

			return autoPtr<RASModel>
				(
					cstrIter()(rho, U, phi, thermophysicalModel)
					);
		}


		// * * * * * * * * * * * * * * * Member Functions  * * * * * * * * * * * * * //

		scalar RASModel::yPlusLam(const scalar kappa, const scalar E) const
		{
			scalar ypl = 11.0;

			for (int i = 0; i < 10; i++)
			{
				ypl = log(E*ypl) / kappa;
			}

			return ypl;
		}


		tmp<volScalarField> RASModel::muEff() const
		{
			tmp<volScalarField> tmuEff
			(
				new volScalarField("muEff", mut() + mu())
			);

			// Apply mut limiter
			tmuEff().internalField() =
				tnbLib::min(tmuEff().internalField(), muRatio_*mu().internalField());

			return tmuEff;
		}


		tmp<scalarField> RASModel::yPlus(const label patchNo, const scalar Cmu) const
		{
			const fvPatch& curPatch = mesh_.boundary()[patchNo];

			tmp<scalarField> tYp(new scalarField(curPatch.size()));
			scalarField& Yp = tYp();

			if (curPatch.isWall())
			{
				Yp = pow(Cmu, 0.25)
					*y_[patchNo]
					* sqrt(k()().boundaryField()[patchNo].patchInternalField())
					/ (
						mu().boundaryField()[patchNo].patchInternalField()
						/ rho_.boundaryField()[patchNo]
						);
			}
			else
			{
				WarningIn
				(
					"tmp<scalarField> RASModel::yPlus(const label patchNo) const"
				) << "Patch " << patchNo << " is not a wall. Returning null field"
					<< nl << endl;

				Yp.setSize(0);
			}

			return tYp;
		}


		void RASModel::correct()
		{
			if (mesh_.changing())
			{
				y_.correct();
			}
		}


		bool RASModel::read()
		{
			if (regIOobject::read())
			{
				lookup("turbulence") >> turbulence_;

				if (const dictionary* dictPtr = subDictPtr(type() + "Coeffs"))
				{
					coeffDict_ <<= *dictPtr;
				}

				k0_.readIfPresent(*this);
				epsilon0_.readIfPresent(*this);
				epsilonSmall_.readIfPresent(*this);
				omega0_.readIfPresent(*this);
				omegaSmall_.readIfPresent(*this);
				readIfPresent("muRatio", muRatio_);

				return true;
			}
			else
			{
				return false;
			}
		}


		// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

	} // End namespace compressible
} // End namespace tnbLib

// ************************************************************************* //
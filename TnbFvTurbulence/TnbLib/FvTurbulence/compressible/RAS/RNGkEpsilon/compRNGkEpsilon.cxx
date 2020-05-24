#include <compRNGkEpsilon.hxx>

#include <addToRunTimeSelectionTable.hxx>
#include <backwardsCompatibilityWallFunctions.hxx>
#include <tnbTime.hxx>

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

namespace tnbLib
{
	namespace compressible
	{
		namespace RASModels
		{

			// * * * * * * * * * * * * * * Static Data Members * * * * * * * * * * * * * //

			defineTypeNameAndDebug(RNGkEpsilon, 0);
			addToRunTimeSelectionTable(RASModel, RNGkEpsilon, dictionary);

			// * * * * * * * * * * * * * * * * Constructors  * * * * * * * * * * * * * * //

			RNGkEpsilon::RNGkEpsilon
			(
				const volScalarField& rho,
				const volVectorField& U,
				const surfaceScalarField& phi,
				const basicThermo& thermophysicalModel
			)
				:
				RASModel(typeName, rho, U, phi, thermophysicalModel),

				Cmu_
				(
					dimensionedScalar::lookupOrAddToDict
					(
						"Cmu",
						coeffDict_,
						0.0845
					)
				),
				C1_
				(
					dimensionedScalar::lookupOrAddToDict
					(
						"C1",
						coeffDict_,
						1.42
					)
				),
				C2_
				(
					dimensionedScalar::lookupOrAddToDict
					(
						"C2",
						coeffDict_,
						1.68
					)
				),
				C3_
				(
					dimensionedScalar::lookupOrAddToDict
					(
						"C3",
						coeffDict_,
						-0.33
					)
				),
				sigmak_
				(
					dimensionedScalar::lookupOrAddToDict
					(
						"sigmak",
						coeffDict_,
						0.71942
					)
				),
				sigmaEps_
				(
					dimensionedScalar::lookupOrAddToDict
					(
						"sigmaEps",
						coeffDict_,
						0.71942
					)
				),
				Prt_
				(
					dimensionedScalar::lookupOrAddToDict
					(
						"Prt",
						coeffDict_,
						1.0
					)
				),
				eta0_
				(
					dimensionedScalar::lookupOrAddToDict
					(
						"eta0",
						coeffDict_,
						4.38
					)
				),
				beta_
				(
					dimensionedScalar::lookupOrAddToDict
					(
						"beta",
						coeffDict_,
						0.012
					)
				),

				k_
				(
					IOobject
					(
						"k",
						runTime_.timeName(),
						U_.db(),
						IOobject::NO_READ,
						IOobject::AUTO_WRITE
					),
					autoCreateK("k", mesh_)
				),
				epsilon_
				(
					IOobject
					(
						"epsilon",
						runTime_.timeName(),
						U_.db(),
						IOobject::NO_READ,
						IOobject::AUTO_WRITE
					),
					autoCreateEpsilon("epsilon", mesh_)
				),
				mut_
				(
					IOobject
					(
						"mut",
						runTime_.timeName(),
						U_.db(),
						IOobject::NO_READ,
						IOobject::AUTO_WRITE
					),
					autoCreateMut("mut", mesh_)
				),
				alphat_
				(
					IOobject
					(
						"alphat",
						runTime_.timeName(),
						U_.db(),
						IOobject::NO_READ,
						IOobject::AUTO_WRITE
					),
					autoCreateAlphat("alphat", mesh_)
				)
			{
				mut_ = Cmu_ * rho_*sqr(k_) / (epsilon_ + epsilonSmall_);
				mut_ = min(mut_, muRatio()*mu());
				mut_.correctBoundaryConditions();

				alphat_ = mut_ / Prt_;
				alphat_.correctBoundaryConditions();

				printCoeffs();
			}


			// * * * * * * * * * * * * * * * Member Functions  * * * * * * * * * * * * * //

			tmp<volSymmTensorField> RNGkEpsilon::R() const
			{
				return tmp<volSymmTensorField>
					(
						new volSymmTensorField
						(
							IOobject
							(
								"R",
								runTime_.timeName(),
								U_.db(),
								IOobject::NO_READ,
								IOobject::NO_WRITE
							),
							((2.0 / 3.0)*I)*k_ - (mut_ / rho_)*dev(twoSymm(fvc::grad(U_))),
							k_.boundaryField().types()
						)
						);
			}


			tmp<volSymmTensorField> RNGkEpsilon::devRhoReff() const
			{
				return tmp<volSymmTensorField>
					(
						new volSymmTensorField
						(
							IOobject
							(
								"devRhoReff",
								runTime_.timeName(),
								U_.db(),
								IOobject::NO_READ,
								IOobject::NO_WRITE
							),
							-muEff()*dev(twoSymm(fvc::grad(U_)))
						)
						);
			}


			tmp<fvVectorMatrix> RNGkEpsilon::divDevRhoReff() const
			{
				return
					(
						-fvm::laplacian(muEff(), U_)
						- fvc::div(muEff()*dev2(T(fvc::grad(U_))))
						);
			}


			bool RNGkEpsilon::read()
			{
				if (RASModel::read())
				{
					Cmu_.readIfPresent(coeffDict());
					C1_.readIfPresent(coeffDict());
					C2_.readIfPresent(coeffDict());
					C3_.readIfPresent(coeffDict());
					Prt_.readIfPresent(coeffDict());
					sigmaEps_.readIfPresent(coeffDict());
					Prt_.readIfPresent(coeffDict());
					eta0_.readIfPresent(coeffDict());
					beta_.readIfPresent(coeffDict());

					return true;
				}
				else
				{
					return false;
				}
			}


			void RNGkEpsilon::correct()
			{
				// Bound in case of topological change
				// HJ, 22/Aug/2007
				if (mesh_.changing())
				{
					bound(k_, k0_);
					bound(epsilon_, epsilon0_);
				}

				if (!turbulence_)
				{
					// Re-calculate viscosity
					mut_ = rho_ * Cmu_*sqr(k_) / (epsilon_ + epsilonSmall_);
					mut_ = min(mut_, muRatio()*mu());
					mut_.correctBoundaryConditions();

					// Re-calculate thermal diffusivity
					alphat_ = mut_ / Prt_;
					alphat_.correctBoundaryConditions();

					return;
				}

				RASModel::correct();

				volScalarField divU = fvc::div(phi_ / fvc::interpolate(rho_));

				if (mesh_.moving())
				{
					divU += fvc::div(mesh_.phi());
				}

				// Change return type due to gradient cacheing.  HJ, 22/Apr/2016
				const tmp<volTensorField> tgradU = fvc::grad(U_);
				const volTensorField& gradU = tgradU();

				volScalarField S2 = (gradU && dev(twoSymm(gradU)));

				volScalarField G("RASModel::G", mut_*S2);

				volScalarField eta = sqrt(mag(S2))*k_ / epsilon_;
				volScalarField eta3 = eta * sqr(eta);

				volScalarField R =
					((eta*(-eta / eta0_ + scalar(1))) / (beta_*eta3 + scalar(1)));

				// Update epsilon and G at the wall
				epsilon_.boundaryField().updateCoeffs();

				// Dissipation equation
				tmp<fvScalarMatrix> epsEqn
				(
					fvm::ddt(rho_, epsilon_)
					+ fvm::div(phi_, epsilon_)
					- fvm::laplacian(DepsilonEff(), epsilon_)
					==
					(C1_ - R)*G*epsilon_ / k_
					- fvm::SuSp(((2.0 / 3.0)*C1_ + C3_)*rho_*divU, epsilon_)
					- fvm::Sp(C2_*rho_*epsilon_ / k_, epsilon_)
				);

				epsEqn().relax();

				// No longer needed: matrix completes at the point of solution
				// HJ, 17/Apr/2012
			//     epsEqn().completeAssembly();

				solve(epsEqn);
				bound(epsilon_, epsilon0_);


				// Turbulent kinetic energy equation

				tmp<fvScalarMatrix> kEqn
				(
					fvm::ddt(rho_, k_)
					+ fvm::div(phi_, k_)
					- fvm::laplacian(DkEff(), k_)
					==
					G - fvm::SuSp(2.0 / 3.0*rho_*divU, k_)
					- fvm::Sp(rho_*(epsilon_) / k_, k_)
				);

				kEqn().relax();
				solve(kEqn);
				bound(k_, k0_);


				// Re-calculate viscosity
				mut_ = rho_ * Cmu_*sqr(k_) / epsilon_;
				mut_ = min(mut_, muRatio()*mu());
				mut_.correctBoundaryConditions();

				// Re-calculate thermal diffusivity
				alphat_ = mut_ / Prt_;
				alphat_.correctBoundaryConditions();
			}


			// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

		} // End namespace RASModels
	} // End namespace compressible
} // End namespace tnbLib

// ************************************************************************* //
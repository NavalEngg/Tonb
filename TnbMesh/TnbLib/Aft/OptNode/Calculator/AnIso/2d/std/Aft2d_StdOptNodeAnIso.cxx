#include <Aft2d_StdOptNodeAnIso.hxx>

#include <Aft2d_OptNodeAnIso_Analytical.hxx>
#include <Aft2d_EdgeAnIso.hxx>
#include <Aft2d_MetricPrcsrAnIso.hxx>
#include <TnbError.hxx>
#include <OSstream.hxx>

void tnbLib::Aft2d_StdOptNodeAnIso::Perform()
{
#ifdef _DEBUG
	if (NOT Front())
	{
		FatalErrorIn(FunctionSIG)
			<< "no front entity has been found!" << endl
			<< abort(FatalError);
	}

	if (NOT MetricMap())
	{
		FatalErrorIn(FunctionSIG)
			<< "no metric map has been found!" << endl
			<< abort(FatalError);
	}

	if (NOT MetricMap()->MetricFunction())
	{
		FatalErrorIn(FunctionSIG)
			<< "no metric function has been found!" << endl
			<< abort(FatalError);
	}
#endif // _DEBUG

	if (Size() <= 0)
	{
		FatalErrorIn(FunctionSIG)
			<< "invalid element size has been detected" << endl
			<< abort(FatalError);
	}

	auto m = MetricMap()->CalcMetric(Front()->Centre());
	Aft2d_OptNodeAnIso_Analytical alg(Size(), m, *Front());
	alg.Perform();
	Debug_If_Condition_Message(NOT alg.IsDone(), "the algorithm is not performed!");

	ChangeCoord() = alg.Coord();
	Change_IsDone() = Standard_True;
}
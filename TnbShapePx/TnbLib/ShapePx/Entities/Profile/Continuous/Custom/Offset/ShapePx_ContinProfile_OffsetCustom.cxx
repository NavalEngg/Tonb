#include <ShapePx_ContinProfile_OffsetCustom.hxx>

#include <SectPx_ExtrProfile.hxx>
#include <TnbError.hxx>
#include <OSstream.hxx>

tnbLib::shapePxLib::ContinProfile_OffsetCustom::ContinProfile_OffsetCustom
(
	const Standard_Integer theIndex, 
	const word & theName
)
	: ContinProfile_Custom(theIndex, theName)
{
	// empty body
}

tnbLib::shapePxLib::ContinProfile_OffsetCustom::ContinProfile_OffsetCustom
(
	const std::shared_ptr<SectPx_ExtrProfile>& theProfile
)
	: theQ_(theProfile)
{
	// empty body
}

tnbLib::shapePxLib::ContinProfile_OffsetCustom::ContinProfile_OffsetCustom
(
	const Standard_Integer theIndex,
	const word & theName,
	const std::shared_ptr<SectPx_ExtrProfile>& theProfile
)
	: ContinProfile_Custom(theIndex, theName)
	, theQ_(theProfile)
{
	// empty body
}

namespace tnbLib
{

	namespace shapePxLib
	{

		static std::vector<Pnt2d> GetOffsets(const SectPx_ExtrProfile& profile)
		{
			std::vector<Pnt2d> points;
			switch (profile.ExtrProfileType())
			{
			case SectPx_ExtrProfileType::uniform:
			{
				points.push_back(profile.Point(0));
				return std::move(points);
			}
			case SectPx_ExtrProfileType::linear:
			{
				points.push_back(profile.Point(0));
				points.push_back(profile.Point(1));
				return std::move(points);
			}
			case SectPx_ExtrProfileType::threePoint:
			{
				auto p0 = profile.Point(0);
				auto p1 = profile.Point(1);
				auto p2 = profile.Point(2);
				Pnt2d p3(p2.X() + (p2.X() - p1.X()), p2.Y() + (p2.Y() - p1.Y()));
				Pnt2d p4(p3.X() + (p1.X() - p0.X()), p3.Y() + (p1.Y() - p0.Y()));
				points.push_back(std::move(p0));
				points.push_back(std::move(p1));
				points.push_back(std::move(p2));
				points.push_back(std::move(p3));
				points.push_back(std::move(p4));
				return std::move(points);
			}
			case SectPx_ExtrProfileType::fivePoint:
			{
				points.push_back(profile.Point(0));
				points.push_back(profile.Point(1));
				points.push_back(profile.Point(2));
				points.push_back(profile.Point(3));
				points.push_back(profile.Point(4));
				return std::move(points);
			}
			case SectPx_ExtrProfileType::nPoint:
			{
				for (Standard_Integer i = 0; i < profile.NbPoints(); i++)
				{
					points.push_back(profile.Point(i));
				}
				return std::move(points);
			}
			default:
			{
				FatalErrorIn(FunctionSIG)
					<< "unspecified type of the profile" << endl
					<< abort(FatalError);
			}
				break;
			}
		}
	}
}

void tnbLib::shapePxLib::ContinProfile_OffsetCustom::Perform()
{
	Debug_Null_Pointer(ExtrudeProfile());
	if (ExtrudeProfile()->ExtrProfileType() IS_EQUAL SectPx_ExtrProfileType::uniform)
	{
		return;
	}

	if (ExtrudeProfile()->ExtrProfileType() IS_EQUAL SectPx_ExtrProfileType::linear)
	{
		return;
	}

	auto offsets = GetOffsets(*ExtrudeProfile());
	
	double* xs = new double[offsets.size()];
	double* ys = new double[offsets.size()];
	for (size_t i = 0; i < offsets.size(); i++)
	{
		const auto& p = offsets[i];
		xs[i] = p.X();
		ys[i] = p.Y();
	}

	const auto nbPoints = offsets.size();
	if (NOT theAcc_)
	{
		theAcc_ = gsl_interp_accel_alloc();
	}
	if (NOT theSpline_)
	{
		switch (theType_)
		{
		case tnbLib::shapePxLib::ContinProfile_OffsetCustom::interplType::cspline:
			theSpline_ = gsl_spline_alloc(gsl_interp_cspline, nbPoints);
			break;
		case tnbLib::shapePxLib::ContinProfile_OffsetCustom::interplType::cspline_periodic:
			theSpline_ = gsl_spline_alloc(gsl_interp_cspline_periodic, nbPoints);
			break;
		case tnbLib::shapePxLib::ContinProfile_OffsetCustom::interplType::linear:
			theSpline_ = gsl_spline_alloc(gsl_interp_linear, nbPoints);
			break;
		case tnbLib::shapePxLib::ContinProfile_OffsetCustom::interplType::polynomial:
			theSpline_ = gsl_spline_alloc(gsl_interp_polynomial, nbPoints);
			break;
		case tnbLib::shapePxLib::ContinProfile_OffsetCustom::interplType::akima:
			theSpline_ = gsl_spline_alloc(gsl_interp_akima, nbPoints);
			break;
		case tnbLib::shapePxLib::ContinProfile_OffsetCustom::interplType::akima_perodic:
			theSpline_ = gsl_spline_alloc(gsl_interp_akima_periodic, nbPoints);
			break;
		case tnbLib::shapePxLib::ContinProfile_OffsetCustom::interplType::steffen:
			theSpline_ = gsl_spline_alloc(gsl_interp_steffen, nbPoints);
			break;
		default:
			break;
		}
	}

	gsl_spline_init(theSpline_, xs, ys, 5);

	delete[] xs;
	delete[] ys;

	Change_IsDone() = Standard_True;
}

Standard_Boolean 
tnbLib::shapePxLib::ContinProfile_OffsetCustom::IsOffset() const
{
	return Standard_True;
}

Standard_Real 
tnbLib::shapePxLib::ContinProfile_OffsetCustom::Lower() const
{
	Debug_Null_Pointer(ExtrudeProfile());
	return ExtrudeProfile()->Lower();
}

Standard_Real 
tnbLib::shapePxLib::ContinProfile_OffsetCustom::Upper() const
{
	Debug_Null_Pointer(ExtrudeProfile());
	return ExtrudeProfile()->Upper();
}

Standard_Real 
tnbLib::shapePxLib::ContinProfile_OffsetCustom::Value
(
	const Standard_Real x
) const
{
	Debug_Null_Pointer(ExtrudeProfile());
	if (NOT IsDone())
	{
		FatalErrorIn(FunctionSIG)
			<< "the interpolation is not performed!" << endl
			<< abort(FatalError);
	}

	if (ExtrudeProfile()->ExtrProfileType() IS_EQUAL SectPx_ExtrProfileType::uniform)
	{
		return ExtrudeProfile()->Point(0).Y();
	}
	else if (ExtrudeProfile()->ExtrProfileType() IS_EQUAL SectPx_ExtrProfileType::linear)
	{
		if (NOT INSIDE(x, Lower(), Upper()))
		{
			FatalErrorIn(FunctionSIG)
				<< " the parameter exceeds the profile boundary" << endl
				<< " - lower: " << Lower() << endl
				<< " - upper: " << Upper() << endl
				<< " - x: " << x << endl
				<< abort(FatalError);
		}
		auto p0 = ExtrudeProfile()->Point(0);
		auto p1 = ExtrudeProfile()->Point(1);

		auto x0 = p0.X();
		auto y0 = p0.Y();

		auto x1 = p1.X();
		auto y1 = p1.Y();

		return y0 + (x - x0)*(y1 - y0) / (x1 - x0);
	}
	else
	{
		return gsl_spline_eval(theSpline_, x, theAcc_);
	}
}

void tnbLib::shapePxLib::ContinProfile_OffsetCustom::Update()
{
	Perform();
}
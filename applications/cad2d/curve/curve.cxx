#include <Pln_CurveTools.hxx>
#include <Pln_Curve.hxx>
#include <Dir2d.hxx>
#include <Entity2d_Polygon.hxx>
#include <Entity2d_Box.hxx>

#include <boost/archive/polymorphic_binary_iarchive.hpp>
#include <boost/archive/polymorphic_binary_oarchive.hpp>

#include <boost/archive/polymorphic_text_iarchive.hpp>
#include <boost/archive/polymorphic_text_oarchive.hpp>

#include <gp_Ax2d.hxx>
#include <gp_Circ2d.hxx>
#include <gp_Elips2d.hxx>
#include <gp_Hypr2d.hxx>
#include <gp_Parab2d.hxx>
#include <Geom2d_Curve.hxx>

namespace tnbLib
{

	typedef std::shared_ptr<Pln_Curve> curve_t;

	curve_t myCurve;

	auto makePoint(double x, double y)
	{
		return Pnt2d(x, y);
	}

	auto makeDir(double x, double y)
	{
		return Dir2d(x, y);
	}

	auto makeVec(double x, double y)
	{
		return Vec2d(x, y);
	}

	auto makeAx(const Pnt2d& p, const Dir2d& v)
	{
		return gp_Ax2d(p, v);
	}

	auto makeGeoCircle(const gp_Ax2d& ax, double r)
	{
		gp_Circ2d g(ax, r);
		return std::move(g);
	}

	auto makeGeoEllips(const gp_Ax2d& ax, double rm, double r)
	{
		gp_Elips2d g(ax, rm, r);
		return std::move(g);
	}

	auto makeGeoHybr(const gp_Ax2d& ax, double rm, double r)
	{
		gp_Hypr2d g(ax, rm, r);
		return std::move(g);
	}

	auto makeGeoParb(const gp_Ax2d& mirror, double focal)
	{
		gp_Parab2d g(mirror, focal);
		return std::move(g);
	}

	auto makeGeoParb(const gp_Ax2d& directrix, const Pnt2d& focal)
	{
		gp_Parab2d g(directrix, focal);
		return std::move(g);
	}

	void makeSegment(const Pnt2d& p0, const Pnt2d& p1)
	{
		auto geom = Pln_CurveTools::MakeSegment(p0, p1);
		myCurve = std::make_shared<Pln_Curve>(std::move(geom));
	}

	void makeCircArc(const Pnt2d& p0, const Pnt2d& p1, const Pnt2d& p2)
	{
		auto geom = Pln_CurveTools::MakeCircArc(p0, p1, p2);
		myCurve = std::make_shared<Pln_Curve>(std::move(geom));
	}

	void makeCircArc(const Pnt2d& p0, const Vec2d& v0, const Pnt2d& p2)
	{
		auto geom = Pln_CurveTools::MakeCircArc(p0, v0, p2);
		myCurve = std::make_shared<Pln_Curve>(std::move(geom));
	}

	void makeCircArc(const gp_Circ2d& c, double a0, double a1)
	{
		auto geom = Pln_CurveTools::MakeCircArc(c, a0, a1);
		myCurve = std::make_shared<Pln_Curve>(std::move(geom));
	}

	void makeCircArc(const gp_Circ2d& c, const Pnt2d& p0, const Pnt2d& p1)
	{
		auto geom = Pln_CurveTools::MakeCircArc(c, p0, p1);
		myCurve = std::make_shared<Pln_Curve>(std::move(geom));
	}

	void makeElipsArc(const gp_Elips2d& e, double a0, double a1)
	{
		auto geom = Pln_CurveTools::MakeElipsArc(e, a0, a1);
		myCurve = std::make_shared<Pln_Curve>(std::move(geom));
	}

	void makeElipsArc(const gp_Elips2d& e, const Pnt2d& p0, const Pnt2d& p1)
	{
		auto geom = Pln_CurveTools::MakeElipsArc(e, p0, p1);
		myCurve = std::make_shared<Pln_Curve>(std::move(geom));
	}

	void makeHyprArc(const gp_Hypr2d& h, double a0, double a1)
	{
		auto geom = Pln_CurveTools::MakeHyprArc(h, a0, a1);
		myCurve = std::make_shared<Pln_Curve>(std::move(geom));
	}

	void makeHyprArc(const gp_Hypr2d& h, const Pnt2d& p0, const Pnt2d& p1)
	{
		auto geom = Pln_CurveTools::MakeHyprArc(h, p0, p1);
		myCurve = std::make_shared<Pln_Curve>(std::move(geom));
	}

	void makeParbArc(const gp_Parab2d& p, double a0, double a1)
	{
		auto geom = Pln_CurveTools::MakeParbArc(p, a0, a1);
		myCurve = std::make_shared<Pln_Curve>(std::move(geom));
	}

	void makeParbArc(const gp_Parab2d& p, const Pnt2d& p0, const Pnt2d& p1)
	{
		auto geom = Pln_CurveTools::MakeParbArc(p, p0, p1);
		myCurve = std::make_shared<Pln_Curve>(std::move(geom));
	}

	void makeCircle(const gp_Circ2d& C)
	{
		auto geom = Pln_CurveTools::MakeCircle(C);
		auto trimmed = Pln_CurveTools::Trim(geom, geom->FirstParameter(), geom->LastParameter());
		myCurve = std::make_shared<Pln_Curve>(std::move(trimmed));
	}

	void makeCircle(const Pnt2d& p0, const Pnt2d& p1, const Pnt2d& p2)
	{
		auto geom = Pln_CurveTools::MakeCircle(p0, p1, p2);
		auto trimmed = Pln_CurveTools::Trim(geom, geom->FirstParameter(), geom->LastParameter());
		myCurve = std::make_shared<Pln_Curve>(std::move(trimmed));
	}

	void makeCircle(const Pnt2d& p0, double r)
	{
		auto geom = Pln_CurveTools::MakeCircle(p0, r);
		auto trimmed = Pln_CurveTools::Trim(geom, geom->FirstParameter(), geom->LastParameter());
		myCurve = std::make_shared<Pln_Curve>(std::move(trimmed));
	}

	void makeCircle(const Pnt2d& c, const Pnt2d& p)
	{
		auto geom = Pln_CurveTools::MakeCircle(c, p);
		auto trimmed = Pln_CurveTools::Trim(geom, geom->FirstParameter(), geom->LastParameter());
		myCurve = std::make_shared<Pln_Curve>(std::move(trimmed));
	}

	void makeEllipse(const gp_Elips2d& E)
	{
		auto geom = Pln_CurveTools::MakeEllipse(E);
		auto trimmed = Pln_CurveTools::Trim(geom, geom->FirstParameter(), geom->LastParameter());
		myCurve = std::make_shared<Pln_Curve>(std::move(trimmed));
	}

	void makeEllipse(const Pnt2d& s0, const Pnt2d& s1, const Pnt2d& c)
	{
		auto geom = Pln_CurveTools::MakeEllipse(s0, s1, c);
		auto trimmed = Pln_CurveTools::Trim(geom, geom->FirstParameter(), geom->LastParameter());
		myCurve = std::make_shared<Pln_Curve>(std::move(trimmed));
	}

	void saveTo(const std::string& name)
	{
		fileName fn(name);
		std::ofstream f(fn);

		boost::archive::polymorphic_text_oarchive oa(f);

		oa << myCurve;
	}

	void loadCurve(const std::string& name)
	{
		fileName fn(name);
		std::ifstream f(fn);

		boost::archive::polymorphic_text_iarchive oa(f);

		oa >> myCurve;
	}

	void exportToPlt(const std::string& name, int n)
	{
		fileName fn(name);
		OFstream f(fn);

		n = std::max(n, 5);
		if (myCurve)
		{
			const auto& geom = myCurve->Geometry();
			const auto u0 = myCurve->FirstParameter();
			const auto u1 = myCurve->LastParameter();
			const auto du = (u1 - u0) / n;

			Entity2d_Polygon poly;
			auto& pnts = poly.Points();
			pnts.reserve(n);
			for (size_t i = 0; i <= n; i++)
			{
				auto u = u0 + i * du;

				pnts.push_back(geom->Value(u));
			}

			poly.ExportToPlt(f);
		}
	}
}

#ifdef DebugInfo
#undef DebugInfo
#endif // DebugInfo

#include <chaiscript/chaiscript.hpp>

namespace tnbLib
{

	typedef std::shared_ptr<chaiscript::Module> module_t;

	void setGlobals(const module_t& mod)
	{
		mod->add(chaiscript::fun([](double x, double y)-> auto {return makePoint(x, y); }), "makePoint");
		mod->add(chaiscript::fun([](double x, double y)-> auto {return makeDir(x, y); }), "makeDirection");

		mod->add(chaiscript::fun([](const Pnt2d& p, const Dir2d& d)-> auto {return makeAx(p, d); }), "makeAxis");

		mod->add(chaiscript::fun([](const gp_Ax2d& ax, double r)-> auto {return makeGeoCircle(ax, r); }), "makeGeoCircle");
		mod->add(chaiscript::fun([](const gp_Ax2d& ax, double rm, double r)-> auto {return makeGeoEllips(ax, rm, r); }), "makeGeoEllips");
		mod->add(chaiscript::fun([](const gp_Ax2d& ax, double rm, double r)-> auto {return makeGeoHybr(ax, rm, r); }), "makeGeoHybr");
		mod->add(chaiscript::fun([](const gp_Ax2d& mirror, double focal)-> auto {return makeGeoParb(mirror, focal); }), "makeGeoParab");
		mod->add(chaiscript::fun([](const gp_Ax2d& directrix, const Pnt2d& focal)-> auto {return makeGeoParb(directrix, focal); }), "makeGeoParab");

		mod->add(chaiscript::fun([](const Pnt2d& p0, const Pnt2d& p1)-> auto {return makeSegment(p0, p1); }), "makeSegment");
		mod->add(chaiscript::fun([](const Pnt2d& p0, const Pnt2d& p1, const Pnt2d& p2)-> auto {return makeCircArc(p0, p1, p2); }), "makeCircArc");
		mod->add(chaiscript::fun([](const Pnt2d& p0, const Vec2d& v0, const Pnt2d& p2)-> auto {return makeCircArc(p0, v0, p2); }), "makeCircArc");
		mod->add(chaiscript::fun([](const gp_Circ2d& c, double a0, double a1)-> auto {return makeCircArc(c, a0, a1); }), "makeCircArc");
		mod->add(chaiscript::fun([](const gp_Circ2d& c, const Pnt2d& p0, const Pnt2d& p1)-> auto {return makeCircArc(c, p0, p1); }), "makeCircArc");

		mod->add(chaiscript::fun([](const gp_Elips2d& c, double a0, double a1)-> auto {return makeElipsArc(c, a0, a1); }), "makeElipsArc");
		mod->add(chaiscript::fun([](const gp_Elips2d& c, const Pnt2d& p0, const Pnt2d& p1)-> auto {return makeElipsArc(c, p0, p1); }), "makeElipsArc");

		mod->add(chaiscript::fun([](const gp_Hypr2d& c, double a0, double a1)-> auto {return makeHyprArc(c, a0, a1); }), "makeHyprArc");
		mod->add(chaiscript::fun([](const gp_Hypr2d& c, const Pnt2d& p0, const Pnt2d& p1)-> auto {return makeHyprArc(c, p0, p1); }), "makeHyprArc");

		mod->add(chaiscript::fun([](const gp_Parab2d& c, double a0, double a1)-> auto {return makeParbArc(c, a0, a1); }), "makeParbArc");
		mod->add(chaiscript::fun([](const gp_Parab2d& c, const Pnt2d& p0, const Pnt2d& p1)-> auto {return makeParbArc(c, p0, p1); }), "makeParbArc");

		mod->add(chaiscript::fun([](const gp_Circ2d& c)-> auto {return makeCircle(c); }), "makeCircle");
		mod->add(chaiscript::fun([](const Pnt2d& p0, const Pnt2d& p1, const Pnt2d& p2)-> auto {return makeCircle(p0, p1, p2); }), "makeCircle");
		mod->add(chaiscript::fun([](const Pnt2d& p0, double r)-> auto {return makeCircle(p0, r); }), "makeCircle");
		mod->add(chaiscript::fun([](const Pnt2d& c, const Pnt2d& p)-> auto {return makeCircle(c, p); }), "makeCircle");

		mod->add(chaiscript::fun([](const gp_Elips2d& c)-> auto {return makeEllipse(c); }), "makeElips");
		mod->add(chaiscript::fun([](const Pnt2d& s0, const Pnt2d& s1, const Pnt2d& c)-> auto {return makeEllipse(s0, s1, c); }), "makeElips");

		mod->add(chaiscript::fun([](const std::string& name)-> void {saveTo(name); }), "saveTo");
		mod->add(chaiscript::fun([](const std::string& name)-> void {loadCurve(name); }), "loadCurve");
		mod->add(chaiscript::fun([](const std::string& name, int n)-> void {exportToPlt(name, n); }), "exportToPlt");
	}

	std::string getString(char* argv)
	{
		std::string argument(argv);
		return std::move(argument);
	}

	Standard_Boolean IsEqualCommand(char* argv, const std::string& command)
	{
		auto argument = getString(argv);
		return argument IS_EQUAL command;
	}
}

using namespace tnbLib;

int main(int argc, char *argv[])
{
	FatalError.throwExceptions();


	if (argc <= 1)
	{
		Info << " - No command is entered" << endl
			<< " - For more information use '--help' command" << endl;
		FatalError.exit();
	}

	if (argc IS_EQUAL 2)
	{
		if (IsEqualCommand(argv[1], "--help"))
		{
			Info << "this is help" << endl;
		}
		else if (IsEqualCommand(argv[1], "--run"))
		{
			chaiscript::ChaiScript chai;

			auto mod = std::make_shared<chaiscript::Module>();

			setGlobals(mod);

			chai.add(mod);

			fileName myFileName("curve");

			try
			{
				chai.eval_file(myFileName);
			}
			catch (const chaiscript::exception::eval_error& x)
			{
				Info << x.pretty_print() << endl;
			}
			catch (const error& x)
			{
				Info << x.message() << endl;
			}
			catch (const std::exception& x)
			{
				Info << x.what() << endl;
			}
		}
	}
	else
	{
		Info << " - No valid command is entered" << endl
			<< " - For more information use '--help' command" << endl;
		FatalError.exit();
	}

}
#include <PtdModel_Shape_PropBlade.hxx>

tnbLib::ptdModel::Shape_PropBlade::Shape_PropBlade
(
	const std::shared_ptr<Cad_Shape>& theShape
)
	: Shape_PropEntity(theShape)
{
	//- empty body
}

tnbLib::ptdModel::Shape_PropBlade::Shape_PropBlade
(
	std::shared_ptr<Cad_Shape>&& theShape
)
	: Shape_PropEntity(std::move(theShape))
{
	//- empty body
}

tnbLib::ptdModel::Shape_PropBlade::Shape_PropBlade
(
	const Standard_Integer theIndex,
	const word& theName, 
	const std::shared_ptr<Cad_Shape>& theShape
)
	: Shape_PropEntity(theIndex, theName, theShape)
{
	//- empty body
}

tnbLib::ptdModel::Shape_PropBlade::Shape_PropBlade
(
	const Standard_Integer theIndex, 
	const word& theName,
	std::shared_ptr<Cad_Shape>&& theShape
)
	: Shape_PropEntity(theIndex, theName, std::move(theShape))
{
	//- empty body
}


#include <Marine_CmpSection.hxx>

#include <Marine_Section.hxx>

TNB_SAVE_IMPLEMENTATION(tnbLib::Marine_CmpSection)
{
	ar & boost::serialization::base_object<Marine_CoordinatedEntity>(*this);
	ar & Sections();
}

TNB_LOAD_IMPLEMENTATION(tnbLib::Marine_CmpSection)
{
	ar & boost::serialization::base_object<Marine_CoordinatedEntity>(*this);
	ar & ChangeSections();
}
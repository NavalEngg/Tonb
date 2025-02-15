#include <Global_File.hxx>

#include <Global_Macros.hxx>
#include <TnbError.hxx>
#include <OSstream.hxx>

#include <OSspecific.hxx>

#include <boost\filesystem.hpp>

const tnbLib::word tnbLib::file::homeDir = "TnbHome";

std::string 
tnbLib::file::GetHomeDir()
{
	if (NOT env(homeDir))
	{
		FatalErrorIn(FunctionSIG)
			<< "no \"TnbHome\" environment value has been defined!" << endl
			<< abort(FatalError);
	}

	auto val = getEnv(homeDir);
	return std::move(val);
}

std::string 
tnbLib::file::GetSystemFile(const std::string & theAppName)
{
	auto current = boost::filesystem::current_path().string();
	boost::filesystem::path p(current + R"(\system\)" + theAppName);

	if (boost::filesystem::exists(p))
	{
		return ".\\system\\" + theAppName;
	}
	else
	{
		auto dir = GetHomeDir();
		boost::filesystem::path p1(dir + R"(\)" + theAppName);
		if (boost::filesystem::exists(p1))
		{
			return p1.string();
		}
		else
		{
			FatalErrorIn(FunctionSIG)
				<< "no system file has been found!" << endl
				<< " - application: " << theAppName << endl
				<< abort(FatalError);
		}
	}
	return std::string();
}

std::vector<boost::filesystem::path> 
tnbLib::file::GetAllFileNames(const boost::filesystem::path& p)
{
	boost::filesystem::directory_iterator end_itr;

	std::vector<boost::filesystem::path> names;
	for (boost::filesystem::directory_iterator iter(p); iter != end_itr; iter++)
	{
		if (boost::filesystem::is_regular_file(iter->path()))
		{
			auto file = iter->path();
			names.push_back(std::move(file));
		}
	}
	return std::move(names);
}

std::vector<boost::filesystem::path> 
tnbLib::file::GetAllFileNames
(
	const boost::filesystem::path & p,
	const std::string & extension
)
{
	boost::filesystem::directory_iterator end_itr;

	std::vector<boost::filesystem::path> names;
	for (boost::filesystem::directory_iterator iter(p); iter != end_itr; iter++)
	{
		if (boost::filesystem::is_regular_file(iter->path()) && iter->path().extension() == extension)
		{
			auto file = iter->path();
			names.push_back(std::move(file));
		}
	}
	return std::move(names);
}

boost::filesystem::path 
tnbLib::file::GetSingleFile
(
	const boost::filesystem::path& p, 
	const std::string& extension
)
{
	auto files = tnbLib::file::GetAllFileNames(p);

	size_t nbFiles = 0;
	boost::filesystem::path file;
	for (const auto& x : files)
	{
		if (x.extension().string() IS_EQUAL extension)
		{
			file = x;
			nbFiles++;
		}
	}

	if (nbFiles NOT_EQUAL 1)
	{
		FatalErrorIn(FunctionSIG)
			<< "no *." << extension << " has been found." << endl
			<< abort(FatalError);
	}
	return file.stem();
}

void tnbLib::file::CheckExtension(const std::string& e)
{
	boost::filesystem::path p(e);
	if (p.has_extension())
	{
		FatalErrorIn(FunctionSIG)
			<< "no extension is accepted for a filename" << endl
			<< abort(FatalError);
	}
}
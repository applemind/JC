#include "CtFileMGR.h"
#include "CtFile.h"

CtFileMGR::CtFileMGR(void)
{
}


CtFileMGR::~CtFileMGR(void)
{
}

bool CtFileMGR::is_exist(const string &file_name)
{
	return true;
}

bool CtFileMGR::cp(const string &source_file_name, const string &dest_file_name, const bool force /* = false */)
{
	string cmd;
	int ret;
#ifdef WIN32
	{	/*** miscrosoft ***/
		cmd.append("copy "+source_file_name+" "+dest_file_name);
// 		cmd.append(source_file_name);
// 		cmd.append(" ");
// 		cmd.append(dest_file_name);
		ret = system(cmd.c_str());
	}
#else
	{

	}
#endif

	return true;
}
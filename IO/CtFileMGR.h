#ifndef __CT_FILEMGR_H__
#define __CT_FILEMGR_H__

#include <string>

using std::string;

class CtFileMGR
{
public:
	explicit CtFileMGR(void);
	virtual ~CtFileMGR(void);
	
	static bool is_exist(const string &file_name);
 	static bool cp(const string &source_file_name, const string &dest_file_name, const bool force = false);
// 	static bool move(string &source_file, string &dest_path);
};

#endif
#ifndef __CT_FILE_H__
#define __CT_FILE_H__

#include "global.h"
#include <string>

#ifdef WIN32
#include <WTypes.h>
typedef HANDLE CtFileHandle; /*文件句柄类型*/
#else
typedef int CtFileHandle;	 /*文件句柄类型*/
#endif

using std::string;

/*
 * class :  CtFile  -  文件访问类，用于打开、读取、写入文件等操作
 */
class CtFile
{
public:
	enum FileError
	{
		kNoError,
		kOpenModeError,
		kFileNameError,
		kReadError,
		kWriteError,
		kFatalError,
		kResourceError,
		kAbortError,
		kTimeOutError,
		kUnspecifiedError,
		kRemoveError,
		kRenameError,
		kPositionError,
		kResizeError,
		kPermissionsError,
		kCopyError
	};

	enum Permission 
	{
		kReadUser  = 0x0400, kWriteUser  = 0x0200, kExeUser  = 0x0100,
		kReadGroup = 0x0040, kWriteGroup = 0x0020, kExeGroup = 0x0010,
		kReadOther = 0x0004, kWriteOther = 0x0002, kExeOther = 0x0001
	};

	enum OpenModeFlag 
	{
		kNotOpen   = 0x0000,		/*not open*/

		/* access mode */
		kAccessModeFlagsMask = 0x000f,
		kReadOnly  = 0x0001,		/*open for reading only*/
		kWriteOnly = 0x0002,		/*open for writing only*/
		kReadWrite = kReadOnly | kWriteOnly, /*open for reading and writing*/

		/* file creation flags */
		kFileCreationFlagsMask = 0x00f0,
		kCreate    = 0x0010,		/* create and open file if file doesn't exist*/
		kExcl      = 0x0020,		/* open only if file doesn't already exist */
		kTrunc     = 0x0040,		/* open and truncated to length 0 */
		kNoCtty    = 0x0080,

		/* file status flags, can be retrieved and modified using fcntl */
		kAppend    = 0x0100,		/* writes done at eof */
		kAsync     = 0x0200,		/* enable signal-driven I/O */
		kNonBlock  = 0x0400,		/* non block */
		kText      = 0x0800,		/* file mode is text (translated) */
		kBinary    = 0x1000			/* file mode is binary (untranslated) */
	};
	enum LseekWhence
	{	/*位移量*/
		kSeekBegin,  /*绝对位移量*/
		kSeekCur,   /*相对于当前位置的位移量*/
		kSeekEnd    /*相对于文件结尾的位移量*/
	};

	explicit CtFile(void);
	virtual ~CtFile(void);

	
	bool open(const string& file_name, const int open_mode); /*打开文件*/
	void close(void); /*关闭文件*/
	int write(const void *data, const size_t len);/*从当前位置开始写入*/
	int write(const void *data, const size_t len, const off_t offset, const LseekWhence whence);/*指定位置开始写入*/
	int read(void *data, const size_t len);       /*从当前位置开始读取*/
	int read(void *data, const size_t len, const off_t offset, const LseekWhence whence); /*指定位置读取*/
	
	inline const string& get_name(void) const { return m_file_name; }             /*获取文件名*/
	inline bool is_open(void) {return (m_open_mode != kNotOpen);}                 /*是否打开文件*/
	inline bool is_readable(void) {return (m_open_mode&kReadOnly)==kReadOnly;}    /*是否可读*/
	inline bool is_writable(void ) {return (m_open_mode&kWriteOnly)==kWriteOnly;} /*是否可写*/

// 	inline bool readonly_open(const string& file_name) { return open(file_name, kReadOnly); }				/*只读打开文件*/
// 	inline bool writeonly_open(const string& file_name) { return open(file_name, (kWriteOnly|kCreate) );}	/*只写打开文件*/
// 	inline bool rw_open(const string& file_name) { return open(file_name, kReadWrite|kAppend); }					/*读写打开文件*/

private:
	inline off_t seek(const off_t offset, const LseekWhence whence); /*修改文件内部位置指针*/
	CtFileHandle m_file_handle; /*文件句柄*/
	string m_file_name;			/*操作的文件名称*/
	OpenModeFlag m_open_mode;	/*记录文件打开类型*/
};

inline int CtFile::write(const void *data, const size_t len, const off_t offset, const LseekWhence whence)
{
	return (-1 == seek(offset, whence)) ? -1 : write(data,len);
}

inline int CtFile::read(void *data, const size_t len, const off_t offset, const LseekWhence whence)
{
	return  (-1 == seek(offset, whence)) ? -1 : read(data,len);
}

#endif
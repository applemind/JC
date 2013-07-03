#ifndef __CT_FILE_H__
#define __CT_FILE_H__

#include "global.h"
#include <string>

#ifdef WIN32
#include <WTypes.h>
typedef HANDLE CtFileHandle; /*�ļ��������*/
#else
typedef int CtFileHandle;	 /*�ļ��������*/
#endif

using std::string;

/*
 * class :  CtFile  -  �ļ������࣬���ڴ򿪡���ȡ��д���ļ��Ȳ���
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
	{	/*λ����*/
		kSeekBegin,  /*����λ����*/
		kSeekCur,   /*����ڵ�ǰλ�õ�λ����*/
		kSeekEnd    /*������ļ���β��λ����*/
	};

	explicit CtFile(void);
	virtual ~CtFile(void);

	
	bool open(const string& file_name, const int open_mode); /*���ļ�*/
	void close(void); /*�ر��ļ�*/
	int write(const void *data, const size_t len);/*�ӵ�ǰλ�ÿ�ʼд��*/
	int write(const void *data, const size_t len, const off_t offset, const LseekWhence whence);/*ָ��λ�ÿ�ʼд��*/
	int read(void *data, const size_t len);       /*�ӵ�ǰλ�ÿ�ʼ��ȡ*/
	int read(void *data, const size_t len, const off_t offset, const LseekWhence whence); /*ָ��λ�ö�ȡ*/
	
	inline const string& get_name(void) const { return m_file_name; }             /*��ȡ�ļ���*/
	inline bool is_open(void) {return (m_open_mode != kNotOpen);}                 /*�Ƿ���ļ�*/
	inline bool is_readable(void) {return (m_open_mode&kReadOnly)==kReadOnly;}    /*�Ƿ�ɶ�*/
	inline bool is_writable(void ) {return (m_open_mode&kWriteOnly)==kWriteOnly;} /*�Ƿ��д*/

// 	inline bool readonly_open(const string& file_name) { return open(file_name, kReadOnly); }				/*ֻ�����ļ�*/
// 	inline bool writeonly_open(const string& file_name) { return open(file_name, (kWriteOnly|kCreate) );}	/*ֻд���ļ�*/
// 	inline bool rw_open(const string& file_name) { return open(file_name, kReadWrite|kAppend); }					/*��д���ļ�*/

private:
	inline off_t seek(const off_t offset, const LseekWhence whence); /*�޸��ļ��ڲ�λ��ָ��*/
	CtFileHandle m_file_handle; /*�ļ����*/
	string m_file_name;			/*�������ļ�����*/
	OpenModeFlag m_open_mode;	/*��¼�ļ�������*/
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
#include "CtFile.h"

#ifdef WIN32
#include <WinNT.h>
#else
#endif

CtFile::CtFile(void)
	:m_open_mode(kNotOpen),m_file_name("")
{
}


CtFile::~CtFile(void)
{
}



bool CtFile::open(const string &file_name, const int open_mode)
{
	if( is_open() )
		return false;
	if( (open_mode & kAccessModeFlagsMask) == kNotOpen )
	{	/*没指定打开方式，返回错误*/
		return false;
	}
#ifdef WIN32
	{	/********* windows *********/	
		DWORD desired_access = 0;		/*文件的操作属性*/
		DWORD creation_disposition=0;	/*文件操作*/
		//DWORD flags_and_attributes;		/*文件属性*/
		//DWORD share_mode = 0;			/*文件的共享属性*/
		//LPSECURITY_ATTRIBUTES security_attributes;	/*文件的安全特性*/


		/*设置文件的操作属性*/
		if( open_mode & kReadOnly )
			desired_access |= GENERIC_READ;
		if( open_mode & kWriteOnly )
			desired_access |= GENERIC_WRITE;

		/*设置文件操作*/
		switch( open_mode& kFileCreationFlagsMask )
		{
		case kCreate:
			creation_disposition = OPEN_ALWAYS;      /*如文件不存在则创建它*/
			break;
		case kCreate|kExcl: 
			creation_disposition = CREATE_NEW;       /*创建文件,如文件存在则会出错*/
			break;
		// 	case kCreate|kTrunc:
		// 		creation_disposition = CREATE_ALWAYS;    /*创建一个新文件,如果该文件已经存在,函数将覆盖已存在的文件并清除已存在的文件属性*/
		// 		break;
		case kCreate|kTrunc:
			creation_disposition = TRUNCATE_EXISTING; /*将现有文件缩短为零长度*/
			break;
		default:
			creation_disposition = OPEN_EXISTING;     /*打开一个文件，文件不存在则失败*/
		}
		m_file_handle = CreateFile( file_name.c_str(), desired_access, FILE_SHARE_READ, NULL, creation_disposition, FILE_ATTRIBUTE_NORMAL,NULL);
		if( INVALID_HANDLE_VALUE == m_file_handle )
			return false;
	}
#else
	{	/*** unix ***/
		m_file_handle = open( file_name, );
		if( m_file_handle < 0 )
			return false;
	}
#endif

	if( open_mode & kAppend )
		seek(0, kSeekEnd);

	m_open_mode = (OpenModeFlag)open_mode;
	m_file_name = file_name;
	return true;
}

void CtFile::close(void)
{
#ifdef WIN32
/*** microsoft ***/
	CloseHandle(m_file_handle);
#else
/*** unix ***/
	close(m_file_handle);
#endif
	m_open_mode = kNotOpen;
	m_file_name.clear();
}

/**
 * write  -  写入函数
 * Param : data 要写入的数据
 *         len 数据长度
 * Return: 默认为已写入的数据长度，-1为写入失败 
 */
int CtFile::write(const void *data, const size_t len)
{
	if( ! ( is_open() && is_writable() ))
		return -1;
	if( data == nullptr || len == 0 )
		return -1;

	int ret;
#ifdef WIN32
	{	/*** microsoft ***/
		if( ! WriteFile(m_file_handle,data, len, (LPDWORD)&ret, nullptr) )
			ret = -1;
	}
#else
	{	/*** Unix ***/
		ret = write(m_file_handle, data, len);
	}
#endif
	return ret;
}

/**
 * read  -  读取文件数据
 * Param : data 保存读取的数据
 *         len 要读取的数据长度
 * Return: 默认返回已读取到的数据长度，返回－1为读取失败
 */
int CtFile::read(void *data, const size_t len)
{
	if( ! ( is_open() && is_readable() ) )
		return -1;
	if( data == nullptr )
		return -1;
	
	int ret;
#ifdef WIN32
	{	/*** microsoft ***/
		if( ! ReadFile(m_file_handle, data, len, (LPDWORD)&ret, nullptr) )
			ret = -1;
	}
#else
	{	/*** unix ***/
		ret = read(m_file_handle, data, len);
	}
#endif
	return ret;
}

/**
 * seek  -  修改文件内部位置指针
 * Param : offset 位移量
 *         whence 起始位置
 * Return: 成功则返回的是当前文件数据的偏移量，失败返回－1
 */
off_t CtFile::seek(off_t offset, LseekWhence whence)
{
#ifdef WIN32
	{	/*** microsoft ***/
		return SetFilePointer(m_file_handle, offset, NULL, whence); /**/
	}
#else
	{	/*** unix ***/
		return seek(m_file_handle, offset, whence);
	}
#endif
}
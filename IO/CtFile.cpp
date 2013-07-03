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
	{	/*ûָ���򿪷�ʽ�����ش���*/
		return false;
	}
#ifdef WIN32
	{	/********* windows *********/	
		DWORD desired_access = 0;		/*�ļ��Ĳ�������*/
		DWORD creation_disposition=0;	/*�ļ�����*/
		//DWORD flags_and_attributes;		/*�ļ�����*/
		//DWORD share_mode = 0;			/*�ļ��Ĺ�������*/
		//LPSECURITY_ATTRIBUTES security_attributes;	/*�ļ��İ�ȫ����*/


		/*�����ļ��Ĳ�������*/
		if( open_mode & kReadOnly )
			desired_access |= GENERIC_READ;
		if( open_mode & kWriteOnly )
			desired_access |= GENERIC_WRITE;

		/*�����ļ�����*/
		switch( open_mode& kFileCreationFlagsMask )
		{
		case kCreate:
			creation_disposition = OPEN_ALWAYS;      /*���ļ��������򴴽���*/
			break;
		case kCreate|kExcl: 
			creation_disposition = CREATE_NEW;       /*�����ļ�,���ļ�����������*/
			break;
		// 	case kCreate|kTrunc:
		// 		creation_disposition = CREATE_ALWAYS;    /*����һ�����ļ�,������ļ��Ѿ�����,�����������Ѵ��ڵ��ļ�������Ѵ��ڵ��ļ�����*/
		// 		break;
		case kCreate|kTrunc:
			creation_disposition = TRUNCATE_EXISTING; /*�������ļ�����Ϊ�㳤��*/
			break;
		default:
			creation_disposition = OPEN_EXISTING;     /*��һ���ļ����ļ���������ʧ��*/
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
 * write  -  д�뺯��
 * Param : data Ҫд�������
 *         len ���ݳ���
 * Return: Ĭ��Ϊ��д������ݳ��ȣ�-1Ϊд��ʧ�� 
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
 * read  -  ��ȡ�ļ�����
 * Param : data �����ȡ������
 *         len Ҫ��ȡ�����ݳ���
 * Return: Ĭ�Ϸ����Ѷ�ȡ�������ݳ��ȣ����أ�1Ϊ��ȡʧ��
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
 * seek  -  �޸��ļ��ڲ�λ��ָ��
 * Param : offset λ����
 *         whence ��ʼλ��
 * Return: �ɹ��򷵻ص��ǵ�ǰ�ļ����ݵ�ƫ������ʧ�ܷ��أ�1
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
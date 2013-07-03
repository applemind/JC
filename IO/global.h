/*
 * global.h  -  ͨ��ͷ�ļ������峣�����ͺͺ���
 * ���ߣ� �����
 * ��ʷ�� 20130701  �����  ����
 * ������ �����microsoftƽ̨�ϣ��ַ���ʹ��"unicode"
 *        ��unixƽ̨�ϣ��ַ���ʹ��"utf-8"
 */

#ifndef __GLOBAL_H__
#define __GLOBAL_H__

/*** �������Ͷ��� ***/
typedef char  CtInt8;
typedef short CtInt16;
typedef long  CtInt32;
typedef long long CtInt64;
typedef unsigned char CtUint8;
typedef unsigned short CtUint16;
typedef unsigned long CtUint32;
typedef unsigned long long CtUint64;


#ifdef _DEBUG
#include <stdio.h>
#define PDEBUG(format, ...) printf(format, ##__VA_ARGS__)
#else
#define PDEBUG(format, ...)
#endif


/***��ֹʹ�ÿ������캯����"operator="��ֵ�����ĺ꣬�����"private"��ʹ��*/
#define DISALLOW_COPY_AND_ASSIGN(TypeName) \
	TypeName(const TypeName &); \
	void operator=(const TypeName &)


#endif
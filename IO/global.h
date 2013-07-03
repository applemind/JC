/*
 * global.h  -  通用头文件，定义常用类型和函数
 * 作者： 杨焕杰
 * 历史： 20130701  杨焕杰  创建
 * 申明： 如果在microsoft平台上，字符集使用"unicode"
 *        在unix平台上，字符集使用"utf-8"
 */

#ifndef __GLOBAL_H__
#define __GLOBAL_H__

/*** 基本类型定义 ***/
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


/***禁止使用拷贝构造函数和"operator="赋值操作的宏，在类的"private"中使用*/
#define DISALLOW_COPY_AND_ASSIGN(TypeName) \
	TypeName(const TypeName &); \
	void operator=(const TypeName &)


#endif
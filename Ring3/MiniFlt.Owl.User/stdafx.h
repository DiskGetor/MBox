#pragma once
#define WIN32_LEAN_AND_MEAN

#include <Windows.h>
#include <fltUser.h>

#include <new>

// �ڴ˴����ó�����Ҫ������ͷ�ļ�

using NTSTATUS = LONG;

#ifndef STATUS_SUCCESS
#define STATUS_SUCCESS ((NTSTATUS)0x00000000L) 
#endif
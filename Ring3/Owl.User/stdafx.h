#pragma once

// �� Windows ͷ���ų�����ʹ�õ�����
#define WIN32_LEAN_AND_MEAN          

#include <Windows.h>
#include <WinIoCtl.h>
#include <strsafe.h>
#include <process.h>

#include <new>

// �ڴ˴����ó�����Ҫ������ͷ�ļ�

using NTSTATUS = LONG;

#ifndef STATUS_SUCCESS
#define STATUS_SUCCESS ((NTSTATUS)0x00000000L) 
#endif

#include <Owl\MBox.OwlProtocol.h>

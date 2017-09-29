#include <wdm.h>
#include "KTL.Memory.New.h"
#include "KTL.Macro.h"

const ktl::u32 DefaultPoolTag = KTL$CompileTime$ByteSwap32$Macro('KNew');
POOL_TYPE DefaultPoolType  = POOL_TYPE::NonPagedPoolNx;

//////////////////////////////////////////////////////////////////////////

//
// Compatible with XP
//

void KtlInitializeDefaultPoolTypeWithRuntime(
    ktl::u32 aRuntimeFlags)
{
    NTSTATUS vStatus;
    ktl::u32 vMajorVersion;
    ktl::u32 vMinorVersion;
    RTL_OSVERSIONINFOW vVersionInfo{ sizeof(vVersionInfo) };

    vStatus = RtlGetVersion(&vVersionInfo);

    if (!NT_VERIFY(NT_SUCCESS(vStatus))) 
    {
        vMajorVersion = 5;
        vMinorVersion = 0;
    }
    else 
    {
        vMajorVersion = vVersionInfo.dwMajorVersion;
        vMinorVersion = vVersionInfo.dwMinorVersion;
    }

    if ((aRuntimeFlags & DrvRtPoolNxOptIn) != 0)
    {
        //
        // Discover whether NX pool support is available on this platform, and,
        // if so, initialize the default non-paged pool type.
        //

        if ((vMajorVersion < 6) ||
            (vMajorVersion == 6 && vMinorVersion < 2))
        {
            DefaultPoolType = POOL_TYPE::NonPagedPool;
        }
    }
}

//
// Check buffer 
//

#ifdef DBG
// �ڿ��� Driver Verifier ֮��, ���ڻ�ı� Pool ����, ����Ҫ��ʱ�ر� Tag ��֤
#define KTL$CompileTime$DisableCheckPoolTag$Macro 1
#endif

#ifndef KTL$CompileTime$DisableCheckPoolTag$Macro

#pragma warning( push )
#pragma warning( disable: 4201)
struct PoolHeader
{
    union
    {
        struct
        {
            struct
            {
                UINT16       PreviousSize : 8;
                UINT16       PoolIndex : 8;
            };
            struct
            {
                UINT16       BlockSize : 8;
                UINT16       PoolType : 8;
            };
        };
        ktl::u32    Filling;
    };
    ktl::u32        PoolTag;
    union
    {
        struct _EPROCESS* ProcessBilled;
        struct
        {
            UINT16       AllocatorBackTraceIndex;
            UINT16       PoolTagHash;
        };
    };
};
#pragma warning( pop )

void CheckPoolTag(void* aPtr, ktl::u32 aTag)
{
    if (PAGE_ALIGN(aPtr) == aPtr)
    {
        //
        // ��֧��
        // ��Ϊ�洢��ҳ�ص� nt!PoolBigPageTable ����,
        // �Լ����������С�� nt!PoolBigPageTableSize ��δ����.
        //
        return;
    }

    PoolHeader *vPoolHeader = (PoolHeader*)(ktl::uintptr(aPtr) - sizeof(PoolHeader));
    if (aTag != vPoolHeader->PoolTag)
    {
        // See https://docs.microsoft.com/en-us/windows-hardware/drivers/debugger/bug-check-0x19--bad-pool-header
        KeBugCheckEx(
            BAD_POOL_HEADER,
            0x21,
            ktl::uintptr(aPtr),
            ktl::uintptr(vPoolHeader->BlockSize),
            ktl::uintptr(aTag));
    }
}
#else

#define CheckPoolTag(aPtr, aTag)

#endif

//////////////////////////////////////////////////////////////////////////
// replaceable usual deallocation functions
//////////////////////////////////////////////////////////////////////////

void * __cdecl operator new(size_t aSize) NOEXCEPT$TYPE
{
    // ����C++��׼����sizeΪ1ʱ������1�ֽ�

    if (0 == aSize)
    {
        aSize = 1;
    }
    
    return ExAllocatePoolWithTag(POOL_TYPE(DefaultPoolType), aSize, DefaultPoolTag);
}

void __cdecl operator delete(void * aPtr) NOEXCEPT$TYPE
{
    if (nullptr == aPtr)
    {
        return;
    }

    CheckPoolTag(aPtr, DefaultPoolTag);
    return ExFreePoolWithTag(aPtr, DefaultPoolTag);
}

void * __cdecl operator new(size_t aSize, POOL_TYPE aPoolType) NOEXCEPT$TYPE
{
    if (0 == aSize)
    {
        aSize = 1;
    }

    return ExAllocatePoolWithTag(POOL_TYPE(aPoolType), aSize, DefaultPoolTag);
}

void * __cdecl operator new(
    size_t aSize,
    POOL_TYPE aPoolType,
    unsigned long aTag) NOEXCEPT$TYPE
{
    if (0 == aSize)
    {
        aSize = 1;
    }

    return ExAllocatePoolWithTag(POOL_TYPE(aPoolType), aSize, aTag);
}

void __cdecl operator delete(void * aPtr, unsigned long aTag) NOEXCEPT$TYPE
{
    if (nullptr == aPtr)
    {
        return;
    }

    CheckPoolTag(aPtr, aTag);
    return ExFreePoolWithTag(aPtr, aTag);
}

void * __cdecl operator new[](size_t aSize) NOEXCEPT$TYPE
{
    if (0 == aSize)
    {
        aSize = 1;
    }

    return ExAllocatePoolWithTag(POOL_TYPE(DefaultPoolType), aSize, DefaultPoolTag);
}

void __cdecl operator delete[](void * aPtr) NOEXCEPT$TYPE
{
    if (nullptr == aPtr)
    {
        return;
    }

    CheckPoolTag(aPtr, DefaultPoolTag);
    return ExFreePoolWithTag(aPtr, DefaultPoolTag);
}

void * __cdecl operator new[](size_t aSize, POOL_TYPE aPoolType) NOEXCEPT$TYPE
{
    if (0 == aSize)
    {
        aSize = 1;
    }

    return ExAllocatePoolWithTag(POOL_TYPE(aPoolType), aSize, DefaultPoolTag);
}

void * __cdecl operator new[](
    size_t aSize,
    POOL_TYPE aPoolType,
    unsigned long aTag) NOEXCEPT$TYPE
{
    if (0 == aSize)
    {
        aSize = 1;
    }

    return ExAllocatePoolWithTag(POOL_TYPE(aPoolType), aSize, aTag);
}

void __cdecl operator delete[](void * aPtr, unsigned long aTag) NOEXCEPT$TYPE
{
    if (nullptr == aPtr)
    {
        return;
    }

    CheckPoolTag(aPtr, aTag);
    return ExFreePoolWithTag(aPtr, aTag);
}


//////////////////////////////////////////////////////////////////////////
// replaceable placement deallocation functions
//////////////////////////////////////////////////////////////////////////

void * __cdecl operator new(size_t /*aSize*/, void * aPtr) NOEXCEPT$TYPE
{
    return aPtr;
}

void * __cdecl operator new[](size_t /*aSize*/, void * aPtr) NOEXCEPT$TYPE
{
    return aPtr;
}

// void __cdecl operator delete(void * /*aPtr*/, void *) NOEXCEPT$TYPE
// {
// }
// 
// void __cdecl operator delete[](void * /*aPtr*/, void *) NOEXCEPT$TYPE
// {
// }


//////////////////////////////////////////////////////////////////////////
// sized class - specific deallocation functions
//////////////////////////////////////////////////////////////////////////

void __cdecl operator delete(void * aPtr, size_t /*aSize*/) NOEXCEPT$TYPE
{
    if (nullptr == aPtr)
    {
        return;
    }

    CheckPoolTag(aPtr, DefaultPoolTag);
    return ExFreePoolWithTag(aPtr, DefaultPoolTag);
}

void __cdecl operator delete[](void * aPtr, size_t /*aSize*/) NOEXCEPT$TYPE
{
    if (nullptr == aPtr)
    {
        return;
    }

    CheckPoolTag(aPtr, DefaultPoolTag);
    return ExFreePoolWithTag(aPtr, DefaultPoolTag);
}

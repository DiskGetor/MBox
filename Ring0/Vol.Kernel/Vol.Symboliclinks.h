#pragma once
#include <KTL\KTL.Memory.New.h>


namespace MBox
{
    namespace Vol
    {
        namespace Symboliclinks
        {
            NTSTATUS QuerySymboliclinkObjectName(
                PUNICODE_STRING aObjectName,
                const PUNICODE_STRING aSymboliclink,
                BOOLEAN aIsCaseInsensitive,
                HANDLE aRootDirectory,
                UINT32* aNeedBytes);

            NTSTATUS ReferenceSymboliclinkObjectName(
                PUNICODE_STRING* aObjectName,
                const PUNICODE_STRING aSymboliclink,
                BOOLEAN aIsCaseInsensitive = TRUE,
                HANDLE aRootDirectory = nullptr,
                POOL_TYPE aPoolType = DefaultPoolType,
                UINT32 aTag = DefaultPoolTag);

            // ��ȡ���������������õĶ�����.
            // ����: Sym3->Sym2->Sym1, ��ô��ѯ Sym3 ���õ� Sym1
            NTSTATUS ReferenceSymboliclinkFinalObjectName(
                PUNICODE_STRING* aObjectName,
                const PUNICODE_STRING aSymboliclink,
                BOOLEAN aIsCaseInsensitive = TRUE,
                HANDLE aRootDirectory = nullptr,
                POOL_TYPE aPoolType = DefaultPoolType,
                UINT32 aTag = DefaultPoolTag);

            void DeferenceSymboliclinkObjectName(
                PUNICODE_STRING aObjectName);
        }
    }
}

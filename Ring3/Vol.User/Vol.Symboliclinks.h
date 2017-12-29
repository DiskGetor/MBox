#pragma once
#include <Microsoft\MBox.Base.h>

namespace MBox::Vol::Symboliclinks
{
    HRESULT QuerySymboliclinkObjectName(
        UnicodeString* aObjectName,
        const UnicodeString* aSymboliclink,
        BOOLEAN aIsCaseInsensitive,
        HANDLE aRootDirectory,
        UINT32 * aNeedBytes);

    HRESULT ReferenceSymboliclinkObjectName(
        UnicodeString** aObjectName,
        const UnicodeString* aSymboliclink,
        BOOLEAN aIsCaseInsensitive = TRUE,
        HANDLE aRootDirectory = nullptr);

    // ��ȡ���������������õĶ�����.
    // ����: Sym3->Sym2->Sym1, ��ô��ѯ Sym3 ���õ� Sym1
    HRESULT ReferenceSymboliclinkFinalObjectName(
        UnicodeString** aObjectName,
        const UnicodeString* aSymboliclink,
        BOOLEAN aIsCaseInsensitive = TRUE,
        HANDLE aRootDirectory = nullptr);

    void DeferenceSymboliclinkObjectName(
        UnicodeString* aObjectName);
}

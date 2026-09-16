// Stub for Strsafe.h on macOS/Linux
#pragma once
static inline HRESULT StringCbCopyA(char* pszDest, size_t cbDest, const char* pszSrc) {
    size_t len = strlen(pszSrc);
    if (len >= cbDest) return STRSAFE_E_INVALID_LENGTH;
    memcpy(pszDest, pszSrc, len + 1);
    return S_OK;
}
#define STRSAFE_E_INVALID_LENGTH 0x800700CF

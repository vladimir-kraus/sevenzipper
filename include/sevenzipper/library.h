#pragma once

#include <Windows.h>

#include <QString>

#if defined MAKE_SEVENZIPPER_LIB
    #define SEVENZIPPER_LIB_EXPORT Q_DECL_EXPORT
#else
    #define SEVENZIPPER_LIB_EXPORT Q_DECL_IMPORT
#endif

namespace SevenZipper {

class SEVENZIPPER_LIB_EXPORT Library
{
public:
    explicit Library(const QString& libraryPath);
    virtual ~Library();

    bool isValid() const;
    bool createObject(const GUID* clsID, const GUID* interfaceID, void** out) const;

private:
    typedef UINT32 (WINAPI* CreateObjectFunc)(const GUID* clsID, const GUID* interfaceID, void** out);

    QString m_libraryPath;
    HMODULE m_library = nullptr;
    CreateObjectFunc m_createObjectFunc = nullptr;
};

} // namespace SevenZipper

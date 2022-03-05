#include "library.h"

namespace SevenZipper {

Library::Library(const QString& libraryPath) :
    m_libraryPath(libraryPath)
{
    m_library = LoadLibrary(reinterpret_cast<LPCWSTR>(m_libraryPath.utf16()));

    if (m_library != nullptr)
    {
        m_createObjectFunc = reinterpret_cast<CreateObjectFunc>(GetProcAddress(m_library, "CreateObject"));
    }
}

Library::~Library()
{
    FreeLibrary(m_library);
}

bool Library::isValid() const
{
    return m_library != nullptr && m_createObjectFunc != nullptr;
}

bool Library::createObject(const GUID* clsID, const GUID* interfaceID, void** out) const
{
    if (!isValid())
    {
        return false;
    }

    return m_createObjectFunc(clsID, interfaceID, out) == S_OK;
}

} // namespace SevenZipper

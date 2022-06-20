#pragma once

#include <functional>

#include "Windows.h"

#include <QString>

#include "7-zip/CPP/Common/MyCom.h"
#include "7-zip/CPP/7zip/Archive/IArchive.h"
#include "7-zip/CPP/7zip/IPassword.h"

namespace SevenZipper {

class OpenCallback : public IArchiveOpenCallback,
                     public IArchiveOpenVolumeCallback,
                     public IArchiveOpenSetSubArchiveName,
                     public ICryptoGetTextPassword,
                     protected CMyUnknownImp
{
public:
    explicit OpenCallback(std::function<QString()> passwordGetter);

    MY_UNKNOWN_IMP3(IArchiveOpenVolumeCallback, IArchiveOpenSetSubArchiveName, ICryptoGetTextPassword)

    STDMETHOD(SetTotal)(const UInt64* files, const UInt64* bytes);
    STDMETHOD(SetCompleted)(const UInt64* files, const UInt64* bytes);
    STDMETHOD(GetProperty)(PROPID propID, PROPVARIANT* value);
    STDMETHOD(GetStream)(const wchar_t* name, IInStream** inStream);
    STDMETHOD(SetSubArchiveName)(const wchar_t* name);
    STDMETHOD(CryptoGetTextPassword)(BSTR* password);

    QString usedPassword() const;
    bool aborted() const;

private:
    QString m_usedPassword;
    std::function<QString()> m_passwordGetter;
    bool m_aborted = false;
};

} // namespace SevenZipper

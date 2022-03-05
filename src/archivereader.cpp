#include "archivereader.h"

#include <functional>

#include "7-zip/CPP/Common/MyCom.h"
#include "7-zip/CPP/Common/Common.h"
#include "7-zip/CPP/7zip/PropID.h"
#include "7-zip/CPP/7zip/Archive/IArchive.h"
#include "7-zip/CPP/7zip/Common/FileStreams.h"
#include "7-zip/CPP/7zip/Common/StreamObjects.h"

#include "archiveformat.h"
#include "opencallback.h"
#include "library.h"

static QString propVariantToQString(const PROPVARIANT &var)
{
    return (var.bstrVal == nullptr) ? QString() : QString::fromWCharArray(var.bstrVal, SysStringLen(var.bstrVal));
}

static bool propVariantToBool(const PROPVARIANT &var)
{
    return var.boolVal == VARIANT_TRUE;
}

static uint32_t propVariantToUInt32(const PROPVARIANT &var)
{
    switch (var.vt)
    {
        case VT_UI1:
            return var.bVal;
        case VT_UI2:
            return var.uiVal;
        case VT_UINT:
            return var.uintVal;
        case VT_UI4:
            return var.ulVal;
        default:
            return {};
    }
}

static uint64_t propVariantToUInt64(const PROPVARIANT &var)
{
    switch (var.vt)
    {
        case VT_UI1:
            return var.bVal;
        case VT_UI2:
            return var.uiVal;
        case VT_UINT:
            return var.uintVal;
        case VT_UI4:
            return var.ulVal;
        case VT_UI8:
            return var.uhVal.QuadPart;
        default:
            return {};
    }
}

static qint64 propVariantToTimestamp(const PROPVARIANT &var)
{
    if (var.vt != VT_FILETIME)
    {
        return -1;
    }
    qint64 timestamp = var.filetime.dwHighDateTime;
    timestamp <<= 32;
    timestamp += var.filetime.dwLowDateTime;
    return timestamp;
}

namespace SevenZipper {

ArchiveReader::ArchiveReader(Library &library, const QString &archiveFile, std::function<QString()> passwordGetter) :
    m_library(library),
    m_archiveFile(archiveFile),
    m_passwordGetter(passwordGetter)
{
}

ArchiveReader::Result ArchiveReader::load()
{
    m_items.clear();

    auto fileStreamSpec = new CInFileStream();
    CMyComPtr<IInStream> file_stream = fileStreamSpec;
    if (!fileStreamSpec->Open(reinterpret_cast<LPCWSTR>(m_archiveFile.utf16())))
    {
        return Result::Error;
    }

    unsigned char format = ArchiveFormat::detectFromFile(m_archiveFile);
    const GUID format_GUID = { 0x23170F69, 0x40C1, 0x278A, { 0x10, 0x00, 0x00, 0x01, 0x10, format, 0x00, 0x00 } };

    CMyComPtr<IInArchive> inArchive;
    const GUID IID_IInArchive = {0x23170F69, 0x40C1, 0x278A, {0x00, 0x00, 0x00, 0x06, 0x00, 0x60, 0x00, 0x00}};
    if (!m_library.createObject(&format_GUID, &IID_IInArchive, reinterpret_cast<void**>(&inArchive)))
    {
        return Result::Error;
    }

    auto openCallbackSpec = new OpenCallback(m_passwordGetter);
    CMyComPtr<IArchiveOpenCallback> openCallback = openCallbackSpec;

    HRESULT res = inArchive->Open(file_stream, nullptr, openCallback);
    if (res != S_OK)
    {
        if (openCallbackSpec->aborted())
        {
            return Result::Aborted;
        }
        return Result::Error;
    }

    uint32_t items_count;
    res = inArchive->GetNumberOfItems(&items_count);
    if (res != S_OK)
    {
        return Result::Error;
    }

    m_items.reserve(items_count);

    for (uint32_t index = 0; index < items_count; ++index)
    {
        ArchiveItem item;

        {
            PROPVARIANT var = {};
            HRESULT res = inArchive->GetProperty(index, static_cast<PROPID>(kpidPath), &var);
            if (res == S_OK)
            {
                item.path = propVariantToQString(var);
                item.path.replace(QLatin1Char('\\'), QLatin1Char('/'));
            }
        }

        {
            PROPVARIANT var = {};
            HRESULT res = inArchive->GetProperty(index, static_cast<PROPID>(kpidIsDir), &var);
            if (res == S_OK)
            {
                if (propVariantToBool(var) && !item.path.endsWith(QLatin1Char('/')))
                {
                    item.path.append(QLatin1Char('/'));
                }
            }
        }

        {
            PROPVARIANT var = {};
            HRESULT res = inArchive->GetProperty(index, static_cast<PROPID>(kpidSize), &var);
            if (res == S_OK)
            {
                item.size = propVariantToUInt64(var);
            }
        }

        {
            PROPVARIANT var = {};
            HRESULT res = inArchive->GetProperty(index, static_cast<PROPID>(kpidAttrib), &var);
            if (res == S_OK)
            {
                item.attributes = propVariantToUInt32(var);
            }
        }

        {
            PROPVARIANT var = {};
            HRESULT res = inArchive->GetProperty(index, static_cast<PROPID>(kpidMTime), &var);
            if (res == S_OK)
            {
                item.timestamp = propVariantToTimestamp(var);
            }
        }

        m_items.append(item);
    }

    m_usedPassword = openCallbackSpec->usedPassword();
    return Result::Success;
}

const QVector<ArchiveItem> &ArchiveReader::items() const
{
    return m_items;
}

const QString &ArchiveReader::usedPassword() const
{
    return m_usedPassword;
}

} // namespace SevenZipper

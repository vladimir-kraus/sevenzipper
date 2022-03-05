#include "opencallback.h"

#include "7-zip/CPP/Common/Common.h"

namespace SevenZipper {

OpenCallback::OpenCallback(std::function<QString()> passwordGetter) :
    m_passwordGetter(passwordGetter)
{ }

STDMETHODIMP OpenCallback::SetTotal(const UInt64* /*files*/, const UInt64* /*bytes*/)
{
    return S_OK;
}

STDMETHODIMP OpenCallback::SetCompleted(const UInt64* /*files*/, const UInt64* /*bytes*/)
{
    return S_OK;
}

STDMETHODIMP OpenCallback::GetProperty(PROPID /*propID*/, PROPVARIANT* /*value*/)
{
    return S_OK;
}

STDMETHODIMP OpenCallback::GetStream(const wchar_t* /*name*/, IInStream** /*inStream*/)
{
    return S_OK;
}

STDMETHODIMP OpenCallback::SetSubArchiveName(const wchar_t* /*name*/)
{
    return S_OK;
}

STDMETHODIMP OpenCallback::CryptoGetTextPassword(BSTR* password)
{
    if (m_usedPassword.isEmpty())
    {
        m_usedPassword = m_passwordGetter();

        if (m_usedPassword.isEmpty())
        {
            m_aborted = true;
            return S_FALSE;
        }
    }

    return StringToBstr(LPCOLESTR(PCWSTR(m_usedPassword.utf16())), password);
}

QString OpenCallback::usedPassword() const
{
    return m_usedPassword;
}

bool OpenCallback::aborted() const
{
    return m_aborted;
}

} // namespace SevenZipper

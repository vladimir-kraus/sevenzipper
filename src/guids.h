#pragma once

#include <guiddef.h>

// IStream.h
extern "C" const GUID IID_ISequentialInStream;
extern "C" const GUID IID_ISequentialOutStream;
extern "C" const GUID IID_IInStream;
extern "C" const GUID IID_IOutStream;
extern "C" const GUID IID_IStreamGetSize;
extern "C" const GUID IID_IStreamGetProps;
extern "C" const GUID IID_IStreamGetProps2;

// ICoder.h
extern "C" const GUID IID_ICompressProgressInfo;

// IPassword.h
extern "C" const GUID IID_ICryptoGetTextPassword;
extern "C" const GUID IID_ICryptoGetTextPassword2;

// IArchive.h
extern "C" const GUID IID_ISetProperties;
extern "C" const GUID IID_IInArchive;
extern "C" const GUID IID_IOutArchive;
extern "C" const GUID IID_IArchiveExtractCallback;
extern "C" const GUID IID_IArchiveOpenVolumeCallback;
extern "C" const GUID IID_IArchiveOpenSetSubArchiveName;
extern "C" const GUID IID_IArchiveUpdateCallback;
extern "C" const GUID IID_IArchiveUpdateCallback2;

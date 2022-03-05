#pragma once

#include <QString>

namespace SevenZipper {

namespace ArchiveFormat {

unsigned char detectFromFile(const QString& archivePath);

} // namespace ArchiveFormat

} // namespace SevenZipper

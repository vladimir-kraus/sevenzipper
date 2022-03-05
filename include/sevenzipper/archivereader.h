#pragma once

#include <QString>
#include <QVector>

#if defined MAKE_SEVENZIPPER_LIB
    #define SEVENZIPPER_LIB_EXPORT Q_DECL_EXPORT
#else
    #define SEVENZIPPER_LIB_EXPORT Q_DECL_IMPORT
#endif

namespace SevenZipper {

class Library;

struct ArchiveItem
{
    /*
     * Path has universal separators. Dirs end with separator.
     */
    QString path;
    qint64 size = 0;
    qint64 timestamp = 0;
    uint64_t attributes = 0;
};

class SEVENZIPPER_LIB_EXPORT ArchiveReader
{
public:
    enum class Result { Success, Error, Aborted };

    explicit ArchiveReader(Library &library, const QString &archiveFile, std::function<QString()> passwordGetter);

    Result load();

    const QVector<ArchiveItem> &items() const;
    const QString &usedPassword() const;

private:
    Library &m_library;
    QString m_archiveFile;
    std::function<QString()> m_passwordGetter;
    QVector<ArchiveItem> m_items;
    QString m_usedPassword;
};

} // namespace SevenZipper

Q_DECLARE_TYPEINFO(SevenZipper::ArchiveItem, Q_MOVABLE_TYPE);

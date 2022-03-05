#include "archiveformat.h"

#include "Windows.h"

#include <QHash>

#include "7-zip/CPP/7zip/IStream.h"

namespace SevenZipper {

namespace ArchiveFormat {

enum ArchiveFormatValue : unsigned char {
    Unknown = 0x00,
    Zip = 0x01,
    BZip2 = 0x02,
    Rar = 0x03,
    Arj = 0x04,
    Z = 0x05,
    Lzh = 0x06,
    SevenZip = 0x07,
    Cab = 0x08,
    Nsis = 0x09,
    Lzma = 0x0A,
    Lzma86 = 0x0B,
    Xz = 0x0C,
    Ppmd = 0x0D,
    COFF = 0xC6,
    Ext = 0xC7,
    VMDK = 0xC8,
    VDI = 0xC9,
    QCow = 0xCA,
    GPT = 0xCB,
    Rar5 = 0xCC,
    IHex = 0xCD,
    Hxs = 0xCE,
    TE = 0xCF,
    UEFIc = 0xD0,
    UEFIs = 0xD1,
    SquashFS = 0xD2,
    CramFS = 0xD3,
    APM = 0xD4,
    Mslz = 0xD5,
    Flv = 0xD6,
    Swf = 0xD7,
    Swfc = 0xD8,
    Ntfs = 0xD9,
    Fat = 0xDA,
    Mbr = 0xDB,
    Vhd = 0xDC,
    Pe = 0xDD,
    Elf = 0xDE,
    Macho = 0xDF,
    Udf = 0xE0,
    Xar = 0xE1,
    Mub = 0xE2,
    Hfs = 0xE3,
    Dmg = 0xE4,
    Compound = 0xE5,
    Wim = 0xE6,
    Iso = 0xE7,
    Chm = 0xE9,
    Split = 0xEA,
    Rpm = 0xEB,
    Deb = 0xEC,
    Cpio = 0xED,
    Tar = 0xEE,
    GZip = 0xEF };

unsigned char detectFromFile(const QString &archivePath)
{
    static const QHash<QString, ArchiveFormatValue> lookup = {
                { QStringLiteral("7z"),       SevenZip },
                { QStringLiteral("bzip2"),    BZip2 },
                { QStringLiteral("bz2"),      BZip2 },
                { QStringLiteral("tbz2"),     BZip2 },
                { QStringLiteral("tbz"),      BZip2 },
                { QStringLiteral("gz"),       GZip },
                { QStringLiteral("gzip"),     GZip },
                { QStringLiteral("tgz"),      GZip },
                { QStringLiteral("tar"),      Tar },
                { QStringLiteral("wim"),      Wim },
                { QStringLiteral("swm"),      Wim },
                { QStringLiteral("xz"),       Xz },
                { QStringLiteral("txz"),      Xz },
                { QStringLiteral("zip"),      Zip },
                { QStringLiteral("zipx"),     Zip },
                { QStringLiteral("jar"),      Zip },
                { QStringLiteral("xpi"),      Zip },
                { QStringLiteral("odt"),      Zip },
                { QStringLiteral("ods"),      Zip },
                { QStringLiteral("odp"),      Zip },
                { QStringLiteral("docx"),     Zip },
                { QStringLiteral("xlsx"),     Zip },
                { QStringLiteral("pptx"),     Zip },
                { QStringLiteral("epub"),     Zip },
                { QStringLiteral("001"),      Split },
                { QStringLiteral("ar"),       Deb },
                { QStringLiteral("apm"),      APM },
                { QStringLiteral("arj"),      Arj },
                { QStringLiteral("cab"),      Cab },
                { QStringLiteral("chm"),      Chm },
                { QStringLiteral("chi"),      Chm },
                { QStringLiteral("msi"),      Compound },
                { QStringLiteral("doc"),      Compound },
                { QStringLiteral("xls"),      Compound },
                { QStringLiteral("ppt"),      Compound },
                { QStringLiteral("msg"),      Compound },
                { QStringLiteral("obj"),      COFF },
                { QStringLiteral("cpio"),     Cpio },
                { QStringLiteral("cramfs"),   CramFS },
                { QStringLiteral("deb"),      Deb },
                { QStringLiteral("dmg"),      Dmg },
                { QStringLiteral("dl"),      Pe },
                { QStringLiteral("dylib"),    Macho },
                { QStringLiteral("exe"),      Pe },
                { QStringLiteral("ext"),      Ext },
                { QStringLiteral("ext2"),     Ext },
                { QStringLiteral("ext3"),     Ext },
                { QStringLiteral("ext4"),     Ext },
                { QStringLiteral("fat"),      Fat },
                { QStringLiteral("flv"),      Flv },
                { QStringLiteral("gpt"),      GPT },
                { QStringLiteral("hfs"),      Hfs },
                { QStringLiteral("hfsx"),     Hfs },
                { QStringLiteral("hxs"),      Hxs },
                { QStringLiteral("ihex"),     IHex },
                { QStringLiteral("lzh"),      Lzh },
                { QStringLiteral("lha"),      Lzh },
                { QStringLiteral("lzma"),     Lzma },
                { QStringLiteral("lzma86"),   Lzma86 },
                { QStringLiteral("mbr"),      Mbr },
                { QStringLiteral("mslz"),     Mslz },
                { QStringLiteral("mub"),      Mub },
                { QStringLiteral("nsis"),     Nsis },
                { QStringLiteral("ntfs"),     Ntfs },
                { QStringLiteral("pmd"),      Ppmd },
                { QStringLiteral("qcow"),     QCow },
                { QStringLiteral("qcow2"),    QCow },
                { QStringLiteral("qcow2c"),   QCow },
                { QStringLiteral("rpm"),      Rpm },
                { QStringLiteral("squashfs"), SquashFS },
                { QStringLiteral("te"),       TE },
                { QStringLiteral("udf"),      Udf },
                { QStringLiteral("scap"),     UEFIc },
                { QStringLiteral("uefif"),    UEFIs },
                { QStringLiteral("vmdk"),     VMDK },
                { QStringLiteral("vdi"),      VDI },
                { QStringLiteral("vhd"),      Vhd },
                { QStringLiteral("xar"),      Xar },
                { QStringLiteral("pkg"),      Xar },
                { QStringLiteral("z"),        Z },
                { QStringLiteral("taz"),      Z }
            };

    QString extension;
    int k = archivePath.lastIndexOf(QLatin1Char('.'));

    if (k != -1)
    {
        extension = archivePath.mid(k + 1);
    }

    if (extension.isEmpty())
    {
        return Unknown;
    }

    extension = extension.toLower();

    auto it = lookup.find(extension);
    if (it != lookup.end())
    {
        return it.value();
    }

    // Detecting multivolume archives extension
    if ((extension.at(0) == QLatin1Char('r') || extension.at(0) == QLatin1Char('z')) &&
        (extension.size() == 3 && extension.at(1).isDigit() && extension.at(2).isDigit()))
    {
        // Extension follows the format zXX or rXX, where X is a number in range [0-9]
        return extension.at(0) == QLatin1Char('r') ? Rar : Zip;
    }

    // Note: iso, img and ima extensions can be associated with different formats -> detect by signature
    // TODO

    return Unknown;
}

} // namespace ArchiveFormat

} // namespace SevenZipper

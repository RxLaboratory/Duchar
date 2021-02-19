#ifndef APPVERSION_H
#define APPVERSION_H

#define VERSION_MAJOR 0
#define VERSION_MINOR 1
#define VERSION_BUILD 0
#define VERSION_SUFFIX "Beta"

#define STRINGIFY_VERSION(A, B, C) CONCAT(A, B, C )
#define CONCAT(A, B, C ) STRINGIFY( A##.##B##.##C )
#define STRINGIFY(A) #A

#ifdef QT_DEBUG
#define STR_VERSION STRINGIFY_VERSION(VERSION_MAJOR, VERSION_MINOR, VERSION_BUILD) "-" VERSION_SUFFIX "_development build"
#else
#define STR_VERSION STRINGIFY_VERSION(VERSION_MAJOR, VERSION_MINOR, VERSION_BUILD) "-" VERSION_SUFFIX
#endif

#define VER_FILEVERSION VERSION_MAJOR,VERSION_MINOR,VERSION_BUILD,0
#define STR_FILEVERSION STR_VERSION

// Keep the product version as fixed
#define VER_PRODUCTVERSION VERSION_MAJOR,VERSION_MINOR,VERSION_BUILD,0
#define STR_PRODUCTVERSION STR_VERSION

#define STR_COMPANYNAME "RxLaboratory"
#define STR_FILEDESCRIPTION "DuChar"
#define STR_INTERNALNAME "DuChar"
#define STR_LEGALCOPYRIGHT "Copyright (c) 2021 RxLaboratory, Nicolas Dufresne and contributors"
#define STR_LEGALTRADEMARKS1 "All Rights Reserved"
#define STR_ORIGINALFILENAME "DuChar.exe"
#define STR_PRODUCTNAME "DuChar - Character copier"
#define STR_PRODUCTDESCRIPTION "Clipboard history and favorites"

#define STR_COMPANYDOMAIN "rainboxlab.org"

#define URL_CHAT "http://chat.rainboxlab.org"
#define URL_FORUM "https://forum.rainboxlab.org"
#define URL_DOC ""
#define URL_BUGREPORT ""

#define APP_ICON ":/icons/app"
#define SPLASH_IMAGE ":/images/splash"

#endif // APPVERSION_H

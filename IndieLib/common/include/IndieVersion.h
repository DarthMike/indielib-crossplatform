/*****************************************************************************************
 * File: IndieVersion.h
 * Desc: Version definition
 *****************************************************************************************/

#ifndef Indielib_IndieVersion_h
#define Indielib_IndieVersion_h

/// Version numbering scheme
struct Indie_version{
    unsigned int major;     ///< Significant changes
    unsigned int minor;     ///< Incremental changes
    unsigned int revision;  ///< Bug fixes
};

/// Current version
extern Indie_version IND_VERSION;

#endif

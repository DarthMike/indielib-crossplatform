/*****************************************************************************************
 * File: IndieVersion.h
 * Desc: Version definition
 *****************************************************************************************/

/*
 Copyright: Miguel Angel QuiÒones (2011) (mail:m.quinones.garcia@gmail.com / mikeskywalker007@gmail.com)
 Comments : You are free to use as you want... but it can destroy your computer, so dont blame me about it ;)
 Nevertheless it would be nice if you tell me you are using something I made, just for curiosity
 */


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

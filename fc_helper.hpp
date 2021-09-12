#ifndef FC_HELPER_HPP
#define FC_HELPER_HPP

// helper library for linux font filename search

void fontConfigGetFontFilename(
    char return_buffer[],
    const unsigned int BUFFER_SIZE,
    const char* font_name_search);


#include <string>

// C++ STL version
std::string fontConfigGetFontFilename(
    const char* font_name_search);




#endif // FC_HELPER
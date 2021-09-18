#include "fc_helper.hpp"



#include <fontconfig/fontconfig.h>


#include <cstdio>

#include <stdexcept>

#include <iostream>


void fontConfigGetFontFilename(
    char return_buffer[],
    const unsigned int BUFFER_SIZE,
    const char* font_name_search)
{

    // font_name_search: example "Mono"

    //const unsigned short BUFFER_SIZE = 4096;
    //char return_buffer[BUFFER_SIZE];

    FcInit();
	FcConfig* config = FcInitLoadConfigAndFonts();
	FcPattern* pat = FcNameParse((const FcChar8*)font_name_search);
	FcConfigSubstitute(config, pat, FcMatchPattern);
	FcDefaultSubstitute(pat);

	char* fontFile;
	FcResult result;

	FcPattern* font = FcFontMatch(config, pat, &result);

	if(font)
	{
		FcChar8* file = nullptr; 

		if(FcPatternGetString(font, FC_FILE, 0, &file) == FcResultMatch)
		{
			fontFile = (char*)file;
            snprintf(return_buffer, BUFFER_SIZE, "%s", fontFile);
		}
	}
	FcPatternDestroy(font);

	FcPatternDestroy(pat);
	FcConfigDestroy(config);
	FcFini();

    //return &return_buffer[0];
}


// TODO: could write a version of this function
// which takes a std::string, however at the moment
// there is no need for such a thing from the currently
// existing code



// C++ STL version (other verion)
// TODO: pass font_name_search by value?
std::string fontConfigGetFontFilename(
    std::string font_name_search)
{
	std::cout << __func__ << ": " << font_name_search << std::endl;
	return fontConfigGetFontFilename(font_name_search.c_str());
}


// C++ STL version
// this function is the native one, because of:
// (const FcChar8*)font_name_search cast
std::string fontConfigGetFontFilename(
    const char* font_name_search)
{
	std::cout << __func__ << ": " << font_name_search << std::endl;

	std::string ret;

    // font_name_search: example "Mono"

    //const unsigned short BUFFER_SIZE = 4096;
    //char return_buffer[BUFFER_SIZE];

    FcInit();
	FcConfig* config = FcInitLoadConfigAndFonts();
	FcPattern* pat = FcNameParse((const FcChar8*)font_name_search);
	FcBool success = FcConfigSubstitute(config, pat, FcMatchPattern);
	/*if(success == FcTrue)
	{

	}
	else*/
	if(success == FcFalse)
	{
		throw std::runtime_error(
			"Error: fontConfigGetFontFilename: FcConfigSubstitute failed");
	}
	else
	{

	}
	FcDefaultSubstitute(pat);

	char* fontFile;
	FcResult result;

	FcPattern* font = FcFontMatch(config, pat, &result);

	if(font)
	{
		FcChar8* file = nullptr; 

		if(FcPatternGetString(font, FC_FILE, 0, &file) == FcResultMatch)
		{
			fontFile = (char*)file;
			const int BUFFER_SIZE = 4096;
			char buffer[BUFFER_SIZE];
            int n = snprintf(buffer, BUFFER_SIZE, "%s", fontFile);

			if(n < 0)
			{
				throw std::runtime_error(
					"Error: fontConfigGetFontFilename: snprintf encoding error");
			}
			else if(n >= BUFFER_SIZE)
			{
				//std::cout << "Overflow!" << std::endl;
				// overflow (static size buffer)
				// dynamically allocate storage and re-try

				// overflow
				const int d_buffer_size = n + 1;
				char *d_buffer = new char[d_buffer_size];

				int nn = snprintf(d_buffer, d_buffer_size, "%s", fontFile);
				// negative value = encoding error
				// otherwise, number of bytes written
				// if n < d_buffer_size, then success
				// else error

				if(nn < 0)
				{
					throw std::runtime_error(
						"Error: fontConfigGetFontFilename: snprintf encoding error");
				}
				else if(nn >= d_buffer_size)
				{
					throw std::runtime_error(
						"Error: fontConfigGetFontFilename: buffer overflow");
				}
				else
				{
					std::cout << d_buffer << std::endl;
					ret = std::string(d_buffer);
				}

				delete [] d_buffer;
				d_buffer = nullptr;
			}
			else
			{
				std::cout << buffer << std::endl;
				ret = std::string(buffer);
			}
		}
		else
		{
			std::cout << "Error: font not matched? (2)" << std::endl;
		}
	}
	else
	{
		std::cout << "Error: font not matched? (1)" << std::endl;
	}
	FcPatternDestroy(font);

	FcPatternDestroy(pat);
	FcConfigDestroy(config);
	FcFini();

	std::cout << "return: " << ret << std::endl;
    return ret;
}
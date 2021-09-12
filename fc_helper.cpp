#include "fc_helper.hpp"



#include <fontconfig/fontconfig.h>


#include <cstdio>

#include <stdexcept>


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



// C++ STL version
std::string fontConfigGetFontFilename(
    const char* font_name_search)
{

	std::string ret;

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
			const unsigned int BUFFER_SIZE = 4096;
			char buffer[BUFFER_SIZE];
            int n = snprintf(buffer, BUFFER_SIZE, "%s", fontFile);

			if(n >= BUFFER_SIZE)
			{
				// overflow
				const int d_buffer_size = n + 1;
				char *d_buffer = new char[d_buffer_size];

				int nn = snprintf(d_buffer, d_buffer_size, "%s", fontFile);
				if(nn >= d_buffer_size)
				{
					throw std::runtime_error("Error: fontConfigGetFontFilename()");
				}
				else
				{
					ret = d_buffer;
				}

				delete [] d_buffer;
				d_buffer = nullptr;
			}
			else
			{
				ret = buffer;
			}
		}
	}
	FcPatternDestroy(font);

	FcPatternDestroy(pat);
	FcConfigDestroy(config);
	FcFini();

    return ret;
}
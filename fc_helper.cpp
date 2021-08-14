#include "fc_helper.hpp"


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
		FcChar8* file = NULL; 

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
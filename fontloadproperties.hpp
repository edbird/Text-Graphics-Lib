#ifndef FONTLOADPROPERTIES_HPP
#define FONTLOADPROPERTIES_HPP

#include <string>

class FontLoadProperties
{

public:

    FontLoadProperties(const std::string& font_full_path, const int font_size)
        : font_full_path(font_full_path)
        , font_size(font_size)
    {
    }

    std::string GetUniqueKey() const
    {
        return font_full_path + std::to_string(font_size);
    }

private:

    int font_size;
    std::string font_full_path;

};

#endif // FONTLOADPROPERTIES_HPP

#ifndef EXCEPTIONTYPES_HPP
#define EXCEPTIONTYPES_HPP


#include <exception>
#include <string>



class ExceptionBase : public std::exception
{

    public:

    ExceptionBase(const std::string &what)
        : m_what(what)
    {
    }

    virtual
    ~ExceptionBase()
    {

    }

    virtual
    const char* what() const noexcept
    {
        return m_what.c_str();
    }



    private:

    std::string m_what;

};


class SDLLibException : public ExceptionBase
{

    public:

    SDLLibException(const std::string &what)
        : ExceptionBase(what)
    {

    }

    virtual
    ~SDLLibException()
    {

    }

};


class TTFLibException : public ExceptionBase
{

    public:

    TTFLibException(const std::string &what)
        : ExceptionBase(what)
    {

    }

    virtual
    ~TTFLibException()
    {

    }

};


#endif // EXCEPTIONTYPES_HPP
#ifndef ERRORTYPES_HPP
#define ERRORTYPES_HPP


#include <exception>
#include <string>



class ErrorBase
{

    public:

    ErrorBase(const std::string &what)
        : m_what(what)
    {
    }

    virtual
    ~ErrorBase()
    {

    }

    std::string what() const
    {
        return m_what;
    }



    private:

    std::string m_what;

};


class SDLLibError : public ErrorBase
{

    public:

    SDLLibError(const std::string &what)
        : ErrorBase(what)
    {

    }

    virtual
    ~SDLLibError()
    {

    }

};


class TTFLibError : public ErrorBase
{

    public:

    TTFLibError(const std::string &what)
        : ErrorBase(what)
    {

    }

    virtual
    ~TTFLibError()
    {

    }

};


#endif // ERRORTYPES_HPP
#ifndef ERRORTYPES_HPP
#define ERRORTYPES_HPP



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

    virtual
    ~SDLLibError()
    {

    }

};


class TTFLibError : public ErrorBase
{

    public:

    virtual
    ~SDLLibError()
    {

    }

};


#endif // ERRORTYPES_HPP
#ifndef CPPARGLIB_H
#define CPPARGLIB_H

class cpparglib
{
private:
    char **argv;
    int argc;
public:
    ~cpparglib();
    cpparglib(int argcA, char *argvA[]);
    char *parseParameterForValue(char *ParameterName);
    bool parseParameter(char *ParameterName);
};

#endif // CPPARGLIB_H

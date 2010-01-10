#include "extrect.h"
#include "string.h"

ExtRect::ExtRect()
{
}
ofstream& ExtRect::operator>>(ofstream& os)
{
    char byteArray[16];
    int tmp = this->left();
    memcpy(byteArray, &tmp, 4);

    tmp = this->right();
    memcpy(byteArray+4, &tmp, 4);

    tmp =this->top();
    memcpy(byteArray+8, &tmp, 4);

    tmp = this->bottom();
    memcpy(byteArray+12, &tmp, 4);

    for(int i = 0 ; i < 16 ; i++ )
        os << byteArray[i];

    return os;
}
ifstream& ExtRect::operator<<(ifstream& is)
{
    char bytearray[16];
    int tmp;

    for(int i = 0 ; i < 16 ; i++ )
        is >> bytearray[i];

    memcpy(&tmp, bytearray, 4);
    this->setLeft(tmp);

    memcpy(&tmp, bytearray+4, 4);
    this->setRight(tmp);

    memcpy(&tmp, bytearray+8, 4);
    this->setTop(tmp);

    memcpy(&tmp, bytearray+12, 4);
    this->setBottom(tmp);
    return is;
}
/*
ExtRect * ExtRect::operator=(QRect *rect)
{
    return ExtRect();
}

QRect& ExtRect::operator=(ExtRect& extrect)
{
    return QRect();
}*/

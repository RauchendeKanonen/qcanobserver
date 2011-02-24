/*
    QCANObserver - A CAN Sniffer
    Copyright (C) <2009>  <Netseal>

    This program is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with this program.  If not, see <http://www.gnu.org/licenses/>.
*/
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
        os.put(byteArray[i]);

    return os;
}
ifstream& ExtRect::operator<<(ifstream& is)
{
    char bytearray[16];
    int tmp;

    for(int i = 0 ; i < 16 ; i++ )
        is.get(bytearray[i]);

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

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
#ifndef EXTRECT_H
#define EXTRECT_H

#include <QRect>
#include <iostream>
#include <fstream>

using namespace std;

class ExtRect : public QRect
{
public:
    ExtRect();
    ofstream& operator>>(ofstream& os);
    ifstream& operator<<(ifstream& is);
    ExtRect * operator= (QRect* other)
    {
        this->setBottom(other->bottom());
        this->setTop(other->top());
        this->setLeft(other->left());
        this->setRight(other->right());
        return this;
    }
    QRect* operator= (ExtRect* other)
    {
        QRect *rect = new QRect();
        rect->setBottom(other->bottom());
        rect->setTop(other->top());
        rect->setLeft(other->left());
        rect->setRight(other->right());
        return rect;
    }
};

#endif // EXTRECT_H

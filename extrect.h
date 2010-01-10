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

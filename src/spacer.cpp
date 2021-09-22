#include "spacer.h"

Spacer::Spacer(QSizePolicy::Policy hPolicy, QSizePolicy::Policy vPolicy, QWidget *parent)
    : QVBoxLayout(parent)
{
    addSpacerItem(new QSpacerItem(0,0,hPolicy,vPolicy));
}

Spacer::Spacer(short hPolicy, short vPolicy, QWidget *parent)
    : QVBoxLayout(parent)
{
    if (hPolicy == 1 && vPolicy == 1)
        addSpacerItem(new QSpacerItem(0, 0, QSizePolicy::Expanding, QSizePolicy::Expanding));
    else if (hPolicy == 1 && vPolicy == 0)
        addSpacerItem(new QSpacerItem(0, 0, QSizePolicy::Expanding, QSizePolicy::Minimum));
    else if (hPolicy == 0 && vPolicy == 1)
        addSpacerItem(new QSpacerItem(0, 0, QSizePolicy::Minimum, QSizePolicy::Expanding));
    else if (hPolicy == 0 && vPolicy == 0)
        addSpacerItem(new QSpacerItem(0, 0, QSizePolicy::Minimum, QSizePolicy::Minimum));
}

Spacer::~Spacer()
{
    delete this->spacerItem();
}

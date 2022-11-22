#ifndef _ENTITY_H_
#define _ENTITY_H_

#include "Object.h"
#include "Util.h"

class Entity : public Object
{
  protected:
    uint hp;

  public:
    virtual bool TakeDamage(uint damage) = 0;
};

#endif

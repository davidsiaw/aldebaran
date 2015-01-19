#ifndef VBO_INTERFACE_HPP
#define VBO_INTERFACE_HPP

#include <memory>
#include <vector>

#include "primitives.hpp"
#include "shader_interface.hpp"

class VboInterface
{
public:
    virtual Element* GetElements() const=0;
    virtual int GetElementCount() const=0;
    virtual GLenum GetBufferType() const=0;
    virtual bool Changed() const=0;             // tell the scene elements have changed
    virtual void ChangeCommitted()=0;           // scene acknowledges that elements have changed
};

#endif // VBO_INTERFACE_HPPs

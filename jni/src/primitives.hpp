//
//  primitives.hpp
//  aldebaran
//
//  Created by David Siaw on 2014/10/19.
//  Copyright (c) 2014年 David Siaw. All rights reserved.
//

#ifndef PRIMITIVES_HPP
#define PRIMITIVES_HPP

#include "opengl.hpp"

#pragma pack(push,1)
struct Vertex
{
    float x,y,z;
};

struct Color
{
    uint8_t a,r,g,b;
};

struct TexCoord
{
    float x,y;
};

struct Element
{
    Vertex v;
    Color c;
    TexCoord t;
    float tileOffsetW;	// The x animation offset of each tile
    float tileOffsetH;	// The y animation offset of each tile
    float numtiles;		// Number of tiles in texture
};

struct Triangle
{
    Element e[3];
};

struct Quad
{
    Element e[4];
};

#pragma pack(pop)

#endif // PRIMITIVES_HPP

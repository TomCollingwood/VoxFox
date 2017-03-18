/// \file Voxel.h
/// \brief Voxel  Defines the Voxel struct that is used by all the other classes.
///               I chose struct as I did not need any functions for the object Voxel
/// \author Thomas Collingwood
/// \version 1.0
/// \date 13/3/17 Updated to NCCA Coding standard
/// Revision History : See https://github.com/TomCollingwood/VoxFox

#ifndef VOXEL_H
#define VOXEL_H

typedef struct Voxel{
  float nx;
  float ny;
  float nz;
  float u;
  float v;
  float r;
  float g;
  float b;
  int index;

  Voxel():nx(0.0f),
    ny(0.0f),
    nz(0.0f),
    u(0.0f),
    v(0.0f),
    r(1.0f),
    g(0.0f),
    b(0.0f),
    index(0){}

  Voxel(float _u, float _v) : u(_u), v(_v){}
  Voxel(float _nx, float _ny, float _nz, float _u, float _v) : nx(_nx), ny(_ny), nz(_nz), u(_u), v(_v){}
  Voxel(float _r, float _g, float _b) : r(_r), g(_g), b(_b), u(-2.0f){}
}Voxel;

#endif // DATASTRUCTS_H

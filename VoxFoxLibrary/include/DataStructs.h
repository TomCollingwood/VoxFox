#ifndef DATASTRUCTS_H
#define DATASTRUCTS_H

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
    r(0.0f),
    g(0.0f),
    b(0.0f),
    index(0){}

  Voxel(float _u, float _v) : u(_u), v(_v){}

  Voxel(float _nx, float _ny, float _nz, float _u, float _v) : nx(_nx), ny(_ny), nz(_nz), u(_u), v(_v){}
}Voxel;

typedef struct PrimaryNodeIndex{
  int primIndex;
  bool initialized;

  PrimaryNodeIndex(int _index) :
    primIndex(_index),
    initialized(true){}

  PrimaryNodeIndex() :
    primIndex(0),
    initialized(false){}
}PrimaryNodeIndex;

typedef struct SecondaryNodeIndex{
  int primIndex;
  int secIndex;
  bool initialized;

  SecondaryNodeIndex(int _pindex, int _sindex) :
    primIndex(_pindex),
    secIndex(_sindex),
    initialized(true){}

  SecondaryNodeIndex() :
    primIndex(0),
    secIndex(0),
    initialized(false){}
}SecondaryNodeIndex;

typedef struct LeafNodeIndex{
  int primIndex;
  int secIndex;
  int leafIndex;
  bool initialized;

  LeafNodeIndex(int _pindex, int _sindex, int _lindex) :
    primIndex(_pindex),
    secIndex(_sindex),
    leafIndex(_lindex),
    initialized(true){}

  LeafNodeIndex() :
    primIndex(0),
    secIndex(0),
    leafIndex(0),
    initialized(false){}
}LeafNodeIndex;

#endif // DATASTRUCTS_H

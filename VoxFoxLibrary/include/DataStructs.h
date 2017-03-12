#ifndef DATASTRUCTS_H
#define DATASTRUCTS_H

struct Voxel{
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
};

struct PrimaryNodeIndex{
  int primIndex;
  bool initialized;

  PrimaryNodeIndex(int _index) :
    primIndex(_index),
    initialized(true){}

  PrimaryNodeIndex() :
    primIndex(0),
    initialized(false){}
};

struct SecondaryNodeIndex{
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
};

struct LeafNodeIndex{
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
};

#endif // DATASTRUCTS_H

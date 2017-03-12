#ifndef ROOTNODE_H
#define ROOTNODE_H

#include <iostream>
#include <vector>
#include <glm/glm.hpp>

#include <ngl/Vec3.h>
#include <ngl/Vec4.h>
#include <ngl/BBox.h>
#include <ngl/Obj.h>
#include <ngl/ShaderLib.h>
#include <cmath>

#include "LeafNode.h"
#include "PrimaryNode.h"
#include "DataStructs.h"

class RootNode
{
public:
  RootNode();

  void calculatePolys();

  void addVoxel(glm::vec3 _position, struct Voxel _data);
  void addVoxel(float _x, float _y, float _z) {addVoxel(glm::vec3(_x,_y,_z));}
  void addVoxel(glm::vec3 _position) {addVoxel(_position,Voxel());}

  bool isVoxel(glm::vec3 _position);
  //Voxel getVoxel(glm::vec3 _position);

  void printVertexes();

  //void loadVBO(ngl::ShaderLib* shader, constexpr auto shaderProgram, GLuint vbo, GLuint nbo, float * normals);

  // ACCESSORS
  float getVertexFloat(int i);
  int   getVertexSize();
  std::vector<float> getVertexes();
  std::vector<float> getNormals();
  int getSize();
  std::vector<PrimaryNode> getChildren() {return m_primChildren;}
  //std::vector<float> * getVertexes() {return m_vertexes; }

  // SHAPES
  void createSphere(glm::vec3 _position, int _radius);
  void createTorus(glm::vec3 _position, glm::vec2 _t);
  void createBox(ngl::Vec3 _min, ngl::Vec3 _max);

  // OBJ
  void importObj(ngl::Obj * _mesh);
  void importAccurateObj(ngl::Obj * _mesh);

  bool intersectBox(glm::vec3 _ray, glm::vec3 _origin, glm::vec3 _min, glm::vec3 _max);
  void fill();

  bool full;
  bool empty = true;

  void addVoxelLine(ngl::Vec3 p0, ngl::Vec3 p1, ngl::Vec3 n0, ngl::Vec3 n1, struct Voxel _voxel);

  const float m_primUnit = 10.0;
  const float m_secUnit = 1.25;
  const float m_leafUnit = 0.15625;
  const float m_voxUnit = 0.01953125;


private:
  glm::vec3 min, max;
  std::vector<LeafNode *> m_leafTable;
  std::vector<PrimaryNode> m_primChildren;
  struct LeafNodeIndex m_leafAccessor = LeafNodeIndex();
  struct SecondaryNodeIndex m_secAccessor = SecondaryNodeIndex();
  struct PrimaryNodeIndex m_primAccessor =PrimaryNodeIndex();

  int numberOfVoxels=0;
  std::vector<float> m_vertexes;
  std::vector<float> m_normals;
  std::vector<float> m_texturecoordinates;
};

#endif // ROOTNODE_H

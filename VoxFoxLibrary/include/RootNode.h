/// \file RootNode.h
/// \brief RootNode.h  Essentially the overarching class for the voxel data structure.
///                    There is only one RootNode for the tree and the rootnode is the tree.
/// \author Thomas Collingwood
/// \version 1.0
/// \date 13/3/17 Updated to NCCA Coding standard
/// Revision History : See https://github.com/TomCollingwood/VoxFox


#ifndef ROOTNODE_H
#define ROOTNODE_H

#include <iostream>
#include <vector>
#include "glm/glm.hpp"
#include <ngl/Vec3.h>
#include <ngl/Vec4.h>
#include <ngl/BBox.h>
#include <ngl/Obj.h>
#include <ngl/ShaderLib.h>
#include <cmath>

#include "LeafNode.h"
#include "PrimaryNode.h"

class RootNode
{
public:
  RootNode();


  void operator+=(RootNode const& r);

  //----------------------------------------------------------------------------------------------------------------------
  /// \brief addVoxel   adds Voxel at specified _position to RootNode
  /// \param _position  position in worldspace of the voxel
  /// \param _data      the information of the voxel to be added (see DataStructs.h)
  //----------------------------------------------------------------------------------------------------------------------
  void addVoxel(glm::vec3 const &_position, Voxel const &_data);

  //----------------------------------------------------------------------------------------------------------------------
  /// \brief isVoxel        checks if a Voxel is present within the RootNode at _position
  /// \param[in] _position  position in worldspace of the voxel
  /// \return               if Voxel is present at _position
  //----------------------------------------------------------------------------------------------------------------------
  bool isVoxel(glm::vec3 const &_position) ;

  //----------------------------------------------------------------------------------------------------------------------
  /// \brief calculatePolys This function calculates m_vertexes, m_normals and m_texturecoords for OpenGL use from VoxelData.
  //----------------------------------------------------------------------------------------------------------------------
  void calculatePolys();

  //----------------------------------------------------------------------------------------------------------------------
  /// \brief getVertexSize  returns how many vertexes there are in m_vertexes
  /// \return               m_vertexes.size()
  //----------------------------------------------------------------------------------------------------------------------
  int getVertexSize();

  // Accessors
  std::vector<float> getVertexes();
  std::vector<float> getNormals();
  std::vector<float> getTextureCoords();
  std::vector<PrimaryNode *> * getChildren() {return &m_primChildren;}

  // SHAPES
  //----------------------------------------------------------------------------------------------------------------------
  /// \brief createSphere   Creates a sphere of variable radius at a location.
  /// \param[in] _position      Centre point of the sphere to create
  /// \param[in] _radius        The radius of the sphere to create
  //----------------------------------------------------------------------------------------------------------------------
  void createSphere(glm::vec3 const &_position, int const &_radius);

  //----------------------------------------------------------------------------------------------------------------------
  /// \brief createTorus    Creates a torus / ring of variable radii
  /// \param[in] _position  Centre point of the torus to create
  /// \param[in] _t         The two radius (inner and outter) of the torus
  //----------------------------------------------------------------------------------------------------------------------
  void createTorus(glm::vec3 const &_position, glm::vec2 const &_t);

  //----------------------------------------------------------------------------------------------------------------------
  /// \brief createBox  Creates a box
  /// \param[in] _min   minimum point of box
  /// \param[in] _max   minimum point of box
  //----------------------------------------------------------------------------------------------------------------------
  void createBox(ngl::Vec3 const &_min, ngl::Vec3 const &_max);

  // OBJ
  //----------------------------------------------------------------------------------------------------------------------
  /// \brief importQuickObj Quickly imports an polygonal .obj file to the RootNode as voxel data
  ///                       It is done by simply drawing in the bounding boxes of the polygons
  /// \param[in] _mesh      The ngl::Obj object of the mesh
  //----------------------------------------------------------------------------------------------------------------------
  void importQuickObj(ngl::Obj * _mesh);

  //----------------------------------------------------------------------------------------------------------------------
  /// \brief importAccurateObj  Slowly imports a polygonal .obj file to the RootNode as voxel data
  ///                           See readme for how it is done
  /// \param[in] _mesh          The ngl::Obj object of the mesh
  /// \param[in] scale          If you would like to scale the resulting voxels if the mesh is too small
  //----------------------------------------------------------------------------------------------------------------------
  void importAccurateObj(ngl::Obj * _mesh, float const &scale);


  bool intersectBox(glm::vec3 _ray, glm::vec3 _origin, glm::vec3 _min, glm::vec3 _max);


  void fill();


  bool full;
  bool empty = true;

  void addVoxelLine(ngl::Vec3 p0, ngl::Vec3 p1, ngl::Vec3 n0, ngl::Vec3 n1, Voxel _voxel);

  const float m_primUnit = 10.0;
  const float m_secUnit = 1.25;
  const float m_leafUnit = 0.15625;
  const float m_voxUnit = 0.01953125;
  std::vector<PrimaryNode *> m_primChildren;

private:
  glm::vec3 min, max;

  const int depth = 3;
  std::vector<LeafNode *> m_leafTable;

  LeafNode * m_leafAccessor =nullptr;
  SecondaryNode * m_secAccessor =nullptr;
  PrimaryNode * m_primAccessor =nullptr;
  int numberOfVoxels=0;
  std::vector<float> m_vertexes;
  std::vector<float> m_normals;
  std::vector<float> m_textureCoords;
};

#endif // ROOTNODE_H

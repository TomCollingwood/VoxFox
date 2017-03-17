/// \file VoxFoxTree.h
/// \brief VoxFoxTree.h  Essentially the overarching class for the voxel data structure.
///                    There is only one VoxFoxTree for the tree and the VoxFoxTree is the tree.
/// \author Thomas Collingwood
/// \version 1.0
/// \date 13/3/17 Updated to NCCA Coding standard
/// Revision History : See https://github.com/TomCollingwood/VoxFox


#ifndef VoxFoxTree_H
#define VoxFoxTree_H

#include <iostream>
#include <vector>
#include <queue>
#include <glm/glm.hpp>
#include <ngl/Vec3.h>
#include <ngl/Vec4.h>
#include <ngl/BBox.h>
#include <ngl/Obj.h>
#include <ngl/ShaderLib.h>
#include <ngl/BBox.h>
#include <cmath>

#include "LeafNode.h"
#include "PrimaryNode.h"

class VoxFoxTree
{
public:
  VoxFoxTree();
  ~VoxFoxTree();
  VoxFoxTree(const VoxFoxTree & _r);
  VoxFoxTree & operator=(const VoxFoxTree & _r);

  //----------------------------------------------------------------------------------------------------------------------
  /// \brief operator +   Union operator that takes two VoxFoxTrees and gives VoxFoxTree of union of both of them
  /// \param[in] _r       RHS of operator
  /// \return             Union of RHS and LHS VoxFoxTree
  //----------------------------------------------------------------------------------------------------------------------
  VoxFoxTree operator|(VoxFoxTree _r) ;
  VoxFoxTree operator-(VoxFoxTree _r) ;

  // Same as &
  VoxFoxTree operator+(VoxFoxTree _r);

  //----------------------------------------------------------------------------------------------------------------------
  /// \brief translate          Translates all voxels in the VoxFoxTree by the worldspace input
  /// \param[in] _translation   amount to translate the VoxFoxTree
  //----------------------------------------------------------------------------------------------------------------------
  void translate(glm::vec3 const &_translation);

  //----------------------------------------------------------------------------------------------------------------------
  /// \brief addVoxel       adds Voxel at specified _position to VoxFoxTree
  /// \param[in] _position  position in worldspace of the voxel
  /// \param[in] _data      the information of the voxel to be added (see DataStructs.h)
  //----------------------------------------------------------------------------------------------------------------------
  void addVoxel(glm::vec3 const &_position, Voxel const &_data);

  //----------------------------------------------------------------------------------------------------------------------
  /// \brief isVoxel        checks if a Voxel is present within the VoxFoxTree at _position
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
  std::vector<float> getColors();
  std::vector<PrimaryNode *> * getChildren() {return &m_primChildren;}

  // SHAPES
  //----------------------------------------------------------------------------------------------------------------------
  /// \brief createSphere   Creates a sphere of variable radius at a location.
  /// \param[in] _position  Centre point of the sphere to create
  /// \param[in] _radius    The radius of the sphere to create
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

  //----------------------------------------------------------------------------------------------------------------------
  /// \brief importQuickObj Quickly imports an polygonal .obj file to the VoxFoxTree as voxel data
  ///                       It is done by simply drawing in the bounding boxes of the polygons
  /// \param[in] _mesh      The ngl::Obj object of the mesh
  /// \param[in] _size
  //----------------------------------------------------------------------------------------------------------------------
  void importQuickObj(ngl::Obj * _mesh, float const &_size);

  //----------------------------------------------------------------------------------------------------------------------
  /// \brief importObjRGB   Sluggishly imports a polygonal .obj file to the VoxFoxTree as voxel data
  ///                       See readme for how it is done. It uses texture and UVs to fetch rgb from texture.
  /// \param[in] _mesh      The ngl::Obj object of the mesh
  /// \param[in] _texture   The ngl::Texture for the mesh and to grab rgb values
  /// \param[in] _size      The height of the imported voxels in worldspace
  //----------------------------------------------------------------------------------------------------------------------
  void importObjRGB(ngl::Obj * _mesh, ngl::Image * _texture,float const &_size);

  //----------------------------------------------------------------------------------------------------------------------
  /// \brief importAccurateObj  Slowly imports a polygonal .obj file to the VoxFoxTree as voxel data
  ///                           See readme for how it is done. It imports UVs and does not fetch rgb from texture.
  /// \param[in] _mesh          The ngl::Obj object of the mesh
  /// \param[in] _size          The height of the imported voxels in worldspace
  //----------------------------------------------------------------------------------------------------------------------
  void importObjUV(ngl::Obj * _mesh, float const &size);

  // As the functions below also set the accessors we can use these as getters also by using accessors after calling them.
  bool isLeaf(glm::vec3 _position);
  bool isSecondary(glm::vec3 _position);
  bool isPrimary(glm::vec3 _position);

  // only adds if not present
  void addLeaf(LeafNode newLeaf);
  void addSecondary(SecondaryNode newSecondary);

  LeafNode * getLeaf(glm::vec3 _position);
  SecondaryNode * getSecondary(glm::vec3 _position);
  PrimaryNode * getPrimary(glm::vec3 _position);

  bool intersectBox(glm::vec3 _ray, glm::vec3 _origin, glm::vec3 _min, glm::vec3 _max);

  // never got working  :-(
  //void fill(VoxFoxTree *_r);

  void addVoxelLine(ngl::Vec3 p0, ngl::Vec3 p1, ngl::Vec3 n0, ngl::Vec3 n1, Voxel _voxel);

  float m_primUnit = 10.0;
  float m_secUnit = 1.25;
  float m_leafUnit = 0.15625;
  float m_voxUnit = 0.01953125;
  std::vector<PrimaryNode *> m_primChildren;

private:
  glm::vec3 min, max;
  // The accessors speed up addVoxel by up to four times on larger imports
  LeafNode * m_leafAccessor =nullptr;
  SecondaryNode * m_secAccessor =nullptr;
  PrimaryNode * m_primAccessor =nullptr;
  int numberOfVoxels=0;
  std::vector<float> m_vertexes;
  std::vector<float> m_normals;
  std::vector<float> m_textureCoords;
  std::vector<float> m_colors;
};

#endif // VoxFoxTree_H

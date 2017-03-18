/// \file PrimaryNode.h
/// \brief PrimaryNode.h  The highest node level that VoxFoxTree has.
///                       PrimaryNode contains the SecondaryNodes (up to 8^3)
/// \author Thomas Collingwood
/// \version 1.0
/// \date 13/3/17 Updated to NCCA Coding standard
/// Revision History : See https://github.com/TomCollingwood/VoxFox

#ifndef PRIMARYNODE_H
#define PRIMARYNODE_H

#include <iostream>
#include <vector>
#include "glm/glm.hpp"
#include "LeafNode.h"
#include "SecondaryNode.h"
class VoxFoxTree;

class PrimaryNode
{
public:
  PrimaryNode();
  ~PrimaryNode();
  PrimaryNode(glm::vec3 _origin);
  PrimaryNode(PrimaryNode const &p);
  PrimaryNode & operator=(PrimaryNode const &p);

  //----------------------------------------------------------------------------------------------------------------------
  /// \brief operator + Intersection set operator (retains LHS data)
  /// \param[in] _p     The other PrimaryNode to intersect with.
  /// \return           Intersected PrimaryNode that retains the LHS voxel data
  //----------------------------------------------------------------------------------------------------------------------
  PrimaryNode operator +(PrimaryNode const &_p) const;

  //----------------------------------------------------------------------------------------------------------------------
  /// \brief operator |   Union operator that takes two PrimaryNodes and gives PrimaryNode of union of both of them
  /// \param[in] _p       RHS of operator
  /// \return             Union of RHS and LHS PrimaryNode
  //----------------------------------------------------------------------------------------------------------------------
  PrimaryNode operator |(PrimaryNode const &_p) const;

  //----------------------------------------------------------------------------------------------------------------------
  /// \brief operator - Subtract set operator
  /// \param[in] _p     PrimaryNode to subtract
  /// \return           Subtracted PrimaryNode
  //----------------------------------------------------------------------------------------------------------------------
  PrimaryNode operator -(PrimaryNode const &_p) const;

  //----------------------------------------------------------------------------------------------------------------------
  /// \brief addVoxel               tries to add voxel to the PrimaryNode
  /// \param[in] _position          position in worldspace of voxel
  /// \param[in] _voxel             the information of the voxel to be added
  /// \param[inout] io_secAccessor  the secondary accessor to change (last accessed SecondaryNode)
  /// \param[inout] io_leafAccessor the leaf accessor to change (last accessed LeafNode)
  //----------------------------------------------------------------------------------------------------------------------
  void addVoxel(glm::vec3 _position, Voxel _voxel, SecondaryNode ** io_secAccessor, LeafNode ** io_leafAccessor);

  //----------------------------------------------------------------------------------------------------------------------
  /// \brief isVoxel                checks if a Voxel is present within PrimaryNode at _position
  /// \param[in] _position          position in worldspace of the voxel
  /// \param[inout] io_secAccessor  the secondary accessor to change (last accessed SecondaryNode)
  /// \param[inout] io_leafAccessor the leaf accessor to change (last accessed LeafNode)
  /// \return                       if Voxel is present at _position
  //----------------------------------------------------------------------------------------------------------------------
  bool isVoxel(glm::vec3 _position, SecondaryNode ** io_secAccessor, LeafNode ** io_leafAccessor) const;

  glm::vec3 getOrigin() const {  return m_origin; }

  bool isLeaf(glm::vec3 _position, SecondaryNode ** _sec, LeafNode ** _leaf) const;
  bool isSecondary(glm::vec3 _position, SecondaryNode ** _secondary) const;

  //----------------------------------------------------------------------------------------------------------------------
  /// \brief m_secChildren  The SecondaryNode children are stored here
  //----------------------------------------------------------------------------------------------------------------------
  std::vector<SecondaryNode *> m_secChildren;

  // Unique ID for the PrimaryNode within tree. Trees can have PrimaryNodes with same id.
  // Used for quick compare in set operators
  int idx, idy, idz;
private:
  glm::vec3 m_origin;
  float unitChildLength = 1.25;
  float m_primUnit = 10.0;
  float m_secUnit = 1.25;
  float m_leafUnit = 0.15625;
  float m_voxUnit = 0.01953125;
};

#endif // PRIMARYNODE_H

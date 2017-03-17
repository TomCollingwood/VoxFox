/// \file SecondaryNode.h
/// \brief SecondaryNode.h  Second level node that contains and is reponsible for the LeafNodes
/// \author Thomas Collingwood
/// \version 1.0
/// \date 13/3/17 Updated to NCCA Coding standard
/// Revision History : See https://github.com/TomCollingwood/VoxFox

#ifndef SECONDARYNODE_H
#define SECONDARYNODE_H

#include <iostream>
#include <vector>

#include "LeafNode.h"

class RootNode;
class PrimaryNode;

class SecondaryNode
{
public:
  SecondaryNode();
  ~SecondaryNode();
  SecondaryNode(glm::vec3 _origin);
  SecondaryNode(SecondaryNode const &s);

  SecondaryNode operator +(SecondaryNode const &_s) const;

  //----------------------------------------------------------------------------------------------------------------------
  /// \brief addVoxel               tries to add voxel to the SecondaryNode
  /// \param[in] _position          position in worldspace of the voxel
  /// \param[in] _voxel             the information of the voxel to be added
  /// \param[inout] io_leafAccessor the leaf accessor to change (last accessed LeafNode)
  /// \return                       if _position is within the SecondaryNode and is added
  ///                               (used for accessors in Root::addVoxel(..))
  //----------------------------------------------------------------------------------------------------------------------
  bool addVoxel(glm::vec3 _position, Voxel _voxel, LeafNode ** o_leafAccessor);

  //----------------------------------------------------------------------------------------------------------------------
  /// \brief isVoxel              checks if a Voxel is present within the SecondaryNode at _position
  /// \param[in] _position        position in worldspace of the voxel
  /// \param[inout] _leafAccessor the leaf accessor to change (last accessed LeafNode)
  /// \return                     if Voxel is present at _position
  //----------------------------------------------------------------------------------------------------------------------
  bool isVoxel(glm::vec3 const &_position, LeafNode ** _leafAccessor);

  //----------------------------------------------------------------------------------------------------------------------
  /// \brief getOrigin  returns the origin vector of node (minimum point in Node)
  /// \return           origin vector of node
  //----------------------------------------------------------------------------------------------------------------------
  glm::vec3 getOrigin() const {return m_origin; }

  //----------------------------------------------------------------------------------------------------------------------
  /// \brief isLeaf         Checks to see if the leaf at the _position is within m_leafChildren of the SecondaryNode
  /// \param[in] _position  The position which the LeafNode searched for occupies
  /// \param[out] o_leaf    If there is a leaf, the leaf is deposited in this double pointer (as a pointer).
  /// \return               True if leaf is present at _position
  //----------------------------------------------------------------------------------------------------------------------
  bool isLeaf(glm::vec3 _position, LeafNode ** o_leaf);

  void shiftOrigin(int _x, int _y, int _z);


  //----------------------------------------------------------------------------------------------------------------------
  /// \brief m_leafChildren vector of pointers to the LeafNode children
  //----------------------------------------------------------------------------------------------------------------------
  std::vector<LeafNode *> m_leafChildren;

  /// These ids taken together are unique for each SecondaryNode. Used when taking union of two nodes.
  int idx, idy, idz;

private:
  glm::vec3 m_origin;
  const float m_primUnit = 10.0;
  const float m_secUnit = 1.25;
  const float m_leafUnit = 0.15625;
  const float m_voxUnit = 0.01953125;

};

#endif // SECONDARYNODE_H

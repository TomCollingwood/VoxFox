#include "include/VoxFoxTree.h"

#define NEWOBIMPORT

VoxFoxTree::VoxFoxTree()
{
}

VoxFoxTree::VoxFoxTree(const VoxFoxTree &_r)
{
  for(auto& p : _r.m_primChildren)
  {
    m_primChildren.push_back(new PrimaryNode(*p));
  }
}

VoxFoxTree::~VoxFoxTree()
{
  for(auto& p : m_primChildren)
  {
    delete p;
  }
  m_primChildren.clear();
}

// Deep copy
VoxFoxTree & VoxFoxTree::operator =(const VoxFoxTree & _r)
{
  if(this!=&_r)
  {
    // DELETE OLD DATA
    for(auto& p : m_primChildren)
    {
      delete p;
    }
    m_primChildren.clear();
    m_vertexes.clear();
    m_normals.clear();
    m_textureCoords.clear();
    m_colors.clear();

    m_leafAccessor=nullptr;
    m_primAccessor=nullptr;
    m_secAccessor=nullptr;


    // INSERT NEW DATA
    for(auto& p : _r.m_primChildren)
    {
      m_primChildren.push_back(new PrimaryNode(*p));
    }

    m_vertexes=_r.m_vertexes;
    m_normals= _r.m_normals;
    m_textureCoords = _r.m_textureCoords;
    m_colors = _r.m_colors;
  }
  return *this;
}

VoxFoxTree VoxFoxTree::operator+(VoxFoxTree r)
{
  VoxFoxTree retRoot;
  std::vector<bool> jfound;
  for(int i =0 ; i<m_primChildren.size(); ++i)
  {
    jfound.push_back(false);
  }
  for(auto & i : r.m_primChildren)
  {
    bool found = false;
    for(int j =0; j<m_primChildren.size(); ++j)
    {
      if(i->idx==m_primChildren[j]->idx && i->idy==m_primChildren[j]->idy && i->idz==m_primChildren[j]->idz)
      {
        retRoot.m_primChildren.push_back(new PrimaryNode(*m_primChildren[j]+*i));
        jfound[j]=true;
        found = true;
        break;
      }
    }
    if(!found)
    {
      retRoot.m_primChildren.push_back(new PrimaryNode(*i));
    }
  }
  for(int i =0 ; i<jfound.size(); ++i)
  {
    if(!jfound[i]) retRoot.m_primChildren.push_back(new PrimaryNode(*m_primChildren[i]));
  }
  return retRoot;
}

std::vector<float> VoxFoxTree::getVertexes()
{
  return m_vertexes;
}


std::vector<float> VoxFoxTree::getNormals()
{
  return m_normals;
}

std::vector<float> VoxFoxTree::getTextureCoords()
{
  return m_textureCoords;
}

std::vector<float> VoxFoxTree::getColors()
{
  return m_colors;
}

int VoxFoxTree::getVertexSize()
{
  return m_vertexes.size();
}

bool VoxFoxTree::isVoxel(glm::vec3 const &_position)
{
  if(m_leafAccessor!=nullptr && m_leafAccessor->isVoxel(_position)) return true;
  else if ( m_secAccessor!=nullptr && m_secAccessor->isVoxel(_position,&m_leafAccessor)) return true;
//  else
//  {
    for (auto &prim : m_primChildren) // access by reference to avoid copying
        {
          if(_position[0]<prim->getOrigin()[0] || _position[0]>=prim->getOrigin()[0]+m_primUnit) continue;
          if(_position[1]<prim->getOrigin()[1] || _position[1]>=prim->getOrigin()[1]+m_primUnit) continue;
          if(_position[2]<prim->getOrigin()[2] || _position[2]>=prim->getOrigin()[2]+m_primUnit) continue;
          m_primAccessor=prim;
          return prim->isVoxel(_position,&m_secAccessor,&m_leafAccessor);
        }
    return false;
//  }
}

void VoxFoxTree::addVoxel(glm::vec3 const &_position, Voxel const &_data)
{
  if((m_leafAccessor==nullptr && m_secAccessor==nullptr) ||
     (m_leafAccessor!=nullptr && !m_leafAccessor->addVoxel(_position, _data) &&
     m_secAccessor!=nullptr && !m_secAccessor->addVoxel(_position,_data,&m_leafAccessor)))
  {
    bool found = false;
    for (auto &prim : m_primChildren) // access by reference to avoid copying
        {
          if(_position[0]<prim->getOrigin()[0] || _position[0]>=prim->getOrigin()[0]+m_primUnit) continue;
          if(_position[1]<prim->getOrigin()[1] || _position[1]>=prim->getOrigin()[1]+m_primUnit) continue;
          if(_position[2]<prim->getOrigin()[2] || _position[2]>=prim->getOrigin()[2]+m_primUnit) continue;
          m_primAccessor=prim;
          prim->addVoxel(_position,_data,&m_secAccessor,&m_leafAccessor);
          found = true;
          break;
        }
    if(!found)
    {
      glm::vec3 newOrigin = floor(_position/m_primUnit)*m_primUnit;
      m_primChildren.push_back(new PrimaryNode(newOrigin));
      m_primChildren.back()->addVoxel(_position,_data,&m_secAccessor,&m_leafAccessor);
      min=glm::vec3(glm::min(min.x,newOrigin.x),glm::min(min.y,newOrigin.y),glm::min(min.z,newOrigin.z));
      max=glm::vec3(glm::max(min.x,newOrigin.x+m_primUnit),glm::max(min.y,newOrigin.y+m_primUnit),glm::max(min.z,newOrigin.z+m_primUnit));
    }

}
  numberOfVoxels++;
}

bool VoxFoxTree::isLeaf(glm::vec3 _position)
{
  for (auto& prim: m_primChildren) // access by reference to avoid copying
  {
    if(_position[0]<prim->getOrigin()[0] || _position[0]>=prim->getOrigin()[0]+m_primUnit) continue;
    if(_position[1]<prim->getOrigin()[1] || _position[1]>=prim->getOrigin()[1]+m_primUnit) continue;
    if(_position[2]<prim->getOrigin()[2] || _position[2]>=prim->getOrigin()[2]+m_primUnit) continue;
    return prim->isLeaf(_position,&m_secAccessor,&m_leafAccessor);
  }
  return false;
}

bool VoxFoxTree::isSecondary(glm::vec3 _position)
{
  for (auto& prim: m_primChildren) // access by reference to avoid copying
  {
    if(_position[0]<prim->getOrigin()[0] || _position[0]>=prim->getOrigin()[0]+m_primUnit) continue;
    if(_position[1]<prim->getOrigin()[1] || _position[1]>=prim->getOrigin()[1]+m_primUnit) continue;
    if(_position[2]<prim->getOrigin()[2] || _position[2]>=prim->getOrigin()[2]+m_primUnit) continue;
    return prim->isSecondary(_position,&m_secAccessor);
  }
  return false;
}

bool VoxFoxTree::isPrimary(glm::vec3 _position)
{
  for (auto& prim: m_primChildren) // access by reference to avoid copying
  {
    if(_position[0]<prim->getOrigin()[0] || _position[0]>=prim->getOrigin()[0]+m_primUnit) continue;
    if(_position[1]<prim->getOrigin()[1] || _position[1]>=prim->getOrigin()[1]+m_primUnit) continue;
    if(_position[2]<prim->getOrigin()[2] || _position[2]>=prim->getOrigin()[2]+m_primUnit) continue;
    m_primAccessor=prim;
    return true;
  }
  return false;
}

void VoxFoxTree::translate(const glm::vec3 &_translation)
{
  std::vector<glm::vec3> ourTranslatedPositions;
  std::vector<Voxel> ourTranslatedVoxels;

  // This is the simple approach.
  // It got extremely complicated if I tried a "smarter" approach by moving the origins and voxels by hand.
  // More complicated than the hardest sudoku.

  for (auto &prim : m_primChildren)
  {
    for(auto &sec : prim->m_secChildren)
    {
      for(auto &leaf : sec->m_leafChildren)
      {
        for(auto &vox : leaf->m_VoxelData)
        {
          ourTranslatedVoxels.push_back(vox);
          float x = (int)(vox.index/(8*8));
          float y = (int)(vox.index/8)-x*8;
          float z = vox.index - x*8*8 - y*8;
          glm::vec3 ourPosition = glm::vec3(x,y,z)*m_voxUnit + leaf->getOrigin() + _translation;
          ourTranslatedPositions.push_back(ourPosition);
        }
      }
    }
    delete prim;
  }
  m_primChildren.clear();
  m_primAccessor=nullptr;
  m_secAccessor=nullptr;
  m_leafAccessor=nullptr;
  for(int i =0; i<ourTranslatedPositions.size(); ++i)
  {
    addVoxel(ourTranslatedPositions[i],ourTranslatedVoxels[i]);
  }
}



void VoxFoxTree::calculatePolys()
{
    for (auto &prim : m_primChildren) // access by reference to avoid copying
    {
      for(auto &sec : prim->m_secChildren)
      {
        for(auto &leaf : sec->m_leafChildren)
        {
          int voxelindex=0;
          for(int i = 0; i<64; ++i)
          {
            for(int j = 0; j<8 ; ++j)
            {
              if(leaf->m_VoxelMap[i] & (1<<j))
              {
                float _x = (i/8)*m_voxUnit +leaf->getOrigin()[0];
                float _y = (i-(floor(i/8)*8))*m_voxUnit+leaf->getOrigin()[1];
                float _z = j*m_voxUnit+leaf->getOrigin()[2];
                float _u =  m_voxUnit;//*10;

                int numberOfFaces =0;

                // This checks whether needed to draw each face of voxel
                // If there is voxel next to voxel then will not add the quad
                // in between as never seen

                // BACK FACE
                // isVoxel
                if((j>0 && !(leaf->m_VoxelMap[i] & (1<<(j-1))))     ||      (j==0 && !isVoxel(glm::vec3(_x,_y,_z-m_voxUnit))))
                {
                  // 1
                  m_vertexes.push_back(_x);
                  m_vertexes.push_back(_y);
                  m_vertexes.push_back(_z);

                  m_vertexes.push_back(_x);
                  m_vertexes.push_back(_y+_u);
                  m_vertexes.push_back(_z);

                  m_vertexes.push_back(_x+_u);
                  m_vertexes.push_back(_y);
                  m_vertexes.push_back(_z);

                  // 2
                  m_vertexes.push_back(_x);
                  m_vertexes.push_back(_y+_u);
                  m_vertexes.push_back(_z);

                  m_vertexes.push_back(_x+_u);
                  m_vertexes.push_back(_y+_u);
                  m_vertexes.push_back(_z);

                  m_vertexes.push_back(_x+_u);
                  m_vertexes.push_back(_y);
                  m_vertexes.push_back(_z);

                  ++numberOfFaces;

                }
                // FRONT FACE
                if((j<7 && !(leaf->m_VoxelMap[i] & (1<<(j+1))))       ||        (j==7 && !isVoxel(glm::vec3(_x,_y,_z+m_voxUnit))))
                {
                  // 3
                  m_vertexes.push_back(_x);
                  m_vertexes.push_back(_y);
                  m_vertexes.push_back(_z+_u);

                  m_vertexes.push_back(_x+_u);
                  m_vertexes.push_back(_y);
                  m_vertexes.push_back(_z+_u);

                  m_vertexes.push_back(_x);
                  m_vertexes.push_back(_y+_u);
                  m_vertexes.push_back(_z+_u);
                  // 4
                  m_vertexes.push_back(_x);
                  m_vertexes.push_back(_y+_u);
                  m_vertexes.push_back(_z+_u);

                  m_vertexes.push_back(_x+_u);
                  m_vertexes.push_back(_y);
                  m_vertexes.push_back(_z+_u);

                  m_vertexes.push_back(_x+_u);
                  m_vertexes.push_back(_y+_u);
                  m_vertexes.push_back(_z+_u);
                  ++numberOfFaces;
                }
                // LEFT FACE
                if((i>7 && !(leaf->m_VoxelMap[i-8] & (1<<j)))    ||  (i<=7 && !isVoxel(glm::vec3(_x-m_voxUnit,_y,_z))))
                {
                  // 5
                  m_vertexes.push_back(_x);
                  m_vertexes.push_back(_y+_u);
                  m_vertexes.push_back(_z);

                  m_vertexes.push_back(_x);
                  m_vertexes.push_back(_y);
                  m_vertexes.push_back(_z);

                  m_vertexes.push_back(_x);
                  m_vertexes.push_back(_y);
                  m_vertexes.push_back(_z+_u);
                  // 6
                  m_vertexes.push_back(_x);
                  m_vertexes.push_back(_y+_u);
                  m_vertexes.push_back(_z);

                  m_vertexes.push_back(_x);
                  m_vertexes.push_back(_y);
                  m_vertexes.push_back(_z+_u);

                  m_vertexes.push_back(_x);
                  m_vertexes.push_back(_y+_u);
                  m_vertexes.push_back(_z+_u);

                  ++numberOfFaces;
                }
                // RIGHT FACE
                if( (i<56 && !(leaf->m_VoxelMap[i+8] & (1<<j)))  ||   (i>=56 && !isVoxel(glm::vec3(_x+m_voxUnit,_y,_z))))
                {
                  // 7
                  m_vertexes.push_back(_x+_u);
                  m_vertexes.push_back(_y);
                  m_vertexes.push_back(_z);

                  m_vertexes.push_back(_x+_u);
                  m_vertexes.push_back(_y+_u);
                  m_vertexes.push_back(_z);

                  m_vertexes.push_back(_x+_u);
                  m_vertexes.push_back(_y);
                  m_vertexes.push_back(_z+_u);
                  // 8
                  m_vertexes.push_back(_x+_u);
                  m_vertexes.push_back(_y);
                  m_vertexes.push_back(_z+_u);

                  m_vertexes.push_back(_x+_u);
                  m_vertexes.push_back(_y+_u);
                  m_vertexes.push_back(_z);

                  m_vertexes.push_back(_x+_u);
                  m_vertexes.push_back(_y+_u);
                  m_vertexes.push_back(_z+_u);

                  ++numberOfFaces;
                }

                if((i%8<7 && !(leaf->m_VoxelMap[i+1] & (1<<j)))  ||   (i%8==7 && !isVoxel(glm::vec3(_x,_y+m_voxUnit,_z))))
                {
                  // 9
                  m_vertexes.push_back(_x);
                  m_vertexes.push_back(_y+_u);
                  m_vertexes.push_back(_z);

                  m_vertexes.push_back(_x);
                  m_vertexes.push_back(_y+_u);
                  m_vertexes.push_back(_z+_u);

                  m_vertexes.push_back(_x+_u);
                  m_vertexes.push_back(_y+_u);
                  m_vertexes.push_back(_z+_u);
                  // 10
                  m_vertexes.push_back(_x);
                  m_vertexes.push_back(_y+_u);
                  m_vertexes.push_back(_z);

                  m_vertexes.push_back(_x+_u);
                  m_vertexes.push_back(_y+_u);
                  m_vertexes.push_back(_z+_u);

                  m_vertexes.push_back(_x+_u);
                  m_vertexes.push_back(_y+_u);
                  m_vertexes.push_back(_z);

                  ++numberOfFaces;
                }
                if((i%8>0 && !(leaf->m_VoxelMap[i-1] & (1<<j)))  ||  (i%8==0 && !isVoxel(glm::vec3(_x,_y-m_voxUnit,_z))))
                {
                  // 11
                  m_vertexes.push_back(_x);
                  m_vertexes.push_back(_y);
                  m_vertexes.push_back(_z);

                  m_vertexes.push_back(_x+_u);
                  m_vertexes.push_back(_y);
                  m_vertexes.push_back(_z);

                  m_vertexes.push_back(_x+_u);
                  m_vertexes.push_back(_y);
                  m_vertexes.push_back(_z+_u);
                  // 12
                  m_vertexes.push_back(_x);
                  m_vertexes.push_back(_y);
                  m_vertexes.push_back(_z);

                  m_vertexes.push_back(_x+_u);
                  m_vertexes.push_back(_y);
                  m_vertexes.push_back(_z+_u);

                  m_vertexes.push_back(_x);
                  m_vertexes.push_back(_y);
                  m_vertexes.push_back(_z+_u);

                  ++numberOfFaces;
                }
                if((int)leaf->m_VoxelData.size()>=voxelindex+1)
                {
                  for(int o=0; o<numberOfFaces*6; ++o)
                  {
                    m_normals.push_back(leaf->m_VoxelData[voxelindex].nx);
                    m_normals.push_back(leaf->m_VoxelData[voxelindex].ny);
                    m_normals.push_back(leaf->m_VoxelData[voxelindex].nz);

                    m_textureCoords.push_back(leaf->m_VoxelData[voxelindex].u);
                    m_textureCoords.push_back(leaf->m_VoxelData[voxelindex].v);

                    m_colors.push_back(leaf->m_VoxelData[voxelindex].r);
                    m_colors.push_back(leaf->m_VoxelData[voxelindex].g);
                    m_colors.push_back(leaf->m_VoxelData[voxelindex].b);
                  }
                }
                ++voxelindex;
              }
            }
          }

        }
      }
}
}
void VoxFoxTree::createSphere(glm::vec3 const &_position, int const &_radius)
{
  for(int x = -_radius; x<_radius; ++x)
  {
    for(int y = -_radius; y<_radius; ++y)
      {
      for(int z = -_radius; z<_radius; ++z)
        {
          glm::vec3 pos = glm::vec3(x,y,z);
          pos*=m_voxUnit;
          pos+=_position;
          glm::vec3 vect = pos-_position;
          if(glm::length(vect) < _radius*m_voxUnit)
          {
            addVoxel(pos,Voxel());
          }
        }
      }
  }
}

void VoxFoxTree::createTorus(glm::vec3 const &_position, glm::vec2 const &_t)
{
  int _radius = (_t.y / m_voxUnit) + 1;
  for(int x = -_radius; x<_radius; ++x)
  {
    for(int y = -_radius; y<_radius; ++y)
      {
      for(int z = -_radius; z<_radius; ++z)
        {
          glm::vec3 pos = glm::vec3(x,y,z)*m_voxUnit;
          pos+=_position;

          glm::vec2 q = glm::vec2(glm::length(glm::vec2(pos.x,pos.z))-_t.x,pos.y);
          if(glm::length(q) < _t.y)
          {
            addVoxel(pos,Voxel());
          }
        }
      }
  }
}

void VoxFoxTree::createBox(ngl::Vec3 const &_min, ngl::Vec3 const &_max)
{
  ngl::Vec3 diff = _max-_min;
  diff=diff/m_voxUnit;

  for(int i = 0; i<(int)std::ceil(diff.m_x); ++i)
  {
    for(int j = 0; j<(int)std::ceil(diff.m_y); ++j)
    {
      for(int k = 0; k<(int)std::ceil(diff.m_z); ++k)
      {
        addVoxel(glm::vec3(_min.m_x+i*m_voxUnit,_min.m_y+j*m_voxUnit,_min.m_z+k*m_voxUnit),Voxel());
      }
    }
  }
}

void VoxFoxTree::importQuickObj(ngl::Obj * _mesh, float const &_size)
{
  std::vector<ngl::Vec3> verts = _mesh->getVertexList();
  std::vector<ngl::Face> objFaceList = _mesh->getFaceList();

  _mesh->calcDimensions();

  ngl::BBox boundingBox = _mesh->getBBox();
  float meshheight = boundingBox.height();
  float scale = _size/meshheight;
  ngl::Vec3 centre = boundingBox.center();

  for(std::vector<ngl::Face>::iterator itr=objFaceList.begin(); itr!=objFaceList.end(); ++itr)
  {
    int numVertexInFace = itr->m_vert.size();
    ngl::Vec3 min = verts[itr->m_vert[0]];
    ngl::Vec3 max = min;

    for(int i=1;i<numVertexInFace;i++)
    {
      if(verts[itr->m_vert[i]].m_x<min.m_x) {min.m_x=verts[itr->m_vert[i]].m_x*scale - centre.m_x;}
      if(verts[itr->m_vert[i]].m_y<min.m_y) {min.m_y=verts[itr->m_vert[i]].m_y*scale - centre.m_y;}
      if(verts[itr->m_vert[i]].m_z<min.m_z) {min.m_z=verts[itr->m_vert[i]].m_z*scale - centre.m_z;}

      if(verts[itr->m_vert[i]].m_x>max.m_x) {max.m_x=verts[itr->m_vert[i]].m_x*scale - centre.m_x;}
      if(verts[itr->m_vert[i]].m_y>max.m_y) {max.m_y=verts[itr->m_vert[i]].m_y*scale - centre.m_y;}
      if(verts[itr->m_vert[i]].m_z>max.m_z) {max.m_z=verts[itr->m_vert[i]].m_z*scale - centre.m_z;}
    }
    if(numVertexInFace==3) createBox(min,max);
    }
}

void VoxFoxTree::importObjUV(ngl::Obj * _mesh,float const &_size)
{

  bool interpnormals = true;
  std::vector<ngl::Vec3> verts = _mesh->getVertexList();
  std::vector<ngl::Face> objFaceList = _mesh->getFaceList();
  //std::vector<ngl::Vec3> normalList = _mesh->getNormalList();

  _mesh->calcDimensions();

  ngl::BBox boundingBox = _mesh->getBBox();
  float meshheight = boundingBox.height();
  float scale = _size/meshheight;
  ngl::Vec3 centre = boundingBox.center();

  std::vector<ngl::Vec3> vertNormals = std::vector<ngl::Vec3>(verts.size());
  std::fill(vertNormals.begin(), vertNormals.end(), ngl::Vec3(0.0f,0.0f,0.0f));
  std::vector<int> numberOfFacesPerVert = std::vector<int>(verts.size());
  std::fill(numberOfFacesPerVert.begin(), numberOfFacesPerVert.end(), 0);

  std::vector<ngl::Vec3> vertTex = _mesh->getTextureCordList();

  if(interpnormals)
  {
  for(int i = 0; i<objFaceList.size(); ++i)
  {

    ngl::Vec3 tmpNormal;

    tmpNormal = (verts[objFaceList[i].m_vert[1]]-verts[objFaceList[i].m_vert[0]]).cross(verts[objFaceList[i].m_vert[2]]-verts[objFaceList[i].m_vert[0]]);
    tmpNormal.normalize();
    for(int j =0; j<objFaceList[i].m_vert.size(); ++j)
    {
      vertNormals[objFaceList[i].m_vert[j]] += tmpNormal;
      numberOfFacesPerVert[objFaceList[i].m_vert[j]]++;
    }
  }

  for(int i =0; i<(int)vertNormals.size(); ++i)
  {
    vertNormals[i]= vertNormals[i]/numberOfFacesPerVert[i] ;
    vertNormals[i].normalize();
  }
  }

    for(std::vector<ngl::Face>::iterator itr=objFaceList.begin(); itr!=objFaceList.end(); ++itr)
    {
      if(itr->m_vert.size()==3)
      {
        ngl::Vec3 tmpNormal = (verts[itr->m_vert[1]]-verts[itr->m_vert[0]]).cross(verts[itr->m_vert[2]]-verts[itr->m_vert[0]]);

        //-------------------------VERTEXES----------------------------
        ngl::Vec3 a, b, c, e1;
        a = verts[itr->m_vert[0]]*scale - centre;
        b = verts[itr->m_vert[1]]*scale - centre;
        c = verts[itr->m_vert[2]]*scale - centre;
        e1 = b - a;

        ngl::Vec3 ta, tb, tc;
        ta = vertTex[itr->m_tex[0]];
        tb = vertTex[itr->m_tex[1]];
        tc = vertTex[itr->m_tex[2]];


        //--------------------------NORMALS-----------------------------
        ngl::Vec3 na, nb, nc;

        if(interpnormals)
        {
          na = vertNormals[itr->m_vert[0]];
          nb = vertNormals[itr->m_vert[1]];
          nc = vertNormals[itr->m_vert[2]];
        }

      int steps = std::ceil(e1.length()/m_voxUnit);
      ngl::Vec3 vecStep = e1/(2*steps); // scaled

      for(int i =0; i<(steps+1)*2; ++i) //scaled
      {
        ngl::Vec3 pos = a+(vecStep*i);
        ngl::Vec3 posnormal = ngl::lerp(na,nb,((float)i)/((float)(((steps+1)*2)-1)));
        ngl::Vec3 postexture = ngl::lerp(ta,tb,((float)i)/((float)(((steps+1)*2)-1)));
        //ngl::Vec2 posUV = ;
        ngl::Vec3 line = c-pos;
        int jsteps = std::ceil(line.length()/m_voxUnit);

        if(i!=steps)
        {
          if(i%2==1) jsteps/=2;
          else if(i%4==2) jsteps*=0.75;
          else if(i%8==4) jsteps*=0.875;
        }

        ngl::Vec3 mystep = line*(m_voxUnit / (3*line.length())); //scaled

        for(int j = 0; j<jsteps*3; ++j) //scaled
        {
          float inNX, inNY, inNZ;
          float inU, inV;
          if(interpnormals)
          {
            ngl::Vec3 posnormal2 = lerp(posnormal,nc,((float)j)/((float)((jsteps*3) - 1)));
            ngl::Vec3 postexture2 = lerp(postexture,tc,((float)j)/((float)((jsteps*3) - 1)));
            inNX = posnormal2.m_x;
            inNY = posnormal2.m_y;
            inNZ = posnormal2.m_z;
            inU = postexture2.m_x;
            inV = postexture2.m_y;
          }
          else
          {
            inNX = tmpNormal.m_x;
            inNY = tmpNormal.m_y;
            inNZ = tmpNormal.m_z;
            inU = ta.m_x;
            inV = tb.m_y;
          }

          Voxel toinsert = Voxel(inNX,inNY,inNZ,inU,inV);
          addVoxel(glm::vec3(pos.m_x,pos.m_y,pos.m_z),toinsert);
          pos = pos + mystep;
        }
      }
    }
 }
 printf("Number of voxels: %d",numberOfVoxels);
}


void VoxFoxTree::importObjRGB(ngl::Obj * _mesh, ngl::Image * _texture, float const &_size)
{

  bool interpnormals = true;
  std::vector<ngl::Vec3> verts = _mesh->getVertexList();
  std::vector<ngl::Face> objFaceList = _mesh->getFaceList();
  //std::vector<ngl::Vec3> normalList = _mesh->getNormalList();

  _mesh->calcDimensions();

  ngl::BBox boundingBox = _mesh->getBBox();
  float meshheight = boundingBox.height();
  float scale = _size/meshheight;
  ngl::Vec3 centre = boundingBox.center();


  std::vector<ngl::Vec3> vertNormals = std::vector<ngl::Vec3>(verts.size());
  std::fill(vertNormals.begin(), vertNormals.end(), ngl::Vec3(0.0f,0.0f,0.0f));
  std::vector<int> numberOfFacesPerVert = std::vector<int>(verts.size());
  std::fill(numberOfFacesPerVert.begin(), numberOfFacesPerVert.end(), 0);

  std::vector<ngl::Vec3> vertTex = _mesh->getTextureCordList();

  if(interpnormals)
  {
  for(int i = 0; i<objFaceList.size(); ++i)
  {

    ngl::Vec3 tmpNormal;

    tmpNormal = (verts[objFaceList[i].m_vert[1]]-verts[objFaceList[i].m_vert[0]]).cross(verts[objFaceList[i].m_vert[2]]-verts[objFaceList[i].m_vert[0]]);
    tmpNormal.normalize();
    for(int j =0; j<objFaceList[i].m_vert.size(); ++j)
    {
      vertNormals[objFaceList[i].m_vert[j]] += tmpNormal;
      numberOfFacesPerVert[objFaceList[i].m_vert[j]]++;
    }
  }

  for(int i =0; i<(int)vertNormals.size(); ++i)
  {
    vertNormals[i]= vertNormals[i]/numberOfFacesPerVert[i] ;
    vertNormals[i].normalize();
  }
  }

    for(std::vector<ngl::Face>::iterator itr=objFaceList.begin(); itr!=objFaceList.end(); ++itr)
    {
      if(itr->m_vert.size()==3)
      {
        ngl::Vec3 tmpNormal = (verts[itr->m_vert[1]]-verts[itr->m_vert[0]]).cross(verts[itr->m_vert[2]]-verts[itr->m_vert[0]]);

        //-------------------------VERTEXES----------------------------
        ngl::Vec3 a, b, c, e1;
        a = verts[itr->m_vert[0]]*scale - centre;
        b = verts[itr->m_vert[1]]*scale - centre;
        c = verts[itr->m_vert[2]]*scale - centre;
        e1 = b - a;

        ngl::Vec3 ta, tb, tc;
        ta = vertTex[itr->m_tex[0]];
        tb = vertTex[itr->m_tex[1]];
        tc = vertTex[itr->m_tex[2]];


        //--------------------------NORMALS-----------------------------
        ngl::Vec3 na, nb, nc;

        if(interpnormals)
        {
          na = vertNormals[itr->m_vert[0]];
          nb = vertNormals[itr->m_vert[1]];
          nc = vertNormals[itr->m_vert[2]];
        }

      int steps = std::ceil(e1.length()/m_voxUnit);
      ngl::Vec3 vecStep = e1/(2*steps); // scaled

      for(int i =0; i<(steps+1)*2; ++i) //scaled
      {
        ngl::Vec3 pos = a+(vecStep*i);
        ngl::Vec3 posnormal = ngl::lerp(na,nb,((float)i)/((float)(((steps+1)*2)-1)));
        ngl::Vec3 postexture = ngl::lerp(ta,tb,((float)i)/((float)(((steps+1)*2)-1)));
        //ngl::Vec2 posUV = ;
        ngl::Vec3 line = c-pos;
        int jsteps = std::ceil(line.length()/m_voxUnit);

        if(i!=steps)
        {
          if(i%2==1) jsteps/=2;
          else if(i%4==2) jsteps*=0.75;
          else if(i%8==4) jsteps*=0.875;
        }

        ngl::Vec3 mystep = line*(m_voxUnit / (3*line.length())); //scaled

        for(int j = 0; j<jsteps*3; ++j) //scaled
        {
          float inNX, inNY, inNZ;
          float inU, inV;
          if(interpnormals)
          {
            ngl::Vec3 posnormal2 = lerp(posnormal,nc,((float)j)/((float)((jsteps*3) - 1)));
            ngl::Vec3 postexture2 = lerp(postexture,tc,((float)j)/((float)((jsteps*3) - 1)));
            inNX = posnormal2.m_x;
            inNY = posnormal2.m_y;
            inNZ = posnormal2.m_z;
            inU = postexture2.m_x;
            inV = postexture2.m_y;
          }
          else
          {
            inNX = tmpNormal.m_x;
            inNY = tmpNormal.m_y;
            inNZ = tmpNormal.m_z;
            inU = ta.m_x;
            inV = tb.m_y;
          }
          Voxel toinsert = Voxel(inNX,inNY,inNZ,inU,inV);
          if(inU>1.0f || inU<0.0f || inV>1.0f || inV<0.0f)
          {
            toinsert.r = 1.0f;
            toinsert.g = 0.0f;
            toinsert.b = 0.0f;
          }
          else
          {
            toinsert.r = (float)_texture->getColour(inU,inV).m_r/255.0f;
            toinsert.g = (float)_texture->getColour(inU,inV).m_g/255.0f;
            toinsert.b = (float)_texture->getColour(inU,inV).m_b/255.0f;
          }

          addVoxel(glm::vec3(pos.m_x,pos.m_y,pos.m_z),toinsert);
          pos = pos + mystep;
        }
      }
    }
 }
 printf("Number of voxels: %d",numberOfVoxels);
}



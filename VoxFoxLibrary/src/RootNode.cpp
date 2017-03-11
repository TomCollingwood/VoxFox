#include "include/RootNode.h"

RootNode::RootNode()
{
  m_primAccessor= {initialized=false};
  m_secAccessor= {initialized=false};
  m_leafAccessor= {initialized=false};
}

std::vector<float> RootNode::getVertexes()
{
  return m_vertexes;
}

std::vector<float> RootNode::getNormals()
{
  return m_normals;
}

int RootNode::getSize()
{
  return m_primChildren.size();
}

float RootNode::getVertexFloat(int i)
{
  return m_vertexes.at(i);
}

int RootNode::getVertexSize()
{
  return m_vertexes.size();
}

void RootNode::printVertexes()
{
  for (std::vector<float>::const_iterator i = m_vertexes.begin(); i != m_vertexes.end(); ++i)
      std::cout << *i << ' ';
}
bool RootNode::isVoxel(glm::vec3 _position)
{
  if(m_leafAccessor!=nullptr && m_leafAccessor->isVoxel(_position)) return true;
  else if ( m_secAccessor!=nullptr && m_secAccessor->isVoxel(_position,&m_leafAccessor)) return true;
//  else
//  {
    for (auto &prim : m_primChildren) // access by reference to avoid copying
        {
          if(_position[0]<prim.getOrigin()[0] || _position[0]>=prim.getOrigin()[0]+m_primUnit) continue;
          if(_position[1]<prim.getOrigin()[1] || _position[1]>=prim.getOrigin()[1]+m_primUnit) continue;
          if(_position[2]<prim.getOrigin()[2] || _position[2]>=prim.getOrigin()[2]+m_primUnit) continue;
          m_primAccessor={prim.getIndex(),true};
          return prim.isVoxel(_position,&m_secAccessor,&m_leafAccessor);
        }
    return false;
//  }
}

LeafNode * RootNode::getLeaf(LeafAccessor _leafAcc)
{
  return &(m_primChildren[_leafAcc.primIndex].m_secChildren[_leafAcc.secIndex].m_leafChildren[_leafAcc.leafIndex]);
}

SecondaryNode * RootNode::getSecondary(SecAccessor _secAcc)
{
  return &(m_primChildren[_secAcc.primIndex].m_secChildren[_secAcc.secIndex]);
}

void RootNode::addVoxel(glm::vec3 _position, Voxel _data)
{
  if((!m_leafAccessor.initialized && !m_secAccessor.initialized) ||
     (m_leafAccessor.initialized && !getLeaf(m_leafAccessor)->addVoxel(_position, _data) &&
     m_secAccessor.initialized && !getSecondary(m_secAccessor)->addVoxel(_position,_data,&m_leafAccessor)))
  {
    bool found = false;
    for (auto &prim : m_primChildren) // access by reference to avoid copying
    {
      if(_position[0]<prim.getOrigin()[0] || _position[0]>=prim.getOrigin()[0]+m_primUnit) continue;
      if(_position[1]<prim.getOrigin()[1] || _position[1]>=prim.getOrigin()[1]+m_primUnit) continue;
      if(_position[2]<prim.getOrigin()[2] || _position[2]>=prim.getOrigin()[2]+m_primUnit) continue;
      m_primAccessor={prim.getIndex(),true};
      prim.addVoxel(_position,_data,m_secAccessor,m_leafAccessor);
      found = true;
      break;
    }
    if(!found)
    {
      glm::vec3 newOrigin = floor(_position/m_primUnit)*m_primUnit;

      int primSize = m_primChildren.size();
      m_primChildren.push_back(PrimaryNode(newOrigin,primSize));

      m_primChildren.back().addVoxel(_position,_data,&m_secAccessor,&m_leafAccessor);
      min=glm::vec3(glm::min(min.x,newOrigin.x),glm::min(min.y,newOrigin.y),glm::min(min.z,newOrigin.z));
      max=glm::vec3(glm::max(min.x,newOrigin.x+m_primUnit),glm::max(min.y,newOrigin.y+m_primUnit),glm::max(min.z,newOrigin.z+m_primUnit));
    }
}
  numberOfVoxels++;
}

//                glm::vec3 one = glm::vec3(_x,_y,k*incre+_z);
//                glm::vec3 two = glm::vec3(_x+_u,_y,(n.x*_x + n.y*_y + n.z*(k*_u+_z) - n.x*(_x+_u) - n.z*_y)/n.y); // last ones you are trying to find
//                glm::vec3 thr = glm::vec3(_x+_u,_y+_u,(n.x*_x + n.y*_y + n.z*(k*_u+_z) - n.x*(_x+_u) - n.z*(_y+_u))/n.y) ;
//                glm::vec3 fou = glm::vec3(_x,_y+_u,(n.x*_x + n.y*_y + n.z*(k*_u+_z) - n.x*_x - n.z*(_y+_u))/n.y) ;

void RootNode::calculatePolys()
{
 m_vertexes.clear();

    //glm::vec3 DOF = glm::vec3(-MV.m_02,-MV.m_12,-MV.m_22);

  //  DOF[0] = static_cast<float>(MV.m_02); // x
  //  DOF[1] = static_cast<float>(MV.m_12); // y
  //  DOF[2] = static_cast<float>(MV.m_22); // z

    for (auto &prim : m_primChildren) // access by reference to avoid copying
    {
      //prim.draw(m_vertexes, DOF);
      for(auto &sec : prim.m_secChildren)
      {
        for(auto &leaf : sec.m_leafChildren)
        {
          int voxelindex=0;
          for(int i = 0; i<64; ++i)
          {
            for(int j = 0; j<8 ; ++j)
            {
              if(leaf.m_VoxelMap[i] & (1<<j))
              {
                float _x = (i/8)*m_voxUnit+leaf.m_origin[0];
                float _y = (i-(floor(i/8)*8))*m_voxUnit+leaf.m_origin[1];
                float _z = j*m_voxUnit+leaf.m_origin[2];
                float _u =  m_voxUnit;//*10;



                int numberOfFaces =0;

        //        for(int k = 0; k<5; ++k)
        //        {
        //          glm::vec3 one = glm::vec3(_x,_y,k*_u+_z);
        //          glm::vec3 two = glm::vec3(_x+_u,_y,(n.x*_x + n.y*_y + n.z*_z - n.x*(_x+_u) - n.y*+y)/n.z);
        //          glm::vec3 thr = glm::vec3(_x+_u,_y+_u,(n.x*_x + n.y*_y + n.z*_z - n.x*(_x+_u) - n.y*+y)/n.z);
        //        }

                /*
                float x1 = 0.4;
                float z1 = 0.4;
                float y1 = (n.x*_x + n.y*_y + n.z*_z - n.x*x1 - n.z*z1)/n.y;
                float x2 = 0.2;
                float z2 = 0.4;
                float y2 = (n.x*_x + n.y*_y + n.z*_z - n.x*x2 - n.z*z2)/n.y;
                m_vertexes.push_back(_x);
                m_vertexes.push_back(_y);
                m_vertexes.push_back(_z);
                m_vertexes.push_back(x1);
                m_vertexes.push_back(y1);
                m_vertexes.push_back(z1);
                m_vertexes.push_back(x2);
                m_vertexes.push_back(y2);
                m_vertexes.push_back(z2);
                */


                // BACK FACE
                // isVoxel
                if((j>0 && !(leaf.m_VoxelMap[i] & (1<<(j-1))))     ||      (j==0 && !isVoxel(glm::vec3(_x,_y,_z-m_voxUnit))))
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
                if((j<7 && !(leaf.m_VoxelMap[i] & (1<<(j+1))))       ||        (j==7 && !isVoxel(glm::vec3(_x,_y,_z+m_voxUnit))))
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
                if((i>7 && !(leaf.m_VoxelMap[i-8] & (1<<j)))    ||  (i<=7 && !isVoxel(glm::vec3(_x-m_voxUnit,_y,_z))))
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
                if( (i<=56 && !(leaf.m_VoxelMap[i+8] & (1<<j)))  ||   (i>56 && !isVoxel(glm::vec3(_x+m_voxUnit,_y,_z))))
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

                if((i%8<7 && !(leaf.m_VoxelMap[i+1] & (1<<j)))  ||   (i%8==7 && !isVoxel(glm::vec3(_x,_y+m_voxUnit,_z))))
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
                if((i%8>0 && !(leaf.m_VoxelMap[i-1] & (1<<j)))  ||  (i%8==0 && !isVoxel(glm::vec3(_x,_y-m_voxUnit,_z))))
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
                if(leaf.m_VoxelData.size()>=voxelindex+1)
                {
                  for(int o=0; o<numberOfFaces*6; ++o)
                  {
                    m_normals.push_back(leaf.m_VoxelData[voxelindex].nx);
                    m_normals.push_back(leaf.m_VoxelData[voxelindex].ny);
                    m_normals.push_back(leaf.m_VoxelData[voxelindex].nz);
                    m_texturecoordinates.push_back(leaf.m_VoxelData[voxelindex].u);
                    m_texturecoordinates.push_back(leaf.m_VoxelData[voxelindex].v);
                  }
                }
                // */
                ++voxelindex;
              }
            }
          }
        }
      }
}
}
void RootNode::createSphere(glm::vec3 _position, int _radius)
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

  for(int i = 0; i<m_vertexes.size(); i+=9)
  {
    glm::vec3 a = glm::vec3(m_vertexes[i+0],m_vertexes[i+1],m_vertexes[i+2]);
    glm::vec3 b = glm::vec3(m_vertexes[i+3],m_vertexes[i+4],m_vertexes[i+5]);
    glm::vec3 c = glm::vec3(m_vertexes[i+6],m_vertexes[i+7],m_vertexes[i+8]);
    glm::vec3 A = b - a;
    glm::vec3 B = c - a;
    glm::vec3 N = glm::cross(A,B);
    N = glm::normalize(N);
    for(int j=0; j<3; ++j)
    {
      m_normals.push_back(N[0]);
      m_normals.push_back(N[1]);
      m_normals.push_back(N[2]);
    }
  }
}

void RootNode::createTorus(glm::vec3 _position, glm::vec2 _t)
{
  int _radius = 50;
  for(int x = -_radius; x<_radius; ++x)
  {
    for(int y = -_radius; y<_radius; ++y)
      {
      for(int z = -_radius; z<_radius; ++z)
        {
          glm::vec3 pos = glm::vec3(x,y,z);
          pos*=m_voxUnit;
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

//void RootNode::loadVBO(ngl::ShaderLib* shader, constexpr auto shaderProgram, GLuint vbo, GLuint nbo, float * normals)
//{
//  g_minindBuffer(GL_ARRAY_BUFFER, vbo);
//  g_minufferData(GL_ARRAY_BUFFER, amountVe_maxexData * sizeof(float), m_vertexes->data(), GL_STATIC_DRAW);

//  g_minindBuffer(GL_ARRAY_BUFFER, nbo);
//  g_minufferData(GL_ARRAY_BUFFER, amountVe_maxexData * sizeof(float), normals, GL_STATIC_DRAW);

//  g_minindBuffer(GL_ARRAY_BUFFER, vbo);
//  GLint pos = glGetAttribLocation(shader->getProgramID(shaderProgram), "Ve_maxexPosition");
//  glEnableVe_maxexAttribArray(pos);
//  glVe_maxexAttribPointer(pos,3,GL_FLOAT,GL_FALSE,3*sizeof(float),0);

//  g_minindBuffer(GL_ARRAY_BUFFER, nbo);
//  GLint n = glGetAttribLocation(shader->getProgramID(shaderProgram), "Ve_maxexNormal");
//  glEnableVe_maxexAttribArray(n);
//  glVe_maxexAttribPointer(n,3,GL_FLOAT,GL_FALSE,3*sizeof(float), 0);
//}

//void createRoundBox( glm::vec3 p, glm::vec3 b, float r )
//{
//  return length(max(abs(p)-b,0.0))-r;

//  for(int x = -_radius; x<_radius; ++x)
//  {
//    for(int y = -_radius; y<_radius; ++y)
//      {
//      for(int z = -_radius; z<_radius; ++z)
//        {
//          glm::vec3 pos = glm::vec3(x,y,z);
//          pos*=unitVoxelLength;
//          pos+=_position;

//          glm::vec2 q = glm::vec2(glm::length(glm::vec2(pos.x,pos.z))-_t.x,pos.y);
//          if(glm::length(q) < _t.y)
//          {
//            addVoxel(pos);
//          }
//        }
//      }
//  }
//}

void RootNode::createBox(ngl::Vec3 _min, ngl::Vec3 _max)
{
  _max=_max*10;
  _min=_min*10;
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
  //printf("poo%f",diff.m_x);
}

void RootNode::importObj(ngl::Obj * _mesh)
{
  std::vector<ngl::Vec3> verts = _mesh->getVertexList();
  std::vector<ngl::Face> objFaceList = _mesh->getFaceList();
  for(std::vector<ngl::Face>::iterator itr=objFaceList.begin(); itr!=objFaceList.end(); ++itr)
  {
    int numVertexInFace = itr->m_vert.size();
    ngl::Vec3 min = verts[itr->m_vert[0]];
    ngl::Vec3 max = min;

    for(int i=1;i<numVertexInFace;i++)
    {
      if(verts[itr->m_vert[i]].m_x<min.m_x) {min.m_x=verts[itr->m_vert[i]].m_x;}
      if(verts[itr->m_vert[i]].m_y<min.m_y) {min.m_y=verts[itr->m_vert[i]].m_y;}
      if(verts[itr->m_vert[i]].m_z<min.m_z) {min.m_z=verts[itr->m_vert[i]].m_z;}

      if(verts[itr->m_vert[i]].m_x>max.m_x) {max.m_x=verts[itr->m_vert[i]].m_x;}
      if(verts[itr->m_vert[i]].m_y>max.m_y) {max.m_y=verts[itr->m_vert[i]].m_y;}
      if(verts[itr->m_vert[i]].m_z>max.m_z) {max.m_z=verts[itr->m_vert[i]].m_z;}
      //addVoxel(glm::vec3(verts[itr->m_vert[i]].m_x,verts[itr->m_vert[i]].m_y,verts[itr->m_vert[i]].m_z));
    }
    if(numVertexInFace==3) createBox(min,max);
    }
//  int scale = 4;
//  for(auto& i : verts)
//  {
//    addVoxel(glm::vec3(i.m_x*scale,i.m_y*scale,i.m_z*scale));
//  }

}

void RootNode::importAccurateObj(ngl::Obj * _mesh)
{
    std::vector<ngl::Vec3> verts = _mesh->getVertexList();
    std::vector<ngl::Vec3> textures = _mesh->getTextureCordList();
    std::vector<ngl::Face> objFaceList = _mesh->getFaceList();
    std::vector<ngl::Vec3> normalList = _mesh->getNormalList();


    std::vector<ngl::Vec3> vertNormals = std::vector<ngl::Vec3>(verts.size());
    std::fill(vertNormals.begin(), vertNormals.end(), 0);
    std::vector<int> numberOfFacesPerVert = std::vector<int>(verts.size());

    for(int i = 0; i<objFaceList.size(); ++i)
    {
      ngl::Vec3 tmpNormal;

      tmpNormal = (verts[objFaceList[i].m_vert[1]]-verts[objFaceList[i].m_vert[0]]).cross(verts[objFaceList[i].m_vert[2]]-verts[objFaceList[i].m_vert[0]]);
      tmpNormal.normalize();
      for(int j =0; j<objFaceList[i].m_vert.size(); ++j)
      {
        vertNormals[objFaceList[i].m_vert[j]] = tmpNormal;
      }
      numberOfFacesPerVert[i]=objFaceList[i].m_vert.size();
    }

    // FOR EACH FACE
    for(std::vector<ngl::Face>::iterator itr=objFaceList.begin(); itr!=objFaceList.end(); ++itr)
    {
      if(itr->m_vert.size()==3)
      {
        ngl::Vec3 a, b, c, e1;
        a = verts[itr->m_vert[0]]*1;
        b = verts[itr->m_vert[1]]*1;
        c = verts[itr->m_vert[2]]*1;
        e1 = b - a;
        int steps = std::ceil(e1.length()/m_voxUnit);
        ngl::Vec3 vecStep = e1/(2*steps); // scaled

        ngl::Vec3 an, bn, cn;
        an = vertNormals[itr->m_vert[0]]/numberOfFacesPerVert[itr->m_vert[0]];
        bn = vertNormals[itr->m_vert[1]]/numberOfFacesPerVert[itr->m_vert[1]];
        cn = vertNormals[itr->m_vert[2]]/numberOfFacesPerVert[itr->m_vert[2]];
        an.normalize();
        bn.normalize();
        cn.normalize();


//        an = normalList[itr->m_vert[0]];
//        bn = normalList[itr->m_vert[1]];
//        cn = normalList[itr->m_vert[2]];

        float au = textures[itr->m_vert[0]].m_x;
        float av = textures[itr->m_vert[0]].m_y;
        float bu = textures[itr->m_vert[1]].m_x;
        float bv = textures[itr->m_vert[1]].m_y;
        float cu = textures[itr->m_vert[2]].m_x;
        float cv = textures[itr->m_vert[2]].m_y;
        // FOR EACH LINE
        steps = (steps+1)*2; //scaled
        for(int i =0; i<steps; ++i)
        {
          ngl::Vec3 pos = a+(vecStep*i);
          ngl::Vec3 line = c-pos;
          int jsteps = std::ceil(line.length()/m_voxUnit);
          float howFarAlongLine = 0;
          if(steps>1) howFarAlongLine=((float)i)/((float)(steps-1));
          float lineu = ngl::lerp(au,bu,howFarAlongLine);
          float linev = ngl::lerp(av,bv,howFarAlongLine);
          ngl::Vec3 linenormal = ngl::lerp(an,bn,howFarAlongLine);
          if(i!=steps)
          {
            if(i%2==1) jsteps/=2;
            else if(i%4==2) jsteps*=0.75;
            else if(i%8==4) jsteps*=0.875;
          }

          ngl::Vec3 mystep = line*(m_voxUnit / (3*line.length())); //scaled

          // FOR EACH VOXEL ON LINE
          jsteps*=3; //scaled
          for(int j = 0; j<jsteps; ++j)
          {
            float howFarAlongLine2=0;
            if(jsteps>1) howFarAlongLine2=((float)j)/((float)(jsteps-1));
            Voxel insertVoxel = Voxel();
            insertVoxel.u=ngl::lerp(lineu, cu, howFarAlongLine2);
            insertVoxel.v=ngl::lerp(linev, cv, howFarAlongLine2);
            ngl::Vec3 voxelNormal = lerp(linenormal,cn,howFarAlongLine2);
            voxelNormal.normalize();
            if(voxelNormal.m_x==voxelNormal.m_x && voxelNormal.m_y==voxelNormal.m_y && voxelNormal.m_z==voxelNormal.m_z)
            {
              insertVoxel.nx = voxelNormal.m_x;
              insertVoxel.ny = voxelNormal.m_y;
              insertVoxel.nz = voxelNormal.m_z;
            }
            addVoxel(glm::vec3(pos.m_x,pos.m_y,pos.m_z),insertVoxel);
            pos = pos + mystep;
          }
        }

          /*
          // SEE https://github.com/Forceflow/cuda_voxelizer/blob/master/src/voxelize.cu

          // COMPUTE COMMON TRIANGLE PROPE_maxIES
          glm::vec3 v0 = glm::vec3(ve_maxs[itr->m_ve_max[0]].m_x, ve_maxs[itr->m_ve_max[0]].m_y, ve_maxs[itr->m_ve_max[0]].m_z) ;//- info.bbox.min; // get v0 and move to origin
          glm::vec3 v1 = glm::vec3(ve_maxs[itr->m_ve_max[1]].m_x, ve_maxs[itr->m_ve_max[1]].m_y, ve_maxs[itr->m_ve_max[1]].m_z) ;//- info.bbox.min; // get v1 and move to origin
          glm::vec3 v2 = glm::vec3(ve_maxs[itr->m_ve_max[2]].m_x, ve_maxs[itr->m_ve_max[2]].m_y, ve_maxs[itr->m_ve_max[2]].m_z) ;//- info.bbox.min; // get v2 and move to origin
          glm::vec3 e0 = v1 - v0;
          glm::vec3 e1 = v2 - v1;
          glm::vec3 e2 = v0 - v2;
          glm::vec3 n = glm::normalize(glm::cross(e0, e1));

          //COMPUTE TRIANGLE BBOX IN GRID
          //AABox<glm::vec3> t_bbox_world(glm::min(v0, glm::min(v1, v2)), glm::max(v0, glm::max(v1, v2)));
          glm::vec3 t_bbox_world_min = glm::min(v0, glm::min(v1, v2));
          glm::vec3 t_bbox_world_max = glm::max(v0, glm::max(v1, v2));


          glm::ivec3 t_bbox_grid_min = t_bbox_world_min / unitVoxelLength;//, glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(info.gridsize-1, info.gridsize-1, info.gridsize-1));
          glm::ivec3 t_bbox_grid_max = t_bbox_world_max / unitVoxelLength;//, glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(info.gridsize-1, info.gridsize-1, info.gridsize-1));

          // PREPARE PLANE TEST PROPE_maxIES
          if (n.x > 0.0f) { c.x = unitVoxelLength; }
          if (n.y > 0.0f) { c.y = unitVoxelLength; }
          if (n.z > 0.0f) { c.z = unitVoxelLength; }
          float d1 = glm::dot(n, (c - v0));
          float d2 = glm::dot(n, ((delta_p - c) - v0));

          // PREPARE PROJECTION TEST PROPE_maxIES
          // XY plane
          glm::vec2 n_xy_e0(-1.0f*e0.y, e0.x);
          glm::vec2 n_xy_e1(-1.0f*e1.y, e1.x);
          glm::vec2 n_xy_e2(-1.0f*e2.y, e2.x);
          if (n.z < 0.0f) {
            n_xy_e0 = -n_xy_e0;
            n_xy_e1 = -n_xy_e1;
            n_xy_e2 = -n_xy_e2;
          }
          float d_xy_e0 = (-1.0f * glm::dot(n_xy_e0, glm::vec2(v0.x, v0.y))) + glm::max(0.0f, unitVoxelLength*n_xy_e0[0]) + glm::max(0.0f, unitVoxelLength*n_xy_e0[1]);
          float d_xy_e1 = (-1.0f * glm::dot(n_xy_e1, glm::vec2(v1.x, v1.y))) + glm::max(0.0f, unitVoxelLength*n_xy_e1[0]) + glm::max(0.0f, unitVoxelLength*n_xy_e1[1]);
          float d_xy_e2 = (-1.0f * glm::dot(n_xy_e2, glm::vec2(v2.x, v2.y))) + glm::max(0.0f, unitVoxelLength*n_xy_e2[0]) + glm::max(0.0f, unitVoxelLength*n_xy_e2[1]);
          // YZ plane
          glm::vec2 n_yz_e0(-1.0f*e0.z, e0.y);
          glm::vec2 n_yz_e1(-1.0f*e1.z, e1.y);
          glm::vec2 n_yz_e2(-1.0f*e2.z, e2.y);
          if (n.x < 0.0f) {
            n_yz_e0 = -n_yz_e0;
            n_yz_e1 = -n_yz_e1;
            n_yz_e2 = -n_yz_e2;
          }
          float d_yz_e0 = (-1.0f * glm::dot(n_yz_e0, glm::vec2(v0.y, v0.z))) + glm::max(0.0f, unitVoxelLength*n_yz_e0[0]) + glm::max(0.0f, unitVoxelLength*n_yz_e0[1]);
          float d_yz_e1 = (-1.0f * glm::dot(n_yz_e1, glm::vec2(v1.y, v1.z))) + glm::max(0.0f, unitVoxelLength*n_yz_e1[0]) + glm::max(0.0f, unitVoxelLength*n_yz_e1[1]);
          float d_yz_e2 = (-1.0f * glm::dot(n_yz_e2, glm::vec2(v2.y, v2.z))) + glm::max(0.0f, unitVoxelLength*n_yz_e2[0]) + glm::max(0.0f, unitVoxelLength*n_yz_e2[1]);
          // ZX plane
          glm::vec2 n_zx_e0(-1.0f*e0.x, e0.z);
          glm::vec2 n_zx_e1(-1.0f*e1.x, e1.z);
          glm::vec2 n_zx_e2(-1.0f*e2.x, e2.z);
          if (n.y < 0.0f) {
            n_zx_e0 = -n_zx_e0;
            n_zx_e1 = -n_zx_e1;
            n_zx_e2 = -n_zx_e2;
          }
          float d_xz_e0 = (-1.0f * glm::dot(n_zx_e0, glm::vec2(v0.z, v0.x))) + glm::max(0.0f, unitVoxelLength*n_zx_e0[0]) + glm::max(0.0f, unitVoxelLength*n_zx_e0[1]);
          float d_xz_e1 = (-1.0f * glm::dot(n_zx_e1, glm::vec2(v1.z, v1.x))) + glm::max(0.0f, unitVoxelLength*n_zx_e1[0]) + glm::max(0.0f, unitVoxelLength*n_zx_e1[1]);
          float d_xz_e2 = (-1.0f * glm::dot(n_zx_e2, glm::vec2(v2.z, v2.x))) + glm::max(0.0f, unitVoxelLength*n_zx_e2[0]) + glm::max(0.0f, unitVoxelLength*n_zx_e2[1]);

          // test possible grid boxes for overlap
          for (int z = t_bbox_grid_min.z; z <= t_bbox_grid_max.z; z++){
            for (int y = t_bbox_grid_min.y; y <= t_bbox_grid_max.y; y++){
              for (int x = t_bbox_grid_min.x; x <= t_bbox_grid_max.x; x++){
                // size_t location = x + (y*info.gridsize) + (z*info.gridsize*info.gridsize);
                // if (checkBit(voxel_table, location)){ continue; }
                // TRIANGLE PLANE THROUGH BOX TEST
                glm::vec3 p(x*unitVoxelLength, y*unitVoxelLength, z*unitVoxelLength);
                float nDOTp = glm::dot(n, p);
                if ((nDOTp + d1) * (nDOTp + d2) > 0.0f){ continue; }

                // PROJECTION TESTS
                // XY
                glm::vec2 p_xy(p.x, p.y);
                if ((glm::dot(n_xy_e0, p_xy) + d_xy_e0) < 0.0f){ continue; }
                if ((glm::dot(n_xy_e1, p_xy) + d_xy_e1) < 0.0f){ continue; }
                if ((glm::dot(n_xy_e2, p_xy) + d_xy_e2) < 0.0f){ continue; }

                // YZ
                glm::vec2 p_yz(p.y, p.z);
                if ((glm::dot(n_yz_e0, p_yz) + d_yz_e0) < 0.0f){ continue; }
                if ((glm::dot(n_yz_e1, p_yz) + d_yz_e1) < 0.0f){ continue; }
                if ((glm::dot(n_yz_e2, p_yz) + d_yz_e2) < 0.0f){ continue; }

                // XZ
                glm::vec2 p_zx(p.z, p.x);
                if ((glm::dot(n_zx_e0, p_zx) + d_xz_e0) < 0.0f){ continue; }
                if ((glm::dot(n_zx_e1, p_zx) + d_xz_e1) < 0.0f){ continue; }
                if ((glm::dot(n_zx_e2, p_zx) + d_xz_e2) < 0.0f){ continue; }

                addVoxel(p);
                //atomicAdd(&voxel_count, 1);
//                if (mo_maxon_order){
//                  size_t location = mo_maxonEncode_LUT(x, y, z);
//                  setBit(voxel_table, location);
//                } else {
//                  size_t location = x + (y*info.gridsize) + (z*info.gridsize*info.gridsize);
//                  setBit(voxel_table, location);
//                }
                continue;
              }
            }
          }
          // */

    }
 }
}

bool RootNode::intersectBox(glm::vec3 _ray, glm::vec3 _origin, glm::vec3 _min, glm::vec3 _max)
{
  glm::vec3 rayn = glm::normalize(_ray);
  glm::vec3 dirfrac;
  // _ray is unit direction vector of ray
  dirfrac.x = 1.0f / rayn.x;
  dirfrac.y = 1.0f / rayn.y;
  dirfrac.z = 1.0f / rayn.z;
  // _min is the corner of AABB with minimal coordinates - left bottom, _max is maximal corner
  // _origin is origin of ray
  float t1 = (_min.x - _origin.x)*dirfrac.x;
  float t2 = (_max.x - _origin.x)*dirfrac.x;
  float t3 = (_min.y - _origin.y)*dirfrac.y;
  float t4 = (_max.y - _origin.y)*dirfrac.y;
  float t5 = (_min.z - _origin.z)*dirfrac.z;
  float t6 = (_max.z - _origin.z)*dirfrac.z;

  float tmin = glm::max(glm::max(glm::min(t1, t2), glm::min(t3, t4)), glm::min(t5, t6));
  float tmax = glm::min(glm::min(glm::max(t1, t2), glm::max(t3, t4)), glm::max(t5, t6));

  // if tmax < 0, ray (line) is intersecting AABB, but whole AABB is behing us
  if (tmax < 0)
  {
      //t = tmax;
      return false;
  }

  // if tmin > tmax, ray doesn't intersect AABB
  if (tmin > tmax)
  {
      //t = tmax;
      return false;
  }

  //t = tmin;
  return true;
}

void RootNode::fill()
{
  bool inside = false;
  bool inVoxels = false;
  std::vector<glm::vec3> beginEndStrips;

  glm::vec3 min, max; // min and max for all primary nodes
  min = max = m_primChildren[0].getOrigin();
  for(auto& i : m_primChildren)
  {
    min = glm::vec3(glm::min(i.getOrigin().x,min.x),glm::min(i.getOrigin().y,min.y),glm::min(i.getOrigin().z,min.z));
    max = glm::vec3(glm::max(i.getOrigin().x+10.0f,max.x),glm::max(i.getOrigin().y+10.0f,max.y),glm::max(i.getOrigin().z+10.0f,max.z));
  }

  glm::vec3 raydir=glm::vec3(0.0,0.0,1.0);
  for(int i = min.x/m_voxUnit; i<max.x/m_voxUnit; ++i)
  {
    for(int j = min.x/m_voxUnit; j<max.x/m_voxUnit; ++j)
    {
      int numInstersections=0;
      glm::vec3 rayorig = glm::vec3(i*m_voxUnit,j*m_voxUnit,min.z);
      for(auto& p : m_primChildren)
      {
        glm::vec3 posPrim=rayorig;

        if(p.getOrigin()[0]<=posPrim[0] && p.getOrigin()[0]+m_primUnit>posPrim[0] &&
           p.getOrigin()[1]<=posPrim[1] && p.getOrigin()[1]+m_primUnit>posPrim[1] &&
           p.getOrigin()[2]<=posPrim[2] && p.getOrigin()[2]+m_primUnit>posPrim[2])
        {



          for(auto& s : p.m_secChildren)
          {
            //glm::vec3 posSec = rayorig;
            if(intersectBox(raydir,rayorig,s.getOrigin(),s.getOrigin()+m_secUnit))
            {
              for(auto& l : s.m_leafChildren)
              {
                if(intersectBox(raydir,rayorig,l.getOrigin(),l.getOrigin()+m_leafUnit))
                {
                  glm::vec3 origleaf = rayorig;
                  origleaf.z=l.getOrigin().z;
                  glm::vec3 currentpos = origleaf;

                  for(int k=0; k<8; ++k)
                  {
                    if(!inVoxels && isVoxel(currentpos))
                    {
                       numInstersections++;
                       inVoxels=true;
                       if(inside==true)
                       {
                         inside=false;
                         beginEndStrips.push_back(currentpos-glm::vec3(0,0,m_voxUnit));
                       }
                    }
                    else if(inVoxels && !isVoxel(currentpos))
                    {
                      inVoxels=false;
                      if(numInstersections%2==0)
                      {
                        inside=true;
                        beginEndStrips.push_back(currentpos);
                      }
                    }
                    currentpos+=glm::vec3(0,0,m_voxUnit);
                  }
                }
              }
            }

          }
        }

        posPrim+=glm::vec3(0,0,m_primUnit);
      }
      if(beginEndStrips.size()%2==1) beginEndStrips.pop_back();
    }
  }

  //RootNode filler = RootNode();
  for(int i=1; i<beginEndStrips.size(); i+=2)
  {
    glm::vec3 linelength = beginEndStrips[i]-beginEndStrips[i-1];
    int numVoxels = linelength[2]/m_voxUnit;
    glm::vec3 currentPos = beginEndStrips[i];
    for(int j=0; j<numVoxels; ++j)
    {
      addVoxel(currentPos,Voxel());
      currentPos+=glm::vec3(0,0,m_voxUnit);
    }
  }

}

void RootNode::addVoxelLine(ngl::Vec3 p0, ngl::Vec3 p1, ngl::Vec3 n0, ngl::Vec3 n1, Voxel _voxel) {
    std::vector<glm::vec3> visited = std::vector<glm::vec3>(0);
    Voxel ourVoxel = _voxel;
    int gx0idx = p0.m_x/m_voxUnit ;
    int gy0idx = p0.m_y/m_voxUnit;
    int gz0idx = p0.m_z/m_voxUnit;

    int gx1idx = p1.m_x/m_voxUnit;
    int gy1idx = p1.m_y/m_voxUnit;
    int gz1idx = p1.m_z/m_voxUnit;

    int sx = gx1idx > gx0idx ? 1 : gx1idx < gx0idx ? -1 : 0;
    int sy = gy1idx > gy0idx ? 1 : gy1idx < gy0idx ? -1 : 0;
    int sz = gz1idx > gz0idx ? 1 : gz1idx < gz0idx ? -1 : 0;

    int gx = gx0idx;
    int gy = gy0idx;
    int gz = gz0idx;

    //Planes for each axis that we will next cross
    int gxp = gx0idx + (gx1idx > gx0idx ? 1 : 0);
    int gyp = gy0idx + (gy1idx > gy0idx ? 1 : 0);
    int gzp = gz0idx + (gz1idx > gz0idx ? 1 : 0);

    //Only used for multiplying up the error margins
    float vx = p1.m_x - p0.m_x;
    float vy = p1.m_y - p0.m_y;
    float vz = p1.m_z - p0.m_z;

    //Error is normalized to vx * vy * vz so we only have to multiply up
    float vxvy = vx * vy;
    float vxvz = vx * vz;
    float vyvz = vy * vz;

    //Error from the next plane accumulators, scaled up by vx*vy*vz
    // gx0 + vx * rx === gxp
    // vx * rx === gxp - gx0
    // rx === (gxp - gx0) / vx
    float errx = (((float)gxp) - p0.m_x) * vyvz;
    float erry = (((float)gyp) - p0.m_y) * vxvz;
    float errz = (((float)gzp) - p0.m_z) * vxvy;

    float derrx = sx * vyvz;
    float derry = sy * vxvz;
    float derrz = sz * vxvy;

    do {
        visited.push_back(glm::vec3(gx,gy,gz)*m_voxUnit);

        if (gx >= gx1idx && gy >= gy1idx && gz >= gz1idx) break;

        //Which plane do we cross first?
        float xr = std::abs(errx);
        float yr = std::abs(erry);
        float zr = std::abs(errz);

        if (sx != 0 && (sy == 0 || xr < yr) && (sz == 0 || xr < zr)) {
            gx += sx;
            errx += derrx;
        }
        else if (sy != 0 && (sz == 0 || yr < zr)) {
            gy += sy;
            erry += derry;
        }
        else if (sz != 0) {
            gz += sz;
            errz += derrz;
        }

    } while (true);
    int visitedsize = visited.size();
    for(int i =0; i<visitedsize; ++i)
    {
      ngl::Vec3 normal = (n0*((float)i/(float)visitedsize)) + n1*  ((float)(visitedsize-i))/(float)visitedsize;
      ourVoxel.nx = normal.m_x;
      ourVoxel.ny = normal.m_y;
      ourVoxel.nz = normal.m_z;
      addVoxel(visited[i],ourVoxel);
    }
}



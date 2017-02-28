#include "include/RootNode.h"

RootNode::RootNode()
{
  m_vertexes = new std::vector<float>(0);
  m_normals = new std::vector<float>(0);

}

std::vector<float> * RootNode::getVertexes()
{
  return m_vertexes;
}

std::vector<float> * RootNode::getNormals()
{
  return m_normals;
}

int RootNode::getSize()
{
  return m_primChildren.size();
}

float RootNode::getFloat(int i)
{
  return m_vertexes->at(i);
}

int RootNode::getVertexSize()
{
  return m_vertexes->size();
}

void RootNode::printVertexes()
{
  for (std::vector<float>::const_iterator i = m_vertexes->begin(); i != m_vertexes->end(); ++i)
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
          if(_position[0]<prim->getOrigin()[0] || _position[0]>=prim->getOrigin()[0]+unitChildLength) continue;
          if(_position[1]<prim->getOrigin()[1] || _position[1]>=prim->getOrigin()[1]+unitChildLength) continue;
          if(_position[2]<prim->getOrigin()[2] || _position[2]>=prim->getOrigin()[2]+unitChildLength) continue;
          m_primAccessor=prim;
          return prim->isVoxel(_position,&m_secAccessor,&m_leafAccessor);
        }
    return false;
//  }
}

void RootNode::addVoxel(glm::vec3 _position)
{
  if((m_leafAccessor==nullptr && m_secAccessor==nullptr) ||
     (m_leafAccessor!=nullptr && !m_leafAccessor->addVoxel(_position) &&
     m_secAccessor!=nullptr && !m_secAccessor->addVoxel(_position,&m_leafAccessor)))
  {
    bool found = false;
    for (auto &prim : m_primChildren) // access by reference to avoid copying
        {
          if(_position[0]<prim->getOrigin()[0] || _position[0]>=prim->getOrigin()[0]+unitChildLength) continue;
          if(_position[1]<prim->getOrigin()[1] || _position[1]>=prim->getOrigin()[1]+unitChildLength) continue;
          if(_position[2]<prim->getOrigin()[2] || _position[2]>=prim->getOrigin()[2]+unitChildLength) continue;
          m_primAccessor=prim;
          prim->addVoxel(_position,&m_secAccessor,&m_leafAccessor);
          found = true;
          break;
        }
    if(!found)
    {
      glm::vec3 newOrigin = floor(_position/unitChildLength)*unitChildLength;
      m_primChildren.push_back(new PrimaryNode(newOrigin));
      m_primChildren.back()->addVoxel(_position,&m_secAccessor,&m_leafAccessor);
      min=glm::vec3(glm::min(min.x,newOrigin.x),glm::min(min.y,newOrigin.y),glm::min(min.z,newOrigin.z));
      max=glm::vec3(glm::max(min.x,newOrigin.x+unitChildLength),glm::max(min.y,newOrigin.y+unitChildLength),glm::max(min.z,newOrigin.z+unitChildLength));
    }

}
  numberOfVoxels++;
}

//                glm::vec3 one = glm::vec3(_x,_y,k*incre+_z);
//                glm::vec3 two = glm::vec3(_x+_u,_y,(n.x*_x + n.y*_y + n.z*(k*_u+_z) - n.x*(_x+_u) - n.z*_y)/n.y); // last ones you are trying to find
//                glm::vec3 thr = glm::vec3(_x+_u,_y+_u,(n.x*_x + n.y*_y + n.z*(k*_u+_z) - n.x*(_x+_u) - n.z*(_y+_u))/n.y) ;
//                glm::vec3 fou = glm::vec3(_x,_y+_u,(n.x*_x + n.y*_y + n.z*(k*_u+_z) - n.x*_x - n.z*(_y+_u))/n.y) ;

void RootNode::calculatePolys(ngl::Mat4 MV)
{
  m_vertexes->clear();

    glm::vec3 DOF = glm::vec3(-MV.m_02,-MV.m_12,-MV.m_22);

  //  DOF[0] = static_cast<float>(MV.m_02); // x
  //  DOF[1] = static_cast<float>(MV.m_12); // y
  //  DOF[2] = static_cast<float>(MV.m_22); // z

    for (auto &prim : m_primChildren) // access by reference to avoid copying
    {
      //prim->draw(m_vertexes, DOF);
      for(auto &sec : prim->m_secChildren)
      {
        for(auto &leaf : sec->m_leafChildren)
        {
          for(int i = 0; i<64; ++i)
          {
            for(int j = 0; j<8 ; ++j)
            {
              if(leaf->m_VoxelData[i] & (1<<j))
              {
                float _x = (i/8)*unitVoxelLength +leaf->m_origin[0];
                float _y = (i-(floor(i/8)*8))*unitVoxelLength+leaf->m_origin[1];
                float _z = j*unitVoxelLength+leaf->m_origin[2];
                float _u =  unitVoxelLength;//*10;




                glm::vec3 n = DOF;

                float incre = _u/5;

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
                m_vertexes->push_back(_x);
                m_vertexes->push_back(_y);
                m_vertexes->push_back(_z);
                m_vertexes->push_back(x1);
                m_vertexes->push_back(y1);
                m_vertexes->push_back(z1);
                m_vertexes->push_back(x2);
                m_vertexes->push_back(y2);
                m_vertexes->push_back(z2);
                */


                // BACK FACE
                // isVoxel
                if((j>0 && !(leaf->m_VoxelData[i] & (1<<(j-1))))     ||      (j==0 && !isVoxel(glm::vec3(_x,_y,_z-unitVoxelLength))))
                {
                  // 1
                  m_vertexes->push_back(_x);
                  m_vertexes->push_back(_y);
                  m_vertexes->push_back(_z);

                  m_vertexes->push_back(_x);
                  m_vertexes->push_back(_y+_u);
                  m_vertexes->push_back(_z);

                  m_vertexes->push_back(_x+_u);
                  m_vertexes->push_back(_y);
                  m_vertexes->push_back(_z);

                  // 2
                  m_vertexes->push_back(_x);
                  m_vertexes->push_back(_y+_u);
                  m_vertexes->push_back(_z);

                  m_vertexes->push_back(_x+_u);
                  m_vertexes->push_back(_y+_u);
                  m_vertexes->push_back(_z);

                  m_vertexes->push_back(_x+_u);
                  m_vertexes->push_back(_y);
                  m_vertexes->push_back(_z);

                }
                // FRONT FACE
                if((j<7 && !(leaf->m_VoxelData[i] & (1<<(j+1))))       ||        (j==7 && !isVoxel(glm::vec3(_x,_y,_z+unitVoxelLength))))
                {
                  // 3
                  m_vertexes->push_back(_x);
                  m_vertexes->push_back(_y);
                  m_vertexes->push_back(_z+_u);

                  m_vertexes->push_back(_x+_u);
                  m_vertexes->push_back(_y);
                  m_vertexes->push_back(_z+_u);

                  m_vertexes->push_back(_x);
                  m_vertexes->push_back(_y+_u);
                  m_vertexes->push_back(_z+_u);
                  // 4
                  m_vertexes->push_back(_x);
                  m_vertexes->push_back(_y+_u);
                  m_vertexes->push_back(_z+_u);

                  m_vertexes->push_back(_x+_u);
                  m_vertexes->push_back(_y);
                  m_vertexes->push_back(_z+_u);

                  m_vertexes->push_back(_x+_u);
                  m_vertexes->push_back(_y+_u);
                  m_vertexes->push_back(_z+_u);
                }
                // LEFT FACE
                if((i>7 && !(leaf->m_VoxelData[i-8] & (1<<j)))    ||  (i<=7 && !isVoxel(glm::vec3(_x-unitVoxelLength,_y,_z))))
                {
                  // 5
                  m_vertexes->push_back(_x);
                  m_vertexes->push_back(_y+_u);
                  m_vertexes->push_back(_z);

                  m_vertexes->push_back(_x);
                  m_vertexes->push_back(_y);
                  m_vertexes->push_back(_z);

                  m_vertexes->push_back(_x);
                  m_vertexes->push_back(_y);
                  m_vertexes->push_back(_z+_u);
                  // 6
                  m_vertexes->push_back(_x);
                  m_vertexes->push_back(_y+_u);
                  m_vertexes->push_back(_z);

                  m_vertexes->push_back(_x);
                  m_vertexes->push_back(_y);
                  m_vertexes->push_back(_z+_u);

                  m_vertexes->push_back(_x);
                  m_vertexes->push_back(_y+_u);
                  m_vertexes->push_back(_z+_u);
                }
                // RIGHT FACE
                if( (i<=56 && !(leaf->m_VoxelData[i+8] & (1<<j)))  ||   (i>56 && !isVoxel(glm::vec3(_x+unitVoxelLength,_y,_z))))
                {
                  // 7
                  m_vertexes->push_back(_x+_u);
                  m_vertexes->push_back(_y);
                  m_vertexes->push_back(_z);

                  m_vertexes->push_back(_x+_u);
                  m_vertexes->push_back(_y+_u);
                  m_vertexes->push_back(_z);

                  m_vertexes->push_back(_x+_u);
                  m_vertexes->push_back(_y);
                  m_vertexes->push_back(_z+_u);
                  // 8
                  m_vertexes->push_back(_x+_u);
                  m_vertexes->push_back(_y);
                  m_vertexes->push_back(_z+_u);

                  m_vertexes->push_back(_x+_u);
                  m_vertexes->push_back(_y+_u);
                  m_vertexes->push_back(_z);

                  m_vertexes->push_back(_x+_u);
                  m_vertexes->push_back(_y+_u);
                  m_vertexes->push_back(_z+_u);
                }

                if((i%8<7 && !(leaf->m_VoxelData[i+1] & (1<<j)))  ||   (i%8==7 && !isVoxel(glm::vec3(_x,_y+unitVoxelLength,_z))))
                {
                // 9
                m_vertexes->push_back(_x);
                m_vertexes->push_back(_y+_u);
                m_vertexes->push_back(_z);

                m_vertexes->push_back(_x);
                m_vertexes->push_back(_y+_u);
                m_vertexes->push_back(_z+_u);

                m_vertexes->push_back(_x+_u);
                m_vertexes->push_back(_y+_u);
                m_vertexes->push_back(_z+_u);
                // 10
                m_vertexes->push_back(_x);
                m_vertexes->push_back(_y+_u);
                m_vertexes->push_back(_z);

                m_vertexes->push_back(_x+_u);
                m_vertexes->push_back(_y+_u);
                m_vertexes->push_back(_z+_u);

                m_vertexes->push_back(_x+_u);
                m_vertexes->push_back(_y+_u);
                m_vertexes->push_back(_z);
                }
                if((i%8>0 && !(leaf->m_VoxelData[i-1] & (1<<j)))  ||  (i%8==0 && !isVoxel(glm::vec3(_x,_y-unitVoxelLength,_z))))
                {
                // 11
                m_vertexes->push_back(_x);
                m_vertexes->push_back(_y);
                m_vertexes->push_back(_z);

                m_vertexes->push_back(_x+_u);
                m_vertexes->push_back(_y);
                m_vertexes->push_back(_z);

                m_vertexes->push_back(_x+_u);
                m_vertexes->push_back(_y);
                m_vertexes->push_back(_z+_u);
                // 12
                m_vertexes->push_back(_x);
                m_vertexes->push_back(_y);
                m_vertexes->push_back(_z);

                m_vertexes->push_back(_x+_u);
                m_vertexes->push_back(_y);
                m_vertexes->push_back(_z+_u);

                m_vertexes->push_back(_x);
                m_vertexes->push_back(_y);
                m_vertexes->push_back(_z+_u);
                }
                // */
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
          pos*=unitVoxelLength;
          pos+=_position;
          glm::vec3 vect = pos-_position;
          if(glm::length(vect) < _radius*unitVoxelLength)
          {
            addVoxel(pos);
          }
        }
      }
  }

  for(int i = 0; i<m_vertexes->size(); i+=9)
  {
    glm::vec3 a = glm::vec3(m_vertexes->at(i+0),m_vertexes->at(i+1),m_vertexes->at(i+2));
    glm::vec3 b = glm::vec3(m_vertexes->at(i+3),m_vertexes->at(i+4),m_vertexes->at(i+5));
    glm::vec3 c = glm::vec3(m_vertexes->at(i+6),m_vertexes->at(i+7),m_vertexes->at(i+8));
    glm::vec3 A = b - a;
    glm::vec3 B = c - a;
    glm::vec3 N = glm::cross(A,B);
    N = glm::normalize(N);
    for(int j=0; j<3; ++j)
    {
      m_normals->push_back(N[0]);
      m_normals->push_back(N[1]);
      m_normals->push_back(N[2]);
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
          pos*=unitVoxelLength;
          pos+=_position;

          glm::vec2 q = glm::vec2(glm::length(glm::vec2(pos.x,pos.z))-_t.x,pos.y);
          if(glm::length(q) < _t.y)
          {
            addVoxel(pos);
          }
        }
      }
  }
}

//void RootNode::loadVBO(ngl::ShaderLib* shader, constexpr auto shaderProgram, GLuint vbo, GLuint nbo, float * normals)
//{
//  glBindBuffer(GL_ARRAY_BUFFER, vbo);
//  glBufferData(GL_ARRAY_BUFFER, amountVertexData * sizeof(float), m_vertexes->data(), GL_STATIC_DRAW);

//  glBindBuffer(GL_ARRAY_BUFFER, nbo);
//  glBufferData(GL_ARRAY_BUFFER, amountVertexData * sizeof(float), normals, GL_STATIC_DRAW);

//  glBindBuffer(GL_ARRAY_BUFFER, vbo);
//  GLint pos = glGetAttribLocation(shader->getProgramID(shaderProgram), "VertexPosition");
//  glEnableVertexAttribArray(pos);
//  glVertexAttribPointer(pos,3,GL_FLOAT,GL_FALSE,3*sizeof(float),0);

//  glBindBuffer(GL_ARRAY_BUFFER, nbo);
//  GLint n = glGetAttribLocation(shader->getProgramID(shaderProgram), "VertexNormal");
//  glEnableVertexAttribArray(n);
//  glVertexAttribPointer(n,3,GL_FLOAT,GL_FALSE,3*sizeof(float), 0);
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

void RootNode::drawBox(ngl::Vec3 _min, ngl::Vec3 _max)
{
  _max=_max*10;
  _min=_min*10;
  ngl::Vec3 diff = _max-_min;
  diff=diff/unitVoxelLength;

  for(int i = 0; i<(int)std::ceil(diff.m_x); ++i)
  {
    for(int j = 0; j<(int)std::ceil(diff.m_y); ++j)
    {
      for(int k = 0; k<(int)std::ceil(diff.m_z); ++k)
      {
        addVoxel(glm::vec3(_min.m_x+i*unitVoxelLength,_min.m_y+j*unitVoxelLength,_min.m_z+k*unitVoxelLength));
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
    if(numVertexInFace==3) drawBox(min,max);
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
  std::vector<ngl::Face> objFaceList = _mesh->getFaceList();
  for(std::vector<ngl::Face>::iterator itr=objFaceList.begin(); itr!=objFaceList.end(); ++itr)
  {
    if(itr->m_vert.size()==3)
    {
      ngl::Vec3 a, b, c, e1;
      a = verts[itr->m_vert[0]]*3 ;//*15- ngl::Vec3(0,1,0);
      b = verts[itr->m_vert[1]]*3 ;//*15- ngl::Vec3(0,1,0);
      c = verts[itr->m_vert[2]]*3 ;//*15- ngl::Vec3(0,1,0);
      e1 = b - a;
      int steps = std::ceil(e1.length()/unitVoxelLength);
      ngl::Vec3 vecStep = e1/steps;

      for(int i =0; i<steps+1; ++i)
      {
        ngl::Vec3 pos = a+(vecStep*i);
        ngl::Vec3 line = c-pos;
        int jsteps = std::ceil(line.length()/unitVoxelLength);

        if(i!=steps)
        {
          if(i%2==1) jsteps/=2;
          else if(i%4==2) jsteps*=0.75;
          else if(i%8==4) jsteps*=0.875;
        }

        ngl::Vec3 mystep = line*(unitVoxelLength / line.length());

        for(int j = 0; j<=jsteps+1; ++j)
        {
          addVoxel(glm::vec3(pos.m_x,pos.m_y,pos.m_z));
          pos = pos + mystep;
        }
      }
    }
 }
 printf("Number of voxels: %d",numberOfVoxels);
}


void fill()
{

}



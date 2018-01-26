#include "include/NGLScene.h"
#include <QGuiApplication>
#include <QMouseEvent>

#include <ngl/Camera.h>
#include <ngl/Light.h>
#include <ngl/Material.h>
#include <ngl/NGLInit.h>
#include <ngl/NGLStream.h>
#include <ngl/ShaderLib.h>
#include <ngl/VAOPrimitives.h>
#include <cstring>
#include <ctime>

// https://github.com/NCCA/SimpleNGL
// GO TO line 150 to change examples

NGLScene::NGLScene()
{
  setTitle( "Qt5 Simple NGL Demo" );
}

NGLScene::~NGLScene()
{
  std::cout << "Shutting down NGL, removing VAO's and Shaders\n";
}

void NGLScene::resizeGL( int _w, int _h )
{
  m_cam.setShape( 45.0f, static_cast<float>( _w ) / _h, 0.05f, 350.0f );
  m_win.width  = static_cast<int>( _w * devicePixelRatio() );
  m_win.height = static_cast<int>( _h * devicePixelRatio() );
}

void NGLScene::initTexture(const GLuint& texUnit, GLuint &texId, const char *filename) {
    // Set our active texture unit
    glActiveTexture(GL_TEXTURE0 + texUnit);

    // Load up the image using NGL routine
    ngl::Image img(filename);

    // Create storage for our new texture
    glGenTextures(1, &texId);

    // Bind the current texture
    glBindTexture(GL_TEXTURE_2D, texId);

    // Transfer image data onto the GPU using the teximage2D call
    glTexImage2D (
                GL_TEXTURE_2D,    // The target (in this case, which side of the cube)
                0,                // Level of mipmap to load
                img.format(),     // Internal format (number of colour components)
                img.width(),      // Width in pixels
                img.height(),     // Height in pixels
                0,                // Border
                GL_RGB,          // Format of the pixel data
                GL_UNSIGNED_BYTE, // Data type of pixel data
                img.getPixels()); // Pointer to image data in memory

    // Set up parameters for our texturedeertexture
    glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
}

void NGLScene::initializeGL()
{
  #define BUFFER_OFFSET(i) ((float *)NULL + (i))
  // we must call that first before any other GL commands to load and link the
  // gl commands from the lib, if that is not done program will crash
  ngl::NGLInit::instance();

  glClearColor( 0.0f, 0.0f, 0.0f, 1.0f ); // Black Background
  // enable depth testing for drawing
  glEnable( GL_DEPTH_TEST );
// enable multisampling for smoother drawing
#ifndef USINGIOS_
  glEnable( GL_MULTISAMPLE );
#endif
  // now to load the shader and set the values
  // grab an instance of shader manager
  ngl::ShaderLib* shader = ngl::ShaderLib::instance();
  // we are creating a shader called Phong to save typos
  // in the code create some constexpr
  constexpr auto shaderProgram = "Phong";
  constexpr auto vertexShader  = "PhongVertex";
  constexpr auto fragShader    = "PhongFragment";
  // create the shader program
  shader->createShaderProgram( shaderProgram );
  // now we are going to create empty shaders for Frag and Vert
  shader->attachShader( vertexShader, ngl::ShaderType::VERTEX );
  shader->attachShader( fragShader, ngl::ShaderType::FRAGMENT );
  // attach the source
  shader->loadShaderSource( vertexShader, "shaders/PhongVertex.glsl" );
  shader->loadShaderSource( fragShader, "shaders/PhongFragment.glsl" );
  // compile the shaders
  shader->compileShader( vertexShader );
  shader->compileShader( fragShader );
  // add them to the program
  shader->attachShaderToProgram( shaderProgram, vertexShader );
  shader->attachShaderToProgram( shaderProgram, fragShader );


  // now we have associated that data we can link the shader
  shader->linkProgramObject( shaderProgram );
  // and make it active ready to load values
  ( *shader )[ shaderProgram ]->use();

  // the shader will use the currently active material and light0 so set them
  ngl::Material m( ngl::STDMAT::GOLD );
  // load our material values to the shader into the structure material (see Vertex shader)
  m.loadToShader( "material" );
  // Now we will create a basic Camera from the graphics library
  // This is a static camera so it only needs to be set once
  // First create Values for the camera position
  ngl::Vec3 from( 1, 1, 1 );
  ngl::Vec3 to( 0, 0, 0 );
  ngl::Vec3 up( 0, 1, 0 );
  // now load to our new camera
  m_cam.set( from, to, up );
  // set the shape using FOV 45 Aspect Ratio based on Width and Height
  // The final two are near and far clipping planes of 0.5 and 10
  m_cam.setShape( 45.0f, 720.0f / 576.0f, 0.05f, 350.0f );
  shader->setUniform( "viewerPos", m_cam.getEye().toVec3() );
  // now create our light that is done after the camera so we can pass the
  // transpose of the projection matrix to the light to do correct eye space
  // transformations
  ngl::Mat4 iv = m_cam.getViewMatrix();
  iv.transpose();
  ngl::Light light( ngl::Vec3( -2, 5, 2 ), ngl::Colour( 1, 1, 1, 1 ), ngl::Colour( 1, 1, 1, 1 ), ngl::LightModes::POINTLIGHT );
  light.setTransform( iv );
  // load these values to the shader as well
  light.loadToShader( "light" );

  glEnable(GL_CULL_FACE);

  // Beginning my editing of Jon Macey's NGLScene

  myRoot=new VoxFoxTree();

  //------------------------LIBRARY DEMO BEGIN-------------------------
  // EXAMPLE 0: The Title ---------------------------------------------
  // EXAMPLE 1: Gnome Sphere Binary Operations ------------------------
  // EXAMPLE 2: The CSG TREE ------------------------------------------
  // EXAMPLE 3: The Cascade -------------------------------------------
  // EXAMPLE 4: The Horned Deer Gnome ---------------------------------
  // EXAMPLE 5: The Gnome ---------------------------------------------
  // EXAMPLE 6: The RGB Gnomes ----------------------------------------
  // EXAMPLE 7: Idiotically Massive Gnome  ----------------------------
  // EXAMPLE 8: Idiotically Massive Deer  -----------------------------
  // EXAMPLE 9: Three Teapots -----------------------------------------
  //-------------------------------------------------------------------
  Examples::runExample(9,myRoot,"../../VoxFoxLibrary");


  //------------------------CALCULATE POLYGONS-------------------------
  myRoot->calculatePolys();

  // Code below grabbed from: http://stackoverflow.com/a/24192835
  // beginning of citation
  std::string numWithCommas = std::to_string(myRoot->updateVoxCount());
  int insertPosition = numWithCommas.length() - 3;
  while (insertPosition > 0) {
      numWithCommas.insert(insertPosition, ",");
      insertPosition-=3;
  }

  std::string numWithCommas2 = std::to_string(myRoot->getVertexSize()/3);
  insertPosition = numWithCommas2.length() - 3;
  while (insertPosition > 0) {
      numWithCommas2.insert(insertPosition, ",");
      insertPosition-=3;
  }
  // end of citation

  std::cout<<"// Number of Voxels:               "<<numWithCommas<<std::endl;
  std::cout<<"// Number of Polygons              "<<numWithCommas2<<std::endl;
  std::cout<<"// ---------------------------------------------------------------------------------"<<std::endl;

  //------------------LOAD IN VOXEL TREE DATA TO SHADER------------------
   GLuint vao;
   glGenVertexArrays(1, &vao);

   GLuint vbo;
   glGenBuffers(1, &vbo);

   GLuint nbo;
   glGenBuffers(1, &nbo);

   GLuint tbo;
   glGenBuffers(1, &tbo);

   GLuint cbo;
   glGenBuffers(1, &cbo);

   glBindVertexArray(vao);

   int amountVertexData = myRoot->getVertexSize();
   int amountTexData = (int)((float)myRoot->getVertexSize() * (2.0f/3.0f));

   glBindBuffer(GL_ARRAY_BUFFER, vbo);
   glBufferData(GL_ARRAY_BUFFER, amountVertexData * sizeof(float), myRoot->getVertexes().data(), GL_STATIC_DRAW);

   glBindBuffer(GL_ARRAY_BUFFER, nbo);
   glBufferData(GL_ARRAY_BUFFER, amountVertexData * sizeof(float), myRoot->getNormals().data(), GL_STATIC_DRAW);

   glBindBuffer(GL_ARRAY_BUFFER, cbo);
   glBufferData(GL_ARRAY_BUFFER, amountVertexData * sizeof(float), myRoot->getColors().data(), GL_STATIC_DRAW);

   glBindBuffer(GL_ARRAY_BUFFER, vbo);
   GLint pos = glGetAttribLocation(shader->getProgramID(shaderProgram), "VertexPosition");
   glEnableVertexAttribArray(pos);
   glVertexAttribPointer(pos,3,GL_FLOAT,GL_FALSE,3*sizeof(float),0);

   glBindBuffer(GL_ARRAY_BUFFER, nbo);
   GLint n = glGetAttribLocation(shader->getProgramID(shaderProgram), "VertexNormal");
   glEnableVertexAttribArray(n);
   glVertexAttribPointer(n,3,GL_FLOAT,GL_FALSE,3*sizeof(float), 0);

   glBindBuffer(GL_ARRAY_BUFFER, cbo);
   GLint c = glGetAttribLocation(shader->getProgramID(shaderProgram), "VertexColor");
   glEnableVertexAttribArray(c);
   glVertexAttribPointer(c,3,GL_FLOAT,GL_FALSE,3*sizeof(float), 0);
}


void NGLScene::loadMatricesToShader()
{
  ngl::ShaderLib* shader = ngl::ShaderLib::instance();

  ngl::Mat4 MV;
  ngl::Mat4 MVP;
  ngl::Mat3 normalMatrix;
  ngl::Mat4 M;
  M            = m_mouseGlobalTX;
  MV           = m_cam.getViewMatrix() * M;
  MVP          = m_cam.getVPMatrix() * M;
  normalMatrix = MV;
  normalMatrix.inverse().transpose();
  shader->setUniform( "MV", MV );
  shader->setUniform( "MVP", MVP );
  shader->setUniform( "N", normalMatrix );
}

void NGLScene::paintGL()
{
  glViewport( 0, 0, m_win.width, m_win.height );
  // clear the screen and depth buffer
  glClear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT );

  // grab an instance of the shader manager
  ngl::ShaderLib* shader = ngl::ShaderLib::instance();
  ( *shader )[ "Phong" ]->use();

  // Rotation based on the mouse position for our global transform
  ngl::Mat4 rotX;
  ngl::Mat4 rotY;
  ngl::Mat4 rotZ;

  rotX.identity();
  rotY.identity();
  rotZ.identity();

  ngl::Mat4 translate1, translate2;

  translate1.translate(-m_modelPos.m_x,-m_modelPos.m_y,-m_modelPos.m_z);
  translate2.translate(m_modelPos.m_x,m_modelPos.m_y,m_modelPos.m_z);
  static int count =0;
  count++;


  rotX.rotateX( -47.0f +  16.0f + m_win.spinXFace );
  rotY.rotateY( 36.0f + 19.0f + m_win.spinYFace );//+ m_win.spinYFace );
  rotZ.rotateZ(38.0f);


  // We need to transform the m_modelPos according to the rotation
  // Edited by Tom Collingwood
  ngl::Vec4 mycoord = ngl::Vec4(m_modelPos.m_x,m_modelPos.m_y,m_modelPos.m_z,1.0f);
  mycoord = mycoord * rotY * rotX * rotZ;

  m_mouseGlobalTX = rotY * rotX * rotZ;

  m_mouseGlobalTX.m_m[ 3 ][ 0 ] = mycoord.m_x;
  m_mouseGlobalTX.m_m[ 3 ][ 1 ] = mycoord.m_y;
  m_mouseGlobalTX.m_m[ 3 ][ 2 ] = mycoord.m_z;

  loadMatricesToShader();
  glDrawArrays(GL_TRIANGLES,0,myRoot->getVertexSize()/3);
}

//----------------------------------------------------------------------------------------------------------------------

void NGLScene::keyPressEvent( QKeyEvent* _event )
{
  // that method is called every time the main window recives a key event.
  // we then switch on the key value and set the camera in the GLWindow
  switch ( _event->key() )
  {
    // escape key to quit
    case Qt::Key_Escape:
      QGuiApplication::exit( EXIT_SUCCESS );
      break;
// turn on wirframe rendering
#ifndef USINGIOS_
    case Qt::Key_W:
      glPolygonMode( GL_FRONT_AND_BACK, GL_LINE );
      break;
    // turn off wire frame
    case Qt::Key_S:
      glPolygonMode( GL_FRONT_AND_BACK, GL_FILL );
      break;
#endif
    // show full screen
    case Qt::Key_F:
      showFullScreen();
      break;
    // show windowed
    case Qt::Key_N:

      showNormal();
      break;
    case Qt::Key_Space :
      m_win.spinXFace=0;
      m_win.spinYFace=0;
      m_modelPos.set(ngl::Vec3::zero());
    break;
    default:
      break;
  }
  update();

}

#include "NGLScene.h"
#include <iostream>
#include <ngl/Vec3.h>
#include <ngl/Light.h>
#include <ngl/NGLInit.h>
#include <ngl/VAOPrimitives.h>
#include <ngl/Material.h>
#include <ngl/ShaderLib.h>
#include <QColorDialog>
#include <RootNode.h>


//----------------------------------------------------------------------------------------------------------------------
NGLScene::NGLScene( QWidget *_parent ) : QOpenGLWidget( _parent )
{

  // set this widget to have the initial keyboard focus
  setFocus();
  // re-size the widget to that of the parent (in this case the GLFrame passed in on construction)
  this->resize(_parent->size());
	m_wireframe=false;
	m_rotation=0.0;
	m_scale=1.0;
	m_position=0.0;

	m_selectedObject=0;
}

void NGLScene::LoadOBJ(ngl::ShaderLib *shader)
{
  glEnable(GL_CULL_FACE);
  RootNode myRoot = RootNode();
  ngl::Obj * m_mesh = new ngl::Obj("models/bunny.obj");
  std::cout<<"Importing..\n"<<std::endl;
  clock_t begin = clock();
  myRoot.importAccurateObj(m_mesh,40.0f);
  clock_t end = clock();
  double elapsed_secs = double(end - begin) / CLOCKS_PER_SEC;
  std::cout<<"Import took "<<elapsed_secs<<" seconds \n\n"<<std::endl;

  std::cout<<"Filling..\n"<<std::endl;
  begin = clock();
  //myRoot->fill();
  end = clock();
  elapsed_secs = double(end - begin) / CLOCKS_PER_SEC;
  std::cout<<"Filling took "<<elapsed_secs<<" seconds \n\n"<<std::endl;
  //myRoot->importObj(m_mesh);
  //myRoot->drawBox(ngl::Vec3(0,0,0),ngl::Vec3(0.05,0.05,0.05));

  //myRoot->createTorus(glm::vec3(0,0,0),glm::vec2(1,0.06));
  //myRoot->createSphere(glm::vec3(0,0,0),50);

  std::cout<<"Poly Calculating..\n"<<std::endl;
  begin = clock();
  //ngl::Mat4 MV = m_mouseGlobalTX * m_cam.getViewMatrix();
  myRoot.calculatePolys();
  end = clock();
  elapsed_secs = double(end - begin) / CLOCKS_PER_SEC;
  std::cout<<"Polycalc took "<<elapsed_secs<<" seconds \n\n"<<std::endl;


   // GENERATION
   GLuint vao;
   glGenVertexArrays(1, &vao);

   GLuint vbo;
   glGenBuffers(1, &vbo);

   GLuint nbo;
   glGenBuffers(1, &nbo);

   // BINDING AND CALCULATE
   glBindVertexArray(vao);

   int amountVertexData = myRoot.getVertexSize();

   std::cout<<"Normals took "<<elapsed_secs<<" seconds \n\n"<<std::endl;

   glBindBuffer(GL_ARRAY_BUFFER, vbo);
   glBufferData(GL_ARRAY_BUFFER, amountVertexData * sizeof(float), myRoot.getVertexes().data(), GL_STATIC_DRAW);

   glBindBuffer(GL_ARRAY_BUFFER, nbo);
   glBufferData(GL_ARRAY_BUFFER, amountVertexData * sizeof(float), myRoot.getNormals().data(), GL_STATIC_DRAW);

   glBindBuffer(GL_ARRAY_BUFFER, vbo);
   GLint pos = glGetAttribLocation(shader->getProgramID("Phong"), "inVert");
   glEnableVertexAttribArray(pos);
   glVertexAttribPointer(pos,3,GL_FLOAT,GL_FALSE,3*sizeof(float),0);

   glBindBuffer(GL_ARRAY_BUFFER, nbo);
   GLint n = glGetAttribLocation(shader->getProgramID("Phong"), "inNormal");
   glEnableVertexAttribArray(n);
   glVertexAttribPointer(n,3,GL_FLOAT,GL_FALSE,3*sizeof(float), 0);

}

// This virtual function is called once before the first call to paintGL() or resizeGL(),
//and then once whenever the widget has been assigned a new QGLContext.
// This function should set up any required OpenGL context rendering flags, defining display lists, etc.

//----------------------------------------------------------------------------------------------------------------------
void NGLScene::initializeGL()
{

  ngl::NGLInit::instance();
  glClearColor(0.4f, 0.4f, 0.4f, 1.0f);			   // Grey Background
  // enable depth testing for drawing
  glEnable(GL_DEPTH_TEST);
  /// create our camera
  ngl::Vec3 eye(2,2,2);
  ngl::Vec3 look(0,0,0);
  ngl::Vec3 up(0,1,0);

  m_cam.set(eye,look,up);
  m_cam.setShape(45,float(1024/720),0.1,300);
  // now to load the shader and set the values
  // grab an instance of shader manager
  ngl::ShaderLib *shader=ngl::ShaderLib::instance();
  // we are creating a shader called Phong
  shader->createShaderProgram("Phong");
  // now we are going to create empty shaders for Frag and Vert
  shader->attachShader("PhongVertex",ngl::ShaderType::VERTEX);
  shader->attachShader("PhongFragment",ngl::ShaderType::FRAGMENT);
  // attach the source
  shader->loadShaderSource("PhongVertex","shaders/PhongVertex.glsl");
  shader->loadShaderSource("PhongFragment","shaders/PhongFragment.glsl");
  // compile the shaders
  shader->compileShader("PhongVertex");
  shader->compileShader("PhongFragment");
  // add them to the program
  shader->attachShaderToProgram("Phong","PhongVertex");
  shader->attachShaderToProgram("Phong","PhongFragment");
  // now bind the shader attributes for most NGL primitives we use the following
  // layout attribute 0 is the vertex data (x,y,z)
  shader->bindAttribute("Phong",0,"inVert");
  // attribute 1 is the UV data u,v (if present)
  shader->bindAttribute("Phong",1,"inUV");
  // attribute 2 are the normals x,y,z
  shader->bindAttribute("Phong",2,"inNormal");

  // now we have associated this data we can link the shader
  shader->linkProgramObject("Phong");
  // and make it active ready to load values
  (*shader)["Phong"]->use();
  shader->setShaderParam1i("Normalize",1);
  shader->setShaderParam3f("viewerPos",m_cam.getEye().m_x,m_cam.getEye().m_y,m_cam.getEye().m_z);
  // now pass the modelView and projection values to the shader
  // the shader will use the currently active material and light0 so set them
  ngl::Material m(ngl::STDMAT::POLISHEDSILVER);
  m.loadToShader("material");
  // we need to set a base colour as the material isn't being used for all the params
  shader->setShaderParam4f("Colour",0.23125f,0.23125f,0.23125f,1);

  ngl::Light light(ngl::Vec3(2,2,2),ngl::Colour(1,1,1,1),ngl::Colour(1,1,1,1),ngl::LightModes::POINTLIGHT);
  // now create our light this is done after the camera so we can pass the
  // transpose of the projection matrix to the light to do correct eye space
  // transformations
  ngl::Mat4 iv=m_cam.getViewMatrix();
  iv.transpose();
  light.setTransform(iv);
  light.setAttenuation(1,0,0);
  light.enable();
  // load these values to the shader as well
  light.loadToShader("light");

  //ngl::VAOPrimitives *prim=ngl::VAOPrimitives::instance();
  //prim->createSphere("sphere",1.0,40);



  LoadOBJ(shader);



  m_text.reset(  new  ngl::Text(QFont("Arial",18)));
  m_text->setScreenSize(this->size().width(),this->size().height());
  m_text->setColour(1.0,1.0,0.0);
  update();
}

//----------------------------------------------------------------------------------------------------------------------
//This virtual function is called whenever the widget has been resized.
// The new size is passed in width and height.
void NGLScene::resizeGL( int _w, int _h )
{
  m_cam.setShape( 45.0f, static_cast<float>( _w ) / _h, 0.05f, 350.0f );
  m_win.width  = static_cast<int>( _w * devicePixelRatio() );
  m_win.height = static_cast<int>( _h * devicePixelRatio() );
}


void NGLScene::loadMatricesToShader()
{
  ngl::ShaderLib *shader=ngl::ShaderLib::instance();
  (*shader)["Phong"]->use();

  ngl::Mat4 MV;
  ngl::Mat4 MVP;
  ngl::Mat3 normalMatrix;
  ngl::Mat4 M;
  M=m_transform.getMatrix();
  MV=m_transform.getMatrix()*m_cam.getViewMatrix();
  MVP=MV*m_cam.getProjectionMatrix() ;
  normalMatrix=MV;
  normalMatrix.inverse();
  shader->setShaderParamFromMat4("MV",MV);
  shader->setShaderParamFromMat4("MVP",MVP);
  shader->setShaderParamFromMat3("normalMatrix",normalMatrix);
  shader->setShaderParamFromMat4("M",M);
}

//----------------------------------------------------------------------------------------------------------------------
//This virtual function is called whenever the widget needs to be painted.
// this is our main drawing routine
void NGLScene::paintGL()
{
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
  glViewport(0,0,m_win.width,m_win.height);
  ngl::VAOPrimitives *prim=ngl::VAOPrimitives::instance();
  if(m_wireframe == true)
  {
    glPolygonMode(GL_FRONT_AND_BACK,GL_LINE);
  }
  else
  {
    glPolygonMode(GL_FRONT_AND_BACK,GL_FILL);
  }
  ngl::ShaderLib *shader=ngl::ShaderLib::instance();
  (*shader)["Phong"]->use();

	m_transform.setPosition(m_position);
	m_transform.setScale(m_scale);
	m_transform.setRotation(m_rotation);
	loadMatricesToShader();
//	switch(m_selectedObject)
//	{
//		case 0 : prim->draw("teapot"); break;
//		case 1 : prim->draw("sphere"); break;
//		case 2 : prim->draw("cube"); break;
//	}
	m_text->renderText(10,10,"Qt Gui Demo");
}








NGLScene::~NGLScene()
{
}

void NGLScene::toggleWireframe(bool _mode	 )
{
	m_wireframe=_mode;
	update();
}

void NGLScene::setXRotation( double _x		)
{
	m_rotation.m_x=_x;
	update();
}

void NGLScene::setYRotation( double _y	)
{
	m_rotation.m_y=_y;
	update();
}
void NGLScene::setZRotation( double _z )
{
	m_rotation.m_z=_z;
	update();
}

void NGLScene::setXScale( double _x	)
{
	m_scale.m_x=_x;
	update();
}

void NGLScene::setYScale(	 double _y)
{
	m_scale.m_y=_y;
	update();
}
void NGLScene::setZScale( double _z )
{
	m_scale.m_z=_z;
	update();
}

void NGLScene::setXPosition( double _x	)
{
	m_position.m_x=_x;
	update();
}

void NGLScene::setYPosition( double _y	)
{
	m_position.m_y=_y;
	update();
}
void NGLScene::setZPosition( double _z	 )
{
	m_position.m_z=_z;
	update();
}

void NGLScene::setObjectMode(	int _i)
{
	m_selectedObject=_i;
	update();
}
void NGLScene::setColour()
{
	QColor colour = QColorDialog::getColor();
	if( colour.isValid())
	{
    ngl::ShaderLib *shader=ngl::ShaderLib::instance();
    (*shader)["Phong"]->use();
    shader->setShaderParam4f("Colour",colour.redF(),colour.greenF(),colour.blueF(),1.0);
    update();
	}
}

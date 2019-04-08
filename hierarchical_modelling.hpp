#ifndef _COLORCUBE_HPP_
#define _COLORCUBE_HPP_

// Defining the ESCAPE Key Code
#define ESCAPE 27
// Defining the DELETE Key Code
#define DELETE 127

#include <vector>
#include "gl_framework.hpp"
#include "shader_util.hpp"
#include "glm/vec3.hpp"
#include "glm/vec4.hpp"
#include "glm/mat4x4.hpp"
#include "glm/gtc/matrix_transform.hpp"
#include "glm/gtc/type_ptr.hpp"
#include "hierarchy_node.hpp"

/*// Translation Parameters
GLfloat xpos=0.0,ypos=0.0,zpos=0.0;
// Rotation Parameters
GLfloat xrot=0.0,yrot=0.0,zrot=0.0;*/
// Camera position and rotation Parameters
GLfloat c_xpos = 0.0, c_ypos = 0.0, c_zpos = 10.0;
GLfloat c_up_x = 0.0, c_up_y = 1.0, c_up_z = 0.0;
GLfloat c_xrot=0.0,c_yrot=0.0,c_zrot=0.0;
GLfloat c_xlook=0.0,c_ylook=0.0,c_zlook=0.0;
std::vector<glm::vec4> c_positions, c_ups;

std::vector<float> lid, H_pos, M_pos, H_uaRx,H_uaRz, H_uaLx, H_uaLz, H_laR, H_laL, H_thRx, H_thRz, H_thLx, H_thLz;
std::vector<float> H_legR, H_legL, M_uaRx,M_uaRz, M_uaLx, M_uaLz, M_thRx, M_thRz, M_thLx, M_thLz, H_torso, M_torso, H_head, M_tor_x;
std::vector<float> l1, l2, model_t; // time at model keyframes

float light1 = 1.0;
float light2 = 1.0;

//Running variable to toggle culling on/off
bool enable_culling=true;
//Running variable to toggle wireframe/solid modelling
bool solid=true;
//Enable/Disable perspective view
bool enable_perspective=true;
//Shader program attribs
GLuint vPosition,vColor,vNormal,texCoord,textured,light1_isOn,light2_isOn;

std::vector<glm::mat4> matrixStack;
csX75::Model* model0;
csX75::Model* model1;
csX75::Model* model2;
csX75::Model* table;
csX75::Model* lamp;
csX75::Model* wall_light;
csX75::Model* bookcase;
csX75::Model* sofa;
csX75::Model* tv;
csX75::Model* tv_stand;
csX75::Model* room; 
csX75::Model* door;
csX75::Model* window;
csX75::Model* light_fixture ;
csX75::Model* selected_model;

csX75::HNode* curr_selected_node;


const char * brick = "images/brick.bmp";
const char * all1 = "images/all1.bmp";
const char * sofa_text = "images/sofa_text.bmp";
const char * sofa_dark = "images/sofa_dark.bmp";
const char * shima = "images/shima.bmp";
const char * shima2 = "images/shima2.bmp";
const char * curtains = "images/curtains.bmp";
const char * curtains_side = "images/curtains_side.bmp";
const char * wall = "images/wall.bmp";
const char * wall2 = "images/wall2.bmp";
const char * wood = "images/dark_fine_wood.bmp";
const char * dark_wood = "images/dark_wood.bmp";
const char * dark_brown_wood = "images/dark_brown_wood.bmp";
const char * white = "images/white.bmp";
const char * tv_body = "images/tv_body.bmp";
const char * tv_screen = "images/noise.bmp";
const char * smooth = "images/smooth.bmp";
const char * led = "images/led.bmp";


//-------------------------------------------------------------------------

#endif

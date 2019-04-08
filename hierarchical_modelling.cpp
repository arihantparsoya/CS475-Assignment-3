#include "hierarchical_modelling.hpp"
#include <vector>
#include <iostream>
#include "texture.hpp"
#include "glm/ext.hpp"

GLuint shaderProgram;

glm::mat4 rotation_matrix;
glm::mat4 projection_matrix;
glm::mat4 c_rotation_matrix;
glm::mat4 lookat_matrix;

glm::mat4 model_matrix;
glm::mat4 view_matrix;
glm::mat4 modelview_matrix;

GLuint uModelViewMatrix, normalMatrix, viewMatrix;

const int num_vertices = 36;
const int num_pos = 8;

bool camera_keyframing = false; 
int camera_keyframe_num = 0; 

float camera_animation_duration = 20; //  time during which camera transitions
int animation_duration = 3*60; // total duration till which the animation runs
float animation_start_time = 0.0;

bool animating = false;


//-----------------------------------------------------------------

//Eight vertices in homogenous coordinates
//elongated cuboid , basic arm in the hierarchy;
glm::vec4 positions1[num_pos] = {
  glm::vec4(4.0, -3.7, -3.0, 1.0), //x,y,z,w
  glm::vec4(4.0, -3.7, 3.0, 1.0),
  glm::vec4(4.0, 3.7, 3.0, 1.0),
  glm::vec4(4.0, 3.7, -3.0, 1.0),
  glm::vec4(-4.0, -3.7, -3.0, 1.0), 
  glm::vec4(-4.0, -3.7, 3.0, 1.0),
  glm::vec4(-4.0, 3.7, 3.0, 1.0),
  glm::vec4(-4.0, 3.7, -3.0, 1.0),
};

glm::vec4 positions1_lid[num_pos] = {
  glm::vec4(4.0, -0.1, 0.0, 1.0), 
  glm::vec4(4.0, -0.1, 6.0, 1.0),
  glm::vec4(4.0, 0.1, 6.0, 1.0),
  glm::vec4(4.0, 0.1, 0.0, 1.0),
  glm::vec4(-4.0, -0.1, 0.0, 1.0), 
  glm::vec4(-4.0, -0.1, 6.0, 1.0),
  glm::vec4(-4.0, 0.1, 6.0, 1.0),
  glm::vec4(-4.0, 0.1, 0.0, 1.0),
};

glm::vec4 positions1_torso[num_pos] = {
  glm::vec4(0.8, -1.2, -0.5, 1.0), 
  glm::vec4(0.8, -1.2, 0.5, 1.0),
  glm::vec4(1.0, 1.2, 0.6, 1.0),
  glm::vec4(1.0, 1.2, -0.6, 1.0),
  glm::vec4(-0.8, -1.2, -0.5, 1.0), 
  glm::vec4(-0.8, -1.2, 0.5, 1.0),
  glm::vec4(-1.0, 1.2, 0.6, 1.0),
  glm::vec4(-1.0, 1.2, -0.6, 1.0),
};

glm::vec4 positions1_neck[num_pos] = {
  glm::vec4(0.4, -0.2, -0.4, 1.0), 
  glm::vec4(0.4, -0.2, 0.4, 1.0),
  glm::vec4(0.4, 0.2, 0.4, 1.0),
  glm::vec4(0.4, 0.2, -0.4, 1.0),
  glm::vec4(-0.4, -0.2, -0.4, 1.0), 
  glm::vec4(-0.4, -0.2, 0.4, 1.0),
  glm::vec4(-0.4, 0.2, 0.4, 1.0),
  glm::vec4(-0.4,0.2, -0.4, 1.0),
};

glm::vec4 positions1_head[num_pos] = {
  glm::vec4(0.5, -0.5, -0.5, 1.0), 
  glm::vec4(0.5, -0.5, 0.5, 1.0),
  glm::vec4(0.5, 0.5, 0.5, 1.0),
  glm::vec4(0.5, 0.5, -0.5, 1.0),
  glm::vec4(-0.5, -0.5, -0.5, 1.0), 
  glm::vec4(-0.5, -0.5, 0.5, 1.0),
  glm::vec4(-0.5, 0.5, 0.5, 1.0),
  glm::vec4(-0.5,0.5, -0.5, 1.0),
};

glm::vec4 positions1_uleg[num_pos] = {
  glm::vec4(0.4, -1.6, -0.4, 1.0), 
  glm::vec4(0.4, -1.6, 0.4, 1.0),
  glm::vec4(0.4, 0.0, 0.4, 1.0),
  glm::vec4(0.4, 0.0, -0.4, 1.0),
  glm::vec4(-0.4, -1.6, -0.4, 1.0), 
  glm::vec4(-0.4, -1.6, 0.4, 1.0),
  glm::vec4(-0.4, 0.0, 0.4, 1.0),
  glm::vec4(-0.4,0.0, -0.4, 1.0),
};

glm::vec4 positions1_lleg[num_pos] = {
  glm::vec4(0.25, -1.6, -0.25, 1.0), 
  glm::vec4(0.25, -1.6, 0.25, 1.0),
  glm::vec4(0.3, 0.0, 0.3, 1.0),
  glm::vec4(0.3, 0.0, -0.3, 1.0),
  glm::vec4(-0.25, -1.6, -0.25, 1.0), 
  glm::vec4(-0.25, -1.6, 0.25, 1.0),
  glm::vec4(-0.3, 0.0, 0.3, 1.0),
  glm::vec4(-0.3,0.0, -0.3, 1.0),
};

glm::vec4 positions1_uarm[num_pos] = {
  glm::vec4(0.3, -1.3, -0.3, 1.0), 
  glm::vec4(0.3, -1.3, 0.3, 1.0),
  glm::vec4(0.3, 0.0, 0.3, 1.0),
  glm::vec4(0.3, 0.0, -0.3, 1.0),
  glm::vec4(-0.3, -1.3, -0.3, 1.0), 
  glm::vec4(-0.3, -1.3, 0.3, 1.0),
  glm::vec4(-0.3, 0.0, 0.3, 1.0),
  glm::vec4(-0.3,0.0, -0.3, 1.0),
};

glm::vec4 positions1_larm[num_pos] = {
  glm::vec4(0.2, -1.6, -0.2, 1.0), 
  glm::vec4(0.2, -1.6, 0.2, 1.0),
  glm::vec4(0.25, 0.0, 0.25, 1.0),
  glm::vec4(0.25, 0.0, -0.25, 1.0),
  glm::vec4(-0.2, -1.6, -0.2, 1.0), 
  glm::vec4(-0.2, -1.6, 0.2, 1.0),
  glm::vec4(-0.25, 0.0, 0.25, 1.0),
  glm::vec4(-0.25,0.0, -0.25, 1.0),
};

// Positions for second model
glm::vec4 positions2[num_pos] = {
  glm::vec4(1.0, -1.0, -1.0, 1.0), //x,y,z,w
  glm::vec4(1.0, -1.0, 1.0, 1.0),
  glm::vec4(1.0, 1.0, 1.0, 1.0),
  glm::vec4(1.0, 1.0, -1.0, 1.0),
  glm::vec4(-1.0, -1.0, -1.0, 1.0), 
  glm::vec4(-1.0, -1.0, 1.0, 1.0),
  glm::vec4(-1.0, 1.0, 1.0, 1.0),
  glm::vec4(-1.0, 1.0, -1.0, 1.0),
};

glm::vec4 positions2_lid[num_pos] = {
  glm::vec4(2.0, -0.1, -1.0, 1.0), 
  glm::vec4(2.0, -0.1, 1.0, 1.0),
  glm::vec4(2.0, 0.1, 1.0, 1.0),
  glm::vec4(2.0, 0.1, -1.0, 1.0),
  glm::vec4(0.0, -0.1, -1.0, 1.0), 
  glm::vec4(0.0, -0.1, 1.0, 1.0),
  glm::vec4(0.0, 0.1, 1.0, 1.0),
  glm::vec4(0.0, 0.1, -1.0, 1.0),
};

glm::vec4 positions2_torso[num_pos] = {
  glm::vec4(1.0, 0.0, -0.6, 1.0), 
  glm::vec4(1.0, 0.0, 0.6, 1.0),
  glm::vec4(1.0, -1.0, 0.6, 1.0),
  glm::vec4(1.0, -1.0, -0.6, 1.0),
  glm::vec4(-1.0, 0.0, -0.6, 1.0), 
  glm::vec4(-1.0, 0.0, 0.6, 1.0),
  glm::vec4(-1.0, -1.0, 0.6, 1.0),
  glm::vec4(-1.0, -1.0, -0.6, 1.0),
};

glm::vec4 positions2_neck[num_pos] = {
  glm::vec4(1.0, 1.0, -0.6, 1.0), 
  glm::vec4(1.0, 1.0, 0.6, 1.0),
  glm::vec4(1.0, -0.0, 0.6, 1.0),
  glm::vec4(1.0, -0.0, -0.6, 1.0),
  glm::vec4(-1.0, 1.0, -0.6, 1.0), 
  glm::vec4(-1.0, 1.0, 0.6, 1.0),
  glm::vec4(-1.0, -0.0, 0.6, 1.0),
  glm::vec4(-1.0, -0.0, -0.6, 1.0),
};

glm::vec4 positions2_head[num_pos] = {
  glm::vec4(0.8, 1.1, 0.0, 1.0), 
  glm::vec4(0.8, 1.1, 1.2, 1.0),
  glm::vec4(1.0, 0.0, 1.2, 1.0),
  glm::vec4(1.0, 0.0, 0.0, 1.0),
  glm::vec4(-0.8, 1.1, 0.0, 1.0), 
  glm::vec4(-0.8, 1.1, 1.2, 1.0),
  glm::vec4(-1.0, 0.0, 1.2, 1.0),
  glm::vec4(-1.0, 0.0, 0.0, 1.0),
};

glm::vec4 positions2_uleg[num_pos] = {
  glm::vec4(0.0, -0.5, 0.25, 1.0),
  glm::vec4(0.0, -0.5, -0.25, 1.0),
  glm::vec4(0.0, 0.0, -0.25, 1.0), 
  glm::vec4(0.0, 0.0, 0.25, 1.0),
  glm::vec4(0.5, -0.5, 0.25, 1.0),
  glm::vec4(0.5,-0.5, -0.25, 1.0),
  glm::vec4(0.5, -0.0, -0.25, 1.0), 
  glm::vec4(0.5, -0.0, 0.25, 1.0),
};

glm::vec4 positions2_lleg[num_pos] = {
  glm::vec4(0.0, -0.2, 0.5, 1.0),
  glm::vec4(0.0, -0.2, -0.25, 1.0),
  glm::vec4(0.0, 0.0, -0.25, 1.0), 
  glm::vec4(0.0, 0.0, 0.5, 1.0),
  glm::vec4(0.5, -0.2, 0.5, 1.0),
  glm::vec4(0.5, -0.2, -0.25, 1.0),
  glm::vec4(0.5, 0.0, -0.25, 1.0), 
  glm::vec4(0.5, 0.0, 0.5, 1.0),
};

glm::vec4 positions2_uarm[num_pos] = {
  glm::vec4(-0.1, -1.0, -0.3, 1.0), 
  glm::vec4(-0.1, -1.0, 0.3, 1.0),
  glm::vec4(-0.1, 0.0, 0.3, 1.0),
  glm::vec4(-0.1, 0.0, -0.3, 1.0),
  glm::vec4(0.1, -1.0, -0.3, 1.0), 
  glm::vec4(0.1, -1.0, 0.3, 1.0),
  glm::vec4(0.1, 0.0, 0.3, 1.0),
  glm::vec4(0.1, 0.0, -0.3, 1.0),
};

glm::vec4 positions2_larm[num_pos] = {
  glm::vec4(-0.1, 0.0, -0.3, 1.0), 
  glm::vec4(-0.1, 0.0, 0.3, 1.0),
  glm::vec4(-0.1, 0.2, 0.3, 1.0),
  glm::vec4(-0.1, 0.2, -0.3, 1.0),
  glm::vec4(0.1, 0.0, -0.3, 1.0), 
  glm::vec4(0.1, 0.0, 0.3, 1.0),
  glm::vec4(0.1, 0.2, 0.3, 1.0),
  glm::vec4(0.1, 0.2, -0.3, 1.0),
};

glm::vec4 positions2_band[num_pos] = {
  glm::vec4(0.95, 0.2, -0.1, 1.0), 
  glm::vec4(0.95, 0.2, 1.25, 1.0),
  glm::vec4(1.0, 0.0, 1.25, 1.0),
  glm::vec4(1.0, 0.0, -0.1, 1.0),
  glm::vec4(-0.95, 0.2, -0.1, 1.0), 
  glm::vec4(-0.95, 0.2, 1.25, 1.0),
  glm::vec4(-1.0, 0.0, 1.25, 1.0),
  glm::vec4(-1.0, 0.0, -0.1, 1.0),
};

glm::vec4 positions2_eye[num_pos] = {
  glm::vec4(0.4, 0.7, 0.0, 1.0), 
  glm::vec4(0.4, 0.7, 0.1, 1.0),
  glm::vec4(0.4, 0.0, 0.1, 1.0),
  glm::vec4(0.4, 0.0, 0.0, 1.0),
  glm::vec4(-0.4, 0.7, 0.0, 1.0), 
  glm::vec4(-0.4, 0.7, 0.1, 1.0),
  glm::vec4(-0.4, 0.0, 0.1, 1.0),
  glm::vec4(-0.4, 0.0, 0.0, 1.0),
};

glm::vec4 positions2_eyeball[num_pos] = {
  glm::vec4(0.2, 0.35, 0.0, 1.0), 
  glm::vec4(0.2, 0.35, 0.1, 1.0),
  glm::vec4(0.2, 0.0, 0.1, 1.0),
  glm::vec4(0.2, 0.0, 0.0, 1.0),
  glm::vec4(-0.2, 0.35, 0.0, 1.0), 
  glm::vec4(-0.2, 0.35, 0.1, 1.0),
  glm::vec4(-0.2, 0.0, 0.1, 1.0),
  glm::vec4(-0.2, 0.0, 0.0, 1.0),
};

glm::vec4 position_table_top[num_pos] = {
  glm::vec4(10, 0.8, -10, 1.0), 
  glm::vec4(10, 0.8, 10, 1.0),
  glm::vec4(10, -0.8, 10, 1.0),
  glm::vec4(10, -0.8, -10, 1.0),
  glm::vec4(-10, 0.8, -10, 1.0), 
  glm::vec4(-10, 0.8, 10, 1.0),
  glm::vec4(-10, -0.8, 10, 1.0),
  glm::vec4(-10, -0.8, -10, 1.0),
};

glm::vec4 position_table_legs[num_pos] = {
  glm::vec4(1, -7, -1, 1.0), 
  glm::vec4(1, 7, 1, 1.0),
  glm::vec4(1, -7, 1, 1.0),
  glm::vec4(1, -7, -1, 1.0),
  glm::vec4(-1, 7, -1, 1.0), 
  glm::vec4(-1, 7, 1, 1.0),
  glm::vec4(-1, -7, 1, 1.0),
  glm::vec4(-1, -7, -1, 1.0),
};

glm::vec4 position_sofa_bottom[num_pos] = {
  glm::vec4(15, 2, -8, 1.0), 
  glm::vec4(15, 2, 8, 1.0),
  glm::vec4(15, -2, 8, 1.0),
  glm::vec4(15, -2, -8, 1.0),
  glm::vec4(-15, 2, -8, 1.0), 
  glm::vec4(-15, 2, 8, 1.0),
  glm::vec4(-15, -2, 8, 1.0),
  glm::vec4(-15, -2, -8, 1.0),
};

glm::vec4 position_sofa_padding[num_pos] = {
  glm::vec4(7.5, 2, -8, 1.0), 
  glm::vec4(7.5, 2, 8, 1.0),
  glm::vec4(7.5, -2, 8, 1.0),
  glm::vec4(7.5, -2, -8, 1.0),
  glm::vec4(-7.5, 2, -8, 1.0), 
  glm::vec4(-7.5, 2, 8, 1.0),
  glm::vec4(-7.5, -2, 8, 1.0),
  glm::vec4(-7.5, -2, -8, 1.0),
};
glm::vec4 position_sofa_arms[num_pos] = {
  glm::vec4(2, 10, -12, 1.0), 
  glm::vec4(2, 10, 12, 1.0),
  glm::vec4(2, -10, 12, 1.0),
  glm::vec4(2, -10, -12, 1.0),
  glm::vec4(-2, 10, -12, 1.0), 
  glm::vec4(-2, 10, 12, 1.0),
  glm::vec4(-2, -10, 12, 1.0),
  glm::vec4(-2, -10, -12, 1.0),
};
glm::vec4 position_sofa_back[num_pos] = {
  glm::vec4(7.5, 12, -4, 1.0), 
  glm::vec4(7.5, 12, 4, 1.0),
  glm::vec4(7.5, -12, 4, 1.0),
  glm::vec4(7.5, -12, -4, 1.0),
  glm::vec4(-7.5, 12, -4, 1.0), 
  glm::vec4(-7.5, 12, 4, 1.0),
  glm::vec4(-7.5, -12, 4, 1.0),
  glm::vec4(-7.5, -12, -4, 1.0),
};

glm::vec4 position_bookcase_legs[num_pos] = {
  glm::vec4(0.5, 14, -3.5, 1.0), 
  glm::vec4(0.5, 14, 3.5, 1.0),
  glm::vec4(0.5, -14, 3.5, 1.0),
  glm::vec4(0.5, -14, -3.5, 1.0),
  glm::vec4(-0.5, 14, -3.5, 1.0), 
  glm::vec4(-0.5, 14, 3.5, 1.0),
  glm::vec4(-0.5, -14, 3.5, 1.0),
  glm::vec4(-0.5, -14, -3.5, 1.0),
};

glm::vec4 position_bookcase_back[num_pos] = {
  glm::vec4(8, 14, -0.5, 1.0), 
  glm::vec4(8, 14, 0.5, 1.0),
  glm::vec4(8, -14, 0.5, 1.0),
  glm::vec4(8, -14, -0.5, 1.0),
  glm::vec4(-8, 14, -0.5, 1.0), 
  glm::vec4(-8, 14, 0.5, 1.0),
  glm::vec4(-8, -14, 0.5, 1.0),
  glm::vec4(-8, -14, -0.5, 1.0),
};

glm::vec4 position_bookcase_slabs[num_pos] = {
  glm::vec4(7.5, 0.5, -3.5, 1.0), 
  glm::vec4(7.5, 0.5, 3.5, 1.0),
  glm::vec4(7.5, -0.5, 3.5, 1.0),
  glm::vec4(7.5, -0.5, -3.5, 1.0),
  glm::vec4(-7.5, 0.5, -3.5, 1.0), 
  glm::vec4(-7.5, 0.5, 3.5, 1.0),
  glm::vec4(-7.5, -0.5, 3.5, 1.0),
  glm::vec4(-7.5, -0.5, -3.5, 1.0),
};

glm::vec4 position_lamp_stand[num_pos] = {
  glm::vec4(0.6, 3.5, -0.6, 1.0), 
  glm::vec4(0.6, 3.5, 0.6, 1.0),
  glm::vec4(1.5, -3.5, 1.5, 1.0),
  glm::vec4(1.5, -3.5, -1.5, 1.0),
  glm::vec4(-0.6, 3.5, -0.6, 1.0), 
  glm::vec4(-0.6, 3.5, 0.6, 1.0),
  glm::vec4(-1.5, -3.5, 1.5, 1.0),
  glm::vec4(-1.5, -3.5, -1.5, 1.0),
};

glm::vec4 position_lamp_light[num_pos] = {
  glm::vec4(2, 2.0, -2, 1.0), 
  glm::vec4(2, 2.0, 2, 1.0),
  glm::vec4(3, -2.0, 3, 1.0),
  glm::vec4(3, -2.0, -3, 1.0),
  glm::vec4(-2, 2.0, -2, 1.0), 
  glm::vec4(-2, 2.0, 2, 1.0),
  glm::vec4(-3, -2.0, 3, 1.0),
  glm::vec4(-3, -2.0, -3, 1.0),
};

glm::vec4 position_tv_face[num_pos] = {
  glm::vec4(10, 7.0, -0.5, 1.0), 
  glm::vec4(10, 7.0, 0.5, 1.0),
  glm::vec4(10, -7.0, 0.5, 1.0),
  glm::vec4(10, -7.0, -0.5, 1.0),
  glm::vec4(-10, 7.0, -0.5, 1.0), 
  glm::vec4(-10, 7.0, 0.5, 1.0),
  glm::vec4(-10, -7.0, 0.5, 1.0),
  glm::vec4(-10, -7.0, -0.5, 1.0),
};
glm::vec4 position_tv_neck[num_pos] = {
  glm::vec4(1, 2.5, -0.8, 1.0), 
  glm::vec4(1, 2.5, 0.8, 1.0),
  glm::vec4(1, -2.5, 0.8, 1.0),
  glm::vec4(1, -2.5, -0.8, 1.0),
  glm::vec4(-1, 2.5, -0.8, 1.0), 
  glm::vec4(-1, 2.5, 0.8, 1.0),
  glm::vec4(-1, -2.5, 0.8, 1.0),
  glm::vec4(-1, -2.5, -0.8, 1.0),
};
glm::vec4 position_tv_leg[num_pos] = {
  glm::vec4(6, 0.5, -3, 1.0), 
  glm::vec4(6, 0.5, 3, 1.0),
  glm::vec4(6, -0.5, 3, 1.0),
  glm::vec4(6, -0.5, -3, 1.0),
  glm::vec4(-6, 0.5, -3, 1.0), 
  glm::vec4(-6, 0.5, 3, 1.0),
  glm::vec4(-6, -0.5, 3, 1.0),
  glm::vec4(-6, -0.5, -3, 1.0),
};
glm::vec4 position_tv_screen[num_pos] = {
  glm::vec4(9.5, 6.5, -0.1, 1.0), 
  glm::vec4(9.5, 6.5, 0.1, 1.0),
  glm::vec4(9.5, -6.5, 0.1, 1.0),
  glm::vec4(9.5, -6.5, -0.1, 1.0),
  glm::vec4(-9.5, 6.5, -0.1, 1.0), 
  glm::vec4(-9.5, 6.5, 0.1, 1.0),
  glm::vec4(-9.5, -6.5, 0.1, 1.0),
  glm::vec4(-9.5, -6.5, -0.1, 1.0),
};
glm::vec4 position_tvstand_back[num_pos] = {
  glm::vec4(12, 6, -0.5, 1.0), 
  glm::vec4(12, 6, 0.5, 1.0),
  glm::vec4(12, -6, 0.5, 1.0),
  glm::vec4(12, -6, -0.5, 1.0),
  glm::vec4(-12, 6, -0.5, 1.0), 
  glm::vec4(-12, 6, 0.5, 1.0),
  glm::vec4(-12, -6, 0.5, 1.0),
  glm::vec4(-12, -6, -0.5, 1.0),
};
glm::vec4 position_tvstand_horizontal[num_pos] = {
  glm::vec4(12, 0.5, -5, 1.0), 
  glm::vec4(12, 0.5, 5, 1.0),
  glm::vec4(12, -0.5, 5, 1.0),
  glm::vec4(12, -0.5, -5, 1.0),
  glm::vec4(-12, 0.5, -5, 1.0), 
  glm::vec4(-12, 0.5, 5, 1.0),
  glm::vec4(-12, -0.5, 5, 1.0),
  glm::vec4(-12, -0.5, -5, 1.0),
};
glm::vec4 position_tvstand_virtical[num_pos] = {
  glm::vec4(0.5, 6, -5, 1.0), 
  glm::vec4(0.5, 6, 5, 1.0),
  glm::vec4(0.5, -6, 5, 1.0),
  glm::vec4(0.5, -6, -5, 1.0),
  glm::vec4(-0.5, 6, -5, 1.0), 
  glm::vec4(-0.5, 6, 5, 1.0),
  glm::vec4(-0.5, -6, 5, 1.0),
  glm::vec4(-0.5, -6, -5, 1.0),
};
glm::vec4 position_tvstand_feet[num_pos] = {
  glm::vec4(1.0, 1.5, -1.0, 1.0), 
  glm::vec4(1.0, 1.5, 1.0, 1.0),
  glm::vec4(1.0, -1.5, 1.0, 1.0),
  glm::vec4(1.0, -1.5, -1.0, 1.0),
  glm::vec4(-1.0, 1.5, -1.0, 1.0), 
  glm::vec4(-1.0, 1.5, 1.0, 1.0),
  glm::vec4(-1.0, -1.5, 1.0, 1.0),
  glm::vec4(-1.0, -1.5, -1.0, 1.0),
};

glm::vec4 position_tvstand_slab[num_pos] = {
  glm::vec4(5, 0.3, -5, 1.0), 
  glm::vec4(5, 0.3, 5, 1.0),
  glm::vec4(5, -0.3, 5, 1.0),
  glm::vec4(5, -0.3, -5, 1.0),
  glm::vec4(-5, 0.3, -5, 1.0), 
  glm::vec4(-5, 0.3, 5, 1.0),
  glm::vec4(-5, -0.3, 5, 1.0),
  glm::vec4(-5, -0.3, -5, 1.0),
};

glm::vec4 position_room_front[num_pos] = {
  glm::vec4(40, 57, -0.5, 1.0), 
  glm::vec4(40, 57, 0.5, 1.0),
  glm::vec4(40, -27, 0.5, 1.0),
  glm::vec4(40, -27, -0.5, 1.0),
  glm::vec4(-40, 57, -0.5, 1.0), 
  glm::vec4(-40, 57, 0.5, 1.0),
  glm::vec4(-40, -27, 0.5, 1.0),
  glm::vec4(-40, -27, -0.5, 1.0),
};

glm::vec4 position_room_top[num_pos] = {
  glm::vec4(40, 0.5, -42.5, 1.0), 
  glm::vec4(40, 0.5, 42.5, 1.0),
  glm::vec4(40, -0.5, 42.5, 1.0),
  glm::vec4(40, -0.5, -42.5, 1.0),
  glm::vec4(-40, 0.5, -42.5, 1.0), 
  glm::vec4(-40, 0.5, 42.5, 1.0),
  glm::vec4(-40, -0.5, 42.5, 1.0),
  glm::vec4(-40, -0.5, -42.5, 1.0),
};

glm::vec4 position_room_side[num_pos] = {
  glm::vec4(0.5, 57, -42.5, 1.0), 
  glm::vec4(0.5, 57, 42.5, 1.0),
  glm::vec4(0.5, -34, 42.5, 1.0),
  glm::vec4(0.5, -34, -42.5, 1.0),
  glm::vec4(-0.5, 57, -42.5, 1.0), 
  glm::vec4(-0.5, 57, 42.5, 1.0),
  glm::vec4(-0.5, -34, 42.5, 1.0),
  glm::vec4(-0.5, -34, -42.5, 1.0),
};

glm::vec4 position_room_small[num_pos] = {
  glm::vec4(8, 22.75, -0.5, 1.0), 
  glm::vec4(8, 22.75, 0.5, 1.0),
  glm::vec4(8, -22.75, 0.5, 1.0),
  glm::vec4(8, -22.75, -0.5, 1.0),
  glm::vec4(-8, 22.75, -0.5, 1.0), 
  glm::vec4(-8, 22.75, 0.5, 1.0),
  glm::vec4(-8, -22.75, 0.5, 1.0),
  glm::vec4(-8, -22.75, -0.5, 1.0),
};

glm::vec4 position_door[num_pos] = {
  glm::vec4(8, 22.75, -0.5, 1.0), 
  glm::vec4(8, 22.75, 0.5, 1.0),
  glm::vec4(8, -22.75, 0.5, 1.0),
  glm::vec4(8, -22.75, -0.5, 1.0),
  glm::vec4(-8, 22.75, -0.5, 1.0), 
  glm::vec4(-8, 22.75, 0.5, 1.0),
  glm::vec4(-8, -22.75, 0.5, 1.0),
  glm::vec4(-8, -22.75, -0.5, 1.0),
};

glm::vec4 position_door_nob[num_pos] = {
  glm::vec4(1, 1, -1, 1.0), 
  glm::vec4(1, 1, 1, 1.0),
  glm::vec4(1, -1, 1, 1.0),
  glm::vec4(1, -1, -1, 1.0),
  glm::vec4(-1, 1, -1, 1.0), 
  glm::vec4(-1, 1, 1, 1.0),
  glm::vec4(-1, -1, 1, 1.0),
  glm::vec4(-1, -1, -1, 1.0),
};

glm::vec4 position_window_frame[num_pos] = {
  glm::vec4(1.5, 20, -0.5, 1.0), 
  glm::vec4(1.5, 20, 0.5, 1.0),
  glm::vec4(1.5, -20, 0.5, 1.0),
  glm::vec4(1.5, -20, -0.5, 1.0),
  glm::vec4(-1.5, 20, -0.5, 1.0), 
  glm::vec4(-1.5, 20, 0.5, 1.0),
  glm::vec4(-1.5, -20, 0.5, 1.0),
  glm::vec4(-1.5, -20, -0.5, 1.0),
};

glm::vec4 position_window_bars[num_pos] = {
  glm::vec4(15.5, 1.5, -0.5, 1.0), 
  glm::vec4(15.5, 1.5, 0.5, 1.0),
  glm::vec4(15.5, -1.5, 0.5, 1.0),
  glm::vec4(15.5, -1.5, -0.5, 1.0),
  glm::vec4(-15.5, 1.5, -0.5, 1.0), 
  glm::vec4(-15.5, 1.5, 0.5, 1.0),
  glm::vec4(-15.5, -1.5, 0.5, 1.0),
  glm::vec4(-15.5, -1.5, -0.5, 1.0),
};

glm::vec4 position_lightfixture[num_pos] = {
  glm::vec4(2, 2, -0.5, 1.0), 
  glm::vec4(2, 2, 0.5, 1.0),
  glm::vec4(2, -2, 0.5, 1.0),
  glm::vec4(2, -2, -0.5, 1.0),
  glm::vec4(-2, 2, -0.5, 1.0), 
  glm::vec4(-2, 2, 0.5, 1.0),
  glm::vec4(-2, -2, 0.5, 1.0),
  glm::vec4(-2, -2, -0.5, 1.0),
};


//RGBA colors
glm::vec4 colors_box[num_pos] = {
  glm::vec4(0.5, 0.3, 0.0, 1.0),
  glm::vec4(0.6, 0.4, 0.2, 1.0),
  glm::vec4(0.5, 0.3, 0.0, 1.0),
  glm::vec4(0.6, 0.4, 0.1, 1.0),
  glm::vec4(0.5, 0.3, 0.0, 1.0),
  glm::vec4(0.6, 0.3, 0.1, 1.0),
  glm::vec4(0.5, 0.3, 0.0, 1.0),
  glm::vec4(0.4, 0.3, 0.1, 1.0),
};

glm::vec4 colors_jean[num_pos] = {
  glm::vec4(0.1, 0.5, 0.8, 1.0),
  glm::vec4(0.2, 0.4, 0.9, 1.0),
  glm::vec4(0.3, 0.3, 1.0, 1.0),
  glm::vec4(0.4, 0.2, 0.9, 1.0),
  glm::vec4(0.1, 0.5, 0.9, 1.0),
  glm::vec4(0.2, 0.3, 0.9, 1.0),
  glm::vec4(0.1, 0.5, 0.8, 1.0),
  glm::vec4(0.1, 0.4, 0.7, 1.0)
};
glm::vec4 colors_minion[num_pos] = {
  glm::vec4(0.9, 0.9, 0.0, 1.0),
  glm::vec4(1.0, 1.0, 0.0, 1.0),
  glm::vec4(0.9, 0.9, 0.1, 1.0),
  glm::vec4(1.0, 1.0, 0.0, 1.0),
  glm::vec4(1.0, 0.8, 0.0, 1.0),
  glm::vec4(0.9, 0.9, 0.0, 1.0),
  glm::vec4(0.9, 1.0, 0.1, 1.0),
  glm::vec4(0.9, 0.9, 0.0, 1.0)
};
glm::vec4 colors_skin[num_pos] = {
  glm::vec4(1.0, 0.9, 0.6, 1.0),
  glm::vec4(1.0, 0.8, 0.6, 1.0),
  glm::vec4(0.9, 0.8, 0.6, 1.0),
  glm::vec4(1.0, 1.0, 0.5, 1.0),
  glm::vec4(1.0, 0.8, 0.6, 1.0),
  glm::vec4(0.9, 0.6, 0.6, 1.0),
  glm::vec4(0.9, 0.8, 0.7, 1.0),
  glm::vec4(0.9, 0.9, 0.8, 1.0),
};

glm::vec4 colors_shirt[num_pos] = {
  glm::vec4(0.4, 0.4, 0.4, 1.0),
  glm::vec4(0.3, 0.5, 0.3, 1.0),
  glm::vec4(0.5, 0.4, 0.3, 1.0),
  glm::vec4(0.3, 0.3, 0.4, 1.0),
  glm::vec4(0.4, 0.4, 0.4, 1.0),
  glm::vec4(0.4, 0.4, 0.3, 1.0),
  glm::vec4(0.5, 0.4, 0.5, 1.0),
  glm::vec4(0.4, 0.3, 0.4, 1.0),
};

glm::vec4 colors_eye[num_pos] = {
  glm::vec4(0.9, 1.0, 1.0, 1.0),
  glm::vec4(1.0, 1.0, 0.9, 1.0),
  glm::vec4(0.8, 1.0, 1.0, 1.0),
  glm::vec4(0.9, 0.9, 1.0, 1.0),
  glm::vec4(1.0, 1.0, 0.9, 1.0),
  glm::vec4(0.9, 1.0, 1.0, 1.0),
  glm::vec4(1.0, 0.9, 1.0, 1.0),
  glm::vec4(1.0, 1.0, 1.0, 1.0),
};

glm::vec2 t_coords[4] = {
  glm::vec2( 0.0, 0.0),
  glm::vec2( 0.0, 1.0),
  glm::vec2( 1.0, 0.0),
  glm::vec2( 1.0, 1.0)
};

glm::vec4 color_jean = glm::vec4(0.1, 0.5, 0.8, 1.0);
glm::vec4 color_minion = glm::vec4(0.9, 0.9, 0.0, 1.0);

int tri_idx;
int col_idx;
int tex_idx;
int nor_idx;

glm::vec4 v_positions1[num_vertices]; 
glm::vec4 v_positions1_lid[num_vertices];
glm::vec4 v_positions1_torso[num_vertices];
glm::vec4 v_positions1_neck[num_vertices];
glm::vec4 v_positions1_head[num_vertices];
glm::vec4 v_positions1_uleg[num_vertices];
glm::vec4 v_positions1_lleg[num_vertices];
glm::vec4 v_positions1_uarm[num_vertices];
glm::vec4 v_positions1_larm[num_vertices];
glm::vec4 v_positions2[num_vertices]; 
glm::vec4 v_positions2_lid[num_vertices];
glm::vec4 v_positions2_torso[num_vertices];
glm::vec4 v_positions2_neck[num_vertices];
glm::vec4 v_positions2_head[num_vertices];
glm::vec4 v_positions2_uleg[num_vertices];
glm::vec4 v_positions2_lleg[num_vertices];
glm::vec4 v_positions2_uarm[num_vertices];
glm::vec4 v_positions2_larm[num_vertices];
glm::vec4 v_positions2_band[num_vertices];
glm::vec4 v_positions2_eye[num_vertices];
glm::vec4 v_positions2_eyeball[num_vertices];
glm::vec4 v_positions_table_top[num_vertices];
glm::vec4 v_positions_table_legs[num_vertices];
glm::vec4 v_positions_sofa_bottom[num_vertices];
glm::vec4 v_positions_sofa_padding[num_vertices];
glm::vec4 v_positions_sofa_arms[num_vertices];
glm::vec4 v_positions_sofa_back[num_vertices];
glm::vec4 v_positions_bookcase_legs[num_vertices];
glm::vec4 v_positions_bookcase_back[num_vertices];
glm::vec4 v_positions_bookcase_slabs[num_vertices];
glm::vec4 v_positions_lamp_stand[num_vertices];
glm::vec4 v_positions_lamp_light[num_vertices];
glm::vec4 v_positions_tv_face[num_vertices];
glm::vec4 v_positions_tv_neck[num_vertices];
glm::vec4 v_positions_tv_leg[num_vertices];
glm::vec4 v_positions_tv_screen[num_vertices];
glm::vec4 v_positions_tvstand_back[num_vertices];
glm::vec4 v_positions_tvstand_horizontal[num_vertices];
glm::vec4 v_positions_tvstand_virtical[num_vertices];
glm::vec4 v_positions_tvstand_feet[num_vertices];
glm::vec4 v_positions_tvstand_slab[num_vertices];
glm::vec4 v_positions_room_front[num_vertices];
glm::vec4 v_positions_room_top[num_vertices];
glm::vec4 v_positions_room_side[num_vertices];
glm::vec4 v_positions_room_small[num_vertices];
glm::vec4 v_positions_door[num_vertices];
glm::vec4 v_positions_door_nob[num_vertices];
glm::vec4 v_positions_window_frame[num_vertices];
glm::vec4 v_positions_window_bars[num_vertices];
glm::vec4 v_positions_lightfixture[num_vertices];


glm::vec4 v_normals1[num_vertices]; 
glm::vec4 v_normals1_lid[num_vertices];
glm::vec4 v_normals1_torso[num_vertices];
glm::vec4 v_normals1_neck[num_vertices];
glm::vec4 v_normals1_head[num_vertices];
glm::vec4 v_normals1_uleg[num_vertices];
glm::vec4 v_normals1_lleg[num_vertices];
glm::vec4 v_normals1_uarm[num_vertices];
glm::vec4 v_normals1_larm[num_vertices];
glm::vec4 v_normals2[num_vertices]; 
glm::vec4 v_normals2_lid[num_vertices];
glm::vec4 v_normals2_torso[num_vertices];
glm::vec4 v_normals2_neck[num_vertices];
glm::vec4 v_normals2_head[num_vertices];
glm::vec4 v_normals2_uleg[num_vertices];
glm::vec4 v_normals2_lleg[num_vertices];
glm::vec4 v_normals2_uarm[num_vertices];
glm::vec4 v_normals2_larm[num_vertices];
glm::vec4 v_normals2_band[num_vertices];
glm::vec4 v_normals2_eye[num_vertices];
glm::vec4 v_normals2_eyeball[num_vertices];
glm::vec4 v_normals_table_top[num_vertices];
glm::vec4 v_normals_table_legs[num_vertices];
glm::vec4 v_normals_sofa_bottom[num_vertices];
glm::vec4 v_normals_sofa_padding[num_vertices];
glm::vec4 v_normals_sofa_arms[num_vertices];
glm::vec4 v_normals_sofa_back[num_vertices];
glm::vec4 v_normals_bookcase_legs[num_vertices];
glm::vec4 v_normals_bookcase_back[num_vertices];
glm::vec4 v_normals_bookcase_slabs[num_vertices];
glm::vec4 v_normals_lamp_stand[num_vertices];
glm::vec4 v_normals_lamp_light[num_vertices];
glm::vec4 v_normals_tv_face[num_vertices];
glm::vec4 v_normals_tv_neck[num_vertices];
glm::vec4 v_normals_tv_leg[num_vertices];
glm::vec4 v_normals_tv_screen[num_vertices];
glm::vec4 v_normals_tvstand_back[num_vertices];
glm::vec4 v_normals_tvstand_horizontal[num_vertices];
glm::vec4 v_normals_tvstand_virtical[num_vertices];
glm::vec4 v_normals_tvstand_feet[num_vertices];
glm::vec4 v_normals_tvstand_slab[num_vertices];
glm::vec4 v_normals_room_front[num_vertices];
glm::vec4 v_normals_room_top[num_vertices];
glm::vec4 v_normals_room_side[num_vertices];
glm::vec4 v_normals_room_small[num_vertices];
glm::vec4 v_normals_door[num_vertices];
glm::vec4 v_normals_door_nob[num_vertices];
glm::vec4 v_normals_window_frame[num_vertices];
glm::vec4 v_normals_window_bars[num_vertices];
glm::vec4 v_normals_lightfixture[num_vertices];

glm::vec4 box_color[num_vertices];
glm::vec4 skin_color[num_vertices];
glm::vec4 shirt_color[num_vertices];
glm::vec4 jean_color[num_vertices];
glm::vec4 minion_color[num_vertices];
glm::vec4 eye_color[num_vertices];
glm::vec2 tex_coords[num_vertices];

// quad generates two triangles for each face and assigns colors to the vertices
void quad(int a, int b, int c, int d, glm::vec4 pos[num_pos], glm::vec4 v_pos[num_vertices] )//idk why last one works without &
{
  v_pos[tri_idx] = pos[a]; tri_idx++;
  v_pos[tri_idx] = pos[b]; tri_idx++;
  v_pos[tri_idx] = pos[c]; tri_idx++;
  v_pos[tri_idx] = pos[a]; tri_idx++;
  v_pos[tri_idx] = pos[c]; tri_idx++;
  v_pos[tri_idx] = pos[d]; tri_idx++;
 }

void coloring(int a, int b, int c, int d, glm::vec4 col[num_pos], glm::vec4 v_col[num_vertices])
{
  v_col[col_idx] = col[a]; col_idx++;
  v_col[col_idx] = col[b]; col_idx++;
  v_col[col_idx] = col[c]; col_idx++;
  v_col[col_idx] = col[a]; col_idx++;
  v_col[col_idx] = col[c]; col_idx++;
  v_col[col_idx] = col[d]; col_idx++;
}

void texturing(int a, int b, int c, int d)
{
  tex_coords[tex_idx] = t_coords[1]; tex_idx++;
  tex_coords[tex_idx] = t_coords[0]; tex_idx++;
  tex_coords[tex_idx] = t_coords[2]; tex_idx++;
  tex_coords[tex_idx] = t_coords[1]; tex_idx++;
  tex_coords[tex_idx] = t_coords[2]; tex_idx++;
  tex_coords[tex_idx] = t_coords[3]; tex_idx++;
}

void colorcube(int polygon_type)
{
  tri_idx=0;
  col_idx=0;
  tex_idx = 0;
  glm::vec4 v_pos[num_vertices];
  glm::vec4 pos[num_pos];
  glm::vec4 v_col[num_vertices];
  glm::vec4 col[num_pos];
  glm::vec2 v_tex[num_vertices];
  
  // polygon_type==1 is a box. there is a separate "if" for this once since we don't want to color one of the sides of the box(the top)
  if(polygon_type==1)
  {
    for(int i=0; i<num_pos;i++)
    {
      pos[i] = positions1[i];
      col[i] = colors_box[i];
    }
      
    quad( 1, 0, 3, 2, pos, v_pos);
    quad( 3, 0, 4, 7, pos, v_pos);
    quad( 6, 5, 1, 2, pos, v_pos);
    quad( 4, 5, 6, 7, pos, v_pos);
    quad( 5, 4, 0, 1, pos, v_pos);

    coloring( 3, 6, 7, 7, col, v_col);      // 1-red 0-black 2-green 3-blue 4 yellow 5-pink 6-white 7-lightblue
    coloring( 7, 6, 4, 2, col, v_col);
    coloring( 2, 7, 3, 7, col, v_col);
    coloring( 4, 7, 6, 2, col, v_col);
    coloring( 7, 4, 6, 3, col, v_col);

    for(int i=0; i < num_vertices; i++)
    {
      v_positions1[i] = v_pos[i];
      box_color[i] = v_col[i];
    }
    return;
  }
  
  for(int i=0; i < num_pos;i++)
  {
    switch(polygon_type)
    {   // basically just copying the positions we defined above to "pos".             
      case 2: pos[i] = positions1_lid[i];   col[i]= colors_box[i]; break;
      case 3: pos[i] = positions1_torso[i]; col[i]= colors_shirt[i]; break;
      case 4: pos[i] = positions1_neck[i];  col[i]= colors_skin[i]; break;    
      case 5: pos[i] = positions1_head[i];  col[i]= colors_skin[i]; break;
      case 6: pos[i] = positions1_uleg[i];  col[i]= color_jean; break;  
      case 7: pos[i] = positions1_lleg[i];  col[i]= colors_skin[i]; break;
      case 8: pos[i] = positions1_uarm[i];  col[i]= colors_skin[i]; break;
      case 9: pos[i] = positions1_larm[i];  col[i]= colors_skin[i]; break;
      case 10: pos[i] = positions2_torso[i];col[i] = colors_jean[i]; break;
      case 11: pos[i] = positions2_neck[i]; col[i] = colors_minion[i]; break;     
      case 12: pos[i] = positions2_head[i]; col[i] = colors_minion[i]; break;
      case 13: pos[i] = positions2_uleg[i]; col[i] = colors_jean[i]; break;  
      case 14: pos[i] = positions2_lleg[i]; col[i] = colors_shirt[i];break;
      case 15: pos[i] = positions2_uarm[i]; col[i] = colors_jean[i]; break;
      case 16: pos[i] = positions2_larm[i]; col[i] = colors_minion[i]; break;
      case 17: pos[i] = positions2_band[i]; col[i] = colors_shirt[i]; break;
      case 18: pos[i] = positions2_eye[i];  col[i] = colors_eye[i]; break;
      case 19: pos[i] = positions2_eyeball[i]; col[i] = colors_shirt[i]; break;
      case 20: pos[i] = position_table_top[i]; col[i] = colors_box[i]; break;
      case 21: pos[i] = position_table_legs[i]; col[i] = colors_box[i]; break;
      case 22: pos[i] = position_sofa_bottom[i]; break;
      case 23: pos[i] = position_sofa_padding[i]; break;
      case 24: pos[i] = position_sofa_arms[i]; break;
      case 25: pos[i] = position_sofa_back[i];  break;
      case 26: pos[i] = position_bookcase_legs[i]; col[i] = colors_box[i]; break;
      case 27: pos[i] = position_bookcase_back[i]; col[i] = colors_box[i]; break;
      case 28: pos[i] = position_bookcase_slabs[i]; col[i] = colors_box[i]; break;
      case 29: pos[i] = position_lamp_stand[i]; break;
      case 30: pos[i] = position_lamp_light[i]; break;
      case 31: pos[i] = position_tv_leg[i]; break;
      case 32: pos[i] = position_tv_face[i]; break;
      case 33: pos[i] = position_tv_neck[i]; break;
      case 34: pos[i] = position_tv_screen[i]; break;
      case 35: pos[i] = position_tvstand_back[i]; break;
      case 36: pos[i] = position_tvstand_horizontal[i]; break;
      case 37: pos[i] = position_tvstand_virtical[i]; break;
      case 38: pos[i] = position_tvstand_feet[i]; break;
      case 39: pos[i] = position_tvstand_slab[i]; break;
      case 40: pos[i] = position_room_front[i]; break;
      case 41: pos[i] = position_room_top[i]; break;
      case 42: pos[i] = position_room_side[i]; break;
      case 43: pos[i] = position_room_small[i]; break;
      case 44: pos[i] = position_door[i]; break;
      case 45: pos[i] = position_door_nob[i]; break;
      case 46: pos[i] = position_window_frame[i]; break;
      case 47: pos[i] = position_window_bars[i]; break;
      case 48: pos[i] = position_lightfixture[i]; break;
      default: break;
    }
  }

  quad( 1, 0, 3, 2, pos, v_pos);
  quad( 2, 3, 7, 6, pos, v_pos); 
  quad( 3, 0, 4, 7, pos, v_pos);
  quad( 6, 5, 1, 2, pos, v_pos);
  quad( 4, 5, 6, 7, pos, v_pos);
  quad( 5, 4, 0, 1, pos, v_pos);

  coloring( 1, 0, 3, 2, col, v_col);
  coloring( 2, 3, 7, 6, col, v_col);
  coloring( 3, 0, 4, 7, col, v_col);
  coloring( 6, 5, 1, 2, col, v_col);
  coloring( 4, 5, 6, 7, col, v_col);
  coloring( 5, 4, 0, 1, col, v_col); 

  // now that we got 'v_pos' using 'pos', assign all the vertices in "v_pos" to the correct "v_positions" variable.
  for(int i=0; i <num_vertices ; i++)
  {
    switch(polygon_type)
    {  
    case 2: v_positions1_lid[i] = v_pos[i];   box_color[i] = v_col[i];    break;         
    case 3: v_positions1_torso[i] = v_pos[i]; shirt_color[i] = v_col[i];  break;      
    case 4: v_positions1_neck[i] = v_pos[i];  skin_color[i] = v_col[i];   break;
    case 5: v_positions1_head[i] = v_pos[i];  break;
    case 6: v_positions1_uleg[i] = v_pos[i];  break;
    case 7: v_positions1_lleg[i] = v_pos[i];  break;
    case 8: v_positions1_uarm[i] = v_pos[i];  break;
    case 9: v_positions1_larm[i] = v_pos[i];  break;
    case 10: v_positions2_torso[i] = v_pos[i];jean_color[i] = v_col[i];   break;      
    case 11: v_positions2_neck[i] = v_pos[i]; minion_color[i] = v_col[i]; break;
    case 12: v_positions2_head[i] = v_pos[i]; break;
    case 13: v_positions2_uleg[i] = v_pos[i]; break;
    case 14: v_positions2_lleg[i] = v_pos[i]; break;
    case 15: v_positions2_uarm[i] = v_pos[i]; break;
    case 16: v_positions2_larm[i] = v_pos[i]; break;
    case 17: v_positions2_band[i] = v_pos[i]; break;
    case 18: v_positions2_eye[i] = v_pos[i];  eye_color[i] = v_col[i];    break;
    case 19: v_positions2_eyeball[i] = v_pos[i]; break;
    case 20: v_positions_table_top[i] = v_pos[i]; break;
    case 21: v_positions_table_legs[i] = v_pos[i]; break;
    case 22: v_positions_sofa_bottom[i] = v_pos[i]; break;
    case 23: v_positions_sofa_padding[i] = v_pos[i]; break;
    case 24: v_positions_sofa_arms[i] = v_pos[i]; break;
    case 25: v_positions_sofa_back[i] = v_pos[i]; break;
    case 26: v_positions_bookcase_legs[i] = v_pos[i]; break;
    case 27: v_positions_bookcase_back[i] = v_pos[i]; break;
    case 28: v_positions_bookcase_slabs[i] = v_pos[i]; break;
    case 29: v_positions_lamp_stand[i] = v_pos[i]; break;
    case 30: v_positions_lamp_light[i] = v_pos[i]; break;
    case 31: v_positions_tv_leg[i] = v_pos[i]; break;
    case 32: v_positions_tv_face[i] = v_pos[i]; break;
    case 33: v_positions_tv_neck[i] = v_pos[i]; break;
    case 34: v_positions_tv_screen[i] = v_pos[i]; break;
    case 35: v_positions_tvstand_back[i] = v_pos[i]; break;
    case 36: v_positions_tvstand_horizontal[i] = v_pos[i]; break;
    case 37: v_positions_tvstand_virtical[i] = v_pos[i]; break;
    case 38: v_positions_tvstand_feet[i] = v_pos[i]; break;
    case 39: v_positions_tvstand_slab[i] = v_pos[i]; break;
    case 40: v_positions_room_front[i] = v_pos[i]; break;
    case 41: v_positions_room_top[i] = v_pos[i]; break;
    case 42: v_positions_room_side[i] = v_pos[i]; break;
    case 43: v_positions_room_small[i] = v_pos[i]; break;
    case 44: v_positions_door[i] = v_pos[i]; break;
    case 45: v_positions_door_nob[i] = v_pos[i]; break;
    case 46: v_positions_window_frame[i] = v_pos[i]; break;
    case 47: v_positions_window_bars[i] = v_pos[i]; break;
    case 48: v_positions_lightfixture[i] = v_pos[i]; break;
    default: break;
    }
  }
}
//-----------------------------------------------------------------


void quad_normals(int a, int b, int c, int d, glm::vec4 nor[num_pos], glm::vec4 v_nor[num_vertices] )//idk why last one works without &
{
  v_nor[nor_idx] = nor[a]; nor_idx++;
  v_nor[nor_idx] = nor[b]; nor_idx++;
  v_nor[nor_idx] = nor[c]; nor_idx++;
  v_nor[nor_idx] = nor[a]; nor_idx++;
  v_nor[nor_idx] = nor[c]; nor_idx++;
  v_nor[nor_idx] = nor[d]; nor_idx++;
}

void calculate_normals(int polygon_type)
{
  nor_idx = 0;
  glm::vec4 v_nor[num_vertices];
  glm::vec4 nor[num_pos];
  glm::vec4 avg_vertex = glm::vec4(0.0, 0.0, 0.0, 0.0);

  for(int i = 0; i < num_pos; i++)
  {
    switch(polygon_type)
    {  
    case 1: nor[i] = positions1[i];break;
    case 2: nor[i] = positions1_lid[i];break;
    case 3: nor[i] = positions1_torso[i];break;
    case 4: nor[i] = positions1_neck[i];break;
    case 5: nor[i] = positions1_head[i];break;
    case 6: nor[i] = positions1_uleg[i];break;
    case 7: nor[i] = positions1_lleg[i];break;
    case 8: nor[i] = positions1_uarm[i];break;
    case 9: nor[i] = positions1_larm[i];break;
    case 10: nor[i] = positions2_torso[i];break;
    case 11: nor[i] = positions2_neck[i];break;    
    case 12: nor[i] = positions2_head[i];break;
    case 13: nor[i] = positions2_uleg[i];break;
    case 14: nor[i] = positions2_lleg[i];break;
    case 15: nor[i] = positions2_uarm[i];break;
    case 16: nor[i] = positions2_larm[i];break;
    case 17: nor[i] = positions2_band[i];break;
    case 18: nor[i] = positions2_eye[i];break;
    case 19: nor[i] = positions2_eyeball[i];break;
    case 20: nor[i] = position_table_top[i];break;
    case 21: nor[i] = position_table_legs[i];break;
    case 22: nor[i] = position_sofa_bottom[i];break;
    case 23: nor[i] = position_sofa_padding[i];break;
    case 24: nor[i] = position_sofa_arms[i];break;
    case 25: nor[i] = position_sofa_back[i];break;
    case 26: nor[i] = position_bookcase_legs[i];break;
    case 27: nor[i] = position_bookcase_back[i];break;
    case 28: nor[i] = position_bookcase_slabs[i];break;
    case 29: nor[i] = position_lamp_stand[i];break;
    case 30: nor[i] = position_lamp_light[i];break;
    case 31: nor[i] = position_tv_leg[i];break;
    case 32: nor[i] = position_tv_face[i];break;
    case 33: nor[i] = position_tv_neck[i];break;
    case 34: nor[i] = position_tv_screen[i];break;
    case 35: nor[i] = position_tvstand_back[i];break;
    case 36: nor[i] = position_tvstand_horizontal[i];break;
    case 37: nor[i] = position_tvstand_virtical[i];break;
    case 38: nor[i] = position_tvstand_feet[i];break;
    case 39: nor[i] = position_tvstand_slab[i];break;
    case 40: nor[i] = position_room_front[i];break;
    case 41: nor[i] = position_room_top[i];break;
    case 42: nor[i] = position_room_side[i];break;
    case 43: nor[i] = position_room_small[i];break;
    case 44: nor[i] = position_door[i];break;
    case 45: nor[i] = position_door_nob[i];break;
    case 46: nor[i] = position_window_frame[i];break;
    case 47: nor[i] = position_window_bars[i];break;
    case 48: nor[i] = position_lightfixture[i];break;
    default: break;
    }

  }

  for(int i = 0; i < num_pos ; i++){
    avg_vertex = avg_vertex + nor[i];
  }

  //avg_vertex = avg_vertex;
  avg_vertex = avg_vertex*(1/(float)num_pos);
  avg_vertex = avg_vertex - glm::vec4(0.0, 0.0, 0.0, 1.0); 

  for(int i=0; i < num_pos ; i++){
    nor[i] = nor[i] - avg_vertex;
  }

  quad_normals( 1, 0, 3, 2, nor, v_nor);
  quad_normals( 2, 3, 7, 6, nor, v_nor);
  quad_normals( 3, 0, 4, 7, nor, v_nor);
  quad_normals( 6, 5, 1, 2, nor, v_nor);
  quad_normals( 4, 5, 6, 7, nor, v_nor);
  quad_normals( 5, 4, 0, 1, nor, v_nor); 

  for(int i=0; i <num_vertices ; i++)
  {
    switch(polygon_type)
    {  
    case 1:  v_normals1[i] = v_nor[i];   break;   
    case 2:  v_normals1_lid[i] = v_nor[i];   break;         
    case 3:  v_normals1_torso[i] = v_nor[i]; break;      
    case 4:  v_normals1_neck[i] = v_nor[i];  break;
    case 5:  v_normals1_head[i] = v_nor[i];  break;
    case 6:  v_normals1_uleg[i] = v_nor[i];  break;
    case 7:  v_normals1_lleg[i] = v_nor[i];  break;
    case 8:  v_normals1_uarm[i] = v_nor[i];  break;
    case 9:  v_normals1_larm[i] = v_nor[i];  break;
    case 10: v_normals2_torso[i] = v_nor[i];break;      
    case 11: v_normals2_neck[i] = v_nor[i]; break;
    case 12: v_normals2_head[i] = v_nor[i]; break;
    case 13: v_normals2_uleg[i] = v_nor[i]; break;
    case 14: v_normals2_lleg[i] = v_nor[i]; break;
    case 15: v_normals2_uarm[i] = v_nor[i]; break;
    case 16: v_normals2_larm[i] = v_nor[i]; break;
    case 17: v_normals2_band[i] = v_nor[i]; break;
    case 18: v_normals2_eye[i] = v_nor[i];  break;
    case 19: v_normals2_eyeball[i] = v_nor[i]; break;
    case 20: v_normals_table_top[i] = v_nor[i]; break;
    case 21: v_normals_table_legs[i] = v_nor[i]; break;
    case 22: v_normals_sofa_bottom[i] = v_nor[i]; break;
    case 23: v_normals_sofa_padding[i] = v_nor[i]; break;
    case 24: v_normals_sofa_arms[i] = v_nor[i]; break;
    case 25: v_normals_sofa_back[i] = v_nor[i]; break;
    case 26: v_normals_bookcase_legs[i] = v_nor[i]; break;
    case 27: v_normals_bookcase_back[i] = v_nor[i]; break;
    case 28: v_normals_bookcase_slabs[i] = v_nor[i]; break;
    case 29: v_normals_lamp_stand[i] = v_nor[i]; break;
    case 30: v_normals_lamp_light[i] = v_nor[i]; break;
    case 31: v_normals_tv_leg[i] = v_nor[i]; break;
    case 32: v_normals_tv_face[i] = v_nor[i]; break;
    case 33: v_normals_tv_neck[i] = v_nor[i]; break;
    case 34: v_normals_tv_screen[i] = v_nor[i]; break;
    case 35: v_normals_tvstand_back[i] = v_nor[i]; break;
    case 36: v_normals_tvstand_horizontal[i] = v_nor[i]; break;
    case 37: v_normals_tvstand_virtical[i] = v_nor[i]; break;
    case 38: v_normals_tvstand_feet[i] = v_nor[i]; break;
    case 39: v_normals_tvstand_slab[i] = v_nor[i]; break;
    case 40: v_normals_room_front[i] = v_nor[i]; break;
    case 41: v_normals_room_top[i] = v_nor[i]; break;
    case 42: v_normals_room_side[i] = v_nor[i]; break;
    case 43: v_normals_room_small[i] = v_nor[i]; break;
    case 44: v_normals_door[i] = v_nor[i]; break;
    case 45: v_normals_door_nob[i] = v_nor[i]; break;
    case 46: v_normals_window_frame[i] = v_nor[i]; break;
    case 47: v_normals_window_bars[i] = v_nor[i]; break;
    case 48: v_normals_lightfixture[i] = v_nor[i]; break;
    default: break;
    }
  }
}


void initBuffersGL(void)
{

  // Load shaders and use the resulting shader program
  std::string vertex_shader_file("vshader.glsl");
  std::string fragment_shader_file("fshader.glsl");

  std::vector<GLuint> shaderList;
  shaderList.push_back(csX75::LoadShaderGL(GL_VERTEX_SHADER, vertex_shader_file));
  shaderList.push_back(csX75::LoadShaderGL(GL_FRAGMENT_SHADER, fragment_shader_file));

  shaderProgram = csX75::CreateProgramGL(shaderList);
  glUseProgram( shaderProgram );

  // getting the attributes from the shader program
  vPosition = glGetAttribLocation( shaderProgram, "vPosition" );
  vColor = glGetAttribLocation( shaderProgram, "vColor" ); 
  vNormal = glGetAttribLocation( shaderProgram, "vNormal" ); 
  texCoord = glGetAttribLocation( shaderProgram, "texCoord" );

  textured = glGetUniformLocation( shaderProgram, "textured" ); // True if the current model is textured

  // Lighting 
  light1_isOn = glGetUniformLocation( shaderProgram, "light1_isOn" );  
  light2_isOn = glGetUniformLocation( shaderProgram, "light2_isOn" );  

  normalMatrix =  glGetUniformLocation( shaderProgram, "normalMatrix");
  viewMatrix = glGetUniformLocation( shaderProgram, "viewMatrix");
  uModelViewMatrix = glGetUniformLocation( shaderProgram, "uModelViewMatrix");

  // Creating the hierarchy:
  // We are using the original colorcube function to generate the vertices of the cuboid
  for(int polygon_type=1; polygon_type < 50; polygon_type++) // there are 20 types of polyogns(box+lid+bodyparts of model1);
  {
    colorcube(polygon_type); 
    calculate_normals(polygon_type);
  }

  tex_idx = 0;
  texturing( 1, 0, 3, 2);
  texturing( 2, 3, 7, 6);
  texturing( 3, 0, 4, 7);
  texturing( 6, 5, 1, 2);
  texturing( 4, 5, 6, 7);
  texturing( 5, 4, 0, 1); 
  // define box

  model0 = new csX75::Model();
  model0->torso = new csX75::HNode(NULL,num_vertices,v_positions1,box_color,tex_coords,v_normals1,sizeof(v_positions1),sizeof(box_color),sizeof(tex_coords),wood);
  model0->torso->change_parameters(0.0,-0.5,-2.0,0.0,0.0,0.0);
  model0->neck = new csX75::HNode(model0->torso,num_vertices,v_positions1_lid,box_color,tex_coords,v_normals1_lid,sizeof(v_positions1_lid),sizeof(box_color),sizeof(tex_coords),wood);
  model0->neck->change_parameters(0.0,3.75,-3.0,0.0,0.0,0.0);
  model0->root_node = model0 -> torso;    

  // Define Model 1
  model1 = new csX75::Model();
  model1->torso = new csX75::HNode(NULL,num_vertices,v_positions1_torso,shirt_color,tex_coords,v_normals1_torso,sizeof(v_positions1_torso),sizeof(shirt_color),sizeof(tex_coords),"");
  model1->torso->change_parameters(-2.0,0.2,-2.0,0.0,0.0,0.0);
  model1->neck = new csX75::HNode(model1->torso,num_vertices,v_positions1_neck,skin_color,tex_coords,v_normals1_neck,sizeof(v_positions1_neck),sizeof(skin_color),sizeof(tex_coords),"");
  model1->neck->change_parameters(0.0,1.3,0.0,0.0,0.0,0.0);
  model1->head = new csX75::HNode(model1->neck,num_vertices,v_positions1_head,skin_color,tex_coords,v_normals1_head, sizeof(v_positions1_head),sizeof(skin_color),sizeof(tex_coords),"");
  model1->head->change_parameters(0.0,0.7,0.0,0.0,0.0,0.0);
  model1->ThighL = new csX75::HNode(model1->torso,num_vertices,v_positions1_uleg,jean_color,tex_coords,v_normals1_uleg,sizeof(v_positions1_uleg),sizeof(jean_color),sizeof(tex_coords),"");
  model1->ThighL->change_parameters(0.42,-1.2,0.0,0.0,0.0,0.0);
  model1->ThighR = new csX75::HNode(model1->torso,num_vertices,v_positions1_uleg,jean_color,tex_coords,v_normals1_uleg,sizeof(v_positions1_uleg),sizeof(jean_color),sizeof(tex_coords),"");
  model1->ThighR->change_parameters(-0.42,-1.2,0.0,0.0,0.0,0.0);
  model1->LegL = new csX75::HNode(model1->ThighL,num_vertices,v_positions1_lleg,skin_color,tex_coords,v_normals1_lleg,sizeof(v_positions1_lleg),sizeof(skin_color),sizeof(tex_coords),"");
  model1->LegL->change_parameters(0.0,-1.6,0.0,0.0,0.0,0.0);
  model1->LegR = new csX75::HNode(model1->ThighR,num_vertices,v_positions1_lleg,skin_color,tex_coords,v_normals1_lleg, sizeof(v_positions1_lleg),sizeof(skin_color),sizeof(tex_coords),"");
  model1->LegR->change_parameters(-0.0,-1.6,0.0,0.0,0.0,0.0);
  model1->UArmL = new csX75::HNode(model1->torso,num_vertices,v_positions1_uarm,shirt_color,tex_coords,v_normals1_uarm,sizeof(v_positions1_uarm),sizeof(shirt_color),sizeof(tex_coords),"");
  model1->UArmL->change_parameters(1.3,1.1,0.0,0.0,0.0,0.0);
  model1->UArmR = new csX75::HNode(model1->torso,num_vertices,v_positions1_uarm,shirt_color,tex_coords,v_normals1_uarm,sizeof(v_positions1_uarm),sizeof(shirt_color),sizeof(tex_coords),"");
  model1->UArmR->change_parameters(-1.3,1.1,0.0,0.0,0.0,0.0);
  model1->LArmL = new csX75::HNode(model1->UArmL,num_vertices,v_positions1_larm,skin_color,tex_coords,v_normals1_larm,sizeof(v_positions1_larm),sizeof(skin_color),sizeof(tex_coords),"");
  model1->LArmL->change_parameters(0.0,-1.3,0.0,0.0,0.0,0.0);
  model1->LArmR = new csX75::HNode(model1->UArmR,num_vertices,v_positions1_larm,skin_color,tex_coords,v_normals1_larm,sizeof(v_positions1_larm),sizeof(skin_color),sizeof(tex_coords),"");
  model1->LArmR->change_parameters(0.0,-1.3,0.0,0.0,0.0,0.0);
  model1->root_node = model1->torso;
  curr_selected_node =  model1->root_node;
  selected_model = model1;

  // Define Model 2
  model2 = new csX75::Model();
  model2->torso = new csX75::HNode(NULL,num_vertices,v_positions2_torso,jean_color,tex_coords,v_normals2_torso,sizeof(v_positions2_torso),sizeof(jean_color),sizeof(tex_coords),"");
  model2->torso->change_parameters(2.0,-2.5,-2.0,0.0,0.0,0.0);
  model2->neck = new csX75::HNode(model2->torso,num_vertices,v_positions2_neck,minion_color,tex_coords,v_normals2_neck,sizeof(v_positions2_neck),sizeof(minion_color),sizeof(tex_coords),"");
  model2->neck->change_parameters(0.0,0.0,0.0,0.0,0.0,0.0);
  model2->head = new csX75::HNode(model2->neck,num_vertices,v_positions2_head,minion_color,tex_coords,v_normals2_head, sizeof(v_positions2_head),sizeof(minion_color),sizeof(tex_coords),"");
  model2->head->change_parameters(0.0,1.0,-0.6,0.0,0.0,0.0);
  model2->ThighL = new csX75::HNode(model2->torso,num_vertices,v_positions2_uleg,jean_color,tex_coords,v_normals2_uleg,sizeof(v_positions2_uleg),sizeof(jean_color),sizeof(tex_coords),"");
  model2->ThighL->change_parameters(0.5,-1.0,0.0,0.0,0.0,0.0);
  model2->ThighR = new csX75::HNode(model2->torso,num_vertices,v_positions2_uleg,jean_color,tex_coords,v_normals2_uleg,sizeof(v_positions2_uleg),sizeof(jean_color),sizeof(tex_coords),"");
  model2->ThighR->change_parameters(-1.0,-1.0,0.0,0.0,0.0,0.0);
  model2->LegL = new csX75::HNode(model2->ThighL,num_vertices,v_positions2_lleg,shirt_color,tex_coords,v_normals2_lleg,sizeof(v_positions2_lleg),sizeof(shirt_color),sizeof(tex_coords),"");
  model2->LegL->change_parameters(0.0,-0.5,0.0,0.0,0.0,0.0);
  model2->LegR = new csX75::HNode(model2->ThighR,num_vertices,v_positions2_lleg,shirt_color,tex_coords,v_normals2_lleg,sizeof(v_positions2_lleg),sizeof(shirt_color),sizeof(tex_coords),"");
  model2->LegR->change_parameters(0.0,-0.5,0.0,0.0,0.0,0.0);
  model2->UArmL = new csX75::HNode(model2->torso,num_vertices,v_positions2_uarm,minion_color,tex_coords,v_normals2_uarm,sizeof(v_positions2_uarm),sizeof(minion_color),sizeof(tex_coords),"");
  model2->UArmL->change_parameters(1.1,0.0,0.0,0.0,0.0,0.0);
  model2->UArmR = new csX75::HNode(model2->torso,num_vertices,v_positions2_uarm,minion_color,tex_coords,v_normals2_uarm,sizeof(v_positions2_uarm),sizeof(minion_color),sizeof(tex_coords),"");
  model2->UArmR->change_parameters(-1.1,0.0,0.0,0.0,0.0,0.0);
  model2->LArmL = new csX75::HNode(model2->UArmL,num_vertices,v_positions2_larm,shirt_color,tex_coords,v_normals2_larm,sizeof(v_positions2_larm),sizeof(shirt_color),sizeof(tex_coords),"");
  model2->LArmL->change_parameters(0.0,-1.1,0.0,0.0,0.0,0.0);
  model2->LArmR = new csX75::HNode(model2->UArmR,num_vertices,v_positions2_larm,shirt_color,tex_coords,v_normals2_larm,sizeof(v_positions2_larm),sizeof(shirt_color),sizeof(tex_coords),"");
  model2->LArmR->change_parameters(0.0,-1.1,0.0,0.0,0.0,0.0);
  model2->band = new csX75::HNode(model2->head,num_vertices,v_positions2_band,shirt_color,tex_coords,v_normals2_band,sizeof(v_positions2_band),sizeof(shirt_color),sizeof(tex_coords),"");
  model2->band->change_parameters(0.0,0.4,0.0,0.0,0.0,0.0);
  model2->eye = new csX75::HNode(model2->head,num_vertices,v_positions2_eye,eye_color,tex_coords,v_normals2_eye,sizeof(v_positions2_eye),sizeof(eye_color),sizeof(tex_coords),"");
  model2->eye->change_parameters(0.0,0.25,1.2,0.0,0.0,0.0);
  model2->eyeball = new csX75::HNode(model2->head,num_vertices,v_positions2_eyeball,shirt_color,tex_coords,v_normals2_eyeball,sizeof(v_positions2_eyeball),sizeof(shirt_color),sizeof(tex_coords),"");
  model2->eyeball->change_parameters(0.0,0.4,1.25,0.0,0.0,0.0);
  model2->root_node = model2->torso;

  table = new csX75::Model();
  table->torso = new csX75::HNode(NULL,num_vertices,v_positions_table_top,box_color,tex_coords,v_normals_table_top,sizeof(v_positions_table_top),sizeof(box_color),sizeof(tex_coords),dark_wood);
  table->torso->change_parameters(0.0,-5.0,0.0,0.0,0.0,0.0);
  table->LegL = new csX75::HNode(table->torso,num_vertices,v_positions_table_legs,box_color,tex_coords,v_normals_table_legs,sizeof(v_positions_table_legs),sizeof(box_color),sizeof(tex_coords),dark_wood);
  table->LegL->change_parameters(-9.0,-7.0,9.0,0.0,0.0,0.0);
  table->LegR = new csX75::HNode(table->torso,num_vertices,v_positions_table_legs,box_color,tex_coords,v_normals_table_legs,sizeof(v_positions_table_legs),sizeof(box_color),sizeof(tex_coords),dark_wood);
  table->LegR->change_parameters(9.0,-7.0,9.0,0.0,0.0,0.0);
  table->ThighL = new csX75::HNode(table->torso,num_vertices,v_positions_table_legs,box_color,tex_coords,v_normals_table_legs,sizeof(v_positions_table_legs),sizeof(box_color),sizeof(tex_coords),dark_wood);
  table->ThighL->change_parameters(-9.0,-7.0,-9.0,0.0,0.0,0.0);
  table->ThighR = new csX75::HNode(table->torso,num_vertices,v_positions_table_legs,box_color,tex_coords,v_normals_table_legs,sizeof(v_positions_table_legs),sizeof(box_color),sizeof(tex_coords),dark_wood);
  table->ThighR->change_parameters(9.0,-7.0,-9.0,0.0,0.0,0.0);
  table->root_node = table->torso;

  sofa = new csX75::Model();
  sofa->torso = new csX75::HNode(NULL,num_vertices,v_positions_sofa_bottom,jean_color,tex_coords,v_normals_sofa_bottom,sizeof(v_positions_sofa_bottom),sizeof(jean_color),sizeof(tex_coords),sofa_dark);
  sofa->torso->change_parameters(0.0,-14.0,-25.0,0.0,0.0,0.0);
  sofa->head = new csX75::HNode(sofa->torso,num_vertices,v_positions_sofa_padding,jean_color,tex_coords,v_normals_sofa_padding,sizeof(v_positions_sofa_padding),sizeof(jean_color),sizeof(tex_coords),sofa_dark);
  sofa->head->change_parameters(7.5,4.0,0.0,0.0,0.0,0.0);
  sofa->neck = new csX75::HNode(sofa->torso,num_vertices,v_positions_sofa_padding,jean_color,tex_coords,v_normals_sofa_padding,sizeof(v_positions_sofa_padding),sizeof(jean_color),sizeof(tex_coords),sofa_dark);
  sofa->neck->change_parameters(-7.5,4.0,0.0,0.0,0.0,0.0);
  sofa->LArmR = new csX75::HNode(sofa->torso,num_vertices,v_positions_sofa_arms,jean_color,tex_coords,v_normals_sofa_arms,sizeof(v_positions_sofa_arms),sizeof(jean_color),sizeof(tex_coords),sofa_dark);
  sofa->LArmR->change_parameters(17.0,6.0,-3.0,0.0,0.0,0.0);
  sofa->LArmL = new csX75::HNode(sofa->torso,num_vertices,v_positions_sofa_arms,jean_color,tex_coords,v_normals_sofa_arms,sizeof(v_positions_sofa_arms),sizeof(jean_color),sizeof(tex_coords),sofa_dark);
  sofa->LArmL->change_parameters(-17.0,6.0,-3.0,0.0,0.0,0.0);
  sofa->LegR = new csX75::HNode(sofa->torso,num_vertices,v_positions_sofa_back,jean_color,tex_coords,v_normals_sofa_back,sizeof(v_positions_sofa_back),sizeof(jean_color),sizeof(tex_coords),sofa_dark);
  sofa->LegR->change_parameters(-7.5,10.0,-11.0,0.0,0.0,0.0);
  sofa->LegL = new csX75::HNode(sofa->torso,num_vertices,v_positions_sofa_back,jean_color,tex_coords,v_normals_sofa_back,sizeof(v_positions_sofa_back),sizeof(jean_color),sizeof(tex_coords),sofa_dark);
  sofa->LegL->change_parameters(7.5,10.0,-11.0,0.0,0.0,0.0);
  sofa->root_node = sofa->torso;

  bookcase = new csX75::Model();
  bookcase->torso = new csX75::HNode(NULL,num_vertices,v_positions_bookcase_back,box_color,tex_coords,v_normals_bookcase_back, sizeof(v_positions_bookcase_back),sizeof(box_color),sizeof(tex_coords),dark_wood);
  bookcase->torso->change_parameters(30.0,-5.0,-40.0,0.0,0.0,0.0);
  bookcase->LegL = new csX75::HNode(bookcase->torso,num_vertices,v_positions_bookcase_legs,box_color,tex_coords,v_normals_bookcase_legs,sizeof(v_positions_bookcase_legs),sizeof(box_color),sizeof(tex_coords),dark_wood);
  bookcase->LegL->change_parameters(8.0,0.0,3.0,0.0,0.0,0.0);
  bookcase->LegR = new csX75::HNode(bookcase->torso,num_vertices,v_positions_bookcase_legs,box_color,tex_coords,v_normals_bookcase_legs,sizeof(v_positions_bookcase_legs),sizeof(box_color),sizeof(tex_coords),dark_wood);
  bookcase->LegR->change_parameters(-8.0,0.0,3.0,0.0,0.0,0.0);
  bookcase->head = new csX75::HNode(bookcase->torso,num_vertices,v_positions_bookcase_slabs,box_color,tex_coords,v_normals_bookcase_slabs,sizeof(v_positions_bookcase_slabs),sizeof(box_color),sizeof(tex_coords),dark_wood);
  bookcase->head->change_parameters(0.0,8.0,3.0,0.0,0.0,0.0);
  bookcase->neck = new csX75::HNode(bookcase->torso,num_vertices,v_positions_bookcase_slabs,box_color,tex_coords,v_normals_bookcase_slabs,sizeof(v_positions_bookcase_slabs),sizeof(box_color),sizeof(tex_coords),dark_wood);
  bookcase->neck->change_parameters(0.0,13.5,3.0,0.0,0.0,0.0);
  bookcase->ThighL = new csX75::HNode(bookcase->torso,num_vertices,v_positions_bookcase_slabs,box_color,tex_coords,v_normals_bookcase_slabs,sizeof(v_positions_bookcase_slabs),sizeof(box_color),sizeof(tex_coords),dark_wood);
  bookcase->ThighL->change_parameters(0.0,2.5,3.0,0.0,0.0,0.0);
  bookcase->ThighR = new csX75::HNode(bookcase->torso,num_vertices,v_positions_bookcase_slabs,box_color,tex_coords,v_normals_bookcase_slabs,sizeof(v_positions_bookcase_slabs),sizeof(box_color),sizeof(tex_coords),dark_wood);
  bookcase->ThighR->change_parameters(0.0,-3.0,3.0,0.0,0.0,0.0);
  bookcase->LArmL = new csX75::HNode(bookcase->torso,num_vertices,v_positions_bookcase_slabs,box_color,tex_coords,v_normals_bookcase_slabs,sizeof(v_positions_bookcase_slabs),sizeof(box_color),sizeof(tex_coords),dark_wood);
  bookcase->LArmL->change_parameters(0.0,-8.5,3.0,0.0,0.0,0.0);
  bookcase->root_node = bookcase->torso;

  lamp = new csX75::Model();
  lamp->torso = new csX75::HNode(bookcase->torso,num_vertices,v_positions_lamp_stand,shirt_color,tex_coords,v_normals_lamp_stand,sizeof(v_positions_lamp_stand),sizeof(shirt_color),sizeof(tex_coords),white);
  lamp->torso->change_parameters(0.0,17.0,0.0,0.0,0.0,0.0);
  lamp->head = new csX75::HNode(lamp->torso,num_vertices,v_positions_lamp_light,minion_color,tex_coords,v_normals_lamp_light,sizeof(v_positions_lamp_light),sizeof(minion_color),sizeof(tex_coords),"");
  lamp->head->change_parameters(0.0,4.0,0.0,0.0,0.0,0.0);
  lamp->root_node = lamp->torso;

  tv_stand = new csX75::Model();
  tv_stand->torso = new csX75::HNode(NULL,num_vertices,v_positions_tvstand_back,box_color,tex_coords,v_normals_tvstand_back,sizeof(v_positions_tvstand_back),sizeof(box_color),sizeof(tex_coords),dark_wood);
  tv_stand->torso->change_parameters(0.0,-10.0,35.0,0.0,180.0,0.0);
  tv_stand->neck = new csX75::HNode(tv_stand->torso,num_vertices,v_positions_tvstand_horizontal,box_color,tex_coords,v_normals_tvstand_horizontal,sizeof(v_positions_tvstand_horizontal),sizeof(box_color),sizeof(tex_coords),dark_wood);
  tv_stand->neck->change_parameters(0.0,5.5,4.5,0.0,0.0,0.0);
  tv_stand->head = new csX75::HNode(tv_stand->torso,num_vertices,v_positions_tvstand_horizontal,box_color,tex_coords,v_normals_tvstand_horizontal, sizeof(v_positions_tvstand_horizontal),sizeof(box_color),sizeof(tex_coords),dark_wood);
  tv_stand->head->change_parameters(0.0,-5.5,4.5,0.0,0.0,0.0);
  tv_stand->LArmL = new csX75::HNode(tv_stand->torso,num_vertices,v_positions_tvstand_virtical,box_color,tex_coords,v_normals_tvstand_virtical,sizeof(v_positions_tvstand_virtical),sizeof(box_color),sizeof(tex_coords),dark_wood);
  tv_stand->LArmL->change_parameters(-12.0,0.0,5.0,0.0,0.0,0.0);
  tv_stand->LArmR = new csX75::HNode(tv_stand->torso,num_vertices,v_positions_tvstand_virtical,box_color,tex_coords,v_normals_tvstand_virtical,sizeof(v_positions_tvstand_virtical),sizeof(box_color),sizeof(tex_coords),dark_wood);
  tv_stand->LArmR->change_parameters(12.0,0.0,4.5,0.0,0.0,0.0);
  tv_stand->UArmL = new csX75::HNode(tv_stand->torso,num_vertices,v_positions_tvstand_virtical,box_color,tex_coords,v_normals_tvstand_virtical,sizeof(v_positions_tvstand_virtical),sizeof(box_color),sizeof(tex_coords),dark_brown_wood);
  tv_stand->UArmL->change_parameters(5.0,0.0,4.5,0.0,0.0,0.0);
  tv_stand->UArmR = new csX75::HNode(tv_stand->torso,num_vertices,v_positions_tvstand_virtical,box_color,tex_coords,v_normals_tvstand_virtical,sizeof(v_positions_tvstand_virtical),sizeof(box_color),sizeof(tex_coords),dark_brown_wood);
  tv_stand->UArmR->change_parameters(-5.0,0.0,4.5,0.0,0.0,0.0);
  tv_stand->eye = new csX75::HNode(tv_stand->torso,num_vertices,v_positions_tvstand_slab,box_color,tex_coords,v_normals_tvstand_slab,sizeof(v_positions_tvstand_slab),sizeof(box_color),sizeof(tex_coords),dark_brown_wood);
  tv_stand->eye->change_parameters(0.0,0.0,4.5,0.0,0.0,0.0);
  tv_stand->LegL = new csX75::HNode(tv_stand->torso,num_vertices,v_positions_tvstand_feet,box_color,tex_coords,v_normals_tvstand_feet,sizeof(v_positions_tvstand_feet),sizeof(box_color),sizeof(tex_coords),dark_wood);
  tv_stand->LegL->change_parameters(11.0,-7.0,1.0,0.0,0.0,0.0);
  tv_stand->LegR = new csX75::HNode(tv_stand->torso,num_vertices,v_positions_tvstand_feet,box_color,tex_coords,v_normals_tvstand_feet,sizeof(v_positions_tvstand_feet),sizeof(box_color),sizeof(tex_coords),dark_wood);
  tv_stand->LegR->change_parameters(-11.0,-7.0,1.0,0.0,0.0,0.0);
  tv_stand->ThighL = new csX75::HNode(tv_stand->torso,num_vertices,v_positions_tvstand_feet,box_color,tex_coords,v_normals_tvstand_feet,sizeof(v_positions_tvstand_feet),sizeof(box_color),sizeof(tex_coords),dark_wood);
  tv_stand->ThighL->change_parameters(-11.0,-7.0,8.5,0.0,0.0,0.0);
  tv_stand->ThighR = new csX75::HNode(tv_stand->torso,num_vertices,v_positions_tvstand_feet,box_color,tex_coords,v_normals_tvstand_feet, sizeof(v_positions_tvstand_feet),sizeof(box_color),sizeof(tex_coords),dark_wood);
  tv_stand->ThighR->change_parameters(11.0,-7.0,8.5,0.0,0.0,0.0);
  tv_stand->root_node = tv_stand->torso;

  tv = new csX75::Model();
  tv->torso = new csX75::HNode(tv_stand->torso,num_vertices,v_positions_tv_leg,shirt_color,tex_coords,v_normals_tv_leg, sizeof(v_positions_tv_leg),sizeof(shirt_color),sizeof(tex_coords),tv_body);
  tv->torso->change_parameters(0.0,6.0,4.5,0.0,0.0,0.0);
  tv->head = new csX75::HNode(tv->torso,num_vertices,v_positions_tv_face,shirt_color,tex_coords,v_normals_tv_face,sizeof(v_positions_tv_face),sizeof(shirt_color),sizeof(tex_coords),tv_body);
  tv->head->change_parameters(0.0,10.0,0.0,0.0,0.0,0.0);
  tv->neck = new csX75::HNode(tv->torso,num_vertices,v_positions_tv_neck,shirt_color,tex_coords,v_normals_tv_neck,sizeof(v_positions_tv_neck),sizeof(shirt_color),sizeof(tex_coords),tv_body);
  tv->neck->change_parameters(0.0,2.0,-0.5,0.0,0.0,0.0);
  tv->LArmL = new csX75::HNode(tv->head,num_vertices,v_positions_tv_screen,shirt_color,tex_coords,v_normals_tv_screen,sizeof(v_positions_tv_screen),sizeof(shirt_color),sizeof(tex_coords),tv_screen);
  tv->LArmL->change_parameters(0.0,0.0,0.5,0.0,0.0,0.0);
  tv->root_node = tv->torso;

  room = new csX75::Model();
  room->torso = new csX75::HNode(NULL,num_vertices,v_positions_room_front,shirt_color,tex_coords,v_normals_room_front,sizeof(v_positions_room_front),sizeof(shirt_color),sizeof(tex_coords),curtains);
  room->torso->change_parameters(0.0,8.0,-45.0,0.0,0.0,0.0);

  //room->head = new csX75::HNode(room->torso,num_vertices,v_positions_room_front,shirt_color,tex_coords,sizeof(v_positions_room_front),sizeof(shirt_color),sizeof(tex_coords),wall2);
  //room->head->change_parameters(-16.0,0.0,83.0,0.0,0.0,0.0);

  room->head = new csX75::HNode(room->torso,num_vertices,v_positions_door,shirt_color,tex_coords,v_normals_door,sizeof(v_positions_door),sizeof(shirt_color),sizeof(tex_coords),smooth);
  room->head->change_parameters(31.0,40.5,83.0,0.0,0.0,0.0);

  room->neck = new csX75::HNode(room->torso,num_vertices,v_positions_door,shirt_color,tex_coords,v_normals_door,sizeof(v_positions_door),sizeof(shirt_color),sizeof(tex_coords),smooth);
  room->neck->change_parameters(15.0,40.5,83.0,0.0,0.0,0.0);
  room->ThighR = new csX75::HNode(room->torso,num_vertices,v_positions_door,shirt_color,tex_coords,v_normals_door,sizeof(v_positions_door),sizeof(shirt_color),sizeof(tex_coords),smooth);
  room->ThighR->change_parameters(-1.0,40.5,83.0,0.0,0.0,0.0);
  room->ThighL = new csX75::HNode(room->torso,num_vertices,v_positions_door,shirt_color,tex_coords,v_normals_door,sizeof(v_positions_door),sizeof(shirt_color),sizeof(tex_coords),smooth);
  room->ThighL->change_parameters(15.0,-5.0,83.0,0.0,0.0,0.0);
  room->LegR = new csX75::HNode(room->torso,num_vertices,v_positions_door,shirt_color,tex_coords,v_normals_door,sizeof(v_positions_door),sizeof(shirt_color),sizeof(tex_coords),smooth);
  room->LegR->change_parameters(-1.0,-5.0,83.0,0.0,0.0,0.0);
  room->LegL = new csX75::HNode(room->torso,num_vertices,v_positions_door,shirt_color,tex_coords,v_normals_door,sizeof(v_positions_door),sizeof(shirt_color),sizeof(tex_coords),smooth);
  room->LegL->change_parameters(-17.0,-5.0,83.0,0.0,0.0,0.0);
  room->eye = new csX75::HNode(room->torso,num_vertices,v_positions_door,shirt_color,tex_coords,v_normals_door,sizeof(v_positions_door),sizeof(shirt_color),sizeof(tex_coords),smooth);
  room->eye->change_parameters(-33.0,-5.0,83.0,0.0,0.0,0.0);

  room->LArmL = new csX75::HNode(room->torso,num_vertices,v_positions_room_top,shirt_color,tex_coords,v_normals_room_top,sizeof(v_positions_room_top),sizeof(shirt_color),sizeof(tex_coords),wall2);
  room->LArmL->change_parameters(0.0,59.0,40.0,0.0,0.0,0.0);
  room->LArmR = new csX75::HNode(room->torso,num_vertices,v_positions_room_top,shirt_color,tex_coords,v_normals_room_top,sizeof(v_positions_room_top),sizeof(shirt_color),sizeof(tex_coords),shima2);
  room->LArmR->change_parameters(0.0,-27.0,40.0,0.0,0.0,0.0);
  room->UArmR = new csX75::HNode(room->torso,num_vertices,v_positions_room_side,shirt_color,tex_coords,v_normals_room_side,sizeof(v_positions_room_side),sizeof(shirt_color),sizeof(tex_coords),curtains_side);
  room->UArmR->change_parameters(40.0,7.0,41.0,0.0,0.0,0.0);
  room->UArmL = new csX75::HNode(room->torso,num_vertices,v_positions_room_side,shirt_color,tex_coords,v_normals_room_side,sizeof(v_positions_room_side),sizeof(shirt_color),sizeof(tex_coords),curtains_side);
  room->UArmL->change_parameters(-40.0,7.0,41.0,0.0,0.0,0.0);
  room->root_node = room->torso;

  door = new csX75::Model();
  door->torso = new csX75::HNode(NULL,num_vertices,v_positions_door,shirt_color,tex_coords,v_normals_door,sizeof(v_positions_door),sizeof(shirt_color),sizeof(tex_coords),dark_wood);
  door->torso->change_parameters(31.0,3.0,38,0.0,0.0,0.0);
  door->head = new csX75::HNode(door->torso,num_vertices,v_positions_door_nob,minion_color,tex_coords,v_normals_door_nob,sizeof(v_positions_door_nob),sizeof(minion_color),sizeof(tex_coords),"");
  door->head->change_parameters(5.0,0.0,-1.0,0.0,0.0,0.0);
  door->neck = new csX75::HNode(door->torso,num_vertices,v_positions_door_nob,minion_color,tex_coords,v_normals_door_nob,sizeof(v_positions_door_nob),sizeof(minion_color),sizeof(tex_coords),"");
  door->neck->change_parameters(5.0,0.0,1.0,0.0,0.0,0.0);
  door->root_node = door->torso; 

  window = new csX75::Model();
  window->torso = new csX75::HNode(NULL,num_vertices,v_positions_window_bars,shirt_color,tex_coords,v_normals_window_bars,sizeof(v_positions_window_bars),sizeof(shirt_color),sizeof(tex_coords),wood);
  window->torso->change_parameters(-24.5,65.0,38,0.0,0.0,0.0);
  window->neck = new csX75::HNode(window->torso,num_vertices,v_positions_window_bars,shirt_color,tex_coords,v_normals_window_bars,sizeof(v_positions_window_bars),sizeof(shirt_color),sizeof(tex_coords),wood);
  window->neck->change_parameters(0.0,-38.0,0,0.0,0.0,0.0);
  window->LegL = new csX75::HNode(window->torso,num_vertices,v_positions_window_bars,shirt_color,tex_coords,v_normals_window_bars, sizeof(v_positions_window_bars),sizeof(shirt_color),sizeof(tex_coords),wood);
  window->LegL->change_parameters(0.0,-19.0,0.0,0.0,0.0,0.0);
  window->head = new csX75::HNode(window->torso,num_vertices,v_positions_window_frame,shirt_color,tex_coords,v_normals_window_frame,sizeof(v_positions_window_frame),sizeof(shirt_color),sizeof(tex_coords),wood);
  window->head->change_parameters(-15.0,-20.0,0,0.0,0.0,0.0);
  window->eye = new csX75::HNode(window->torso,num_vertices,v_positions_window_frame,shirt_color,tex_coords,v_normals_window_frame,sizeof(v_positions_window_frame),sizeof(shirt_color),sizeof(tex_coords),wood);
  window->eye->change_parameters(15.0,-19.5,0,0.0,0.0,0.0);  
  window->LegR = new csX75::HNode(window->torso,num_vertices,v_positions_window_frame,shirt_color,tex_coords,v_normals_window_frame,sizeof(v_positions_window_frame),sizeof(shirt_color),sizeof(tex_coords),wood);
  window->LegR->change_parameters(0.0,-19.5,0,0.0,0.0,0.0); 
  window->root_node = window->torso; 

  light_fixture = new csX75::Model();
  light_fixture->torso = new csX75::HNode(NULL,num_vertices,v_positions_lightfixture,minion_color,tex_coords,v_normals_lightfixture,sizeof(v_positions_lightfixture),sizeof(shirt_color),sizeof(tex_coords),led);
  light_fixture->torso->change_parameters(0.0,40.0,37.0,0.0,0.0,0.0);
  light_fixture->root_node = light_fixture->torso; 

}

void render_models(void)
{
  if(enable_perspective)
    projection_matrix = glm::perspective(glm::radians(90.0), 1.0, 0.1, 150.0);
  else
    projection_matrix = glm::ortho(-55.0, 55.0, -55.0, 55.0, -10.0, 100.0);

  view_matrix = projection_matrix*lookat_matrix;

  matrixStack.push_back(view_matrix);
  model0->root_node->render_tree();
  room->root_node->render_tree();
  light_fixture->root_node->render_tree();
  model1->root_node->render_tree();
  model2->root_node->render_tree();
  table->root_node->render_tree();
  sofa->root_node->render_tree();
  bookcase->root_node->render_tree();
  tv_stand->root_node->render_tree();  
  door->root_node->render_tree();
  window->root_node->render_tree();
}

void renderGL(void)
{
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

  matrixStack.clear();

  if(!animating){
      if(camera_keyframing)
        lookat_matrix = glm::lookAt(glm::vec3(c_positions[camera_keyframe_num]),glm::vec3(0.0),glm::vec3(c_ups[camera_keyframe_num]));
      else
      {
        //Creating the lookat and the up vectors for the camera
        c_rotation_matrix = glm::rotate(glm::mat4(1.0f), glm::radians(c_xrot), glm::vec3(1.0f,0.0f,0.0f)); // angle,x,y,z
        c_rotation_matrix = glm::rotate(c_rotation_matrix, glm::radians(c_yrot), glm::vec3(0.0f,1.0f,0.0f));
        c_rotation_matrix = glm::rotate(c_rotation_matrix, glm::radians(c_zrot), glm::vec3(0.0f,0.0f,1.0f));

        glm::vec4 c_pos = glm::vec4(c_xpos,c_ypos,c_zpos, 1.0)*c_rotation_matrix;
        glm::vec4 c_up = glm::vec4(c_up_x,c_up_y,c_up_z, 1.0)*c_rotation_matrix;

        lookat_matrix = glm::lookAt(glm::vec3(c_pos),glm::vec3(0.0),glm::vec3(c_up));
      }
  }

  render_models();
}

long factorial(int i){
  long result = 1;
  for(i = i; i > 1; i--){
    result = result*i;
  }
  return result;
} 

float combinations(int n, int i){
  return factorial(n)/factorial(n - i)/factorial(i);
}

void camera_keyframes_interpolate(void){
  float t = glfwGetTime() - animation_start_time;
  if(t > camera_animation_duration){
    return;
  }

  t = t/camera_animation_duration;
  glm::vec4 c_p = glm::vec4(0.0, 0.0, 0.0, 0.0); // Interpolated Camera Position
  glm::vec4 c_up = glm::vec4(0.0, 0.0, 0.0, 0.0);// Interpolated Camera Up Vector
  int n = c_positions.size();
  float f = 0;
  if(n > 0){
      for(int i = 0; i < n; i++)
      {
        f = (float)(combinations(n - 1, i)*glm::pow(t, i)*glm::pow(1 - t, n - 1 - i));
        c_p = c_p + c_positions[i]*f;
        c_up = c_up + c_ups[i]*f;
      }
      lookat_matrix = glm::lookAt(glm::vec3(c_p),glm::vec3(0.0),glm::vec3(c_up));
  }

  return;
}

void model_keyframes_interpolate(void){
  float t = glfwGetTime() - animation_start_time;
  for(int i = 0; i < model_t.size(); i++){
    if(t <= model_t[i] && i > 0)
    {
      t = (t - model_t[i - 1])/(model_t[i] - model_t[i - 1]);
      model0->neck->rx = lid[i - 1]*(1 - t) + lid[i]*(t);       model0->neck->update_matrices();
      model1->torso->ty  = H_pos[i - 1]*(1 - t) + H_pos[i]*t;   model1->torso->update_matrices();
      model2->torso->ty  = M_pos[i - 1]*(1 - t) + M_pos[i]*t;   model2->torso->update_matrices();
      model1->UArmR->rx  = H_uaRx[i - 1]*(1 - t) + H_uaRx[i]*t; model1->UArmR->update_matrices();
      model1->UArmR->rz  = H_uaRz[i - 1]*(1 - t) + H_uaRz[i]*t; model1->UArmR->update_matrices();
      model1->UArmL->rx  = H_uaLx[i - 1]*(1 - t) + H_uaLx[i]*t; model1->UArmL->update_matrices();
      model1->UArmL->rz  = H_uaLz[i - 1]*(1 - t) + H_uaLz[i]*t; model1->UArmL->update_matrices();
      model1->LArmR->rx  = H_laR[i - 1]*(1 - t) + H_laR[i]*t;   model1->LArmR->update_matrices();
      model1->LArmL->rx  = H_laL[i - 1]*(1 - t) + H_laL[i]*t;   model1->LArmL->update_matrices();
      model1->ThighR->rx  = H_thRx[i - 1]*(1 - t) + H_thRx[i]*t;model1->ThighR->update_matrices();
      model1->ThighR->rz  = H_thRz[i - 1]*(1 - t) + H_thRz[i]*t;model1->ThighR->update_matrices();
      model1->ThighL->rx  = H_thLx[i - 1]*(1 - t) + H_thLx[i]*t;model1->ThighL->update_matrices();
      model1->ThighL->rz  = H_thLz[i - 1]*(1 - t) + H_thLz[i]*t;model1->ThighL->update_matrices();
      model1->LegR->rx  = H_legR[i - 1]*(1 - t) + H_legR[i]*t;  model1->LegR->update_matrices();
      model1->LegL->rx  = H_legL[i - 1]*(1 - t) + H_legL[i]*t;  model1->LegL->update_matrices();
      model2->UArmR->rx  = M_uaRx[i - 1]*(1 - t) + M_uaRx[i]*t; model2->UArmR->update_matrices();
      model2->UArmR->rz  = M_uaRz[i - 1]*(1 - t) + M_uaRz[i]*t; model2->UArmR->update_matrices();
      model2->UArmL->rx  = M_uaLx[i - 1]*(1 - t) + M_uaLx[i]*t; model2->UArmL->update_matrices();
      model2->UArmL->rz  = M_uaLz[i - 1]*(1 - t) + M_uaLz[i]*t; model2->UArmL->update_matrices();
      model2->ThighR->rx  = M_thRx[i - 1]*(1 - t) + M_thRx[i]*t;model2->ThighR->update_matrices();
      model2->ThighR->rz  = M_thRz[i - 1]*(1 - t) + M_thRz[i]*t;model2->ThighR->update_matrices();
      model2->ThighL->rx  = M_thLx[i - 1]*(1 - t) + M_thLx[i]*t;model2->ThighL->update_matrices();
      model2->ThighL->rz  = M_thLz[i - 1]*(1 - t) + M_thLz[i]*t;model2->ThighL->update_matrices();
      model1->torso->ry  = H_torso[i - 1]*(1 - t) + H_torso[i]*t;model1->torso->update_matrices();
      model2->torso->ry  = M_torso[i - 1]*(1 - t) + M_torso[i]*t;model2->torso->update_matrices();
      model1->head->ry  = H_head[i - 1]*(1 - t) + H_head[i]*t;model1->head->update_matrices();
      model2->torso->rx  = M_tor_x[i - 1]*(1 - t) + M_tor_x[i]*t;model2->torso->update_matrices();
      light1  = l1[i - 1]*(1 - t) + l1[i]*t;
      light2  = l2[i - 1]*(1 - t) + l2[i]*t;
      break;
    }
  }
}

void update_animation_parameters(void){
  // Character Positions
  model_keyframes_interpolate();

  // Camera Orientation
  camera_keyframes_interpolate();

  return;
}


int main(int argc, char** argv)
{
  //! The pointer to the GLFW window
  GLFWwindow* window;

  //! Setting up the GLFW Error callback
  glfwSetErrorCallback(csX75::error_callback);

  //! Initialize GLFW
  if (!glfwInit())
    return -1;

  //We want OpenGL 4.0
  glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3); 
  glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
  //This is for MacOSX - can be omitted otherwise
  glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE); 
  //We don't want the old OpenGL 
  glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE); 

  //! Create a windowed mode window and its OpenGL context
  window = glfwCreateWindow(1024, 1024, "CS475Assignment2: Hierarchical Modelling", NULL, NULL);
  if (!window)
    {
      glfwTerminate();
      return -1;
    }
  
  //! Make the window's context current 
  glfwMakeContextCurrent(window);

  //Initialize GLEW
  //Turn this on to get Shader based OpenGL
  glewExperimental = GL_TRUE;
  GLenum err = glewInit();
  if (GLEW_OK != err)
    {
      //Problem: glewInit failed, something is seriously wrong.
      std::cerr<<"GLEW Init Failed : %s"<<std::endl;
    }

  //Print and see what context got enabled
  std::cout<<"Vendor: "<<glGetString (GL_VENDOR)<<std::endl;
  std::cout<<"Renderer: "<<glGetString (GL_RENDERER)<<std::endl;
  std::cout<<"Version: "<<glGetString (GL_VERSION)<<std::endl;
  std::cout<<"GLSL Version: "<<glGetString (GL_SHADING_LANGUAGE_VERSION)<<std::endl;

  //Keyboard Callback
  glfwSetKeyCallback(window, csX75::key_callback);
  //Framebuffer resize callback
  glfwSetFramebufferSizeCallback(window, csX75::framebuffer_size_callback);

  // Ensure we can capture the escape key being pressed below
  glfwSetInputMode(window, GLFW_STICKY_KEYS, GL_TRUE);

  //Initialize GL state
  csX75::initGL();
  initBuffersGL();

  // Loop until the user closes the window
  while (glfwWindowShouldClose(window) == 0)
    {
      if(animating)
      {
        update_animation_parameters();
        if(glfwGetTime() - animation_start_time > animation_duration){
          animating = false;
          std::cout<<"Animation is Completed"<<std::endl;
        }
      }

      // Render here
      renderGL();

      // Swap front and back buffers
      glfwSwapBuffers(window);
      
      // Poll for and process events
      glfwPollEvents();
    }
  
  glfwTerminate();
  return 0;
}

//-------------------------------------------------------------------------


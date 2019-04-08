#ifndef _HNODE_HPP_
#define _HNODE_HPP_

#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include <vector>
#include "glm/vec3.hpp"
#include "glm/vec4.hpp"
#include "glm/mat4x4.hpp"
#include "glm/gtc/matrix_transform.hpp"
#include "glm/gtc/type_ptr.hpp"


#include "gl_framework.hpp"


namespace csX75	 { 

	// A simple class that represents a node in the hierarchy tree
	class HNode {
		//glm::vec4 * vertices;
		//glm::vec4 * colors;
		//GLfloat tx,ty,tz,rx,ry,rz;

		std::size_t vertex_buffer_size;
		std::size_t color_buffer_size;
		std::size_t texture_buffer_size;
		std::size_t normal_buffer_size;

		bool is_textured;

		GLuint num_vertices;
		GLuint vao,vbo,tex;

		glm::mat4 rotation;
		glm::mat4 translation;

		// Shading
		glm::mat4 model_matrix;
		glm::mat3 normal_matrix;
		glm::mat4 modelview_matrix;
		
		std::vector<HNode*> children;
		HNode* parent;		



	  public:
	  	GLfloat tx,ty,tz,rx,ry,rz; // made this public
		HNode (HNode*, GLuint, glm::vec4*,  glm::vec4*, glm::vec2*, glm::vec4*, std::size_t, std::size_t, std::size_t, std::string);
		void update_matrices();

		//HNode (HNode* , glm::vec4*,  glm::vec4*,GLfloat,GLfloat,GLfloat,GLfloat,GLfloat,GLfloat);
		void add_child(HNode*);
		void render(); // renders just the object with the current matrix stack
		void change_parameters(GLfloat,GLfloat,GLfloat,GLfloat,GLfloat,GLfloat);
		void render_tree();  //renders the object along with its all children recursivey
		void inc_rx();
		void inc_ry();
		void inc_rz();
		void dec_rx();
		void dec_ry();
		void dec_rz();
		void set_rx(float r);
		void set_ry(float r);
		void set_rz(float r);
		void set_tx(float t);
		void set_ty(float t);
		void set_tz(float t);
		void out_of_the_box();
		void in_the_box();
	};

	class Model{
		public:
			HNode* root_node;
			HNode* torso;
			HNode* neck;
			HNode* head;
			HNode* UArmR;
			HNode* UArmL;
			HNode* LArmR;
			HNode* LArmL;
			HNode* ThighR;
			HNode* ThighL;
			HNode* LegR;
			HNode* LegL;
			HNode* node1;
			HNode* node2;
			HNode* band;
			HNode* eye;
			HNode* eyeball;
	};

	glm::mat4* multiply_stack(std::vector <glm::mat4> );
};	

#endif
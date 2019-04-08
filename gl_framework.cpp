#include "gl_framework.hpp"
#include "hierarchy_node.hpp"
#include <string>
#include <iostream>
#include <fstream>


extern GLfloat c_xrot,c_yrot,c_zrot, c_ypos, c_xpos, c_zpos, c_up_x, c_up_y,c_up_z;
extern bool enable_perspective;
extern csX75::Model* model1, *model2, *model0, *selected_model;
extern csX75::HNode* curr_selected_node;

extern std::vector<glm::vec4> c_positions, c_ups;

extern std::vector<float> lid, H_pos, M_pos, H_uaRx,H_uaRz, H_uaLx, H_uaLz, H_laR, H_laL, H_thRx, H_thRz, H_thLx, H_thLz;
extern std::vector<float> H_legR, H_legL, M_uaRx,M_uaRz, M_uaLx, M_uaLz, M_thRx, M_thRz, M_thLx, M_thLz, H_torso, M_torso, H_head, M_tor_x;
extern std::vector<float> l1, l2, model_t;

extern glm::vec4 c_pos;
extern glm::mat4 c_rotation_matrix;
extern bool camera_keyframing;
extern int camera_keyframe_num;
extern bool animating;

extern float light1;
extern float light2;

extern float animation_start_time;

namespace csX75
{
  //! Initialize GL State
  void initGL(void)
  {
    //Set framebuffer clear color
    glClearColor(1.0f, 1.0f, 1.0f, 1.0f);
    //Set depth buffer furthest depth
    glClearDepth(1.0);
    //Set depth test to less-than
    glDepthFunc(GL_LESS);
    //Enable depth testing
    glEnable(GL_DEPTH_TEST);
  }
  
  //!GLFW Error Callback
  void error_callback(int error, const char* description)
  {
    std::cerr<<description<<std::endl;
  }
  
  //!GLFW framebuffer resize callback
  void framebuffer_size_callback(GLFWwindow* window, int width, int height)
  {
    //!Resize the viewport to fit the window size - draw to entire window
    glViewport(0, 0, width, height);
  }
  
  //!GLFW keyboard callback
  void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods)
  {
    //!Close the window if the ESC key was pressed

    bool dof_is_3, is_model;
    if (curr_selected_node==selected_model->LegR || curr_selected_node==selected_model->LegL 
        || curr_selected_node==selected_model->LArmL || curr_selected_node==selected_model->LArmR
        || (selected_model == model2 && curr_selected_node==selected_model->head))
      dof_is_3=false;
    else
      dof_is_3=true;

    if(selected_model==model0)
      is_model=false;
    else
      is_model=true;

      if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
        glfwSetWindowShouldClose(window, GL_TRUE);

      // Change Between Models
      else if (key == GLFW_KEY_M && action == GLFW_PRESS){
        if(selected_model == model1)
          selected_model = model2;
        else
          selected_model = model1;
        curr_selected_node = selected_model->torso;
      }
      else if(key == GLFW_KEY_SPACE && action == GLFW_PRESS)
      {
        animating = !animating;
        if(animating){
          animation_start_time = glfwGetTime();
          std::cout<<"Animation Started"<<std::endl;
        }
        else{
          std::cout<<"Animation Stopped"<<std::endl;
        }
      }

      // Change Selection Between the Parts of a Model. box does not count as a model in this case.
      else if (key == GLFW_KEY_1 && action == GLFW_PRESS && is_model)
        curr_selected_node = selected_model->torso;
      else if (key == GLFW_KEY_2 && action == GLFW_PRESS && is_model)
        curr_selected_node = selected_model->neck; 
      else if (key == GLFW_KEY_3 && action == GLFW_PRESS && is_model)
        curr_selected_node = selected_model->head; 
      else if (key == GLFW_KEY_4 && action == GLFW_PRESS && is_model)
        curr_selected_node = selected_model->ThighL; 
      else if (key == GLFW_KEY_5 && action == GLFW_PRESS && is_model)
        curr_selected_node = selected_model->ThighR; 
      else if (key == GLFW_KEY_6 && action == GLFW_PRESS && is_model)
        curr_selected_node = selected_model->LegL; 
      else if (key == GLFW_KEY_7 && action == GLFW_PRESS && is_model)
        curr_selected_node = selected_model->LegR;   
      else if (key == GLFW_KEY_8 && action == GLFW_PRESS && is_model)
        curr_selected_node = selected_model->UArmL; 
      else if (key == GLFW_KEY_9 && action == GLFW_PRESS && is_model)
        curr_selected_node = selected_model->UArmR; 
      else if (key == GLFW_KEY_L && action == GLFW_PRESS && is_model)
        curr_selected_node = selected_model->LArmL; 
      else if (key == GLFW_KEY_R && action == GLFW_PRESS && is_model)
        curr_selected_node = selected_model->LArmR; 
      else if (key == GLFW_KEY_B  && action == GLFW_PRESS && is_model)
      {
        selected_model = model0;
        curr_selected_node = selected_model->torso;
      }
      // Modify the Parts of the Model
      else if (key == GLFW_KEY_LEFT && action == GLFW_PRESS && dof_is_3)
        curr_selected_node->dec_ry();
      else if (key == GLFW_KEY_RIGHT && action == GLFW_PRESS && dof_is_3)
        curr_selected_node->inc_ry();
      else if (key == GLFW_KEY_UP && action == GLFW_PRESS)
        curr_selected_node->dec_rx();
      else if (key == GLFW_KEY_DOWN && action == GLFW_PRESS)
        curr_selected_node->inc_rx();
      else if (key == GLFW_KEY_PAGE_UP && action == GLFW_PRESS && dof_is_3)
        curr_selected_node->dec_rz();
      else if (key == GLFW_KEY_PAGE_DOWN && action == GLFW_PRESS && dof_is_3)
        curr_selected_node->inc_rz();
      else if (key == GLFW_KEY_O  && action == GLFW_PRESS && model0->neck->rx > -180 ) // open
        model0->neck->dec_rx();  
      else if (key == GLFW_KEY_C  && action == GLFW_PRESS && model0->neck->rx < 0.0)  //close
        model0->neck->inc_rx(); 
      else if (key == GLFW_KEY_EQUAL  && action == GLFW_PRESS)
      {
        model1->torso->out_of_the_box(); 
        model2->torso->out_of_the_box();
      }
      else if (key == GLFW_KEY_MINUS    && action == GLFW_PRESS)
      {
        model1->torso->in_the_box(); 
        model2->torso->in_the_box();
      }

      // Change the Camera Perspective
      else if (key == GLFW_KEY_P && action == GLFW_PRESS)
        enable_perspective = !enable_perspective;   
      else if (key == GLFW_KEY_A  && action == GLFW_PRESS)
        c_yrot -= 10.0;
      else if (key == GLFW_KEY_D  && action == GLFW_PRESS)
        c_yrot += 10.0;
      else if (key == GLFW_KEY_W  && action == GLFW_PRESS)
        c_xrot -= 10.0;
      else if (key == GLFW_KEY_S  && action == GLFW_PRESS)
        c_xrot += 10.0;        
      else if (key == GLFW_KEY_Q  && action == GLFW_PRESS)
        c_zrot -= 10.0;
      else if (key == GLFW_KEY_E  && action == GLFW_PRESS)
        c_zrot += 10.0;   
      else if(key == GLFW_KEY_RIGHT_BRACKET  && action == GLFW_PRESS){
        if(light1 < 0.5){
          light1 = 1.0;
        } else{
          light1 = 0.0;
        }
      }
      else if(key == GLFW_KEY_LEFT_BRACKET  && action == GLFW_PRESS){
        if(light2 < 0.5){
          light2 = 1.0;
        } else{
          light2 = 0.0;
        }
      }
      else if (key == GLFW_KEY_J  && action == GLFW_PRESS)
      {
        c_ypos -= 3.0;
      }
      else if (key == GLFW_KEY_U  && action == GLFW_PRESS)
      {
        c_ypos += 3.0;
      }
      else if (key == GLFW_KEY_H  && action == GLFW_PRESS)
      {
        c_xpos -= 3.0;
      }
      else if (key == GLFW_KEY_K && action == GLFW_PRESS)
      {
        c_xpos += 3.0;      
      }
      else if (key == GLFW_KEY_I  && action == GLFW_PRESS)
      {
        c_zpos -= 3.0;
      }
      else if (key == GLFW_KEY_Y  && action == GLFW_PRESS)
      {
        c_zpos += 3.0;   
      }

      else if (key == GLFW_KEY_Z  && action == GLFW_PRESS)
      {
        c_positions.push_back(glm::vec4(c_xpos,c_ypos,c_zpos, 1.0)*c_rotation_matrix);
        c_ups.push_back(glm::vec4(c_up_x,c_up_y,c_up_z, 1.0)*c_rotation_matrix);
      } 

      else if(key == GLFW_KEY_F  && action == GLFW_PRESS)
      {

        lid.push_back(model0->neck->rx);
        H_pos.push_back(model1->torso->ty);
        M_pos.push_back(model2->torso->ty);
        H_uaRx.push_back(model1->UArmR->rx);
        H_uaRz.push_back(model1->UArmR->rz);
        H_uaLx.push_back(model1->UArmL->rx);
        H_uaLz.push_back(model1->UArmL->rz);
        H_laR.push_back(model1->LArmR->rx);
        H_laL.push_back(model1->LArmL->rx);
        H_thRx.push_back(model1->ThighR->rx);
        H_thRz.push_back(model1->ThighR->rz);
        H_thLx.push_back(model1->ThighL->rx);
        H_thLz.push_back(model1->ThighL->rz);
        H_legR.push_back(model1->LegR->rx);
        H_legL.push_back(model1->LegL->rx);
        M_uaRx.push_back(model2->UArmR->rx);
        M_uaRz.push_back(model2->UArmR->rz);
        M_uaLx.push_back(model2->UArmL->rx);
        M_uaLz.push_back(model2->UArmL->rz);
        M_thRx.push_back(model2->ThighR->rx);
        M_thRz.push_back(model2->ThighR->rz);
        M_thLx.push_back(model2->ThighL->rx);
        M_thLz.push_back(model2->ThighL->rz);
        H_torso.push_back(model1->torso->ry);
        M_torso.push_back(model2->torso->ry);
        H_head.push_back(model1->head->ry);
        M_tor_x.push_back(model2->torso->rx);
        l1.push_back(light1);
        l2.push_back(light2);

        std::ofstream myfile;
        myfile.open("keyframes.txt", std::ios_base::app);
          myfile << model0->neck->rx << ",";
          myfile << model1->torso->ty << ",";
          myfile << model2->torso->ty  << ",";
          myfile << model1->UArmR->rx << ",";
          myfile << model1->UArmR->rz << ",";
          myfile << model1->UArmL->rx << ",";
          myfile << model1->UArmL->rz << ",";
          myfile << model1->LArmR->rx << ",";
          myfile << model1->LArmL->rx << ",";
          myfile << model1->ThighR->rx << ",";
          myfile << model1->ThighR->rz << ",";
          myfile << model1->ThighL->rx << ",";
          myfile << model1->ThighL->rz << ",";
          myfile << model1->LegR->rx << ",";
          myfile << model1->LegL->rx << ",";
          myfile << model2->UArmR->rx<< ",";
          myfile << model2->UArmR->rz<< ",";
          myfile << model2->UArmL->rx<< ",";
          myfile << model2->UArmL->rz<< ",";
          myfile << model2->ThighR->rx<< ",";
          myfile << model2->ThighR->rz<< ",";
          myfile << model2->ThighL->rx<< ",";
          myfile << model2->ThighL->rz<< ",";
          myfile << model1->torso->ry<< ",";
          myfile << model2->torso->ry<< ",";
          myfile << model1->head->ry<< ",";
          myfile << model2->torso->rx<< ",";
          myfile << light1<< ",";
          myfile << light2<< ",";

          float t = 0;
          std::cout<<"Enter time"<<std::endl;
          std::cin>> t;
          myfile << t<< "";
          myfile << std::endl;
          model_t.push_back(t);

        myfile.close();
      }
      else if (key == GLFW_KEY_X  && action == GLFW_PRESS) // switch between 2 modes 
      {
        camera_keyframing = !camera_keyframing;
        camera_keyframe_num = 0;
      }
      else if (key == GLFW_KEY_N  && action == GLFW_PRESS) // next position
      {
        if(c_positions.size() - 1 == camera_keyframe_num)
        {
          camera_keyframe_num = 0;
          std::cout<<"Camera Positions are Completed"<<std::endl;
        }
        else{
          camera_keyframe_num++;
        }
      }
      else if (key == GLFW_KEY_G  && action == GLFW_PRESS) // erase file and reset vectors
      {
        std::ofstream myfile;
        myfile.open("camera_positions.txt", std::ofstream::out | std::ofstream::trunc);
        myfile.close(); 
        std::ofstream myfile2;
        myfile2.open("keyframes.txt", std::ofstream::out | std::ofstream::trunc);
        myfile2.close(); 
        c_positions.clear();
        c_ups.clear();
      }

      else if(key == GLFW_KEY_V  && action == GLFW_PRESS) // Export Camera Keyframes 
      {
        std::ofstream myfile;
        myfile.open ("camera_positions.txt");

        for(int i=0;i<c_positions.size();i++)
        {
          myfile << std::to_string(c_positions[i].x) << ",";
          myfile << std::to_string(c_positions[i].y) << ",";
          myfile << std::to_string(c_positions[i].z) << ",";
          myfile << std::to_string(c_positions[i].w) << ",";
          myfile << std::to_string(c_ups[i].x) << ",";
          myfile << std::to_string(c_ups[i].y) << ",";
          myfile << std::to_string(c_ups[i].z) << ",";
          myfile << std::to_string(c_ups[i].w) << "";
          myfile << std::endl;
        }
        
        myfile.close();
        std::cout<<"Camera Keyframes Exported !"<<std::endl;
      }
      else if (key == GLFW_KEY_PERIOD  && action == GLFW_PRESS) // Import Model Keyframes
      {
        lid.clear(); H_pos.clear(); M_pos.clear(); H_uaRx.clear();H_uaRz.clear(); H_uaLx.clear(); 
        H_uaLz.clear(); H_laR.clear(); H_laL.clear(); H_thRx.clear(); H_thRz.clear(); H_thLx.clear(); 
        H_thLz.clear(); H_legR.clear(); H_legL.clear(); M_uaRx.clear();M_uaRz.clear(); M_uaLx.clear(); 
        M_uaLz.clear(); M_thRx.clear(); M_thRz.clear(); M_thLx.clear(); M_thLz.clear(); H_torso.clear(); 
        M_torso.clear(); H_head.clear(); M_tor_x.clear();
        model_t.clear();

        std::ifstream myReadFile;
        std::string extract;
        myReadFile.open("keyframes.txt");  
        int index;  

        std::string _lid = ""; 
        std::string _H_pos = "";
        std::string _M_pos = "";
        std::string _H_uaRx = "";
        std::string _H_uaRz = "";
        std::string _H_uaLx = "";
        std::string _H_uaLz = "";
        std::string _H_laR = "";
        std::string _H_laL = "";
        std::string _H_thRx = "";
        std::string _H_thRz = "";
        std::string _H_thLx = "";
        std::string _H_thLz = "";
        std::string _H_legR = "";
        std::string _H_legL = "";
        std::string _M_uaRx = "";
        std::string _M_uaRz = "";
        std::string _M_uaLx = "";
        std::string _M_uaLz = "";
        std::string _M_thRx = "";
        std::string _M_thRz = "";
        std::string _M_thLx = "";
        std::string _M_thLz = "";
        std::string _H_torso = "";
        std::string _M_torso = "";
        std::string _H_head = "";
        std::string _M_tor_x = "";
        std::string _l1 = "";
        std::string _l2 = "";
        std::string _M_t = "";

        if (myReadFile.is_open()) {
          while (std::getline(myReadFile,extract,'\n')) 
          { 
            _lid = ""; 
            _H_pos = "";
            _M_pos = "";
            _H_uaRx = "";
            _H_uaRz = "";
            _H_uaLx = "";
            _H_uaLz = "";
            _H_laR = "";
            _H_laL = "";
            _H_thRx = "";
            _H_thRz = "";
            _H_thLx = "";
            _H_thLz = "";
            _H_legR = "";
            _H_legL = "";
            _M_uaRx = "";
            _M_uaRz = "";
            _M_uaLx = "";
            _M_uaLz = "";
            _M_thRx = "";
            _M_thRz = "";
            _M_thLx = "";
            _M_thLz = "";
            _H_torso = "";
            _M_torso = "";
            _H_head = "";
            _M_tor_x= "";
            _l1= "";
            _l2= "";
            _M_t = "";
            index = 1;

            for(int i = 0; i < extract.size(); i++){
                if(extract[i] == ','){
                    index++;
                }
                else if(index == 1){
                  _lid += extract[i];
                }
                else if(index == 2){
                  _H_pos+= extract[i];
                }
                else if(index == 3){
                  _M_pos+= extract[i];
                }
                else if(index == 4){
                  _H_uaRx+= extract[i];
                }
                else if(index == 5){
                  _H_uaRz+= extract[i];
                }
                else if(index == 6){
                  _H_uaLx+= extract[i];
                }
                else if(index == 7){
                  _H_uaLz+= extract[i];
                }
                else if(index == 8){
                  _H_laR+= extract[i];
                }
                else if(index == 9){
                  _H_laL+= extract[i];
                }
                else if(index == 10){
                  _H_thRx+= extract[i];
                }
                else if(index == 11){
                  _H_thRz+= extract[i];
                }
                else if(index == 12){
                  _H_thLx+= extract[i];
                }
                else if(index == 13){
                  _H_thLz+= extract[i];
                }
                else if(index == 14){
                  _H_legR+= extract[i];
                }
                else if(index == 15){
                  _H_legL+= extract[i];
                }
                else if(index == 16){
                  _M_uaRx+= extract[i];
                }
                else if(index == 17){
                  _M_uaRz+=extract[i];
                }
                else if(index == 18){
                  _M_uaLx+= extract[i];
                }
                else if(index == 19){
                  _M_uaLz+= extract[i];
                }
                else if(index == 20){
                  _M_thRx+= extract[i];
                }
                else if(index == 21){
                  _M_thRz+= extract[i];
                }
                else if(index == 22){
                  _M_thLx+= extract[i];
                }
                else if(index == 23){
                  _M_thLz+= extract[i];
                }
                else if(index == 24){
                  _H_torso+= extract[i];
                }
                else if(index == 25){
                  _M_torso+= extract[i];
                }
                else if(index == 26){
                  _H_head+= extract[i];
                }
                else if(index == 27){
                  _M_tor_x+= extract[i];
                }
                else if(index == 28){
                  _l1+= extract[i];
                }
                else if(index == 29){
                  _l2+= extract[i];
                }
                else if(index == 30){
                  _M_t+= extract[i];
                }     
            }
            lid.push_back(strtof(_lid.c_str(),0));
            H_pos.push_back(strtof(_H_pos.c_str(),0));
            M_pos.push_back(strtof(_M_pos.c_str(),0));
            H_uaRx.push_back(strtof(_H_uaRx.c_str(),0));
            H_uaRz.push_back(strtof(_H_uaRz.c_str(),0));
            H_uaLx.push_back(strtof(_H_uaLx.c_str(),0));
            H_uaLz.push_back(strtof(_H_uaLz.c_str(),0));
            H_laR.push_back(strtof(_H_laR.c_str(),0));
            H_laL.push_back(strtof(_H_laL.c_str(),0));
            H_thRx.push_back(strtof(_H_thRx.c_str(),0));
            H_thRz.push_back(strtof(_H_thRz.c_str(),0));
            H_thLx.push_back(strtof(_H_thLx.c_str(),0));
            H_thLz.push_back(strtof(_H_thLz.c_str(),0));
            H_legR.push_back(strtof(_H_legR.c_str(),0));
            H_legL.push_back(strtof(_H_legL.c_str(),0));
            M_uaRx.push_back(strtof(_M_uaRx.c_str(),0));
            M_uaRz.push_back(strtof(_M_uaRz.c_str(),0));
            M_uaLx.push_back(strtof(_M_uaLx.c_str(),0));
            M_uaLz.push_back(strtof(_M_uaLz.c_str(),0));
            M_thRx.push_back(strtof(_M_thRx.c_str(),0));
            M_thRz.push_back(strtof(_M_thRz.c_str(),0));
            M_thLx.push_back(strtof(_M_thLx.c_str(),0));
            M_thLz.push_back(strtof(_M_thLz.c_str(),0));
            H_torso.push_back(strtof(_H_torso.c_str(),0));
            M_torso.push_back(strtof(_M_torso.c_str(),0));
            H_head.push_back(strtof(_H_head.c_str(),0));
            M_tor_x.push_back(strtof(_M_tor_x.c_str(),0));
            l1.push_back(strtof(_l1.c_str(),0));
            l2.push_back(strtof(_l2.c_str(),0));
            model_t.push_back(strtof(_M_t.c_str(),0));
          }     
          myReadFile.close();
        }

        std::cout<<"Models Keyframes Imported"<<std::endl;
      }
      else if (key == GLFW_KEY_T  && action == GLFW_PRESS) // Import Camera Keyframes
      {
        c_positions.clear();
        c_ups.clear();

        std::ifstream myReadFile;
        std::string extract;
        std::string nums;

        std::string posx = ""; 
        std::string posy = "";
        std::string posz = "";
        std::string posw = "";
        std::string upx = "";
        std::string upy = "";
        std::string upz = "";
        std::string upw = "";

        int index = 0;
        myReadFile.open("camera_positions.txt");
        if (myReadFile.is_open()) {
          while (std::getline(myReadFile,extract,'\n')) 
          { 
            posx = "";
            posy = "";
            posz = "";
            posw = "";
            upx = "";
            upy = "";
            upz = "";
            upw = "";
            index = 1;
            if(extract != ""){
              camera_keyframing = true;

              for(int i = 0; i < extract.size(); i++){
                if(extract[i] == ','){
                    index++;
                }
                else if(index == 1){
                  posx += extract[i];
                }
                else if(index == 2){
                  posy += extract[i];
                }
                else if(index == 3){
                  posz += extract[i];
                }
                else if(index == 4){
                  posw += extract[i];
                }
                else if(index == 5){
                  upx += extract[i];
                }
                else if(index == 6){
                  upy += extract[i];
                }
                else if(index == 7){
                  upz += extract[i];
                }
                else if(index == 8){
                  upw += extract[i];
                }
            }
            c_positions.push_back(glm::vec4(
                strtof(posx.c_str(),0),
                strtof(posy.c_str(),0),
                strtof(posz.c_str(),0),
                strtof(posw.c_str(),0)
                ));
            c_ups.push_back(glm::vec4(
                strtof(upx.c_str(),0),
                strtof(upy.c_str(),0),
                strtof(upz.c_str(),0),
                strtof(upw.c_str(),0)
                ));
            }
          }     
          myReadFile.close();

          std::cout<<"Imported Camera Keyframes"<<std::endl;
        }
      }  
    }
};  
  



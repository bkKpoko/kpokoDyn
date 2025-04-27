#include "aa_luse.h"
#include "lineSegs.h"
#include "nodePath.h"
#include "pandaFramework.h"
#include "pandaSystem.h"

#include "genericAsyncTask.h"
#include "asyncTaskManager.h"

#include <cstdio>
#include <eigen3/Eigen/Geometry>
#include <eigen3/Eigen/Dense>
#include <string>
#include <vector>

// The global task manager
PT(AsyncTaskManager) taskMgr = AsyncTaskManager::get_global_ptr();
// The global clock
PT(ClockObject) globalClock = ClockObject::get_global_clock();
// Here's what we'll store the camera in.
NodePath camera;

std::vector<NodePath> nodes;

NodePath create_line(PandaFramework& framework, const LVector3f& start, const LVector3f& end, 
                     const LColor& color) {
  LineSegs line;
  line.set_color(color);
  line.set_thickness(3.0f);  // line thicksness
  line.move_to(start);
  line.draw_to(end);

  return framework.get_window(0)->get_render().attach_new_node(line.create());
}


void create_node(PandaFramework& framework, Eigen::Vector3d translate, Eigen::Quaterniond q){
  double axis_len = 5.0;

  Eigen::Affine3d transform = Eigen::Affine3d::Identity();
  transform.linear() = q.normalized().toRotationMatrix();
  transform.translate(translate);

  Eigen::Vector3d coord = transform.translation();

  std::cout << "Affine transformation matrix:\n" 
    << transform.matrix() << std::endl;

  Eigen::Vector3d x(axis_len, 0, 0);
  Eigen::Vector3d y(0, axis_len, 0);
  Eigen::Vector3d z(0, 0, axis_len);

  x = q.normalized().toRotationMatrix() * x;
  y = q.normalized().toRotationMatrix() * y;
  z = q.normalized().toRotationMatrix() * z;

  create_line(framework, 
              LVector3f(coord.x(), coord.y(), coord.z()), 
              LVector3f(x.x(), x.y(), x.z()), 
              LColor(1, 0, 0, 1)
              );
  create_line(framework, 
              LVector3f(coord.x(), coord.y(), coord.z()), 
              LVector3f(y.x(), y.y(), y.z()), 
              LColor(0, 1, 0, 1)
              );
  create_line(framework, 
              LVector3f(coord.x(), coord.y(), coord.z()), 
              LVector3f(z.x(), z.y(), z.z()), 
              LColor(0, 0, 1, 1)
              );
}


NodePath create_coordinate_system(PandaFramework& framework, 
                                  int index, 
                                  Eigen::Vector3d pos,
                                  LQuaternionf q) {
  // Create axes lines 
  LineSegs lines;

  // X (red)
  lines.set_color(1, 0, 0, 1);
  lines.move_to(0, 0, 0);
  lines.draw_to(1, 0, 0);

  // Y (green)
  lines.set_color(0, 1, 0, 1);
  lines.move_to(0, 0, 0);
  lines.draw_to(0, 1, 0);

  // Z (blue)
  lines.set_color(0, 0, 1, 1);
  lines.move_to(0, 0, 0);
  lines.draw_to(0, 0, 1);

  // create node  
  NodePath system = framework.get_window(0)->get_render().attach_new_node(lines.create());
  system.set_name("structural_node_" + std::to_string(index));

  // set start pos  
  system.set_pos(
    pos.x(), pos.y(), pos.z()
  );

  // set start rotation  
  system.set_quat(q);

  return system;
}


AsyncTask::DoneStatus move_system(GenericAsyncTask* task, void* data) {
  for (int i = 0; i < nodes.size(); ++i) {
    nodes[i].set_pos();
    nodes[i].set_quat();
  } 
  return AsyncTask::DS_cont;
}



void create_grid(PandaFramework& framework, int step){
  int border = 100;

  for (int i = -border; i < border; i += step) {
    create_line(framework, 
                LVector3f(i, -border, 0), 
                LVector3f(i, border, 0), 
                LColor(90./255, 90./255, 90./255, 0.5)
                );   
  }

  for (int i = -border; i < border; i += step) {
    create_line(framework, 
                LVector3f(-border, i, 0), 
                LVector3f(border, i, 0), 
                LColor(90./255, 90./255, 90./255, 0.5)
                );       
  }
}


int main(int argc, char *argv[]) {
  // Load the window and set its title.
  PandaFramework framework;
  framework.open_framework(argc, argv);
  framework.set_window_title("kpokoDyn");
  WindowFramework *window = framework.open_window();

  // Get the camera and store it in a variable.
  camera = window->get_camera_group();

  window->setup_trackball();

  create_grid(framework, 10);

  taskMgr->add(new GenericAsyncTask("move_task", &move_system, nullptr));
  // Run the engine.
  framework.main_loop();
  // Shut down the engine when done.
  framework.close_framework();
  return 0;
}

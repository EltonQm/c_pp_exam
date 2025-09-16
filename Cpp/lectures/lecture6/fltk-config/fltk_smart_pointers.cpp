#include <FL/Fl.H>
#include <FL/Fl_Box.H>
#include <FL/Fl_Window.H>
#include <iostream>
#include <memory>
int main(int argc, char **argv) {
  auto window = std::make_unique<Fl_Window>(340, 180);
  auto box = std::make_unique<Fl_Box>(20, 40, 300, 100, "Hello, World!");
  box->box(FL_UP_BOX);
  box->labelfont(FL_BOLD + FL_ITALIC);
  box->labelsize(36);
  box->labeltype(FL_SHADOW_LABEL);
  window->end();
  window->show(argc, argv);
  std::cout << "fltk working!" << std::endl;
  return Fl::run();
}

// No pointers
#include <FL/Fl.H>
#include <FL/Fl_Box.H>
#include <FL/Fl_Window.H>
#include <iostream>
int main(int argc, char **argv) {
  auto window = Fl_Window(340, 180);
  window.color(FL_WHITE);
  auto box = Fl_Box(20, 40, 300, 100, "Hello, World\nno Pointer!");
  box.box(FL_UP_BOX);
  box.color(FL_WHITE);
  box.labelfont(FL_BOLD + FL_ITALIC);
  box.labelsize(36);
  box.labelcolor(FL_BLACK);
  // box.labeltype(FL_SHADOW_LABEL);
  window.end();
  window.show(argc, argv);
  std::cout << "fltk working!" << std::endl;
  return Fl::run();
}
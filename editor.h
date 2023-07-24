#ifndef EDITOR_H
#define EDITOR_H

#include "document.h"
#include <ncurses.h>

class Editor
{
  private:
    Document doc;

  public:
    Editor();
    ~Editor();
    void run();
};

#endif

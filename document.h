#ifndef DOCUMENT_H
#define DOCUMENT_H

#include <list>
#include <string>

class Document
{
    std::list<std::string> lines;
    std::list<std::string>::iterator cur_line;
    size_t cur_col;
    std::pair<int, int> selection_start;
    std::pair<int, int> scroll_offset;

  public:
    Document();
    explicit Document(const std::string &filename);
    void insert(char ch);
    void delete_forward();
    void delete_backward();
    void cursor_left();
    void cursor_right();
    void cursor_up();
    void cursor_down();
    void cursor_home();
    void cursor_end();
    void print();
    void set_selection_start();
    void clear_selection();
    bool selecting = false;

  private:
    void scroll_to_cursor();
};

#endif

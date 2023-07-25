#ifndef DOCUMENT_H
#define DOCUMENT_H

#include <list>
#include <string>

class Document
{
    std::list<std::string> lines;
    std::list<std::string>::iterator curLine;
    size_t curCol;
    std::pair<int, int> selectionStart;
    std::pair<int, int> selectionEnd;

  public:
    Document();
    void insert(char ch);
    void deleteForward();
    void deleteBackward();
    void cursorLeft();
    void cursorRight();
    void cursorUp();
    void cursorDown();
    void cursorHome();
    void cursorEnd();
    void print();
    void drawCursor();
    void moveCursor(int line, int col);
    void setSelectionStart();
    void setSelectionEnd();
    void clearSelection();

    bool selecting = false;
};

#endif

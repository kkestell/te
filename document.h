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
    void clearSelection();

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
    void setSelectionStart(int line, int col);
    void setSelectionEnd(int line, int col);
};

#endif

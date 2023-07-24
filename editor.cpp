#include "editor.h"
#include <clocale>

#define KEY_CTRL(x) ((x)&0x1f)

Editor::Editor()
{
    setlocale(LC_ALL, ""); // set locale to the user's default
    initscr();             // initialize the library and the terminal settings
    raw();                 // disable line buffering
    noecho();              // don't echo input
    keypad(stdscr, TRUE);  // enable reading of function keys, arrow keys etc.

    mousemask(ALL_MOUSE_EVENTS | REPORT_MOUSE_POSITION, nullptr); // enable mouse events
}

Editor::~Editor()
{
    endwin();
}

void Editor::run()
{
    while (true)
    {
        MEVENT event;
        int ch = getch();
        switch (ch)
        {
        case KEY_CTRL('q'):
            goto end;
            break;
        case '\n':
            doc.insert('\n');
            break;
        case KEY_LEFT:
            doc.cursorLeft();
            break;
        case KEY_RIGHT:
            doc.cursorRight();
            break;
        case KEY_UP:
            doc.cursorUp();
            break;
        case KEY_DOWN:
            doc.cursorDown();
            break;
        case KEY_HOME:
            doc.cursorHome();
            break;
        case KEY_END:
            doc.cursorEnd();
            break;
        case KEY_BACKSPACE:
            doc.deleteBackward();
            break;
        case KEY_DC: // DEL key
            doc.deleteForward();
            break;
        case KEY_RESIZE:
            break;
        case KEY_MOUSE:
            if (getmouse(&event) != OK)
                continue;
            if (event.bstate & BUTTON1_PRESSED)
                doc.setSelectionStart(event.y, event.x);
            else if (event.bstate & BUTTON1_RELEASED)
                doc.setSelectionEnd(event.y, event.x);
            else if (event.bstate & BUTTON1_CLICKED)
                doc.moveCursor(event.y, event.x);
            break;
        default:
            if (ch < 256)
                doc.insert(ch);
            break;
        }

        doc.print();
        refresh();
    }
end:
    return;
}

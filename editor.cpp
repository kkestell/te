#include "editor.h"
#include <clocale>
#include <cctype>

#define KEY_CTRL(x) ((x)&0x1f)
#define CTRL_C 3
#define CTRL_V 22
#define CTRL_X 24
#define CTRL_Z 26
#define CTRL_Y 25
#define CTRL_K 11
#define ESC 27

Editor::Editor() : doc("../document.cpp")
{
}

Editor::~Editor()
{
    endwin();
}

void Editor::init()
{
    setlocale(LC_ALL, ""); // set locale to the user's default
    initscr();             // initialize the library and the terminal settings
    raw();                 // disable line buffering
    noecho();              // don't echo input
    keypad(stdscr, TRUE);  // enable reading of function keys, arrow keys etc.
    curs_set(0);           // hide the cursor

#ifdef NCURSES_VERSION
    set_escdelay(25); // allows capturing alt key combinations
#endif
}

void Editor::run()
{
    doc.print();
    refresh();

    while (true)
    {
        MEVENT event;
        int ch = getch();
        switch (ch)
        {
        case KEY_CTRL('q'):
            goto end;
        case KEY_LEFT:
            doc.cursor_left();
            break;
        case KEY_RIGHT:
            doc.cursor_right();
            break;
        case KEY_UP:
            doc.cursor_up();
            break;
        case KEY_DOWN:
            doc.cursor_down();
            break;
        case KEY_HOME:
            doc.cursor_home();
            break;
        case KEY_END:
            doc.cursor_end();
            break;
        case KEY_BACKSPACE:
            doc.delete_backward();
            break;
        case KEY_DC: // DEL key
            doc.delete_forward();
            break;
        case KEY_CTRL('c'):
            // TODO: Copy
            break;
        case KEY_CTRL('v'):
            // TODO: Paste
            break;
        case KEY_CTRL('x'):
            // TODO: Cut
            break;
        case KEY_CTRL('z'):
            // TODO: Undo
            break;
        case KEY_CTRL('y'):
            // TODO: Redo
            break;
        case ESC:
            ch = getch();
            switch (ch)
            {
            case 's':
                doc.set_selection_start();
                break;
            case 'x':
                doc.clear_selection();
                break;
            default:
                // ignore
                break;
            }
        case KEY_RESIZE:
            break;
        default:
            if (ch >= 0 && ch <= 255 && (isprint(ch) || ch == '\n' || ch == '\t'))
                doc.insert((char)ch);
            break;
        }

        doc.print();
    }
end:
    return;
}

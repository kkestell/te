#include "document.h"
#include "utf8.h"
#include <fstream>
#include <list>
#include <ncurses.h>
#include <string>

Document::Document(const std::string& filename)
{
    std::ifstream file(filename);
    std::string line;

    if (file.is_open())
    {
        while (std::getline(file, line))
        {
            lines.push_back(line);
        }
        file.close();
    }
    else
    {
        // If the file cannot be opened, initialize the document with an empty line.
        lines.emplace_back("");
    }

    cur_line = lines.begin();
    cur_col = 0;
    clear_selection();
}

Document::Document()
{
    lines.emplace_back("");
    cur_line = lines.begin();
    cur_col = 0;
    clear_selection();
}

void Document::insert(char ch)
{
    if (ch == '\n')
    {
        // Split the current line into two lines
        std::string tail = utf8::substr(*cur_line, cur_col, utf8::str_length(*cur_line) - cur_col);
        *cur_line = utf8::substr(*cur_line, 0, cur_col);
        lines.insert(std::next(cur_line), tail);
        cur_col = 0;
        cur_line++;
    }
    else
    {
        std::string ch_str(1, ch);
        std::string pre = utf8::substr(*cur_line, 0, cur_col);
        std::string post = utf8::substr(*cur_line, cur_col, utf8::str_length(*cur_line) - cur_col);
        *cur_line = pre + ch_str + post;
        cur_col += 1;
    }
    scroll_to_cursor();
}

void Document::delete_forward()
{
    // If there's no current line, we have nothing to delete
    if (cur_line == lines.end())
        return;

    // If the cursor is at the end of the line
    if (cur_col == utf8::str_length(*cur_line))
    {
        // If the cursor is at the end of the Document
        auto next_line = std::next(cur_line);
        if (next_line == lines.end())
            return;

        // Merge current line with next line
        *cur_line += *next_line;

        // Delete the next line
        lines.erase(next_line);
    }
    else // Cursor is in the middle of the line
    {
        // Manually erase the character at the cursor using substrings
        std::string pre = utf8::substr(*cur_line, 0, cur_col);
        std::string post = utf8::substr(*cur_line, cur_col + 1, utf8::str_length(*cur_line) - cur_col - 1);
        *cur_line = pre + post;
    }
    scroll_to_cursor();
}

void Document::delete_backward()
{
    // If there's no current line, we have nothing to delete
    if (cur_line == lines.end())
        return;

    // If the cursor is at the beginning of the line
    if (cur_col == 0)
    {
        // If the cursor is at the beginning of the Document
        if (cur_line == lines.begin())
            return;

        // Store previous line iterator
        auto prev_line = std::prev(cur_line);

        // Move cursor to the end of the previous line
        cur_col = utf8::str_length(*prev_line);

        // Merge previous line with current line
        *prev_line += *cur_line;

        // Delete the current line
        lines.erase(cur_line);

        // Update current line iterator to previous line
        cur_line = prev_line;
    }
    else // Cursor is in the middle or end of the line
    {
        // Move cursor one character to the left
        cur_col--;

        // Delete the character at the new cursor position
        std::string pre = utf8::substr(*cur_line, 0, cur_col);
        std::string post = utf8::substr(*cur_line, cur_col + 1, utf8::str_length(*cur_line) - cur_col - 1);
        *cur_line = pre + post;
    }
    scroll_to_cursor();
}

void Document::clear_selection()
{
    selection_start = std::make_pair(-1, -1);
    selecting = false;
}

void Document::cursor_left()
{
    if (cur_col > 0)
    {
        cur_col--;
    }
    else if (cur_line != lines.begin())
    {
        cur_line--;
        cur_col = utf8::str_length(*cur_line);
    }
    scroll_to_cursor();
}

void Document::cursor_right()
{
    if (cur_col < utf8::str_length(*cur_line))
    {
        cur_col++;
    }
    else
    {
        auto next_line = std::next(cur_line);
        if (next_line != lines.end())
        {
            cur_line++;
            cur_col = 0;
        }
    }
    scroll_to_cursor();
}

void Document::cursor_up()
{
    if (cur_line != lines.begin())
    {
        // Store current column
        size_t old_col = cur_col;

        // Move cursor to previous line
        --cur_line;

        // Reset cursor to old column or end of line if line is shorter
        cur_col = old_col < utf8::str_length(*cur_line) ? old_col : utf8::str_length(*cur_line);
    }
    scroll_to_cursor();
}

void Document::cursor_down()
{
    auto next_line = std::next(cur_line);
    if (next_line != lines.end())
    {
        // Store current column
        size_t old_col = cur_col;

        // Move cursor to next line
        cur_line++;

        // Reset cursor to old column or end of line if line is shorter
        cur_col = old_col < utf8::str_length(*cur_line) ? old_col : utf8::str_length(*cur_line);
    }
    scroll_to_cursor();
}

void Document::cursor_home()
{
    // Move the cursor to the beginning of the current line
    cur_col = 0;
    scroll_to_cursor();
}

void Document::cursor_end()
{
    // Move the cursor to the end of the current line
    cur_col = utf8::str_length(*cur_line);
    scroll_to_cursor();
}

void Document::print()
{
    // Clear the screen
    erase();

    int line_number = 0;
    for (auto str = std::next(lines.begin(), scroll_offset.first); str != lines.end() && line_number < getmaxy(stdscr); ++str)
    {
        // Calculate the length of the substring
        int sub_len = getmaxx(stdscr) < utf8::str_length(*str) - scroll_offset.second ? getmaxx(stdscr) : utf8::str_length(*str) - scroll_offset.second;

        // Print the line with the correct substring
        mvprintw(line_number, 0, utf8::substr(*str, scroll_offset.second, sub_len).c_str());
        line_number++;
    }


    // Manually draw the cursor
    if (cur_line != lines.end())
    {
        attron(A_REVERSE);
        int char_index = utf8::terminal_to_char_index(*cur_line, cur_col);
        if (char_index < utf8::str_length(*cur_line))
        {
            // Draw the cursor on an existing character
            std::string cursor_str = utf8::substr(*cur_line, char_index, 1);
            mvprintw(std::distance(lines.begin(), cur_line) - scroll_offset.first, cur_col - scroll_offset.second, cursor_str.c_str());
        }
        else
        {
            // Draw the cursor at the end of the line
            mvprintw(std::distance(lines.begin(), cur_line) - scroll_offset.first, cur_col - scroll_offset.second, " ");
        }
        attroff(A_REVERSE);
    }
}

void Document::set_selection_start()
{
    selection_start = std::make_pair(std::distance(lines.begin(), cur_line), cur_col);
    selecting = true;
}

void Document::scroll_to_cursor()
{
    int cursor_line = std::distance(lines.begin(), cur_line);

    // check if the cursor line is above the top of the view
    if (cursor_line < scroll_offset.first)
    {
        scroll_offset.first = cursor_line;
    }
    // check if the cursor line is below the bottom of the view
    else if (cursor_line >= scroll_offset.first + getmaxy(stdscr))
    {
        scroll_offset.first = cursor_line - getmaxy(stdscr) + 1;
    }

    // check if the cursor column is to the left of the view
    if (cur_col < scroll_offset.second)
    {
        scroll_offset.second = cur_col;
    }
    // check if the cursor column is to the right of the view
    else if (cur_col >= scroll_offset.second + getmaxx(stdscr))
    {
        scroll_offset.second = cur_col - getmaxx(stdscr) + 1;
    }
}

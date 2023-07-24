#include "document.h"
#include <list>
#include <ncurses.h>
#include <string>

Document::Document()
{
    lines.emplace_back("");
    curLine = lines.begin();
    curCol = 0;
}

void Document::insert(char ch)
{
    if (ch == '\n')
    {
        // Split the current line into two lines
        std::string tail = curLine->substr(curCol);
        curLine->erase(curCol);
        lines.insert(std::next(curLine), tail);
        curCol = 0;
        curLine++;
    }
    else
    {
        // Insert the character at the cursor position
        curLine->insert(curCol++, 1, ch);
    }
}

void Document::deleteForward()
{
    // If there's no current line, we have nothing to delete
    if (curLine == lines.end())
        return;

    // If the cursor is at the end of the line
    if (curCol == curLine->size())
    {
        // If the cursor is at the end of the Document
        auto next_line = std::next(curLine);
        if (next_line == lines.end())
            return;

        // Merge current line with next line
        *curLine += *next_line;

        // Delete the next line
        lines.erase(next_line);
    }
    else // Cursor is in the middle of the line
    {
        // Delete the character at the cursor
        curLine->erase(curCol, 1);
    }
}

void Document::deleteBackward()
{
    // If there's no current line, we have nothing to delete
    if (curLine == lines.end())
        return;

    // If the cursor is at the beginning of the line
    if (curCol == 0)
    {
        // If the cursor is at the beginning of the Document
        if (curLine == lines.begin())
            return;

        // Store previous line iterator
        auto prev_line = std::prev(curLine);

        // Move cursor to the end of the previous line
        curCol = prev_line->size();

        // Merge previous line with current line
        *prev_line += *curLine;

        // Delete the current line
        lines.erase(curLine);

        // Update current line iterator to previous line
        curLine = prev_line;
    }
    else // Cursor is in the middle or end of the line
    {
        // Move cursor one character to the left
        curCol--;

        // Delete the character at the new cursor position
        curLine->erase(curCol, 1);
    }
}

void Document::clearSelection()
{
    selectionStart = selectionEnd = std::make_pair(-1, -1);
}

void Document::cursorLeft()
{
    if (curCol > 0)
    {
        curCol--;
    }
    else if (curLine != lines.begin())
    {
        curLine--;
        curCol = curLine->size();
    }
}

void Document::cursorRight()
{
    if (curCol < curLine->size())
    {
        curCol++;
    }
    else
    {
        auto next_line = std::next(curLine);
        if (next_line != lines.end())
        {
            curLine++;
            curCol = 0;
        }
    }
}

void Document::cursorUp()
{
    if (curLine != lines.begin())
    {
        // Store current column
        size_t old_col = curCol;

        // Move cursor to previous line
        --curLine;

        // Reset cursor to old column or end of line if line is shorter
        curCol = old_col < curLine->size() ? old_col : curLine->size();
    }
}

void Document::cursorDown()
{
    auto next_line = std::next(curLine);
    if (next_line != lines.end())
    {
        // Store current column
        size_t old_col = curCol;

        // Move cursor to next line
        curLine++;

        // Reset cursor to old column or end of line if line is shorter
        curCol = old_col < curLine->size() ? old_col : curLine->size();
    }
}

void Document::cursorHome()
{
    // Move the cursor to the beginning of the current line
    curCol = 0;
}

void Document::cursorEnd()
{
    // Move the cursor to the end of the current line
    curCol = curLine->size();
}

void Document::print()
{
    // Clear the screen
    clear();

    int line_number = 0;
    for (auto line = lines.begin(); line != lines.end(); line++)
    {
        std::string str = *line;

        // Check if this line is part of the selection
        if (line_number >= selectionStart.first && line_number <= selectionEnd.first)
        {
            int start_col = (line_number == selectionStart.first) ? selectionStart.second : 0;
            int end_col = (line_number == selectionEnd.first) ? selectionEnd.second : str.size();

            // Print characters before the selection
            if (start_col > 0) {
                mvprintw(line_number, 0, str.substr(0, start_col).c_str());
            }

            // Reverse the characters in the selection
            attron(A_REVERSE);
            mvprintw(line_number, start_col, str.substr(start_col, end_col - start_col).c_str());
            attroff(A_REVERSE);

            // Print the rest of the line normally
            if (end_col < str.size()) {
                mvprintw(line_number, end_col, str.substr(end_col).c_str());
            }
        }
        else
        {
            // Print the line normally
            mvprintw(line_number, 0, str.c_str());
        }

        line_number++;
    }

    drawCursor();

    // Update the physical screen
    refresh();
}

void Document::drawCursor()
{
    // Move the cursor to the current line and column
    move(std::distance(lines.begin(), curLine), curCol);
}

void Document::moveCursor(int line, int col)
{
    if (selectionStart.first > -1 && selectionEnd.first > -1)
    {
        clearSelection();
        return;
    }

    // Validate and correct the line number
    if (line < 0)
    {
        line = 0;
    }
    else if (line >= lines.size())
    {
        line = lines.size() - 1;
    }

    // Move the line cursor
    curLine = std::next(lines.begin(), line);

    // Validate and correct the column number
    if (col < 0)
    {
        col = 0;
    }
    else if (col > curLine->size())
    {
        col = curLine->size();
    }

    // Move the column cursor
    curCol = col;
}

void Document::setSelectionStart(int line, int col)
{
    selectionStart = std::make_pair(line, col);
}

void Document::setSelectionEnd(int line, int col)
{
    selectionEnd = std::make_pair(line, col);
}

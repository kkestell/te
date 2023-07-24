void assertEqual(std::string expected, std::string actual, std::string message)
{
    if (expected == actual)
    {
        std::cout << message << ": Test Passed\n";
    }
    else
    {
        std::cout << message << ": Test Failed. Expected '" << expected << "', but got '" << actual << "'\n";
    }
}

void assertEqual(int expected, int actual, std::string message)
{
    if (expected == actual)
    {
        std::cout << message << ": Test Passed\n";
    }
    else
    {
        std::cout << message << ": Test Failed. Expected '" << expected << "', but got '" << actual << "'\n";
    }
}

void testInsert()
{
    Document doc;
    doc.insert('H');
    doc.insert('e');
    doc.insert('l');
    doc.insert('l');
    doc.insert('o');
    doc.insert(',');
    doc.insert(' ');
    doc.insert('w');
    doc.insert('o');
    doc.insert('r');
    doc.insert('l');
    doc.insert('d');
    doc.insert('!');
    assertEqual("Hello, world!", doc.getCurrentLine(), "Insert test");
}

void testNewlineInsert()
{
    Document doc;
    doc.insert('H');
    doc.insert('\n');
    doc.insert('H');
    assertEqual("H", doc.getCurrentLine(), "Newline insert test");
}

void testCursorLeft()
{
    Document doc;
    doc.insert('H');
    doc.cursorLeft();
    assertEqual("H", doc.getCurrentLine(), "Cursor left test");
}

void testCursorHome()
{
    Document doc;
    doc.insert('H');
    doc.cursor_home();
    assertEqual("H", doc.getCurrentLine(), "Cursor home test");
}

void testCursorEnd()
{
    Document doc;
    doc.insert('H');
    doc.cursorEnd();
    assertEqual("H", doc.getCurrentLine(), "Cursor end test");
}

void testDeleteForwardEmptyDocument()
{
    Document doc;
    doc.delete_forward();
    assertEqual("", doc.getCurrentLine(), "Delete forward on an empty Document test");
}

void testDeleteForwardEndOfDocumentMultipleCharacters()
{
    Document doc;
    doc.insert('H');
    doc.insert('i');
    doc.delete_forward();
    assertEqual("Hi", doc.getCurrentLine(), "Delete forward at end of Document with multiple characters test");
}

void testDeleteForwardEndOfLineNotEndOfDocument()
{
    Document doc;

    doc.insert('H');
    doc.insert('i');
    doc.insert('\n');
    doc.insert('B');
    doc.insert('y');
    doc.insert('e');
    doc.cursorUp();
    doc.cursorHome();
    doc.cursor_right();
    doc.cursor_right();
    doc.delete_forward();

    assertEqual("HiBye", doc.getCurrentLine(), "Delete forward end of line not end of Document test");
    assertEqual(0, doc.getCursorLine(), "Delete forward end of line not end of Document test");
    assertEqual(2, doc.getCursorCol(), "Delete forward end of line not end of Document test");
}

void testDeleteForwardMiddleOfLine()
{
    Document doc;
    doc.insert('A');
    doc.insert('B');
    doc.insert('C');
    doc.insert('D');
    doc.cursorLeft();
    doc.cursorLeft();
    doc.deleteForward();
    assertEqual("ABD", doc.getCurrentLine(), "Delete forward in middle of line test");
}

void testCursorRight()
{
    Document doc;
    doc.insert('A');
    doc.insert('B');
    doc.insert('B');
    doc.cursor_right();
    assertEqual("ABC", doc.getCurrentLine(), "Cursor right test");
}

void testCursorRightEndOfDocument()
{
    Document doc;
    doc.insert('A');
    doc.insert('B');
    doc.insert('C');
    doc.cursor_right();
    assertEqual("ABC", doc.getCurrentLine(), "Cursor right at end of Document test");
}

void testCursorRightEndOfLine()
{
    Document doc;
    doc.insert('A');
    doc.insert('B');
    doc.insert('C');
    doc.insert('\n');
    doc.insert('D');
    doc.insert('E');
    doc.insert('F');
    doc.cursorLeft();
    doc.cursorLeft();
    doc.cursorLeft();
    doc.cursorLeft();
    assertEqual(0, doc.getCursorLine(), "Cursor right at end of line test");
    doc.cursorRight();
    assertEqual("DEF", doc.getCurrentLine(), "Cursor right at end of line test");
}

void runTests()
{
    testInsert();
    testNewlineInsert();
    testCursorLeft();
    testCursorRight();
    testCursorHome();
    testCursorEnd();

    testDeleteForwardEmptyDocument();
    testDeleteForwardEndOfDocumentMultipleCharacters();
    testDeleteForwardEndOfLineNotEndOfDocument();
    testDeleteForwardMiddleOfLine();

    testCursorRightEndOfDocument();
    testCursorRightEndOfLine();
}

int main()
{
    runTests();
    return 0;
}
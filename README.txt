James Gammon
rdParser Project 3

To Run:
Simply call make and then ./statements.x <.txt>


If and input fails, it is most likely due to indentation. Try adding a new line a the bottom of the file. That should
allow for the test file to execute properly. It comes from a strange little bug in my Tokenizer.cpp where i'm handling
indentation at levels N>1 straight to N where EOF is the next token. 

SocialNet

Ayaan Maan

Introduction

Command-line simulator for a social network’s (undirected graph) backend services including managing users (nodes), friendships (edges) and 
user content.

How to Run the Program

Compilation

Open your terminal or command prompt, navigate to the directory containing filesystem.cpp, and run the following command to compile the code:

g++ -o socialnet socialnet.cpp -std=c++23

Execution

To start the program, run the compiled executable from your terminal:

./socialnet

In windows run:

./socialnet.exe

The program will then wait for commands.

Commands

The program accepts a series of commands to interact with the network. Commands are case-sensitive and all capitalised.

File Operations

    ADD_USER <username>

        Add a new user to the network.

        Example: ADD_USER A_y12m

    ADD_FRIEND <uesrname1> <username2>

        Adds a friendship between two users. 

        Example: ADD_FRIEND DJi34 Qeik_42

    LIST_FRIENDS <username>

        Prints an alphabetically sorted list of the specified username’s friends.

        Example: LIST_FRIENDS riwj23

    SUGGEST_FRIENDS <username> <N>

        Recommends up to N new friends who are ”friends of a friend” but not already friends.
            
            • Ranking: Recommendations are ranked by the number of mutual friends (descending). Ties are broken by alphabetical order of usernames.
            
            • Edge Cases: If N is 0, output nothing. If fewer than N candidates exist, list all available.

        Example: SUGGEST_FRIENDS JIK24_23d 4

    DEGREES_OF_SEPARATION <uesrname1> <username2>

        Calculates the length of the shortest path of friendships between two usernames. If no path exists, reports -1

        Example: DEGREES_OF_SEPARATION HUh3 Ayr_F42

    ADD_POST <username> <post content>
    
        Adds a post whose content is the post content string, to the posts created by the specified user.

        Example: ADD_POST I_jn4kj My first post!

    OUTPUT_POSTS <username> <N>

        Outputs the most recent N posts of the user, in reverse chronological order. If there are fewer than N posts by the user or N is -1, 
        then outputs all the posts by the user.

        Example: OUTPUT_POSTS FH34J4f_f 10

Application Control

    QUIT or EXIT

        Closes the application.

Input Assumptions

    1. All usernames contain only latin alphabets, numbers 0-9 and underscore symbol. 
       There is no problem with allowing other characters but this was done in spirit of Twitter's allowed usernames.

    2. Any garbage input after valid input in the same line will be ignored.


Error Handling

Incorrect or inconsistent input is handled by printing specific error messages to the console as follows:

    NameError: Invalid username. Only alphabets, numbers 0-9 or underscore _ allowed

    DuplicateError: User <username> already exists

    Error 404: User " <username> not found

    CommandError: Undefined command

    FindError: Atleast one user was not found

    TypeError: Required input was of wrong type

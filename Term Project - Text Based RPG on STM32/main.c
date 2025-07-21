/*
 * Name: Marlon Morales
 * Date: [07/22/2025]
 * Course: CPE 2200 Hardware Programming
 *
 * Term Project - Develop an interactive STM32 text-based RPG simulating survival in space
 * ______________________________________________________________________________________________________________________
 * Objective: Character creation with Easy/Hard mode
 * Make critical decisions to keep the space pod in orbit and survive the descent back home
 * Manage systems within the pod such as power, oxygen, navigation, communications, and solve a puzzle.
 * Explore different story paths based on the user's decision
 * Display ASCII art while keeping it within the memory limits of the microcontroller
 * ______________________________________________________________________________________________________________________
 */

#include <stdio.h>// Standard I/O library for printf, scanf, etc.
extern void initialise_monitor_handles(void); // Semi-hosting setup (for printf output to IDE console)
#include <string.h> // String handling functions ( strcpy, strlen, strcmp)
#include <ctype.h>  // Character handling functions ( isdigit, isalpha)
#include <stdlib.h> // General utilities (atoi)

// Global game state variables
int difficulty, shields, navigation, coolant, navUpgrade, alive = 1;
char character[20];
// Function to clear any leftover characters from input buffer
void clearInput() {
    int c;
    while ((c = getchar()) != '\n' && c != EOF);// Read and discard characters until newline or end-of-file
}



// This function checks for valid input from the user and keeps from falling into continuous loop
//-------------------------------------------------------------------------------------------------
int getNumericInput(int min, int max) {
    char input[10];// Buffer to hold user input as a string
    int number;    // Variable to store the converted integer
    while (1) {    // Infinite loop until valid input is received
        printf(" Selection: "); // Prompt symbol for user input

      // Read a line of input from stdin (up to 9 chars + null terminator)
        if (!fgets(input, sizeof(input), stdin)) {
            printf("                     Input error. Try again.\n");
            clearInput(); // Clear any leftover input
            continue;     // Restart loop
        }

        // Check if input is all digits
        int isNumber = 1;
        for (int i = 0; input[i] != '\0' && input[i] != '\n'; i++) {
            if (!isdigit(input[i])) {
                isNumber = 0; // Found a non-digit character
                break;
            }
        }
        // If non-numeric input, prompt again
        if (!isNumber) {
            printf("                     Please enter a number between %d and %d.\n", min, max);
            continue;
        }
        // Convert string to integer
        number = atoi(input);

        // Check if number is within the allowed range
        if (number >= min && number <= max) {
            return number; // Valid input; return value
        } else {
            printf("                     Please enter a number between %d and %d.\n", min, max);
        }
    }
}



// Function that displays Main Menu ASCII Art 1
//------------------------------------------------------------------------------------------------------
void showTitleArt(){
	    printf("                                                                                                ..x$$$$$$Xx+.                                               \n");
	    printf("                                                                                          ..::....;++x$&&&&&x                                               \n");
	    printf("                                                                                       .::::...  ..x$&&&&&&&$                                               \n");
	    printf("                                                                                    ;+;:::.   ....::X&&&&&&&$                                               \n");
	    printf("                                                                                 .x$$$;..   ..;;;:::;+&&&&&&x                                               \n");
	    printf("                                                                                +$$$$X:  ...xXxx++x;;;;x$&&&:                                               \n");
	    printf("                                                                              .;;$$x;:+x...x$$XxxxX$+;;+++x+                                                \n");
	    printf("                                                                            .;;::::;x$$$X;:$&&&$$$&&x++++xx.                                                \n");
	    printf("                                                             :+xx++;;;++;;.;$$;.. :$$$$$$$++$&&&&&&X+++xxx:                                                 \n");
	    printf("                                                          .+$xxxxxxxxx+;;x$$$$;....;$$$$Xx$$xxXX++x++xxxx;                                                  \n");
	    printf("                                                         ;$&$$$$$$XXXXXXx;;$x:xX;...;X$$XX$&$$x++++xxx++:                                                   \n");
	    printf("                                                        X&&&&&$$$$$$$$$x;:..;$$$$x::::x&&&&&&&&&$xxxx+;.                                                    \n");
	    printf("                                                       X&&&&&&&&$$$$$$X;.. .:x$$$$$+;;;;x&&&&&&&&&&$x;.                                                     \n");
	    printf("                                                      x&&&&&&&&&&&&&&$:.. ..:;++xX$$$+;;;+x&&&&&&&&&;                                                       \n");
	    printf("                                                     :&&&&&&&&&&&&&&&:....:;:..;xxX$&&$++++xxX&&&&x                                                         \n");
	    printf("                                                     $&&&&&&&&&$;    .;:.;:..;xxxXX$&&&&&xxx++;;.                                                           \n");
	    printf("                                                    :&&&&&&&$;      ..;;;::;xxxXX$$$&&&&&&&$x;.                                                             \n");
	    printf("                                                    X&&&&&$:      ..:;x+;+X$$X$$$$X++x$&&&&X:                                                               \n");
	    printf("                                                   .&&&&&;       .;;+$+x$$$$$$$$Xx+xxx++xXx                                                                 \n");
	    printf("                                                   +&&&x    .::::;&$$+$&&&&&&$x++xxxx+xx+:x:                                                                \n");
	    printf("                                                   X&$;  .;;;;::::x$$&&&&&&$x;.;+xxxxxxXx;xx                                                                \n");
	    printf("                                                        ;+++;;;;;$&&&&&&&Xx;:.;xXXXXXX$$x;Xx                                                                \n");
	    printf("                                                       ;xxxxx++x&&&&&&$$X;.  .$$$$$$$$$$x+&x                                                                \n");
	    printf("                                                      .xxxxxxx$&&&&&X;:.    .&&&&&&&&&&&x$&.                                                                \n");
	    printf("                                                     .+xXXXXX&&&&&Xx++;;:.   ;&&&&&&&&&&&X$&;                                                               \n");
	    printf("                                                     ;XX$$$$$&&$Xx++;;.  :$&&&&&&&&&&&$&&;                                                                  \n");
	    printf("                                                    ;xX$$$$$$$Xxxx++;: ;$&&&&&&&&&&&&&&x                                                                    \n");
	    printf("                                                  .+x;;X$$$x   .xx++;+&&&&&&&&&&&&&&$:                                                                      \n");
	    printf("                                                     ;xXXX;    :xxx$&&&&&&&&&&&$x;.                                                                         \n");
	    printf("                                                 .;+xxxx;.   :++:.+&$$Xxx;:..                                                                               \n");
	    printf("                                                .;;;;.                                                                                                      \n");
	    printf("                                               .:.                                                                                                          \n");
	    printf("                                                                                                                                                            \n");
	    printf("                                     ..          ...                                                            ...                                         \n");
	    printf("                       x&$$$$$$$x.;&$++x&$.  .x&$x;+X&x.      x&X     &&$$$$$&&;.$&$$$$$$$;    x&$$$$$&&$.  ;&&x+;x$&$.  &&$$$$$&&x                         \n");
	    printf("                       xx        ;&:     ;x .&x       +&     x$.&X    &;      .&x&;            x$       x$;$$        .&x &x       ;&;                       \n");
	    printf("                       xX:.:..:.  $&x;:..   XX.             +&. .&+   &;      .&x&x.......     x$       x$$$          :&:&x        +$.                      \n");
	    printf("                       x$;;;;;;;.   .;+x$&$.Xx             ;&:   .&;  &&$$$$$&$;.&x;;;;;;:     x&$$$$$$$x.XX          :&:&x        ;$.                      \n");
	    printf("                       xx       .X+       x&;&:        ;&;:&&&&&&&&&. &;        .&;            x$         ;&;         X$.&x        $x                       \n");
	    printf("                       xX.........&&;.   +&+ ;&$:    :$&..$x       x& &;        .&+........    x$          .&&;.   .x&x  &X....:+&&+                        \n");
	    printf("                       .::::::::.   :+xx+:      ;+xxx;   ..         ..:.         :::::::::.    ..             :+xxx;.    :::::::.                           \n");
	    printf("_____________________________________________________________________________________________________________________________________________________       \n");
        printf("\n");
	    printf("                                                                     MAIN MENU        \n");
	    printf("                                                                 -----------------    \n");
	    printf("                                                                 [1] START GAME       \n");
	    printf("                                                                 [2]    EXIT          \n");
}



// Function that displays character selection 1-3 and confirms the choice of the user
//--------------------------------------------------------------------------------------
void selectCharacter() {
    int choice;
    printf("\nChoose Character:\n[1] Engineer  [2] Soldier  [3] Pilot\n");
    choice = getNumericInput(1, 3); // Get validated numeric input from user (1 to 3)

    if (choice == 1) {
        strcpy(character, "Engineer");// Set character name to "Engineer"
        showEngineerArt();            // Show Engineer ASCII art or graphic
    } else if (choice == 2) {
        strcpy(character, "Soldier"); // Set character name to "Soldier"
        showSoldierArt();             // Show Soldier ASCII art or graphic
    } else {
        strcpy(character, "Pilot");   // Set character name to "Pilot"
        showPilotArt();               // Show Pilot ASCII art or graphic
    }
}

//                               ASCII CHARACTER SELECTION FUNCTIONS
//-----------------------------------------------------------------------------------------------//
void showEngineerArt() {
	        printf("\n");
			printf("                                    o o                           \n");
			printf("                                    | |                           \n");
			printf("                                   _L_L_                          \n");
			printf("                               }\\/__-__\\/{                      \n");
			printf("                                }(|~o.o~|){                       \n");
			printf("                                }/\\`-'/ \\{                      \n");
			printf("                                 _/ `U'\\_                        \n");
			printf("                                (  .   .  )                       \n");
			printf("                                | //   \\ |   -- Engineer --      \n");
			printf("                               \\ |  ,  | //                      \n");
			printf("                                \\|=====|//                       \n");
			printf("                                 (|_.^._|)                        \n");
			printf("                                  | | | |                         \n");
			printf("                                  ( ) ( )                         \n");
			printf("                                  |_| |_|                         \n");
			printf("                              _.-' _j L_ '-._                     \n");
			printf("                             (___.'     '.___)                    \n");
			printf(" _______________________________________________________\n");
}

void showSoldierArt() {
	        printf("                                   o   o                                \n");
			printf("                                   \\_//                                \n");
			printf("                                   ( Oo)                    \\|/        \n");
			printf("                                   (_=-)  .===O-  ~~Z~A~P~~ -O-         \n");
			printf("                                   /   \\_/U'               /|\\        \n");
			printf("                                   ||  |_/                              \n");
			printf("                                   \\  |            -- Soldier --       \n");
			printf("                                   {K ||                                \n");
			printf("                                    | PP                                 \n");
			printf("                                    | ||                                 \n");
			printf("                                   (__\\                                \n");
			printf(" _______________________________________________________\n");
}
void showPilotArt() {
	        printf("                                   _____                                \n");
			printf("                                ,-"     "-.                             \n");
			printf("                               / o       o \\                           \n");
			printf("                              /  \\     /   \\                          \n");
			printf("                             /     )-:-(     \\                         \n");
			printf("                            /     ( 6 6 )     \\                        \n");
			printf("                           /      \\ ' /       \\                       \n");
			printf("                          /         )=(         \\      -- Pilot --     \n");
			printf("                         /   o   .--" "--.   o   \\                     \n");
			printf("                        /    I  /  -   -  \\ I    \\                    \n");
			printf("                    .--(    (_}y/\\      / \\y{_)    )--.               \n");
			printf("                   (    .___l\\/_\\_____/__ \\/l___,     )              \n");
			printf("                    \\                                 //                \n");
			printf("                       -._      o O o O o O o       _.-                   \n");
			printf("                          `--Y--.___________.--Y--'                      \n");
			printf("                             |==.___________.==|                          \n");
			printf("                             `==.___________.=='                          \n");
			printf(" _______________________________________________________\n");
}
//_________________________________________________________________________________________________


// Function that handles the selection of the difficulty of the game
//-----------------------------------------------------------------------------------------//
void selectDifficulty() {
    int choice;
    printf("\nSelect Difficulty:\n[1] Easy  [2] Hard\n");
    choice = getNumericInput(1, 2);

    difficulty = choice;
    printf("You selected %s difficulty.\n", (difficulty == 1 ? "Easy" : "Hard"));
}


void showPodSceneASCIIArt(){
printf("	                                  ENTERING SPACE POD...\n");
printf("____________________________________________________________________________________________________\n");
printf("::::::::;:;;:..........:;;;XX+;;+++++++++;++;;;;+++++;;+++;++;++++;++++x$++;:..........;;;;:::::::::\n");
printf(":::::::::;;:;;..........:;+;$x;+;++++++++++++++++++xx++++++++++++++++++$X+;:..........:;;;::::::::::\n");
printf(":;:::::::::;:;;..........:;+;$X++++++;;;;::x$$$$$+x$&&X$Xx;;;;;;++++++XX+;:..........:;;;:::::::::..\n");
printf("+;;:::::::::;;;+:.........;;;;+$;;;;+;;;;;;+$$$X$xX$XXX$Xx+::::::::;;&;+;;..........;;;;:::::::::.:;\n");
printf("++;;::::::::::;:+:........;:+;;&$;::;;;:;;;;x+x+x+xXX+x++;;;;;++++;:x&;;;:.........;:;::::::::::..:;\n");
printf("+x++;::::::::::;:;;...:;;.;::;:;&x;;;;;;::;;xxxXxxXXXXXX+;;;;;;:;+;.&;;;::+:+.....;;;:::::::::.:;:;:\n");
printf(";xx++;;:;::::::::;:;:;.;....::;;+&+:;;;;;;;:x$;X+XXXXx;x+;:::;;;;;.&+;;.....::;;:;;:::::::::..::;;.:\n");
printf("++++xx++;:::::::::;;;+.........;;x$;;;;;;;:;;&+x+X&XXXXX;;:;;;::::X+;+.........;;;::::::::.;+;;+....\n");
printf("x++;;+x+++;;:::::::;;;:........:;:X$;;:;::;;:&&&$$$XXxXx::;:.:;::+xxx;........:;;::::::::;+;;::::...\n");
printf("+;++x+;;+++;;;:::::::;:;........:::$x;:x+;x;;:::::::::.:++Xx++x::xx+:........:+;:::::::.+;;;..+.:;:.\n");
printf("+++++++x;;+++;;;::::::::;........:;;&;;+XXX;;:+xx++xxx+;++XXx+;:xX+:........:;:::::::.;;;+:.;..::::+\n");
printf("xx++++++;;+++++++;::::::+:.......::;;&:;+;:::::;:;;:::;;;::;:;:+$;::;......;:::::::.:;;:;::......;::\n");
printf(":;;+++++..;;;++++++;::::::;;;;::...:;;&xx++;;++:xxxxxx;+;++;+++&;+....:;;::::::::.::;;;....;...+xxx+\n");
printf("+x+++:....:+++;++++++:::::::;:.......+;x:;+++xX:+++;;+++Xxx+;:X+;.......;;:::::.:+;;x..++:...+Xx+;++\n");
printf("+++;:....+++xxxx+;+xx++::::::;;.....:.++;+Xxx+;;;;;:.;x+XxxX;++;;......:;:::::;::;;++:.::xx...+xxxx+\n");
printf(";;:....;+;;+;++;;+;+++xx;:::::;:;:....;++;;;+;.;+;;++:x+::;;+++:....::;::::::..:+++;+;:....:xxxxxxXx\n");
printf("+;;:.:::;;+;;;;x+;x+;;;x++;::::::......;++;;;;+++++++++++;;+xx;.....:::::::..;:;x+;;.;x+++xxxxxx+++x\n");
printf("+++++;;;:::;++++++;:..::++x+;:::::;++++x;:::;+++++++++;;:;;;+++x+..::::::;;+;:++++x;Xx+x+xx+;+;;++;:\n");
printf(".::;;+++x;;;+xx;;:;:::;;.;;;x+++++;::::.;++;:+x+++++xX:::;;;;;..;++x++;;::;:;+;+++x+++++xxx;+++;:;;:\n");
printf(":......::+++++;;;++;;::...:+X+:...:..:..:;+::++++++++;+xx+;::.:;.:;..:;;++;xx;+++xxx++;;;;;+;+;;+;:+\n");
printf(".....:..:.;;;;+++++;;;;;;+++;:.;;:;..x:.+:.:;:;;;;;:::::...;;.;+:.;;...;++;::;;++;:;x+x+;;;+;+:;+;:x\n");
printf(".......;....:.:;::+++x++++;;:...:;;++.:+::$$XXXXXXXXXX$XX&:.;;:;+++:..:;:+;;:++;;+++;+;+;:;;;;++;+;:\n");
printf(":...:.;:....:....::::..+x:.:::...:...;:.+$xXx:XXXXxxxXxxXX&$.;;;........:;:+;+++++++x;;;++x;;+xxx++;\n");
printf(";:....;...;:....:.+x..+x:;:.:::::..:++.XXxXXXXXXXXXXx+x$XXXXX..::.:.::..:;.++x+xx++++:;;++xx+;+++X$+\n");
printf(":;+xx+:+X$x;:;+;.++:.:+:.:.:.:..:..+.x$XxxxXXXxxxx+..$Xxxxxx+x$.;+::.....::;+x+;+++++;;;++xx+;x;:;;:\n");
printf("$x;.:..X.:+xx;;;;++++++..;;+;;;+;;;+;xx++xxxxxxxxx:.+x+++++;::++.;:..:;:.::;xxxX++x$x;;;;::;xX+....+\n");
printf("xx&$...x$$$$Xxx+x$&$:+;...........;+x+;;++;+++:;::;++;;;;:::::::.;;+x++xxxx;::::++++;++x;;:........+\n");
printf("::X$...++XxXxxx;:::Xxx;..:..:;:.:;;xx::::;::::::::;;:::::::::::;.+++xxXXxxx::+xXXxx+x++x:;::...:...+\n");
printf("+;x$:..;+Xxx+::.:::x+::.::::::.:;;;;X:::::::::::::::::::::::;....+;;;;;;+++:.+X++xxxx+++;:;..;+:.:;+\n");
printf("x+;$;...;X++::::::::+.+..;:;;;++x;:+x:::::::::::::::::::::.......;:+x++Xxxx+.+++;++++++++..........:\n");
printf(".:+&x...X$+;::::::::+:+.....;:.:;.;+.+x::::::::::::::............;.:;;::;:..;.+++++++;;;;;+;++++++++\n");
printf(":;+&X...;x+;::::::::x+;:.+;;+;:::::;+;.......................;.:;;.:++.++;:.:;+;:;;++;++;+;:;......;\n");
printf("&&&&$...:;+;++++::;+x:.........:.:::;+;....................+..:::..........;.;x+:....+xx+;;;+;:::;::\n");
printf("&&&&;....;::...;;::...:+..;xX+;:;xXx::++;:..................::;.....;;:+;::::x+.....:+XXx;::.+;;;;;x\n");
printf("..:++.........::...:;+;X$+;;X$xx$X++;:..++;;+;.......::;;.::;..............;x+.;....;Xxx+:;+:+;:;::+\n");
printf(".....::......:x&$X;::;xXXXXx:+xx:......::.:++;::..::.:;:;;++..............:++:;....:+$xx;;+::+;;;;;+\n");
printf(".....xX$&$;..;x+xX$X:.;xXx+:.....:;;+;.....++$xx+$&&&&&x+;:............::;++x:....::XxxX+xx;;;+;;:::\n");
printf("+...;x;X:+XX:..xxXXxx:......;++XX:......:;::;&&$XXX$XX+;;.;:...:...:::::;;x+.:....;x&Xx++x+:;;::;;+.\n");
printf(";+;...+XXxxX$;...:....;.....:;+;......X&&.;;;$$XXXXX$XXx::;:::;:........::;xx::..:;X$xX;:::.;+;;+;;:\n");
printf(";++;...::+;;.......xxXx+Xx+:.......;&&&&X;;;;&&$$$XXXxX&:.;;::;:.:.:;;......;X+++xX&$X+;;;+;+:.::::;\n");
printf(";;+;+...............;+$$;;.......$&&&&&&;x;;:&&$&&;+&X&&:.;;:+::;;..;.;;.......;x+xx$X;+::+;x.+;:+:;\n");
printf(";;;+;+;.....::................+&&&&&$&&x;:.;;&+:;+:;+;+xx.;;;+;;;:::.;;;xxxxxXxXx$x&$;+x;;;;::+;+;;+\n");
printf("++++++;+;;......:;:.........$&&&&&$+++&;;:;::::::::::::::::::::::::::::XX+;;:;;;x&&&xXx::..;.+;++;++\n");
printf(";;;;;;+++;+;.............:&&&&&&x+Xx+X+;;;x:::.:::::::::::.::::::::::::::;x&&X$&&&&&;+:+$;+;.x:;;;++\n");
printf(";;;;;;;+;;;;+;;:.......$&&&&&$$;+;xx+xx;:::::::::::::::::::::::::::::;;;;:.+x::::X$;:::;;;xX;.:;;++;\n");
printf(";+++;;;;+;;++++;+....&&&&&&&Xxxxx;+x+Xxx;:;..:.:::::::::::::::::::::.x;:+:.:&&x;&X;:::::;++:+xX;;;+:\n");
printf(";+++++xX$x+;;;+;+.$&&&&&&&$;;+;$+++.;x;+;;x;:..::::::::::::::..::::........:X&&&X.:::::::;+;+:.:+x:.\n");
printf(";;;;;;++&&&&&&&&&&&&&&$xXxxx+xX+X....:++;x&+::::::::::::::::::::::;;+:......X&&&$;::::::::;::;;++:.:\n");
printf(";;;;;+++.&&&&&&&&&&&&&++;X;++xx+x+:..;;:x&x:::::::::::::::::::::::::::.:;+x++XXx+++;:::::.x;;....:..\n");
printf(";;;;;;;+::&&&&&&&&&&X+xxx+X+x+++xX++;;;+&$:::::::::::::::::::::::::::::::::x:..:..........;..x+++xxX\n");
printf("++++;;;+;.&&&&&&&&Xxx;+X;;Xx+;:..+xx;;+&&:::::::::::::::::::::::::::::::::;xXx..............;:.;x&&&\n");
printf(";;;;;;;;+.X&&&&++++++x+;xxxx+....:++;x&&+:::::::::::::::::::::::::;:::::::::Xxx:..............:+;+::\n");
printf(";;;;;;;;+::&&&&x+++;;;++++;xX+:.++;:X&&;:::::.::::::::::::::::::::::::::;:::;xXx+...............++XX\n");
printf(";;;;;;;;+;.&&&&&x+++++x+;++;xXx;;;;$&&+:::::::::::::::::::::::::::::.:..........;;..............:++&\n");
printf(";;;;;;;;++.x&&&&x+++xx;;;:;;:+x++;$&&;::.:::::::::::::::::::::::::::..::........+;x+;+xx++++xx+x+;&&\n");
printf("____________________________________________________________________________________________________\n");
}

// Function is called when the ship Launched
//-----------------------------------------------------------------------------------------
void showShipASCIIArt() {
    printf("\nShip is now launching, good luck!\n");
	printf("                                    ........................................\n");
    printf("                                    ........................................\n");
    printf("                                    ........................................\n");
    printf("                                    ...................x$...................\n");
    printf("                                    ...................x$...................\n");
    printf("                                    ....X$.............X$............:&;....\n");
    printf("                                    ....&&.............X$............+&X....\n");
    printf("                                    ...;&&.............X&............x&&....\n");
    printf("                                    ...X&&;........;+++$&+++;........$&&;...\n");
    printf("                                    ...&&&$.......++++;$&+++++:......&&&$...\n");
    printf("                                    ..;&&&&:.....;;;;;;$&+;;;;;.....X&&&&:..\n");
    printf("                                    ..X&&&&&.....;;;;;;&&;;;;;;....:&&&&&+..\n");
    printf("                                    ..&&&&&&&...:::::::&&;::::::..;&&&&&&$..\n");
    printf("                                    .;&&&&&&&&&x:::::::&&;::::::$&&&&&&&&&:.\n");
    printf("                                    .x&&&&&&&&&+:::::::&&;::::::x&&&&&&&&&;.\n");
    printf("                                    .;&&&&&&&&&::::::::&&+:::::::&&&&&&&&&..\n");
    printf("                                    ..X&&&&&&&$::::::::&&+:::::::&&&&&&&&+..\n");
    printf("                                    ...&&&&&&&+::::::::&&x:::::::x&&&&&&x...\n");
    printf("                                    ....$&&&&&:::::::::&&x::::::::&&&&&;....\n");
    printf("                                    .....;&&&&:::::::::&&X::::::::&&&$......\n");
    printf("                                    .......+&&:::::::::&&+::::::::&&:.......\n");
    printf("                                    .........:::::::::::::::::::::..........\n");
    printf("                                    ..........::::::::::::::::::::..........\n");
    printf("                                    ..........::::::::::::::::::::..........\n");
    printf("                                    ..........::::::::::::::::::::..........\n");
    printf("                                    ..........::::::$&&&&&&$;:::::..........\n");
    printf("                                    ..........::::$&$:.....$&$::::..........\n");
    printf("                                    ..........:::X&x........+&$:::..........\n");
    printf("                                    ..........:::&&:........:&&:::..........\n");
    printf("                                    ..........:::$&+........;&$:::..........\n");
    printf("                                    ..........::::&&x......+&&::::..........\n");
    printf("                                    ...........::::x&&&&&&&&x:::::..........\n");
    printf("                                    ...........::::::::;;::::::::...........\n");
    printf("                                    ...........::::::::::::::::::...........\n");
    printf("                                    ............::::::::::::::::............\n");
    printf("                                    .............:;x$&&&&&&$x;::............\n");
    printf("                                    .............&&&&&&&&&&&&&&+............\n");
    printf("                                    .............:&&&&&&&&&&&&&.............\n");
    printf("                                    ..............+&&&&&&&&&&&..............\n");
    printf("                                    ...............x&&&&&&&&&+..............\n");
    printf("                                    ................x&&&&&&&x...............\n");
    printf("                                    .................x&&&&&X................\n");
    printf("                                    ..................+&&&x.................\n");
    printf("                                    ....................&+..................\n");
    printf("                                    ........................................\n");
}


// Function that displays backstory
//-----------------------------------------------------------------------------------------
void displayBackstory() {
    printf("\n");
    printf("\n                                            ESCAPE POD INTERIOR\n");
    printf("                                 YOU WAKE UP ALONE, STRAPPED INTO THE ESCAPE POD...\n");
    printf("                             SYSTEMS ARE FAILING. OXYGEN LOW. NAVIGATION OFFLINE...\n");
    printf("                             THE STATUS CONSOLE FLICKERS — ONE CHANCE LEFT TO SURVIVE...\n");
    printf("                          DECIDE QUICKLY: REROUTE POWER, NAVIGATE, OR SIGNAL FOR HELP...\n");
	printf("\n                    -----------------------------------------------------------------------------------------\n");
}


// Function that handles the hex decription code form hex to ASCII text PUZZEL 1
//-----------------------------------------------------------------------------------------
int solveHexPuzzle_1(const char* easy, const char* hard) {
    char input[20];
    int attempts = 0;
	printf("\n                            YOU MUST REROUTE POWER TO BRING SYSTEMS BACK ONLINE...\n");
    printf("\n                            --- DECRYPT THE PASSWORD, SO POWER CAN BE REROUTED ---\n");
	printf("\n             --- 2 ATTEMPTS ONLY, YOU WILL BE LOCKED OUT OF THE SYSTEMS IF DECRYPTION IS INCORRECT ---\n");
	printf("                     -----------------------------------------------------------------------------------------\n");
    while (attempts < 2) {
        if (difficulty == 1) {
            printf("                    Convert the HEX: 0x4B 0x53 0x55 -> ASCII Text:\n"); //KSU
        } else {
            printf("                    Convert the HEX: 0x45 0x4E 0x47 0x49 0x4E 0x45 0x45 0x52 ->> ASCII Text:\n "); //ENGINEER
        }
        scanf("%s", input);
        if ((difficulty == 1 && strcmp(input, easy) == 0) ||
            (difficulty == 2 && strcmp(input, hard) == 0)) {
            return 1;  // correct!
        } else {
            attempts++;
            if (attempts < 2) {
                printf("                Decryption failed. Try again (%d attempt left).\n", 2 - attempts);
            } else {
                printf("                Decryption failed twice. Mission failed.\n");
                return 0;
            }
        }
    }
    return 0;
}

// Function that handles the hex decryption code for Puzzle 2
//-----------------------------------------------------------------------------------------
int solveHexPuzzle_2(const char* easy, const char* hard) {
    char input[20];                                      // Buffer to store user input
    int attempts = 0;                                    // Counter for attempts
    int max_attempts = (difficulty == 1) ? 2 : 1;        // Set max attempts: 2 if Easy, 1 if Hard

                                                        // Intro text for the puzzle
    printf("\n                            FINAL UPGRADE SYSTEM AUTHENTICATION REQUIRED...\n");
    printf("\n                            --- DECRYPT THE FINAL PASSWORD TO SECURE UPGRADE ---\n");
    printf("\n             --- %d ATTEMPT%s ONLY --- SYSTEM WILL LOCK IF DECRYPTION IS INCORRECT ---\n",
           max_attempts, (max_attempts == 1) ? "" : "S");
    printf("                     -----------------------------------------------------------------------------------------\n");

    // Loop for user attempts
    while (attempts < max_attempts) {
        // Show different hex codes based on difficulty
        if (difficulty == 1) {
            printf("                    Convert the HEX: 0x53 0x41 0x46 0x45 -> ASCII Text:\n"); // SAFE
        } else {
            printf("                    Convert the HEX: 0x52 0x45 0x53 0x43 0x55 0x45 -> ASCII Text:\n"); // RESCUE
        }

        scanf("%s", input);                            // Read user input (no spaces)

        // Check if input matches expected word for difficulty
        if ((difficulty == 1 && strcmp(input, easy) == 0) ||
            (difficulty == 2 && strcmp(input, hard) == 0)) {
            return 1;                                 // Return success if correct
        } else {
            attempts++;                               // Increment attempt count
            if (attempts < max_attempts) {
                printf("                Decryption failed. Try again (%d attempt%s left).\n",
                       max_attempts - attempts,
                       (max_attempts - attempts == 1) ? "" : "s");
            } else {
                printf("                Decryption failed. System locked.\n");
                return 0;                             // Return failure after max attempts
            }
        }
    }
    return 0;                                         // Redundant safety return
}


// Function that ask the user how they would like to reroute power to the system
//-----------------------------------------------------------------------------------------
void reroutePower() {
    int choice;
    printf("\n               Reroute Power:                              \n");
	printf("                [1] Shields - Alien fire can be absorbed     \n");
    printf("                [2] Navigation - Asteroid auto-dodge engaged \n");
    choice = getNumericInput(1, 2);
    if (choice == 1) {
        shields = 1;
    printf("                Shields online: \n");
    } else {
        navigation = 1;
    printf("                Navigation online: \n");
    }
    printf("\n                Type 'RUN' or press ENTER to begin descent:\n");
}

// Function that ask the user to begin descent and launch ship by pressing enter or typing RUN
//-----------------------------------------------------------------------------------------
void beginDescent() {
    char run[10];// Buffer to store user input
    while (1) {  // Infinite loop until valid command is entered
        if (!fgets(run, sizeof(run), stdin)) {  // When the player presses enter, fgets() stores the newline \n created and stores as an accepted input
            printf("                Input error. Try again.\n");// Input error (e.g., EOF or read failure)
            continue;
        }

        // Remove newline if present
        run[strcspn(run, "\n")] = 0;

        // If just ENTER or case-insensitive "RUN"
        if (strlen(run) == 0 || strcasecmp(run, "RUN") == 0) {
            printf("                Initiating descent sequence...\n");
            break;
        }

        printf("                Unrecognized command. Please type 'RUN' or press ENTER.\n");
    }
}

// Function that displays ASCII Art descent through space
//---------------------------------------------------------------------------------------------------
void showSpaceASCIArt(){
printf("	        ~+\n");
printf("\n");
printf("	       *       +                  ___---___                    \n");
printf("				      .--         --.      \n");
printf("				    ./   ()      .-. \\.\n");
printf("				   /   o    .   (   )  \\\n");
printf("				  / .            '-'    \\         \n");
printf("				 | ()    .  O         .  |      \n");
printf("				|                         |      \n");
printf("				|    o           ()       |\n");
printf("				|       .--.          O   |            \n");
printf("				 | .   |    |            |\n");
printf("				  \\    `.__.'    o   .  /    \n");
printf("				   \\                   /                   \n");
printf("				    `\\  o    ()      /' JT/jgs         \n");
printf("				      `--___   ___--'\n");
printf("				            ---\n");
printf("	           '                  |\n");
printf("	       ()    .-.,='""'=.    - o -\n");
printf("	             '=/_       \\     |\n");
printf("	          *   |  '=._    |\n");
printf("	               \\     `=./`,        '\n");
printf("	            .   '=.__.=' `='      *\n");
printf("	   +                         +\n");
printf("	        O      *        '       .\n");
printf("	jgs.*'                            *.                    \n");
}

// Function that simulates the ship being back online and pinging signal from nearby planet
// Also handles the location selected by the user.
//-----------------------------------------------------------------------------------------
void chooseDestination() {
    int choice;
    printf("\n              YOU HAVE SUCCESSFULLY REROUTED POWER AND LAUNCHED SHIP COMMUNICATION SYSTEM IS BOOTING \n");
	printf("\n");
	printf("\n  			Pinging 192.168.169.1 with 32 bytes of data:");
	printf("\n  			Reply from 192.168.169.1: bytes=32 time<1ms TTL=128");
	printf("\n  			Reply from 192.168.169.1: bytes=32 time<1ms TTL=128");
	printf("\n  		         Reply from 192.168.169.1: bytes=32 time<1ms TTL=128");
	printf("\n  			Reply from 192.168.169.1: bytes=32 time<1ms TTL=128");
    printf("\n");
	printf("\n  			Ping statistics for 192.168.169.1:");
	printf("\n  		    Packets: Sent = 4, Received = 4, Lost = 0 (0 loss),");
	printf("\n  		    Approximate round trip times in milli-seconds:");
	printf("\n  		    Minimum = 0ms, Maximum = 0ms, Average = 0ms");
	printf("\n");
	printf("\n              COMMUNICATION SUCCESSFUL, YOU HAVE PINGED 2 LOCATIONS MAKE A DECESION  \n");
	printf("\n");
	printf("\n                  Choose Destination: [1] SAKAAR   [2] XANDAR \n");
	printf("\n");
    choice = getNumericInput(1, 2);
    if (choice == 1) strcpy(character, "SAKAAR");
    else strcpy(character, "XANDAR");
}


// Function that handles the decryption code to enter the planet chosen
// You are given 2 attempts to decrypt the code, else you are identfied and captured
//-----------------------------------------------------------------------------------------
void planetPuzzle(const char* planet) {
    char input[20];  // Buffer for player’s code input
    char confirm[10];// Buffer for upgrade confirmation input
    char planetUpper[20]; // Uppercase planet name for matching
    strcpy(planetUpper, planet);
    planetUpper;  // keep planet name uppercase for matching and display

    printf("\n                  --- %s PLANET ENTRY PERMISSIVE ---\n", planet);
    printf("                    TO ENTER %s, YOU MUST DECRYPT THE CODE.\n", planetUpper);
    printf("                    CONVERT THE HEX CODE TO ASCII TEXT.\n");

    if (strcmp(planetUpper, "XANDAR") == 0) {
        printf("                    HEX TO ASCII: 0x58 0x41 0x4E 0x44 0x41 0x52\n");  // XANDAR
    } else if (strcmp(planetUpper, "SAKAAR") == 0) {
        printf("                    HEX TO ASCII: 0x53 0x41 0x4B 0x41 0x41 0x52\n");  // SAKAAR
    } else {
        printf("                    UNKNOWN PLANET.\n"); // Exit if invalid planet
        return;
    }

    int attempts = 0;// Count of attempts
    int success = 0; // Flag for puzzle success


    // Allow up to 2 attempts to input correct decrypted code
    while (attempts < 2) {
        printf("                      ATTEMPT %d OF 2: ", attempts + 1);
        scanf("%s", input);


        if (strcmp(input, planetUpper) == 0) {
            success = 1; // Correct answer
            break; // Exit loop
        } else {
            printf("DECRYPTION FAILED.\n");
        }
        attempts++;
    }

    if (success) {
        printf("SUCCESSFUL! ENTERING %s...\n", planetUpper);
        showPlanetEntryASCIIArt(); // Show entry art/animation

        // Check for XANDAR upgrade option
        if (strcmp(planetUpper, "XANDAR") == 0) {
            printf("       WOULD YOU LIKE TO UPGRADE THE NAVIGATION SYSTEM?  (THIS HELPS YOU MANUALLY NAVIGATE BACK HOME) (yes/no):\n");
            scanf("%s", confirm);
            if (strncmp(confirm, "yes", 3) == 0 || strncmp(confirm, "YES", 3) == 0) {
                navUpgrade = 1; // Enable navigation upgrade
                printf("                      SUCCESS! MANUAL NAVIGATION SYSTEM UPGRADED.\n");
            } else {
                printf("                      NAVIGATION UPGRADE DECLINED.\n");
            }
        }

        // Check for SAKAAR upgrade option
        if (strcmp(planetUpper, "SAKAAR") == 0) {
            printf("       WOULD YOU LIKE TO UPGRADE THE COOLANT SYSTEM?  (THIS ENABLES AUTOPILOT FOR SAFE RETURN) (yes/no):\n");
            scanf("%s", confirm);
            if (strncmp(confirm, "yes", 3) == 0 || strncmp(confirm, "YES", 3) == 0) {
                coolant = 1; // Enable coolant upgrade
                printf("                      SUCCESS! AUTOPILOT COOLANT SYSTEM UPGRADED.\n");
            } else {
                printf("                      COOLANT UPGRADE DECLINED.\n");
            }
        }

    } else {
        printf("                      MISSION FAILED. YOU COULD NOT DECRYPT THE ACCESS CODE TO %s.\n", planetUpper);
        alive = 0;  // Player loses
    }
}

// Function that displays Entered planet ASCII Art
//---------------------------------------------------------------------------------------------------------------------------
void showPlanetEntryASCIIArt(){
printf("                .                                            .\n");
printf("     *   .                  .              .        .   *          .\n");
printf("  .         .                     .       .           .      .        .\n");
printf("        o                             .                   .\n");
printf("         .              .                  .           .\n");
printf("          0     .\n");
printf("                 .          .                 ,                ,    ,\n");
printf(" .          \\          .                         .\n");
printf("      .      \\   ,\n");
printf("   .          o     .                 .                   .            .\n");
printf("     .         \\                 ,             .                .\n");
printf("               #\\##\\#      .                              .        .\n");
printf("             #  #O##\\###                .                        .\n");
printf("   .        #*#  #\\##\\###                       .                     ,\n");
printf("        .   ##*#  #\\##\\##               .                     .\n");
printf("      .      ##*#  #o##\\#         .                             ,       .\n");
printf("          .     *#  #\\#     .                    .             .          ,\n");
printf("                      \\          .                         .\n");
printf("____^/\\___^--____/\\____O______________/\\/\\---/\\___________---______________\n");
printf("   /\\^   ^  ^    ^                  ^^ ^  '\\ ^          ^       ---\n");
printf("         --           -            --  -      -         ---  __       ^\n");
printf("   --  __                      ___--  ^  ^                         --  __\n");
printf("																																												\n");
}

// This function handles the final descent back home
// the user has make a critical thinking decisions to make it back home.
//-----------------------------------------------------------------------------------------
void finalDescent() {
    int choice;
    printf("\n                         Final Descent: [1] Autopilot [2] Manual\n");
    choice = getNumericInput(1, 2);
    if ((choice == 1 && coolant) || (choice == 2 && navUpgrade)) alive = 1;
    else alive = 0;
}


// Function to handle the ASCII Art Victory Screen
//-----------------------------------------------------------------------------------------
void victoryScreen() {
    printf("\n");
    printf("\n");
    printf("               00   00    000000    00    00         00    000   0000000   00    00    000   \n");
    printf("               00   00  000    00   00    00         00  0 000     00      000   00   00000  \n");
    printf("                00000   000    00   00    00         000000000     00      00000000    000   \n");
    printf("                 00     000    00   00    00         0000 0000     00      00  0000          \n");
    printf("                 00       000000     000000          00    000   0000000   00    00    00    \n");
    printf("                                                                                             \n");
    printf("                                                                                             \n");
    printf("                                                                                             \n");
    printf("                                        00000000000000000000000000                           \n");
    printf("                                       00000                 00000                           \n");
    printf("                                 00     0000   0000000000000000000    000                    \n");
    printf("                              0        0000   0000000000000000000      000                   \n");
    printf("                               0        00000000000000000000000000       00                  \n");
    printf("                               00       00000000000000000000000000       00                  \n");
    printf("                                000     00000000000000000000000000    0000                   \n");
    printf("                                       000000000000000000000000000                           \n");
    printf("                                        0000000000000000000000000                            \n");
    printf("                                           0000000000000000000                               \n");
    printf("                                           0000000000000000000                               \n");
    printf("                                              000000000000                                   \n");
    printf("                                                  00000                                      \n");
    printf("                                                   000                                       \n");
    printf("                                                   000                                       \n");
    printf("                                                  00000                                      \n");
    printf("                                               00000000000                                   \n");
    printf("                                              0000000000000                                  \n");
	printf("\n                       *** VICTORY! YOU SURVIVED THE DESCENT WELCOME HOME. ***             \n");
	}


// Function to handle the ASCII ART Game Over Screen
//-----------------------------------------------------------------------------------------
void gameOverScreen() {
    printf("\n");
    printf("\n");
    printf("                    00000000000      0000000000    000000        000000 0000000000000000     \n");
    printf("                  000000000000000 0000000000000000 00000000    00000000 0000000000000000     \n");
    printf("                  000000   000000 0000000   000000 00000000000000000000 0000000000           \n");
    printf("                  000000          0000000   000000 00000000000000000000 00000000000          \n");
    printf("                  000000 00000000 0000000000000000 000000 000000 000000 0000000              \n");
    printf("                  000000  0000000 0000000000000000 000000  00000 000000 0000000              \n");
    printf("                  000000000000000 0000000   000000 000000        000000 000000000000000      \n");
    printf("                    00000000000   0000000   000000 000000        000000 0000000000000000     \n");
    printf("                                                                                             \n");
    printf("                    000000000000    000000    000000 00000000000000000 00000000000000        \n");
    printf("                  0000000000000000  000000    000000 00000000000000000 00000000000000000     \n");
    printf("                  000000    000000  000000    000000 00000000000       0000000    000000     \n");
    printf("                  000000    000000  000000    000000 000000000000      0000000    000000     \n");
    printf("                  000000    000000  000000    000000 0000000           000000000000000       \n");
    printf("                  000000    000000  0000000000000000 0000000           000000000000000       \n");
    printf("                  0000000   000000    000000000000   0000000           0000000 0000000       \n");
    printf("                    000000000000        0000000      00000000000000000 0000000   0000000     \n");
    printf("                     00000000000          00000       0000000000000000  00000     000000     \n");
    printf("\n                                   *** YOU LOSE - SHIP CRASHED  ***                        \n");
}



//  Main function
//-----------------------------------------------------------------------------------------
int main(void) {


initialise_monitor_handles();  // Semi-hosting initializes

    showTitleArt();  // Display main menu ASCII art

    int choice;
    printf("\n[1] Start Game  [2] Exit\n");
    choice = getNumericInput(1, 2);

    if (choice != 1) {
        printf("Exiting game. Goodbye!\n"); // [2]Exit selected by user
        return 0;
    }

    selectCharacter();       // User picks character and shows ASCII Art Characters
    selectDifficulty();      // User picks difficulty
    showPodSceneASCIIArt();  // Display ASCI art of space ship
    displayBackstory();      // Display story line

    if (!solveHexPuzzle_1("KSU", "ENGINEER")) {  // Call Hex Puzzle Function. Easy = KSU, Hard = ENGINEER
        gameOverScreen();                      // Player loses after 2 failed attempts - Call Game Over ASCII Art Function
        return 0;
    }

    reroutePower();           // User reroutes power call function
    beginDescent();           // Begin Descent call function
    showShipASCIIArt();       // Display ASCI art of space ship
    chooseDestination();      // Prompt user to choose destination call function
    showSpaceASCIArt();       // Display ASCI art of space ship
    planetPuzzle(character);  // Handles planet decryption + upgrade offer
    if (!alive) {
        gameOverScreen();
        return 0;
    }
    if (solveHexPuzzle_2("SAFE", "RESCUE")) {
        printf("Upgrade secured!\n");
        // apply upgrade flag here
    } else {
        printf("Upgrade attempt failed.\n");
        gameOverScreen(); // Player loses after 2 failed attempts - Call Game Over ASCII Art Function
        return 0;
    }

    finalDescent();           // Autopilot/manual decision
    if (alive)
        victoryScreen();      // Player Wins Game - Call Victory ASCII ART
    else
        gameOverScreen();     // Player Loses Game - Call Game Over ASCII ART
    while (1);                // Keep Semi-hosting console alive on STM32
    return 0;
}




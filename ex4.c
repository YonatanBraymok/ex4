/******************
Name:
ID: 
Assignment: ex4
*******************/
#include <stdio.h>
#include <string.h>

#define PYRAMID_HEIGHT 5
#define PYRAMID_WIDTH 5
#define MAX_BOARD 20
#define ASCII 126
#define MAX_GRID 30
#define MAX_SLOTS 100
#define MAX_WORD 15
#define NEW_LINE_MARGIN 1 //when scanning the word we are applying the chars to the array including '\n'

//typedefs for task number 5.
typedef struct{ //define a type for slot
    int row;
    int column;
    int length;
    char direction; // 'V' - vertical or 'H' - horizontal.
}Slot;
typedef struct{ //define a type for word
    char word[MAX_WORD + NEW_LINE_MARGIN];
    int length;
    int index;
    int placed; //this will determine whether a word has been placed(1) in the crossword or not (default - 0)
}Word;

//TASK 1
int task1RobotPaths(int column, int row);
//TASK 2
float calculateContribution(float weights[PYRAMID_HEIGHT][PYRAMID_WIDTH],
    int pyramidHeight, int pyramidWidth, int column, int row);
float roundFloat(float number);
void task2HumanPyramid(float weights[PYRAMID_HEIGHT][PYRAMID_WIDTH], int pyramidHeight, int pyramidWidth);
//TASK 3
int task3ParenthesisValidator(char end);
//TASK 4
void task4QueensBattle();
void markQueen(int y, int x, int secIdx[ASCII], int qRows[MAX_BOARD], int qCol[MAX_BOARD],
    char sections[MAX_BOARD][MAX_BOARD], char solution[MAX_BOARD][MAX_BOARD]);
void unmarkQueen(int y, int x, int secIdx[ASCII], int qRows[MAX_BOARD], int qCol[MAX_BOARD],
    char sections[MAX_BOARD][MAX_BOARD], char solution[MAX_BOARD][MAX_BOARD]);
int check(int y, int x, int qRows[MAX_BOARD], int qCol[MAX_BOARD], int secIdx[ASCII],
    char sections[MAX_BOARD][MAX_BOARD], char solution[MAX_BOARD][MAX_BOARD]);
int isNearby(int y, int x, char solution[MAX_BOARD][MAX_BOARD]);
int solveQueenPuzzle(int y, int x, int secIdx[ASCII], int qRows[MAX_BOARD], int qCol[MAX_BOARD],
    char sections[MAX_BOARD][MAX_BOARD], char solution[MAX_BOARD][MAX_BOARD], int N);
//TASK 5
void task5CrosswordGenerator();
void initializeGrid(int n, char grid[MAX_GRID][MAX_GRID]);
int canPlaceWord(Slot slot, Word word, char grid[MAX_GRID][MAX_GRID]);
void placeWord(Slot slot, Word word, char grid[MAX_GRID][MAX_GRID]);
void removeWord(Slot slot, char grid[MAX_GRID][MAX_GRID]);
int solveCrossword(Slot slots[MAX_SLOTS], Word words[MAX_GRID * MAX_GRID], int slotCount, int wordCount,
    int currentSlot, int n, char grid[MAX_GRID][MAX_GRID]);
void printGrid(int n, char grid[MAX_GRID][MAX_GRID]);
int solveCrosswordRecursive(Slot slots[], Word words[], int slotCount, int wordCount, int currentSlot, int currentWord,
    int n, char grid[MAX_GRID][MAX_GRID]);


int main()
{
    int task = -1;
    do
    {
        printf("Choose an option:\n"
               "1. Robot Paths\n"
               "2. The Human Pyramid\n"
               "3. Parenthesis Validation\n"
               "4. Queens Battle\n"
               "5. Crossword Generator\n"
               "6. Exit\n");


        if (scanf("%d", &task))
        {
            //clears buffer.
            //scanf("%*[^\n]");
            //scanf("%*c");

            switch (task)
            {
            case 6:
                printf("Goodbye!\n");
                break;
            case 1: {
                /* In this program we first define 2 integers that will serve as the robot's coordinates.
                 * We ask the user to input the desired coords, and then we call the function while printing. */
                int column, row;
                printf("Please enter the coordinates of the robot (column, row):\n");
                scanf("%d %d", &column, &row);
                printf("The total number of paths the robot can take to reach home is: %d\n",
                task1RobotPaths(column, row));
                break;
            }
            case 2: {
                /* In this program we receive from the user 15 weight inputs of all the cheerleaders.
                 * we check that each value is valid (not negative).
                 * after that we call the task2 function that calculates and prints each cheerleader's contribution
                 * using recursion.*/
                float weights[PYRAMID_HEIGHT][PYRAMID_WIDTH] = {0};
                printf("Please enter the weights of the cheerleaders: \n");
                float validation;
                for (int i = 0; i < PYRAMID_HEIGHT; i++) {
                    for (int j = 0; j <= i; j++) {
                        scanf("%f", &validation);
                        if (validation < 0)
                            break;
                        weights[i][j] = validation;
                    }
                    if (validation < 0)
                        break;
                }
                if(validation < 0) {
                    printf("Negative weights are not supported.\n");
                    break;
                }
                task2HumanPyramid(weights, PYRAMID_HEIGHT, PYRAMID_WIDTH);
                break;
            }
            case 3: {
                /*in this program we ask the user to enter a term for validation.
                 *next thing we do is check if the term entered is balanced correctly, we call of task3 function,
                 *and we're looking for an ending ('\n' - since no parentheses had been opened yet this is our default.
                 *if the function returns true(1) then the term is truly balanced, else, it's not. */
                scanf("%*c");
                printf("Please enter a term for validation:\n");
                if (task3ParenthesisValidator('\n')) {
                    printf("The parentheses are balanced correctly.\n");
                } else
                    printf("The parentheses are not balanced correctly.\n");
                break;
            }
            case 4: {
                //no comments here, entire code written in functions.
                task4QueensBattle();
                //clears buffer
                scanf("%*[^\n]");
                scanf("%*c");
                break;
            }
            case 5:
                task5CrosswordGenerator();
                break;
            default:
                printf("Please choose a task number from the list.\n");
                break;
            }
        }
        else
        {
            scanf("%*s");
        }

    } while (task != 6);
}

/* Task number 1:
 * the next function receives 2 integers (column, row) that represents the coordinates of the robot on the grid.
 * when calling the function, if the robot is in a valid area and is not home, the function calls for itself 2 more
 * times: once after the robot moved left, and once after he moved down.
 * if he left the grid (reached another section of the graph), the function returns 0.
 * for each route the robot takes that eventually reaches home (0,0), the function returns 1. */
int task1RobotPaths(int column, int row)
{
    if (column < 0 || row < 0) //the robot is not in the grid.
        return 0;
    if (column == 0 && row == 0) //the robot has reached home.
        return 1;
    return task1RobotPaths(column - 1, row) + task1RobotPaths(column, row - 1); //the robot is in the grid.
}

/* Task number 2:
 * in this function we calculate the contribution of each cheerleader using the formula:
 * weight + 1/2 * leftShoulderContribution + 1/2 * rightShoulderContribution.
 * the recursion calls the function again for the cheerleaders in each shoulder int order
 * to get their contribution - it goes on and on until the function reach the top cheerleader and stops there.
 * if the recursion called the function for a cheerleader "outside" the pyramid, it stops.
   in the end the function returns the final calculation for the specific cheerleader. */
float calculateContribution(float weights[PYRAMID_HEIGHT][PYRAMID_WIDTH],
    int pyramidHeight, int pyramidWidth, int column, int row)
{
    if(column < 0 || row < 0) //stops if we "left" the pyramid.
        return 0;
    if(row == 0) //row 0 is the top row therefor there is only 1 cheerleader.
        return weights[row][column];
    float leftShoulder = calculateContribution(weights, pyramidHeight, pyramidWidth, column - 1, row - 1);
    float rightShoulder = calculateContribution(weights, pyramidHeight, pyramidWidth, column, row - 1);
    return weights[row][column] + 0.5 * leftShoulder + 0.5 * rightShoulder;

}

//this function rounds a float number to 2 numbers after decimal.
float roundFloat(float number)
{
    return (int)(number * 100 + 0.5) / 100.0;
}

/* in this function we call the recursive function 15 times (for each cheerleader)
 * and print its contribution to the pyramid in according to its position in the pyramid. */
void task2HumanPyramid(float weights[PYRAMID_HEIGHT][PYRAMID_WIDTH], int pyramidHeight, int pyramidWidth)
{
    printf("The total weight on each cheerleader is:\n");
    for (int i = 0; i < pyramidHeight; i++) {
        for (int j = 0; j <= i; j++) {
            printf("%.2f ", //prints 2 numbers after decimal.
                roundFloat(calculateContribution(weights, pyramidHeight, pyramidWidth, j, i)));
        }
        printf("\n"); //print new line in order to separate each level of the pyramid.
    }
}

/* the next function receives a char that indicates what should come next
 * meaning, the value "end" is what the function is looking for. (e.g: if end = ')' we're looking for ')').
 * note: what the function is looking for might not come right after the call, it may be some char ahead.
 * if the function found something that contracts what it's looking for: e.g, the function is looking for ')' but
 * instead it found another type of closing parenthesis like ']', it means the parentheses are not balanced correctly.
 * if the function found the end of the string '\n' when looking for it, it means all parentheses had been closed
 * (if there were some that opened) and we reached the end of the string successfully with balanced parentheses. */
int task3ParenthesisValidator(char end)
{
    char ch;
    if (scanf("%c", &ch) != 1) {
        if (ch != '\n') {
            scanf("%*[^\n]");
            scanf("%*c");
        }
        //if we called the function, and we did not scan anything.
        return 0;
    }
    if (ch == end) //the current scanned char is what we're looking for
        return 1;
    if (ch == ')' || ch == '}' || ch == ']' || ch == '>' || ch == '\n') {
        if (ch != '\n') {
            scanf("%*[^\n]");
            scanf("%*c");
        }
        //contracts what we're looking for/
        return 0;
    }
    /* the next if statements regard the recursion: if a parenthesis had been opened the function stop looking
     * for what it's looking for currently and calls the function again to find the end of this parenthesis.
     * if the return value of the function was true(1) the function continues looking for its previous destination,
     * otherwise, it means that this parenthesis that was open is not closed and that the string is not balanced, and
     * returns 0. */
    if (ch == '(')
        if (!task3ParenthesisValidator(')'))
            return 0;
    if (ch == '{')
        if (!task3ParenthesisValidator('}'))
            return 0;
    if (ch == '[')
        if (!task3ParenthesisValidator(']'))
            return 0;
    if (ch == '<')
        if (!task3ParenthesisValidator('>'))
            return 0;
    //if the current scanned char is nothing of the above then it does not effects our function, and we move on.
    return task3ParenthesisValidator(end);
}

char toUpperCase(char c)
{
    if (c >= 'a' && c <= 'z') {
        return c - ('a' - 'A'); // Convert to uppercase by adjusting ASCII value
    }
    return c; // Return the character as-is if it's not lowercase
}

void task4QueensBattle()
{
    int secIdx[ASCII] = {0}; //we need to consider every char type possible.
    int qRows[MAX_BOARD] = {0}; //this array will keep track of occupied rows.
    int qCol[MAX_BOARD] = {0}; //this array will keep track of occupied columns.
    char sections[MAX_BOARD][MAX_BOARD]; //user's board input
    char solution[MAX_BOARD][MAX_BOARD]; //solution.
    int N; //dimension

    printf("Please enter the board dimensions: \n");
    scanf("%d", &N);
    printf("Please enter a %d*%d puzzle board:\n", N, N);
    //processing user's input into the sections array.
    for(int i = 0; i < N; i++) {
        for(int j = 0; j < N; j++) {
            scanf(" %c", &sections[i][j]);
        }
    }

    //calls recursive function and prints the board if there is a solution.
    if (solveQueenPuzzle(0, 0, secIdx, qRows, qCol, sections, solution, N)) {
        printf("Solution:\n");
        for(int i = 0; i < N; i++) {
            for(int j = 0; j < N; j++) {
                if(solution[i][j] != 'X') {
                    printf("* ");
                    continue;
                }
                printf("%c ", solution[i][j]);
            }
            printf("\n");
        }
    }
    else {
        printf("This puzzle cannot be solved.\n");
    }


}

/*this function will check whether a queens placement is valid. meaning there are no queens in the same
 * row or column, no queens in the same region, and no queens nearby(3x3 block). */
int check(int y, int x, int qRows[MAX_BOARD], int qCol[MAX_BOARD], int secIdx[ASCII],
    char sections[MAX_BOARD][MAX_BOARD], char solution[MAX_BOARD][MAX_BOARD])
{
    char currSection = sections[y][x];
    //all the statements below should return 0 if the check is valid this is why we put NOT(!).
    return (!qRows[y] && !qCol[x] && !secIdx[(int)currSection] && isNearby(y, x, solution));
}

/*this function checks whether there is a queen nearby our queen, since we are already checking the rows and columns
 * here we will be checking the diagonal area close by. if there is no queen there then we return 1, otherwise
 * return 0.
 * in each if case we are optimizing the checks according to the current place on the grid, for example, if we are
 * currently in column number 0, then we have 2 diagonals out of the grid that should not be checked (there is not queen
 * there for sure, checking for one might resolve an error).*/
int isNearby(int y, int x, char solution[MAX_BOARD][MAX_BOARD])
{
    if (y == 0 && x == 0)
        return !(solution[y][x] == 'X');
    if (x == 0)
        return !(solution[y+1][x-1] == 'X' || solution[y+1][x+1] == 'X');
    if (y == 0)
        return !(solution[y-1][x+1] == 'X' || solution[y+1][x+1] == 'X');
    return !(solution[y-1][x+1] == 'X' ||
        solution[y+1][x+1] == 'X' ||
        solution[y-1][x-1] == 'X' ||
        solution[y+1][x-1] == 'X');
}


/*this function is used in order to mark a queen. that means the row and column should be occupied(1), and the section
 * should be occupied */
void markQueen(int y, int x, int secIdx[ASCII], int qRows[MAX_BOARD], int qCol[MAX_BOARD],
    char sections[MAX_BOARD][MAX_BOARD], char solution[MAX_BOARD][MAX_BOARD])
{
    qCol[x] = 1; //mark that the column is now occupied
    qRows[y] = 1; //mark that the row is now occupied
    char currSection = sections[y][x];
    secIdx[(int)currSection] = 1; //mark that the char that represents that section is now occupied
    solution[y][x] = 'X'; //place the queen in the solution
}

//this function does the opposite effect of the previous function, which means unmarking a marked queen.
void unmarkQueen(int y, int x, int secIdx[ASCII], int qRows[MAX_BOARD], int qCol[MAX_BOARD],
    char sections[MAX_BOARD][MAX_BOARD], char solution[MAX_BOARD][MAX_BOARD])
{
    qCol[x] = 0;
    qRows[y] = 0;
    char currSection = sections[y][x];
    secIdx[(int)currSection] = 0;
    solution[y][x] = '*';
}

/* Recursion: this function tries to solve the given puzzle by the user.
 * it checks whether an unoccupied slot is valid to put a queen in, if so, it places one and goes on to the next rows
 * in order to complete the puzzle. if the recursion returns 0 somewhere before the end of the puzzle, we unmark the
 * latest queen and try to place her in another slot - and keep checking if there is a solution to the puzzle.
 */
int solveQueenPuzzle(int y, int x, int secIdx[ASCII], int qRows[MAX_BOARD], int qCol[MAX_BOARD],
    char sections[MAX_BOARD][MAX_BOARD], char solution[MAX_BOARD][MAX_BOARD], int N)
{
    if(N < 3 && N > 1) //if the dimensions of the board are under 3x3 it means 2 queens will surely touch eachother.
        return 0;
    /*next if statement checks if we passed the board size by rows - meaning we placed a queen in each row accordingly
     * and completed the puzzle, if so, we return 1 because there is a solution.*/
    if (y == N && qRows[y-1])
        return 1;
    /*if we passed the board size by columns it means we could not find a spot to place a queen in, meaning there is no
     * solution for this puzzle.*/
    if (x == N)
        return 0;

    if(check(y, x, qRows, qCol, secIdx, sections, solution)) {
        markQueen(y, x, secIdx, qRows, qCol, sections, solution);
        if (!(solveQueenPuzzle(y + 1, 0, secIdx, qRows, qCol, sections, solution, N))) {
            unmarkQueen(y, x, secIdx, qRows, qCol, sections, solution);
            return solveQueenPuzzle(y, x + 1, secIdx, qRows, qCol, sections, solution, N);
        }
    }
    else
        return solveQueenPuzzle(y, x + 1, secIdx, qRows, qCol, sections, solution, N);
    return 1;
}


void task5CrosswordGenerator()
{
    char grid[MAX_GRID][MAX_GRID];
    int n; //dimensions of the grid.
    int slots; //number of slots the crossword should contain.
    int wordsNum = 0; //number of words in the dictionary.
    printf("Please enter the dimensions of the crossword grid: \n");
    scanf("%d", &n);
    printf("Please enter the number of slots in the crossword: \n");
    scanf("%d", &slots);
    Slot slotArr[MAX_SLOTS] = {0};
    printf("Please enter the details for each slot (Row, Column, Length, Direction):\n");
    //initiate the values to the slot array.
    for (int i = 0; i < slots; i++) {
        scanf(" %d %d %d %c", &slotArr[i].row, &slotArr[i].column, &slotArr[i].length, &slotArr[i].direction);
    }
    printf("Please enter the number of words in the dictionary:\n");
    while (wordsNum < slots) {
        scanf("%d", &wordsNum);
        if (wordsNum < slots)
            printf("The dictionary must contain at least %d words. Please enter a valid dictionary size:\n", slots);
    }
    Word words[MAX_GRID * MAX_GRID] = {0}; //words array
    printf("Please enter the words for the dictionary:\n");
    for (int i = 0; i < wordsNum; i++) {
        scanf(" %15s", words[i].word); // Ensure a space before %15s to skip leftover \n
        words[i].length = (int)strlen(words[i].word);
        words[i].index = i;
        words[i].placed = 0;
    }

    initializeGrid(n, grid); //"create" the grid

    //if the given data has a matching solvable crossword grid - print it. else - let the user know it is not solvable.
    if(solveCrosswordRecursive(slotArr, words, slots, wordsNum, 0, 0, n, grid))
        printGrid(n, grid);
    else
        printf("This crossword cannot be solved.\n");
}

//this function initialize the grid with empty cells ('#')
void initializeGrid(int n, char grid[MAX_GRID][MAX_GRID])
{
    for (int i = 0; i < n; i++) {
        for (int j = 0; j < n; j++) {
            grid[i][j] = '#';
        }
    }
}

//this function checks whether a word can be placed in a certain slot.
int canPlaceWord(Slot slot, Word word, char grid[MAX_GRID][MAX_GRID])
{
    if(word.length != slot.length)
        return 0; //slot's length does not match the word's length therefore it cannot be placed there.
    if(slot.direction == 'H') {
        for(int i = 0; i < slot.length; i++) {
            if(grid[slot.row][slot.column + i] != '#' && grid[slot.row][slot.column + i] != word.word[i])
                return 0;
        }
    }
    else if(slot.direction == 'V') {
        for(int i = 0; i < slot.length; i++) {
            if(grid[slot.row + i][slot.column] != '#' && grid[slot.row + i][slot.column] != word.word[i])
                return 0;
        }
    }
    return 1;
}

//this function places a word in the grid.
void placeWord(Slot slot, Word word, char grid[MAX_GRID][MAX_GRID])
{
    if(slot.direction == 'H') {
        for(int i = 0; i < slot.length; i++) {
            grid[slot.row][slot.column + i] = word.word[i];
        }
    }
    else if(slot.direction == 'V') {
        for(int i = 0; i < slot.length; i++) {
            grid[slot.row + i][slot.column] = word.word[i];
        }
    }
}

//this function removes a word from the grid.
void removeWord(Slot slot, char grid[MAX_GRID][MAX_GRID])
{
    if(slot.direction == 'H') {
        for(int i = 0; i < slot.length; i++) {
            grid[slot.row][slot.column + i] = '#';
        }
    }
    else if(slot.direction == 'V') {
        for(int i = 0; i < slot.length; i++) {
            grid[slot.row + i][slot.column] = '#';
        }
    }
}

/*int solveCrossword(Slot slots[MAX_SLOTS], Word words[MAX_GRID * MAX_GRID], int slotCount, int wordCount,
    int currentSlot, int n, char grid[MAX_GRID][MAX_GRID])
{
    if (currentSlot == slotCount) {
        return 1; // base case - if we filled all the slots correctly then the crossword is generated successfully.
    }

    Slot slot = slots[currentSlot];


    for (int i = 0; i < wordCount; i++) {
        if (!words[i].placed && canPlaceWord(slot, words[i], grid)) {
            words[i].placed = 1;
            placeWord(slot, words[i], grid);

            if (solveCrossword(slots, words, slotCount, wordCount, currentSlot + 1, n, grid)) {
                return 1; // Found a solution
            }

            // Backtrack
            removeWord(slot, grid);
            words[i].placed = 0;
        }
    }

    return 0; // No solution found
}*/

// the following function prints the grid that its receiving.
void printGrid(int n, char grid[MAX_GRID][MAX_GRID]) {
    for(int i = 0; i < n; i++) {
        printf("| ");
        for(int j = 0; j < n; j++) {
            printf("%c", grid[i][j]);
            if(j < n - 1)
                printf(" | ");
        }
        printf(" |");
        printf("\n");
    }
}

/*for each word in the dictionary we check if it fits the current slot and can be placed there, if so we place it
     *and move on to next slot until we filled all the slots. if we could not fill them all we backtrack and remove
     *the word we placed and try placing another word. that way we check all the possibilities of word placements.
     *if we did not reach any solution we return 0;*/
int solveCrosswordRecursive(Slot slots[], Word words[], int slotCount, int wordCount, int currentSlot, int currentWord,
    int n, char grid[MAX_GRID][MAX_GRID])
{
    if (currentSlot == slotCount) {
        return 1; // base case: all slots were filled and the crossword had been generated successfully
    }

    // Ff we have exhausted all words for the current slot, backtrack
    if (currentWord == wordCount) {
        return 0; // No solution found for the current slot
    }

    Slot slot = slots[currentSlot];

    // try placing the current word
    if (!words[currentWord].placed && canPlaceWord(slot, words[currentWord], grid)) {
        words[currentWord].placed = 1;
        placeWord(slot, words[currentWord], grid);

        if (solveCrosswordRecursive(slots, words, slotCount, wordCount, currentSlot + 1, 0, n, grid)) {
            return 1; // found a solution
        }

        // backtrack
        removeWord(slot, grid);
        words[currentWord].placed = 0;
    }

    // recur to try the next word
    return solveCrosswordRecursive(slots, words, slotCount, wordCount, currentSlot, currentWord + 1, n, grid);
}

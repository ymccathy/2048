/*****
 * Project 02: Grid game
 * COSC 208, Introduction to Computer Systems, Fall 2023
 *****/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int LEFT_KEY = 97;
int UP_KEY = 119;
int RIGHT_KEY = 100;
int DOWN_KEY = 115;
int SIZE = 4;

//struct gridtrack to keep track of how many spot have been occupied so far
struct gridtrack {
 int number;
};


//to randomly assign a new int 2 to a free spot
//return 1 if assign successfully, 0 if failed
int randassign(int **grid, struct gridtrack *losecheck){
    int rownum = rand()%4;
    int colnum = rand()%4;
    //check failing scenario
    if(losecheck->number>=16){
        return 0;
    }

    while(grid[rownum][colnum]!=0 && losecheck->number<16){
        rownum=rand()%4;
        colnum=rand()%4;
    }
    grid[rownum][colnum]=2;
    return 1;   
}
 

//to initialize the game when user input "NEW GAME"
int **initializeGame(struct gridtrack *losecheck){
    //fill in the 2D grid with 0 first
    int **grid = malloc(sizeof(int *) * 4);
    for(int row = 0; row<4; row++){
        grid[row] = malloc(sizeof(int) * 4);
        
        for(int col=0; col < 4; col++){
            grid[row][col] = 0;
        }
    }
    //randomly assign two 2's into the grid
    randassign(grid,losecheck);
    randassign(grid,losecheck);
    return grid;
}


//keeping track of player's score
int scoreChange(int *score, int change) {
    *score += change;
    return *score;
}


//when player press the key 'w'
int upmerge(int **grid,struct gridtrack *losecheck) {
  int change = 0;//initialize 
    for (int col = 0;col < 4;col++) {
        int rowEnd = 3; 

        for (int row = 3;row >= 0;row--) {

            if (grid[row][col]!= 0) {

                if ( (rowEnd < 3)&&(grid[rowEnd + 1][col] == grid[row][col])) {
                    grid[rowEnd + 1][col]*= 2;

                  change +=  grid[rowEnd + 1][col];

                    grid[row][col]= 0;  //empty it 
                    losecheck->number-=1;
                }

                else {
                    grid[rowEnd][col]= grid[row][col];
                    if (rowEnd!= row) {
                        grid[row][col]= 0; 
                    }
                    rowEnd--;
                }
            }
        }

        for (int i = rowEnd;i >= 0;i--) {
            if(grid[i][col] != 0){
                grid[col][i] = 0;
            }
       }
    }
    return change;
}


//when player press the key 's'
int downmerge(int **grid,struct gridtrack *losecheck){
  int change = 0;//initialize 
    for (int col= 0;col < 4;col++) { 
        int rowStart= 0; //represent the topmost index we can slide into

        for (int row = 0;row < 4;row++) {

            if (grid[row][col]!= 0) { 
                //check if the above number is the same as the current number 
                if ((rowStart > 0) && (grid[rowStart - 1][col] == grid[row][col])) { 
                    grid[rowStart- 1][col] *= 2;
                    
                    change +=   grid[rowStart- 1][col];

                    grid[row][col]= 0; //empty it
                    losecheck->number-=1;
                }

                else {
                    grid[rowStart][col] = grid[row][col];
                    if (rowStart!= row) {
                        grid[row][col]= 0; //empty the original cell if it was moved
                    }
                    rowStart++;
                }
            }
        }
        //after the topmost index to start, all values below will be 0
        for (int i = rowStart;i < 4;i++) {
            if(grid[i][col] != 0){
                grid[col][i] = 0;
            }
        }
    }
    return change;
}


//when player press the key 'a'
int  leftmerge(int **grid,struct gridtrack *losecheck){
      int change=0; 

   for(int row = 0;row < 4; row++){
    int columnStart = 0; //represent the leftmost index we can slide the number into

        for(int col = 0; col < 4;col++){ 
  
            if(grid[row][col] != 0){
              //check if they are the same from the beginning
              
              if (columnStart >0 && (grid[row][columnStart-1] == grid[row][col])) {
                grid[row][columnStart - 1] *= 2;
                
                change += grid[row][columnStart - 1];

                grid[row][col] = 0;//empty it
                losecheck->number-=1;
              }
              
              else {
                grid[row][columnStart] = grid[row][col];
                if (columnStart != col) {
                        grid[row][col] = 0;  // Empty the original cell if it was moved
                }
              columnStart++;
              }
            }
        }
            //after the leftmost index to start , all values will be 0
            for (int i = columnStart; i < 4; i++) {
                if(grid[row][i] != 0){
                    grid[row][i] = 0;
                }
              
            }
        }
        return change;
   }


//when player press the key 'd'
int rightmerge(int **grid,struct gridtrack *losecheck){
    int change =0; 
  for(int row = 0;row < 4; row++){

    int columnEnd = 3; //represent the rightmost index we can slide the number into
    
    for(int col = 3; col >= 0; col--){ 
      
      if(grid[row][col] != 0){
        
        if (columnEnd < 3 && (grid[row][columnEnd + 1] == grid[row][col])) {

          grid[row][columnEnd + 1] *= 2;

          change +=  grid[row][columnEnd + 1];

          grid[row][col] = 0; 
          losecheck->number-=1;
  
          }
        else{
            grid[row][columnEnd] = grid[row][col];
            if (columnEnd != col) {
              grid[row][col] = 0;
             }
             columnEnd--;
        }
    }
    }
        
          for (int i = columnEnd; i >= 0; i--) {
            if(grid[row][i] != 0){
                grid[row][i] = 0;
            }
            }
        }
      return change;
  }


//check key within each game loop
void keycheck(int key, int **grid,struct gridtrack *losecheck, int *score){
  int changeValue = 0; 

    if(key==UP_KEY){
         changeValue = upmerge(grid, losecheck);
    }
    else if(key==LEFT_KEY){
         changeValue = leftmerge(grid,losecheck);
    }
    else if(key==RIGHT_KEY){
          changeValue = rightmerge(grid,losecheck);
    }
    else if(key==DOWN_KEY){
         changeValue = downmerge(grid,losecheck);
    }
    else{//prompt player to enter a new valid key
        printf("YOU KEY IS INVALID, TYPE IN A VALID KEY(W,A,S,D)\n");
        char keystr[10];
        fgets(keystr,sizeof(keystr),stdin);
        keystr[strcspn(keystr,"\n")]='\0';
        if(strcmp(keystr,"END GAME")==0){
            exit(0);
        }
        key = keystr[0];
    }
    scoreChange(score, changeValue);
}


void printgrid(int **grid){
    int row, col;
    //the  boundary
    printf("------------------------\n");
    
    for (row = 3; row >= 0; --row) {

        for (col = 0; col < 4; ++col) {
          //not sure if we dont want to print 0 
          if (grid[row][col] !=0){

            if (grid[row][col]<=10){
                printf("| %d  ", grid[row][col]);
            }
            else if (grid[row][col]<=100){
                printf("| %d ", grid[row][col]);
            }
            else if (grid[row][col]<=1000){
                printf("|%d ", grid[row][col]);
            }
            else if (grid[row][col]<=10000){
                printf("|%d", grid[row][col]);
            }
          }
          else{
                printf("|    ");
          }
          //the space between numbers. except the last column
          if (col < 3) {
                printf(" ");
          }
        }
        printf("|\n");
    }
    printf("------------------------\n");

}


//check if there is a 2048 in the grid
int checkwin(int **grid){
    for(int row = 0; row < 4; row++){
        for(int col = 0; col < 4; col++){
            if(grid[row][col] == 2048){
                return 1;
            }
        }
    }
    return 0;
}


int main() {
    //printing instructions on how to play the game
    printf("WELCOME TO THE GAME 2048!!\n");
    printf("YOU WILL START WITH 2, AND DOUBLE THE NUMBER BY COLLIDING THE SAME ONES.\n");
    printf("THE ONLY WINNING MOVE IS TO GET AN 2048.\n");
    printf("CLICK w(UP), s(DOWN), a(LEFT), d(RIGHT) TO CONTROL GAME.\n");
    printf("COLLIDE SAME NUMBERS WILL DOUBLE THEM.\n");
    printf("YOU WILL LOSE IF YOU ARE OUT OF SPACE IN THE GRID.\n");
    printf("TYPE 'END GAME'(uppercase) IN PROCESS TO END THE GAME.\n");
    printf("TYPE 'NEW GAME'(uppercase) TO START THE GAME.\n");

    //initializing variables
    int score = 0; 
    char gamestart[10];
    fgets(gamestart,10,stdin);
    gamestart[strcspn(gamestart,"\n")]='\0';

    
    if (strcmp(gamestart,"NEW GAME")==0){
        
        struct gridtrack *losecheck=malloc(sizeof(struct gridtrack));

        int **grid=initializeGame(losecheck);//initializing game
        losecheck->number=2;
        printgrid(grid);
        
        //while the user is not lose yet, keep prompting user to press keys
        
        while(losecheck->number<16 && checkwin(grid)==0){
            printf ("SELECT YOUR DIRECTION KEY\n");
            //get key
            char keystr[10];
            fgets(keystr, sizeof(keystr), stdin);
            keystr[strcspn(keystr,"\n")] = '\0';
            //check if it is the END GAME key
            if(strcmp(keystr, "END GAME") == 0){
                return 0;
            }

            //type casting the key from pointer to char value
            int key = keystr[0];

            //during each game loopl, check key first and assign new int 2 second
            keycheck(key, grid, losecheck, &score);
            randassign(grid, losecheck);

            losecheck->number +=1;//increase it every time after randassign

            printf("%d\n", losecheck->number);
            printgrid(grid);
            printf("       Score: %d\n", score);

        }

        //lose game scenario
        if(randassign(grid,losecheck) == 0){
            printf("YOU LOSE THE GAME\n");
            free(losecheck);
                for(int row = 0; row < 4; row++){
                  free(grid[row]);
                  }
                  free(grid);
        }

        //win game scenario
        if(checkwin(grid)!=0){
            printf("YOU WON!\n");
            free(losecheck);
                for(int row = 0; row < 4; row++){
                   free(grid[row]);
                }
                   free(grid);
        }
        
    }
}

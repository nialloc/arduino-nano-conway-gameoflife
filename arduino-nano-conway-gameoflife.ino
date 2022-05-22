/*
 * arduino-nano-conway-gameoflife
 * Niall Ó Cléirigh May 2022
 * A simple implementation of Conway's game of life running on an Arduino Nana 
 *  
 */

#include <Arduino.h>

#define ROWS 32
#define COLS 32

#define SIZE ROWS*COLS/8
char Cells[SIZE];
char NewCells[SIZE];

int stepCount = 0;  // the number of steps executed

// is there a cell at position 'pos'
int get(int pos)  {


    // wrap around checks
    if (pos<0) {
        pos += ROWS*COLS;
    } 
    if (pos >= ROWS*COLS) {
        pos -= ROWS*COLS;
    };

    int i = pos / 8;
    int j = pos % 8;
    
    // if the j-th bit set?
    if ((Cells[i] >> j) & 0x01 ) {
        return 1;
    } else {
        return 0;
    }
}

// set the position 'pos' to value 'value'
void set(int pos, int value) {

  int i = pos / 8;
  int j = pos % 8;
  // if value is 1 then set the j-th bit
  if (value) {
      NewCells[i] |= (1 << j);  // set this bit
  } else {
      NewCells[i] &= ~(1 << j);  // turn off this bit
  }
  
}

// Overpopulation: if a living cell is surrounded by more than three living cells, it dies.
// Stasis: if a living cell is surrounded by two or three living cells, it survives.
// Underpopulation: if a living cell is surrounded by fewer than two living cells, it dies.
// Reproduction: if a dead cell is surrounded by exactly three cells, it becomes a live cell.
   
// step - perform a single step of the Conway's game of life
void step()  {

  // clear out the new cells      
  memset(NewCells, 0, SIZE);

  for (int row=0; row < ROWS; row++) {
    for (int col=0; col < COLS; col++) {
        

        // convert row/col into a postion in the array
        int pos = (col + row*COLS);
        int count = 0;

        // count_neighbours - count the number of cells that are direct neighbours   
        count += get(pos - COLS - 1);  //(row-1,col-1); 
        count += get(pos - COLS);      //(row-1,col  );
        count += get(pos - COLS + 1);  //row-1,col+1);
        
        count += get(pos - 1); //(row,col-1);
        count += get(pos + 1); //(row,col+1);
        
        count += get(pos + COLS -1); //(row+1,col-1);
        count += get(pos + COLS); //(row+1,col  );
        count += get(pos + COLS + 1); //(row+1,col+1);
                
        // if current cell is alive
        if (get(pos) == 1) {
            if (count > 3) {
                set(pos,0);
            } else if (count == 2 || count == 3) {
                set(pos,1);
            } else if (count < 2) {
                set(pos,0);
            }
        } else { // dead cell
            if (count == 3) {
                set(pos,1);
            }
        }
    }
    
  }

  // copy the newcells into the (old) cells
  memcpy(Cells, NewCells, SIZE);
  stepCount++;
}

void print_cells(){

  int count = 0;
  for (int pos=0; pos < ROWS*COLS; pos++) {
    if (get(pos)) {
      Serial.print("X");
    } else {
      Serial.print(" ");
    }
    // end of a row then move down a line
    if ((count+1) % (COLS) == 0) {
      Serial.println();
    }
    count++;
  }
}


void setup() {
  
  Serial.begin(115200);
  Serial.println("starting...");

  // fill it up with random cells.
  for (int pos=0; pos < ROWS*COLS; pos++){
    int r = random(100);
    if (r >= 50) {
      set(pos,1);
    }
  }
  
  // 'set' uses the NewCells variable, so stick it in Cells to start.
  memcpy(Cells, NewCells, SIZE);
  
}

void loop() {

  long int t0 = millis();
  step();
  long int t1 = millis();
  long int t = t1-t0;
  Serial.print("\e[1;1H\e[2J");  // clear the screen
  Serial.println("Conway's Game of Life on Arduino.");
  Serial.println("Niall Ó Cléirigh May 2022");
  Serial.print("time taken ");
  Serial.print(t);
  Serial.print(" ms in step ");
  Serial.print(stepCount);
  Serial.println();  

  print_cells();
  delay(500);

}

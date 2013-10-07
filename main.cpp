#include <graphics.h>
#include <stdio.h>
#include <conio.h>

#define ROW 8
#define COL 8
#define WIDTH 55
#define HEIGHT 55
#define DRAW_START_UP 3    // draw piece in first two rows
#define DRAW_START_DOWN 4  // draw piece in last two rows


enum PieceType { Red, Black };
enum PieceState { OnBoard, Removed };
enum GameState { Welcome, Playing, PlayerWin, CPUWin };

typedef enum PieceType PieceType;
typedef enum PieceState PieceState;
typedef enum GameState GameState;

typedef struct Board Board;
typedef struct Cell Cell;
typedef struct Piece Piece;

typedef struct Board * PtrBoard;
typedef struct Cell * PtrCell;
typedef struct Piece * PtrPiece;

struct Cell {
    
    int Top;
    int Right;
    int Bottom;
    int Left;
    
    int IsOccupied;
    PieceType OccupiedBy;
    
    int Row;
    int Column;
    
    //the piece which this cell contains
    PtrPiece Piece;
    
};

struct Piece {
    
    PieceType Type;
    int IsKing;
    PieceState State;
    
    //cell on which this piece is currently on
    PtrCell Cell;
    
};

struct Board {
    
    Cell Cells[ ROW * COL ];
    
    //12 for 6x6 24 for 8x8
    //Each piece can be on one and only one cell
    Piece Pieces[ 24 ]; 
    
};

//declared globally so that could be accessed / passed to functions easily
Board CheckersBoard;

void DrawBoard(PtrBoard);
void DrawCell(PtrCell, int, int, PtrBoard);
void DrawPiece(PtrBoard, PtrCell, int, int);

int main(void)
{

   int mode, driver = DETECT;
   initgraph(&driver, &mode, NULL); 
   
   
   DrawBoard( &CheckersBoard );
   
   
   getche();
   
   closegraph();
   
   
   
   return 0;

}

void DrawBoard(PtrBoard board)
{
   //Cells will be of blue border
    
   setcolor(BLUE);
   
   //goes from 0 to ROW*COL, we are using it to keep track of cells array in board
   int cellNo = 0;
   
   //goes from 0 to 12, we are using it to keep track of pieces array in board
   int pieceNo = 0;
   
   for(int row = 0; row < ROW; row++)
   {     
       for(int col = 0; col < COL; col++)
        {
           // configure the cell properties so that it can be drawn
            board->Cells[cellNo].Left = WIDTH * col;
            board->Cells[cellNo].Top = HEIGHT * row;
            board->Cells[cellNo].Right = WIDTH * (1+col);
            board->Cells[cellNo].Bottom =  HEIGHT * (1+row);
            
            board->Cells[cellNo].IsOccupied = 0;
            
            board->Cells[cellNo].Row = row;
            board->Cells[cellNo].Column = col;
            
            //Draws the cell configured above
            DrawCell( &board->Cells[cellNo], row, col, board );
            
            // if it is a 'white' cell      AND        this row should have Pieces 
            if ( ( (col + row )  % 2 == 0 ) && ( row < DRAW_START_UP || row > DRAW_START_DOWN) )
            {
                // pieces in first two rows are red, last two rows they're blue
                int drawColor = row < DRAW_START_UP ? RED : BLUE;
                
                DrawPiece(board, &board->Cells[cellNo], pieceNo++, drawColor);
            }
            
            cellNo++;
            
        }
   }
}

void DrawCell(PtrCell cellToDraw, int row, int col, PtrBoard board)
{
    rectangle( cellToDraw->Left, cellToDraw->Top, cellToDraw->Right, cellToDraw->Bottom );
    
    //determines whether the drawn cell is white or black
    //Condition is true in case of white cells only
    
    if ( (col + row )  % 2 == 0)
    {
        int seedX = cellToDraw->Right - 1;
        int seedY =  cellToDraw->Bottom - 1;
                
        setfillstyle(SOLID_FILL, WHITE);
        
        floodfill( seedX, seedY, BLUE );       
    }
}

void DrawPiece(PtrBoard board, PtrCell cell, int pieceNo, int color)
{
    PieceType type = color == RED ? Red : Black ;

    // configure the properties of associated cell
    cell->IsOccupied = 1;
    cell->OccupiedBy = type;

    board->Pieces[pieceNo].IsKing = 0;
    board->Pieces[pieceNo].State = OnBoard;
    board->Pieces[pieceNo].Type = type;

    //store a reference to this piece in cell
    cell->Piece = &board->Pieces[pieceNo];
    
    //set draw color of circle / piece
    setcolor(color);

    //center of circle also used as seed
    int circleX = (cell->Left  + cell->Right) / 2;
    int circleY = (cell->Top + cell->Bottom) / 2;

    circle( circleX ,circleY, 20  );

    setfillstyle(SOLID_FILL, color);

    floodfill( circleX, circleY, color );
    
    // reset draw color to draw the next rectangle / cell
    setcolor(BLUE);
}
#include <iostream>
#include <vector>
#include <iomanip>
#include <cmath>
#include <mpi.h>

using namespace std;

int main() {
	
	double xyDimension {100};
	
    double mainGrid[100][100] = {0};
    double updatedGrid[100][100] = {0};
    double changeGrid[100][100] = {0};
	
	
    bool keepGoing {true};
    double largestChange {0};
    double change {0};
    
    mainGrid[50][50] = 10.0;
    mainGrid[40][60] = 7.2;
    mainGrid[70][25] = -1.2;
    
    updatedGrid[50][50] = 10.0;
    updatedGrid[40][60] = 7.2;
    updatedGrid[70][25] = -1.2;

	MPI::Init ( argc, argv );
    int rank = MPI::COMM_WORLD.Get_rank ( ):
    int size = MPI::COMM_WORLD.Get_size ( );
	
	
	
	
//	MPI_Type_vector( 1, 1, 100, MPI_DOUBLE, &rowtype);
//	MPI_Type_commit(&rowtype);
//	
//	if (rank == 0){
//		MPI_Send(&vector, 100, MPI_INT, 1, 0, MPI_COMM_WORLD);
//		
//    else if (rank == size -1) {
//		
//		}
//	else {
//		
//		
//		}
	double pRow = xyDimension/size;
	double pRowLast = xyDimension;
    pRow = floor(pRow);
	pRowLast = pRow;
	
	
	// lets calculate what FROM, TO values do we have to work with
 	if(rank < size-1) // if I'm not the last one
 	{
 		FROM = rank*pRow;
		TO = FROM+pRow;
	}
	else  // I'm the last one
	{
 		FROM = rank*pRow;
 		TO = FROM + pRowLast;
 	}
 	
 	// every worker reports their scope
 	cout << "FROM: " << FROM << " -- TO: " << TO << " \r\n";
	
	//passing data up
	if( !(rank % 2)  && rank < rank-1 )
    {
       MPI_Send(&mainGrid[0][TO-1], xyDimension, MPI_DOUBLE, rank + 1, 222 , MPI_COMM_WORLD); // 222 - unique tag
       //cout << world_rank << " sending to " << world_rank + 1 << " \r\n";
    }
    // odd ones get data from beneath (except if lowest)
    if( (rank % 2)  && rank>1 )
    {
       MPI_Recv(&mainGrid[0][FROM-1], xyDimension, MPI_DOUBLE, rank - 1, 222 , MPI_COMM_WORLD , MPI_STATUS_IGNORE); // 222 - unique tag
       //cout << world_rank << " receiving from " << world_rank - 1 << " \r\n";
    }
    // odd ones pass data up (except if on top)
    if( (rank % 2)  && rank < (rank-1) ) 
    {
       MPI_Send(&mainGrid[0][TO-1], xyDimension, MPI_DOUBLE, rank + 1, 333 , MPI_COMM_WORLD); // 333 - unique tag
       //cout << world_rank << " sending to " << world_rank + 1 << " \r\n";
    }
    // even ones get data from beneath (except if lowest)
    if( !(rank % 2)  && rank > 0)
    {
      MPI_Recv(&mainGrid[0][FROM-1], xyDimension, MPI_DOUBLE, rank - 1, 333 , MPI_COMM_WORLD , MPI_STATUS_IGNORE); // 333 - unique tag
       //cout << world_rank << " receiving from " << world_rank - 1 << " \r\n";
    }
	
	
	// Passing data DOWN
    // even ones pass data down (except lowest)
    if( !(rank % 2)  && rank > 0 ) 
    {
       MPI_Send(&mainGrid[FROM][0], xyDimension, MPI_DOUBLE, rank - 1, 222 , MPI_COMM_WORLD); // 222 - unique tag
       //cout << world_rank << " sending to " << world_rank - 1 << "tol: " << FROM << "amimegy: " << old_array[FROM][0] <<" \r\n";
    }
    // odd ones get data from above (except if at top)
    if( (rank % 2)  && rank < (rank - 1))  
    {
       MPI_Recv(&mainGrid[TO][0], xyDimension, MPI_DOUBLE, rank + 1, 222 , MPI_COMM_WORLD , MPI_STATUS_IGNORE); // 222 - unique tag
       //cout << world_rank << " receiving from " << world_rank + 1 << "ig: " << TO+1 << "amijon: " << old_array[TO+1][0] <<" \r\n";
    }
 // odd ones pass data down
    if( (rank % 2) )  // if we are odd, we are at least the second node (counting goes from zero)
    {
       MPI_Send(&mainGrid[FROM][0], xyDimension, MPI_DOUBLE, rank - 1, 333 , MPI_COMM_WORLD); // 333 - unique tag
       //cout << world_rank << " sending to " << world_rank - 1 << "tol:" << FROM << " \r\n";
    }
    // even ones get data from above
    if( !(rank % 2)  && rank < (rank -1) ) // even ones get from above
    {
       MPI_Recv(&mainGrid[TO][0], xyDimension, MPI_DOUBLE, rank + 1, 333 , MPI_COMM_WORLD , MPI_STATUS_IGNORE); // 333 - unique tag
       //cout << world_rank << " receiving from " << world_rank + 1 << "ig: " << TO+1 << " \r\n";
    }
	
	
	
	
	
 	
// we start the outer iteration - every worker runs this iteration on their own set of lines
	
	while (keepGoing == true){       
		for (int i {0}; i < xyDimension; i++){
            for (int j {rank*(xyDimension/size)}; j < (rank+1)*(xyDimension/size); j++){
                if (i == 0 && j == 0) {
                    updatedGrid[i][j] = (mainGrid[i][j] + mainGrid[i+1][j] + mainGrid[i][j+1])/5;
                    } 
                else if (i == 0 && j != 99 && j != 0){
                    updatedGrid[i][j] = (mainGrid[i][j] + mainGrid[i+1][j] + mainGrid[i][j-1] + mainGrid[i][j+1])/5;
                    }
                else if (i == 0 && j == 99) {
                    updatedGrid[i][j] = (mainGrid[i][j] + mainGrid[i+1][j] + mainGrid[i][j-1])/5;
                    }
                else if (i == 99 && j == 0) {
                    updatedGrid[i][j] = (mainGrid[i][j] + mainGrid[i-1][j] + mainGrid[i][j+1])/5;
                    } 
                else if (i == 99 && j != 99 && j != 0){
                    updatedGrid[i][j] = (mainGrid[i][j] + mainGrid[i-1][j] + mainGrid[i][j-1] + mainGrid[i][j+1])/5;
                    }
                else if (i == 99 && j == 99) {
                    updatedGrid[i][j] = (mainGrid[i][j] + mainGrid[i-1][j] + mainGrid[i][j-1])/5;
                    }
                else if (j == 0 && i != 99 && i != 0){
                    updatedGrid[i][j] = (mainGrid[i][j] + mainGrid[i+1][j] + mainGrid[i-1][j] + mainGrid[i][j+1])/5;
                    }
                else if (j == 99 && i != 99 && i != 0){
                    updatedGrid[i][j] = (mainGrid[i][j] + mainGrid[i+1][j] + mainGrid[i-1][j] + mainGrid[i][j-1])/5;
                    }
                else if ((i != 50 || j != 50) && (i != 40 || j != 60) && (i != 70 || j != 25)){
                    updatedGrid[i][j] = (mainGrid[i][j]+mainGrid[i][j-1]+mainGrid[i][j+1]+mainGrid[i-1][j]+mainGrid[i+1][j])/5;
                    }
                }
            }
        

		for (int b {0}; b < xyDimension; b++ ){
            for (int n {rank*(xyDimension/size)}; n < (rank+1)*(xyDimension/size); n++ ){
                changeGrid[b][n] = abs(mainGrid[b][n]) - abs(updatedGrid[b][n]);
                change = abs(changeGrid[b][n]);
                if (change > largestChange){
                    largestChange = change; 
                    }				
                mainGrid[b][n] = updatedGrid[b][n];
                }
            }
		
		
		
		
		// for every worker id (world_rank) we do some send or receive
 		for(int p=1;p<world_size;p++) 
 		{
 			
 			if(rank == p) // if I'm a sender
 			{
 			    cout << "I'm (" << world_rank << ") sending lines from:  " << FROM << " to: " << TO << " \r\n";
 				for(int r=FROM;r<TO;r++)  // I send my own lines
 				{
 					MPI_Send(&mainGrid[0][r], xyDimension, MPI_DOUBLE, 0 , p*1000+r , MPI_COMM_WORLD); // 999 - unique tag				// itt az unique tag ne legyen a for " R "je? megelozik egymast?
 				}
 			}
 			else if ( rank == 0) // I'm not "p" and not the others, I'm worker one (0)
 			{
 				// what FROM, TO values should I read from? From every worker (p value) once. receive_from is rFROM    and     receive_to is rTO
 				if(p<size-1) // not the last (the last has some extra work)
 				{
 					rFROM = pRow*p; 
 					rTO = pRow*(p+1);
 				}
 				else // the last ones work
 				{
 					rFROM = pRow*p;
 					rTO = xyDimension;
 				}
 				// cout << "receiving (" << world_rank << ") lines from: " << p << " interval: " << rFROM << " - " << rTO << " \r\n";
 				for(int r=rFROM;r<rTO;r++)
 				{
 					MPI_Recv(&updatedGrid[0][r], xyDimension, MPI_DOUBLE, p , p*1000+r , MPI_COMM_WORLD, MPI_STATUS_IGNORE); // 999 - unique tag								
 				}
 			}
 		}
		
		
		
        if (largestChange == 0){
            keepGoing = false;
            }
		largestChange = 0;
		
		
		
		
        }
		
        cout.precision(2);    
        for (int i {0}; i < xyDimension; i++ ){
            cout << endl;
            for (int j {0}; j < xyDimension; j++){
                cout << mainGrid[i][j];
                }
            }

    }  
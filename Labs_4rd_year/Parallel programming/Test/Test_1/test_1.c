#include <stdio.h> 
#include <string.h>
#include <mpi.h> 
int main(int argc, char **argv) 
{ int rank, size, tag, rc, i; 
   MPI_Status status; char message[20]; 
   rc = MPI_Init(&argc, &argv); 
   rc = MPI_Comm_size(MPI_COMM_WORLD, &size); 
   rc = MPI_Comm_rank(MPI_COMM_WORLD, &rank); 
   if (rank == 0) { 
       sprintf(message,"%d",rank);
       for (i=1; i<size; i++) rc = MPI_Send(message, 1, MPI_INT, i, 0,   MPI_COMM_WORLD); 
   } 
   else 
       rc = MPI_Recv(message, 14, MPI_INT, 0, 0, MPI_COMM_WORLD, &status); 
   printf( "process %d : %s\n", rank, message); 
   rc = MPI_Finalize(); 
   return 0;
 }

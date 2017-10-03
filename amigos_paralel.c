#include <stdio.h>
#include <mpi.h>
#include <stdlib.h>
#include <sys/time.h>

int suma(int x, int rank)
{
    int res = 1;
    int i = 0;
    for (i = 2; i < x/2; i++)
    {
        if (x % i == 0)
            res += i;
    }
    
    return res;
}
int main(int argc, char *argv[])
{
    int rank, size;
    MPI_Init(&argc, &argv);

    MPI_Comm_rank(MPI_COMM_WORLD, &rank); //0
    MPI_Comm_size(MPI_COMM_WORLD, &size); //1

    if (rank == 0) {
        printf("entre proceso %d\n", rank);
        MPI_Status status;
        printf("Ingresar el valor inicial\n");
        int a;
        scanf("%d", &a);
        printf("Ingresar el valor final\n");
        int b;
        scanf("%d", &b);
        
        //a = 0;
        //b = 2000;
        MPI_Send(&a, 1, MPI_INT, 1, 0, MPI_COMM_WORLD);
        MPI_Send(&b, 1, MPI_INT, 1, 1, MPI_COMM_WORLD);

        //printf("%d, %d\n", rank, size);
        struct timeval tv;
        gettimeofday(&tv, NULL);
        unsigned int start = tv.tv_sec;

        // Esperar mensaje de proceso 1
        int x = 0, y = 0;
        while(1) {
            MPI_Recv(&x, 1, MPI_INT, 1, 0, MPI_COMM_WORLD, &status);
            MPI_Recv(&y, 1, MPI_INT, 1, 1, MPI_COMM_WORLD, &status);
            //printf("[%d]: Message received\n", rank);
            //printf("Status:: Source: %d | Tag: %d \n", status.MPI_SOURCE, status.MPI_TAG);

            if(x == -1)
                break;

            printf("%d y %d son amigos\n", x, y);
            printf("%d y %d son amigos\n", y , x);
        }

        gettimeofday(&tv, NULL);
        unsigned int end = tv.tv_sec;
        printf("%d segundos\n", end - start);
        
    }

    if(rank == 1) {
        //printf("entre proceso %d\n", rank);
        MPI_Status status;
        int a, b;
        MPI_Recv(&a, 1, MPI_INT, 0, 0, MPI_COMM_WORLD, &status);
        //printf("[%d]: Message received %d\n", rank, a);
        MPI_Recv(&b, 1, MPI_INT, 0, 1, MPI_COMM_WORLD, &status);
        //printf("[%d]: Message received %d\n", rank, b);
        int x = 0, res1, res2;

        for (x = a; x < b; x++) {
            int y = x + 1;
            for (; y <= b; y++) {
                // Send x to process 2
                MPI_Send(&x, 1, MPI_INT, 2, 0, MPI_COMM_WORLD);
                // Send y to process 3
                MPI_Send(&y, 1, MPI_INT, 3, 1, MPI_COMM_WORLD);
                // Barrier

                MPI_Recv(&res1, 1, MPI_INT, 2, 0, MPI_COMM_WORLD, &status);
                //printf("[%d]: Message received %d \n", rank, res1);
                MPI_Recv(&res2, 1, MPI_INT, 3, 1, MPI_COMM_WORLD, &status);
                //printf("[%d]: Message received %d \n", rank, res2);
                //printf("ahi te va el output %d, %d \n", res1, res2);
                if (res1 == y && res2 == x) {
                    // send x and y to process 0 for printing
                    //printf("iguales\n"); 
                    MPI_Send(&x, 1, MPI_INT, 0, 0, MPI_COMM_WORLD);
                    MPI_Send(&y, 1, MPI_INT, 0, 1, MPI_COMM_WORLD);
                }
            }
        }

        // send finalize to process 0
        x = -1;
        MPI_Send(&x, 1, MPI_INT, 0, 0, MPI_COMM_WORLD);
        MPI_Send(&x, 1, MPI_INT, 0, 1, MPI_COMM_WORLD);
        // End process 2
        MPI_Send(&x, 1, MPI_INT, 2, 0, MPI_COMM_WORLD);
        // End process 3
        MPI_Send(&x, 1, MPI_INT, 3, 1, MPI_COMM_WORLD);
    }

    if(rank == 2) {
        while(1) {
            //printf("entre proceso %d\n", rank);
            MPI_Status status;
            int x = 0;
            // Wait for message from process 1
            MPI_Recv(&x, 1, MPI_INT, 1, 0, MPI_COMM_WORLD, &status);
            
            // End process
            if(x == -1) break;

            int res = suma(x, rank);

            // Send res to process 1
            MPI_Send(&res, 1, MPI_INT, 1, 0, MPI_COMM_WORLD);
        }
    }
    if(rank == 3) {
        while(1){
            //printf("entre proceso %d\n", rank);
            MPI_Status status;
            int y = 0;
            // Wait for message from process 1
            MPI_Recv(&y, 1, MPI_INT, 1, 1, MPI_COMM_WORLD, &status);
            
            // End process
            if(y == -1) break;

            int res = suma(y, rank);

            // Send res to process 1
            MPI_Send(&res, 1, MPI_INT, 1, 1, MPI_COMM_WORLD);
        }
    }


    MPI_Finalize();
}

#include <stdio.h>
#include <stdlib.h>

#define WANTED_SIZE 16

void matrix_print(size_t matrix[], size_t height, size_t width)
{
    for(size_t r=0; r<height; r++)
    {
        for(size_t c=0; c<width; c++)
        {
            if (matrix[width*r+c])
                printf("1");
            else
                printf(" ");
        }
        printf("\n");
    }
}
void matrix_random_fill(size_t matrix[], size_t height, size_t width)
{
    for(size_t i=0; i<width*height; i++)
    {
        matrix[i] = (size_t)rand() % 2;
    }
}

//this function put the matrix at the center of a square (larger)
void matrix_put_in_square(size_t matrix[], size_t square[],
    size_t height, size_t width, size_t square_size)
{
    size_t startRow = square_size/2 - height/2;
    size_t startCol = square_size/2 - width/2;
    
    for(size_t r=0; r < height; r++)
    {
        for(size_t c=0; c < width; c++)
        {
            square[square_size*(startRow+r)+(startCol+c)] = matrix[width*(r)+c];
        }
    }
}

//resize a square into a smaller one of size n*n (n being a divisoof )
void square_resize(size_t square[], size_t resized[],
    size_t square_size, size_t resized_size)
{
    size_t ratio = square_size/resized_size;
    for(size_t r=0; r<resized_size; r++)
    {
        for(size_t c=0; c<resized_size; c++)
        {
            size_t nbOne = 0;
            for (size_t i=r*ratio; i<(r+1)*ratio; i++)
            {
                for (size_t j=c*ratio; j<(c+1)*ratio; j++)
                {
                    if (square[square_size*i+j])
                        nbOne++;
                }
            }
            size_t value = (nbOne >= ratio*ratio/2)? 1 : 0 ;
            resized[resized_size*r + c] = value;
        }
    }
}

//transform a matrix into a squared one of 16x16
void normalize(size_t matrix[], size_t result[], size_t height, size_t width)
{
    size_t square_size;
    square_size = (height>width)?
        height + (WANTED_SIZE - height%WANTED_SIZE):
        width + (WANTED_SIZE - width%WANTED_SIZE);

    size_t *square = calloc(square_size*square_size, sizeof(size_t));

    matrix_put_in_square(matrix, square, height, width, square_size);
    //matrix_print(square, square_size, square_size);

    square_resize(square, result, square_size, WANTED_SIZE);

    free(square);
}

int main()
{
    size_t *result = calloc(WANTED_SIZE*WANTED_SIZE, sizeof(size_t));
    
    size_t height = 29,
        width = 25;
    size_t *matrix = calloc(height*width, sizeof(size_t));

    //matrix_print(matrix, height, width);
    matrix_random_fill(matrix, height, width);
    //matrix_print(matrix, height, width);
    normalize(matrix, result, height, width);
    matrix_print(result, WANTED_SIZE, WANTED_SIZE);

    free(result);
    return 1;
}

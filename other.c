# include <stdio.h>
# include <unistd.h>
# include <stdlib.h>
# include <math.h>

typedef struct  square_s
{
    char        type;
    float       x;
    float       y;
    float       size_x;
    float       size_y;
    int         size_map_x;
    int         size_map_y;
    char        symbol;
}               square_t;

void    draw_drawing(char **drawing)
{
    int i;
    int k;

    i = -1;
    while (drawing[++i])
    {
        k = -1;
        while (drawing[++k])
        {
            write(1, &drawing[i][k], 1);
        }
        write(1, "\n", 1);
    }
}

int is_in_square(int y, int x, square_t *square)
{
    float i_max;
    float k_max;
    float i_min;
    float k_min;

    i_min = (square->y);
    k_min = (square->x);
    i_max = (square->size_y) + square->y;
    k_max = (square->size_x) + (square->x);
    if ((float)y >= i_min && (float)x >= k_min && (float)y <= i_max && (float)x <= k_max)
    {
        if (square->type == 'r')
        {        
            if ((float)y < i_min + 1 || (float)x < k_min + 1 ||
                (float)y + 1 > i_max || (float)x + 1 > k_max )
                return (1);
        }
        else
            return (1);
    }
    return (0);
}

void    fill_drawing(char **drawing, square_t square)
{
    int i;
    int k;

    i = 0;
    while (i < square.size_map_y)
    {
        k = 0;
        while (k < square.size_map_x)
        {
            if (is_in_square(i, k, &square))
                drawing[i][k] = square.symbol;
            k++;
        }
        i++;
    }
}

void free_mas(char **mas)
{
    int i;

    i = 0;
    while (mas[i])
        free(mas[i++]);
    free(mas);
}

char  **get_drawing(char *path)
{
    square_t    square;
    char        **drawing;
    FILE        *file;
    int         count;
    int         size_y;
    int         size_x;
    int         i;
    int         k;

    i = -1;
    if (!(file = fopen(path, "r")))
        return (NULL);
    if (fscanf(file, "%d %d %c\n", &size_y, &size_x, &square.symbol) != 3)
        return (NULL);
    if (size_y > 300 || size_x > 300 || size_y < 0 || size_x < 0)
        return (NULL);
    square.size_map_x = size_x;
    square.size_map_y = size_y;
    drawing = (char**)malloc(sizeof(char*) * (size_y + 1));
    drawing[size_y] = NULL;
    while (++i < size_y)
    {
        drawing[i] = (char*)malloc(sizeof(char) * (size_x + 1));
        k = -1;
        while (++k < size_x)
            drawing[i][k] = square.symbol;
        drawing[i][size_x] = '\0';
    }
    while ((count = fscanf(file, "%c %f %f %f %f %c\n", &square.type, &square.x, &square.y, &square.size_x, &square.size_y, &square.symbol)) == 6)
    {
        if ((square.type != 'r' && square.type != 'R') || square.size_x <= 0.0 || square.size_y <= 0.0)
        {
            free_mas(drawing);
            return (NULL);
        }
        fill_drawing(drawing, square);
    }
    if (count != -1)
    {
        free_mas(drawing);
        return (NULL);
    }
    return (drawing);
}

int main(int argc, char **argv)
{
    char  **drawing;

    if (argc == 2)
    {
        if (!(drawing = get_drawing(argv[1])))
            write(1, "Error: Operation file corrupted\n", 32);
        else
        {
            draw_drawing(drawing);
            free_mas(drawing);
            return (0);
        }
    }
    else
        write(1, "Error: argument\n", 16);
    return (1);
}

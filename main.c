# include <stdio.h>
# include <unistd.h>
# include <stdlib.h>
# include <math.h>

typedef struct  square_s
{
    int         type;
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

void    fill_drawing(char **drawing, square_t square)
{
    int i;
    int k;
    int i_max;
    int k_max;
    int i_min;
    int k_min;


    i = (int)floor(square.y);
    i_max = (int)ceil(square.size_y) + i + 1;
  
    if (ceil(square.y) - square.y > 0)
        i += 1;
    i_min = i;
    while (i < i_max && i < square.size_map_y)
    {
        k = (int)floor(square.x);
        k_max = (int)ceil(square.size_x) + k + 1;
        if (ceil(square.x) - square.x > 0)
            k += 1;
        k_min = k;
        while (k < k_max && k < square.size_map_x)
        {
            if (i >= 0 && k >= 0)
            {
                if (square.type)
                {
                    
                    if (i == i_min || k == k_min ||
                        i == i_max - 1 || k == k_max - 1)
                        drawing[i][k] = square.symbol;
                }
                else
                    drawing[i][k] = square.symbol;
            }
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
    char        ch;
    int k;

    i = -1;
    count = 6;
    if (!(file = fopen(path, "r")))
        return (NULL);
    if (fscanf(file, "%d %d %c\n", &size_y, &size_x, &square.symbol) != 3)
        return (NULL);
    if (size_y > 300 || size_x > 300)
        return (NULL);
    square.size_map_x = size_x;
    square.size_map_y = size_y;
    drawing = (char**)malloc(sizeof(char*) * (size_y + 1));
    drawing[size_y] = NULL;
    while (++i < size_y)
    {
        drawing[i] = (char*)malloc(sizeof(char) * (size_x + 1));
        drawing[i][size_x] = '\0';
    }
    square.type = 0;
    square.x = 0;
    square.y = 0;
    square.size_x = (double)size_x;
    square.size_y = (double)size_y;
    fill_drawing(drawing, square);

    while ((count = fscanf(file, "%c %f %f %f %f %c\n", &ch, &square.x, &square.y, &square.size_x, &square.size_y, &square.symbol)) == 6)
    {
        if (ch == 'r')
            square.type = 1;
        else if (ch == 'R')
            square.type = 0;
        else
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

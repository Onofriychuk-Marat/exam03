# include <stdio.h>
# include <unistd.h>
# include <stdlib.h>
# include <math.h>

typedef struct  circle_s
{
    char        type;
    float       x;
    float       y;
    float       radius;
    int         size_map_x;
    int         size_map_y;
    char        symbol;
}               circle_t;

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

int is_in_circle(float x, float y, circle_t *circle)
{
	float	distance;
	float	distance_sqrt;

	distance_sqrt = sqrtf(pow(x - circle->x, 2) + pow(y - circle->y, 2));
	distance = distance_sqrt - circle->radius;
    if (distance > 0.0 || (circle->type == 'c' && !(distance > -1.00000000)))
        return (0);
	return (1);
}

void    fill_drawing(char **drawing, circle_t circle)
{
    int i;
    int k;


    i = 0;
    while (i < circle.size_map_y)
    {
        k = 0;
        while (k < circle.size_map_x)
        {
            if (is_in_circle((float)k, (float)i, &circle))
                drawing[i][k] = circle.symbol;
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
    circle_t    circle;
    char        **drawing;
    FILE        *file;
    int         count;
    int         size_y;
    int         size_x;
    int         i;
    int         k;
    char        ch;

    i = -1;
    if (!(file = fopen(path, "r")))
        return (NULL);
    if (fscanf(file, "%d %d %c\n", &size_y, &size_x, &circle.symbol) != 3)
        return (NULL);
    if (size_y > 300 || size_y < 0 || size_x > 300 || size_x < 0)
        return (NULL);
    circle.size_map_x = size_x;
    circle.size_map_y = size_y;
    drawing = (char**)malloc(sizeof(char*) * (size_y + 1));
    drawing[size_y] = NULL;
    while (++i < size_y)
    {
        drawing[i] = (char*)malloc(sizeof(char) * (size_x + 1));
        k = -1;
        while (++k < size_x)
            drawing[i][k] = circle.symbol;
        drawing[i][size_x] = '\0';
    }
    while ((count = fscanf(file, "%c %f %f %f %c\n", &circle.type, &circle.x, &circle.y, &circle.radius, &circle.symbol)) == 5)
    {
        if ((circle.type != 'c' && circle.type != 'C') || circle.radius <= 0.0)
        {
            free_mas(drawing);
            return (NULL);
        }
        fill_drawing(drawing, circle);
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

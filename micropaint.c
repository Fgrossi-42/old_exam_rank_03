#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>

typedef struct	s_zone
{
	int		width;
	int		height;
	char	background;
	char	type;
	float	x;
	float	y;
	float   draw_height;
    float   draw_width;
	char	color;
} 				t_zone;

//UTILITY FUNCTIONS

int ft_strlen(char const *str)
{
	int	i = 0;
	while (str[i]) //SIMPLE STRING LENGTH FUNCTION
		i++;
	return (i);
}

int str_error(char const *str, int ret)
{
	write(1, str, ft_strlen(str)); //SIMPLE STRING ERROR FUNCTION
	return (ret);
}

int clear_all(FILE *file, char *drawing)
{
	fclose(file);
	if (drawing)
		free(drawing); //SIMPLE CLOSE AND FREE FILE AND DRAWING
	return (1);
}

int in_rectangle(float x, float y, t_zone *zone)
{
	if (((x < zone->x || (zone->x + zone->draw_width < x))
		|| (y < zone->y)) || (zone->y + zone->draw_height < y))
		return (0);
	if (((x - zone->x < 1.00000000) || ((zone->x + zone->draw_width) - x < 1.00000000)) ||
	((y - zone->y < 1.00000000 || ((zone->y + zone->draw_height) - y < 1.00000000))))
		return (2);                 //CHECK IF POINT IS IN RECTANGLE
	return (1);
}

//END OF UTILITY FUNCTIONS

//DRAWING FUNCTIONS

void draw_shape(char **drawing, t_zone *zone)
{
	int	i;
	int	j;
	int	ret;

	i = 0;
	while (i < zone->height)
	{
		j = 0;
		while (j< zone->width)
		{
			ret = in_rectangle(j, i, zone);     //DRAWS SHAPE
			if ((zone->type == 'r' && ret == 2)
				|| (zone->type == 'R' && ret))
				(*drawing)[(i * zone->width) + j] = zone->color;
			j++;
		}
		i++;
	}
}

void draw_drawing(char *drawing, t_zone *zone)
{
	int	i;

	i = 0;
	while (i < zone->height)
	{
		write(1, drawing + (i * zone->width), zone->width);     //OUTPUTS DRAWING
		write(1, "\n", 1);
		i++;
	}
}

//END OF DRAWING FUNCTIONS

//ACQUIRE VALUES FROM FILE

int get_shape(FILE *file, char **drawing, t_zone *zone)
{
	int		scan_ret;

	while ((scan_ret = fscanf(file, "%c %f %f %f %f %c\n", &zone->type, &zone->x, &zone->y, &zone->draw_width, &zone->draw_height, &zone->color)) == 6)
	{
		if (!(zone->draw_width > 0.00000000 && zone->draw_height > 0.00000000
			&& (zone->type == 'r' || zone->type == 'R')))
			return (0);             //GET SHAPE VALUES FROM FILE
		draw_shape(drawing, zone);
	}
	if (scan_ret != -1)
		return (0);
	return (1);
}

char *get_zone(FILE *file, t_zone *zone)
{
	int		i;
	char	*tmp;

	if (fscanf(file, "%d %d %c\n", &zone->width, &zone->height, &zone->background) != 3)
		return (NULL);
	if (zone->width <= 0 || zone->width > 300 || zone->height <= 0 || zone->height > 300)
		return (NULL);
	if (!(tmp = (char*)malloc(sizeof(*tmp) * (zone->width * zone->height))))
		return (NULL);
	i = 0;
	while (i < zone->width * zone->height)
		tmp[i++] = zone->background;
	return (tmp);
}

//END OF ACQUIRE VALUES FROM FILE

//MAIN FUNCTION

int main(int argc, char **argv)
{
	t_zone	zone;
	char	*drawing;
	FILE	*file;

	zone.width = 0;
	zone.height = 0;
	zone.background = 0;

    //MULTIPLE CHECKS TO MAKE SURE FILE IS VALID, WITH ERROR MESSAGES
	if (argc != 2)
		return (str_error("Error: argument\n", 1));

	if (!(file = fopen(argv[1], "r")))
		return (str_error("Error: Operation file corrupted\n", 1));

	if (!(drawing = get_zone(file, &zone)))
		return (clear_all(file, NULL) && str_error("Error: malloc failed :)\n", 1));

	if (!get_shape(file, &drawing, &zone))
		return (clear_all(file, drawing) && str_error("Error: Operation file corrupted\n", 1));

    //END OF CHECKS

    //DRAWING AND OUTPUT
	draw_drawing(drawing, &zone);
	clear_all(file, drawing);
	return (0);
}

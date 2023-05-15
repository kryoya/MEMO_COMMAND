#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <stdbool.h>
#include <stdio.h>
#include <string.h>

#define BUF_SIZE 512

typedef struct s_size
{
	int		file;
	int		line;
	int		*column_one;
	int		*key_one;
	int		*value_one;
}	t_size;

typedef struct s_file_ary
{
	char	*all_one;
	char	**all_two;
	char	**new_all_two;
	char	**key_two;
	char	**value_two;
}	t_file;

int	get_file_size(char	*filepath)
{
	int		fd;
	int		size;
	int		byte_num;
	char	buf[BUF_SIZE];

	fd = open(filepath, O_RDONLY);
	if (fd == -1)
		return (-1);
	size = 0;
	byte_num = 1;
	while (byte_num != 0)
	{
		byte_num = read(fd, buf, BUF_SIZE);
		if (byte_num == -1)
			return (-1);
		size += byte_num;
	}
	close(fd);
	return (size);
}

char	*read_file(char	*filepath, int	file_size)
{
	int		fd;
	int		byte_num;
	char	*one_file;

	fd = open(filepath, O_RDONLY);
	if (fd < 0)
		return (NULL);
	one_file = (char *)malloc(sizeof(char) * file_size + 1);
	for (int i = 0; i <= file_size; i++)
		one_file[i] = 0;
	byte_num = read(fd, one_file, file_size);
	if (byte_num == -1)
		return (NULL);
	close(fd);
	return (one_file);
}

int		get_line_size(char	*one_ary)
{
	int		line_size;

	line_size = 0;
	for (int i = 0; one_ary[i]; i++)
	{
		if (one_ary[i] == '\n')
			line_size++;
	}
	return (line_size);
}

int	*get_column_size(char	*one_ary, int	line_size)
{
	int		*column_size;
	int		col_idx;

	column_size = (int *)malloc(sizeof(int) * line_size + 2);
	col_idx = 0;
	for (int i = 0; i <= line_size; i++)
		column_size[i] = 0;
	for (int i = 0; col_idx < line_size; i++)
	{
		if (one_ary[i] != '\r' && one_ary[i] != '\n')
			column_size[col_idx]++;
		if (one_ary[i] == '\n')
			col_idx += 1;
	}
	return (column_size);
}

int	*get_key_size(char	**two_file, t_size	size)
{
	int		*key_size;

	key_size = (int *)malloc(sizeof(int) * size.line + 1);
	for (int i = 0; i <= size.line; i++)
		key_size[i] = 0;
	for (int h = 0; two_file[h]; h++)
	{
		for (int w = 0; two_file[h][w] != ':'; w++)
			key_size[h]++;
	}
	return (key_size);
}

int	*get_value_size(t_size	size)
{
	int		*value_size;

	value_size = (int *)malloc(sizeof(int) * size.line + 1);
	for (int i = 0; size.column_one[i]; i++)
		value_size[i] = size.column_one[i] - size.key_one[i] - 2;
	return (value_size);
}

char	**get_two_memory(int	line_size, int	*column_size)
{
	char	**two_file;
	char	*two_file_base;

	two_file = (char **)malloc(sizeof(char *) * line_size + 1);
	for (int i = 0; i < line_size; i++)
		two_file[i] = (char *)malloc(sizeof(char) * column_size[i] + 1);
	for (int h = 0; h < line_size; h++)
	{
		for (int w = 0; w <= column_size[h]; w++)
			two_file[h][w] = '\0';
	}
	two_file[line_size] = NULL;
	return (two_file);
}

char	**trns_to_two(char	*one_ary, t_size	size)
{
	int		two_w;
	int		two_h;
	char	**two_file;

	two_file = get_two_memory(size.line, size.column_one);
	two_w = 0;
	two_h = 0;
	for (int i = 0; one_ary[i]; i++)
	{
		if (one_ary[i] == '\n')
		{
			two_h++;
			two_w = 0;
		}
		else if (one_ary[i] != '\r')
		{
			two_file[two_h][two_w] = one_ary[i];
			two_w++;
		}
	}
	return (two_file);
}

char	**get_key_ary(t_file	file, t_size	size)
{
	char	**key_ary;

	key_ary = get_two_memory(size.line, size.key_one);
	for (int h = 0; h < size.line; h++)
	{
		for (int w = 0; w < size.key_one[h]; w++)
			key_ary[h][w] = file.all_two[h][w];
	}
	return (key_ary);
}

char	**get_value_ary(t_file	file, t_size	size)
{
	char	**value_ary;

	value_ary = get_two_memory(size.line, size.value_one);
	for (int h = 0; file.all_two[h]; h++)
	{
		for (int w = 0; w < size.value_one[h]; w++)
			value_ary[h][w] = file.all_two[h][w + size.key_one[h] + 2];
	}
	return (value_ary);
}

int	check_filepath(char	*filepath, t_file	set_file)
{
	for (int h = 0; set_file.all_two[h]; h++)
	{
		if (strcmp(set_file.all_two[h], filepath) == 0)
			return (1);
	}
	return (0);
}

void	input_filepath(int	ac, char	*av[], t_file	set_file, char	*filepath)
{
	if (av[1][0] == '-')
		strcpy(filepath, "general");
	else if (check_filepath(av[1], set_file))
		strcpy(filepath, av[1]);
	else
		strcpy(filepath, "no_such_file");
	return ;
}

char	*get_filepath(int	ac, char	*av[])
{
	if (ac == 1 || av[1][0] == '-')
		return ("general");
	else
		return (av[1]);
}

int		check_option(int	ac, char	*av[])
{
	int		cnt;
	int		ide_ac;

	if (ac > 5)
		return (-1);
	if (ac == 1)
		return (0);
	if (ac == 2)
	{
		if (av[1][0] == '-')
			return (-1);
		else
			return (0);
	}
	ide_ac = 0;
	if (av[1][0] != '-')
		ide_ac = 1;
	for (int i = 0; i < ac; i++)
	{
		if (strlen(av[i]) > 259)
			return (1);
	}
	if (strcmp(av[1 + ide_ac], "-m") == 0 || strcmp(av[1 + ide_ac], "-make") == 0)
	{
		if (ide_ac == 1)
			return (-1);
		return (1);
	}
	if (strcmp(av[1 + ide_ac], "-a") == 0 || strcmp(av[1 + ide_ac], "-add") == 0)
		return (2);
	if (strcmp(av[1 + ide_ac], "-s") == 0 || strcmp(av[1 + ide_ac], "-search") == 0)
	{
		if (ac != 3 + ide_ac)
			return (-1);
		return (3);
	}
	if (strcmp(av[1 + ide_ac], "-d") == 0 || strcmp(av[1 + ide_ac], "-delete") == 0)
	{
		if (ac != 3 + ide_ac)
			return (-1);
		return (4);
	}
	return (-1);
}

int	input_word(t_size	*size, char	*read_word)
{
	scanf("%s", read_word);
	read_word[strlen(read_word)] = '\0';
	return (0);
}

char	**get_new_all_two(t_file	file, t_size	*size, char	*read_word)
{
	char	**new_all_two;

	new_all_two = get_two_memory((*size).line, (*size).column_one);
	for (int h = 0; h < (*size).line; h++)
	{
		for (int w = 0; w < (*size).column_one[h]; w++)
		{
			if (h < (*size).line - 1)
				new_all_two[h][w] = file.all_two[h][w];
			else
				new_all_two[h][w] = read_word[w];
		}
	}
	return (new_all_two);
}

char	*str_connect(char	*dest, char	*str1, char	*str2, char	*sep)
{
	dest = (char *)malloc(sizeof(char) * (strlen(str1) + strlen(str2) + 1));
	for (int i = 0; i < strlen(dest); i++)
		dest[i] = 0;
	dest = strcat(dest, str1);
	dest = strcat(dest, sep);
	dest = strcat(dest, str2);
	return (dest);
}

char	**add_word(t_file	file, t_size	*size, char	*key, char	*value)
{
	char	*dest;
	char	**new_all_two;

	dest = str_connect(dest, key, value, ": ");
	(*size).line++;
	(*size).column_one[(*size).line - 1] = strlen(dest);
	(*size).column_one[(*size).line] = 0;
	new_all_two = get_new_all_two(file, size, dest);
	return (new_all_two);
}

void	print_two_ary(char	**two_ary)
{
	for (int i = 0; two_ary[i]; i++)
	{
		for (int j = 0; two_ary[i][j]; j++)
			printf("%c", two_ary[i][j]);
		printf("\n");
	}
	return ;
}

void	sort_all_two(char	**two)
{
	char	*tum;

	for (int i = 0; two[i]; i++)
	{
		for (int j = i + 1; two[j]; j++)
		{
			if (two[i] > two[j])
			{
				tum = two[i];
				two[i] = two[j];
				two[j] = tum;
			}
		}
	}
}

/*
int	search_key(t_file	file, t_size	size, char	*key)
{
	int		left;
	int		right;
	int		i;

	left = 0;
	right = size.line;
	printf("%i", size.line);
	printf("there isn't setting_file\n");
	while (left < right)
	{
		i = (left + right) / 2;
		if (strcmp(file.key_two[i], key) == 0)
			return (i);
		if (strcmp(file.key_two[i], key) < 0)
			left = i + 1;
		else
			right = i;
	}
	return (-1);
}
*/

int	search_key(t_file	file, t_size	size, char	*key)
{
	for(int i = 0; i < size.line; i++)
	{
		if (strcmp(file.key_two[i], key) == 0)
			return (i);
	}
	return (-1);
}

void	get_key_value(t_file	*file, t_size	*size)
{
	(*size).key_one = get_key_size((*file).all_two, *size);
	(*size).value_one = get_value_size(*size);
	(*file).key_two = get_key_ary(*file, *size);
	(*file).value_two = get_value_ary(*file, *size);
}

void	get_set_file(t_file	*set_file, t_size	*set_size)
{
	if (get_file_size("setting_file") == -1)
	{
		printf("there isn't setting_file\n");
		exit (1);
	}
	else
		(*set_size).file = get_file_size("setting_file");
	(*set_file).all_one = read_file("setting_file", (*set_size).file);
	(*set_size).line = get_line_size((*set_file).all_one);
	(*set_size).column_one = get_column_size((*set_file).all_one, (*set_size).line);
	(*set_file).all_two = trns_to_two((*set_file).all_one, (*set_size));
	free((*set_file).all_one);
}

void	strcat_pass(char	*filepath)
{
	char	home[260];

	strcpy(home, getenv("HOME"));
	strcat(home, "/.memo_set_dict/");
	strcat(home, filepath);
	strcpy(filepath, home);
	printf("%s\n", home);
}

void	get_spec_file(t_file	*file, t_size	*size, t_file	set_file, int	ac, char	*av[])
{
	char	filepath[260];

	input_filepath(ac, av, set_file, filepath);
	if (strcmp(filepath, "no_such_file") == 0)
	{
		printf("no such file\n");
		exit (1);
	}
	strcat_pass(filepath);
	(*size).file = get_file_size(filepath);
	(*file).all_one = read_file(filepath, (*size).file);
	(*size).line = get_line_size((*file).all_one);
	(*size).column_one = get_column_size((*file).all_one, (*size).line);
	(*file).all_two = trns_to_two((*file).all_one, *size);
	free((*file).all_one);
}

void	setting_file(t_file		*file, t_size	*size, int	ac, char	*av[])
{
	t_size	set_size;
	t_file	set_file;

	get_set_file(&set_file, &set_size);
	get_spec_file(file, size, set_file, ac, av);
	free(set_size.column_one);
	free(set_file.all_two);
}

int		write_file(char	*filepath, t_file	file)
{
	int		buf_num;
	FILE	*fp;

	fp = fopen(filepath, "w");
	if (fp == NULL)
		return (1);
	for (int i = 0; file.all_two[i]; i++)
	{
		buf_num = fprintf(fp, "%s\n", file.all_two[i]);
		if (buf_num < 0)
			return (1);
	}
	fclose(fp);
	return (0);
}

void	process_add(int	ac, char	*av[], t_file	*file, t_size	*size)
{
	char	value[260];
	char	key[260];
	int		ide_ac;

	ide_ac = 0;
	if (av[1][0] != '-')
		ide_ac = 1;
	if (ac == 3 + ide_ac)
	{
		strcpy(value, av[2 + ide_ac]);
		snprintf(key, 260, "%d", (*size).line);
	}
	else
	{
		strcpy(key, av[2 + ide_ac]);
		strcpy(value, av[3 + ide_ac]);
	}
	(*file).all_two = add_word(*file, size, key, value);
	get_key_value(file, size);
	sort_all_two((*file).all_two);
	write_file(get_filepath(ac, av), *file);
}

int	process_search(int	ac, char	*av[], t_file	file, t_size	size)
{
	int		test2;
	int		ide_ac;
	char	key[260];

	ide_ac = 0;
	if (av[1][0] != '-')
		ide_ac = 1;
	strcpy(key, av[2 + ide_ac]);
	test2 = search_key(file, size, key);
	if (test2 == -1)
	{
		printf("no such key\n");
		return (0);
	}
	else
	{
		fprintf(stdout, "%s\n", file.value_two[test2]);
		return (0);
	}
}

int	process_make(char	*av[])
{
	FILE	*fp;

	fp = fopen(av[2], "a");
	if (fp == 0)
		return (1);
	fclose(fp);
	return (0);
}

void	add_setting(char	*av[])
{
	char	**new_all_two;
	t_file	set_file;
	t_size	set_size;

	get_set_file(&set_file, &set_size);
	set_size.line++;
	set_size.column_one[set_size.line - 1] = strlen(av[2]);
	set_size.column_one[set_size.line] = 0;
	set_file.all_two = get_new_all_two(set_file, &set_size, av[2]);
	write_file("setting_file", set_file);
}

void	process_option(int	ac, char	*av[], t_file	*file, t_size	*size)
{
	int		opn_idx;

	opn_idx = check_option(ac, av);
	if (opn_idx == -1)
	{
		printf("option error\n");
		return ;
	}
	else if (opn_idx == 0)
		print_two_ary((*file).all_two);
	else if (opn_idx == 1)
	{
		if (process_make(av))
		{
			printf("make file error\n");
			return ;
		}
		add_setting(av);
	}
	else if (opn_idx == 2)
		process_add(ac, av, file, size);
	else if (opn_idx == 3)
		if (process_search(ac, av, *file, *size) == -1)
			printf("no such key\n");
	else if (opn_idx == 4)
	{
		printf("delete\n");
	}
	return ;
}

int	main(int	ac, char	*av[])
{
	t_size	size;
	t_file	file;

	setting_file(&file, &size, ac, av);
	get_key_value(&file, &size);
	process_option(ac, av, &file, &size);
	return (0);
}

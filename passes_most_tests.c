#include "get_next_line.h"
#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

size_t	ft_strlen(const char *s)
{
	int	i;
	static int j = 1;

	// printf("This is the %d. time this function was called.\n", j);
	j++;
	i = 0;
	if (!s || s[0] == '\0')
		return (i);
	while (s[i] != '\0')
		i++;
	return (i);
}

// char	*calloc(size_t number, size_t size)
// {
// 	char	*dest;
// 	size_t	i;

// 	i = 0;
// 	if (number == 0 || size == 0)
// 		return (NULL);
// 	dest = malloc(number * size);
// 	if (dest == NULL)
// 		return (NULL);
// 	while (i < (number * size))
// 	{
// 		dest[i] = '\0';
// 		i++;
// 	}
// 	return (dest);
// }

char	*ft_strdup(char *s)
{
	char	*dest;
	int		i;
	int		s_len;

	i = 0;
	s_len = ft_strlen(s);
	dest = malloc(1 * (s_len + 2));
	// printf("mallocing at %p.\n", dest);
	if (dest == 0)
		return (NULL);
	while (i < s_len)
	{
		dest[i] = s[i];
		i++;
	}
	dest[i] = '\0';
	dest[i + 1] = '\0';
	// printf("freeing memory address %p.\n", s);
	free (s);
	return (dest);
}

char	*ft_strchr(const char *s, int c)
{
	int		i;

	i = 0;
	while (s[i] != '\0')
	{
		if (s[i] == (char)c)
			return ((char *)(&s[i]));
		i++;
	}
	if ((char)c == '\0' && s[i] == '\0')
		return ((char *)(&s[i]));
	return (0);
}

char	*ft_add_temp(char *remainder, char *temporary)
{
	char	*new;
	int		i;
	int		k;
	int		len;

	if (temporary == NULL)
		return (ft_strdup(remainder));
	len = (ft_strlen(remainder) + ft_strlen(temporary));
	new = calloc((len + 2), 1);
	// printf("Calloc in add_temp. Memory address is: %p\n", new);
	i = 0;
	if (new == NULL)
		return (NULL);
	while (remainder[i] != '\0')
	{
		new[i] = remainder[i];
		i++;
	}
	k = 0;
	while (temporary[k] != '\0')
	{
		new[i + k] = temporary[k];
		k++;
	}
	return (new);
}

char	*ft_update_buffer(char *remainder, char *temporary)
{
	char	*new;
	int		i;

	i = 0;
	if (remainder == NULL)
	{
		new = calloc(ft_strlen(temporary)+ 2, 1);
		// printf("Calloc in update_buffer. Memory address is: %p\n", new);
		while (temporary[i] != '\0')
		{
			new[i] = temporary[i];
			i++;
		}
	}
	else if (temporary[0] == '\0')
	{
		// printf("freeing memory address %p.\n", temporary);
		free(temporary);
		return (ft_strdup(remainder));
	}
	else
		new = ft_add_temp(remainder, temporary);
	// printf("freeing memory address %p.\n", remainder);
	free(temporary);
	free (remainder);
	return (new);
}

char	*ft_substr_free(char *s, unsigned int start, size_t len)
{
	char	*substr;
	size_t	total_len;
	size_t	i;

	if (!s)
	{
		free(s);
		return (NULL);
	}
	if (!len || start >= (ft_strlen(s)))
	{
		free(s);
		return (NULL);
	}
	substr = calloc(len + 1, 1);
	// printf("Calloc in substr_free. Memory address is: %p\n", substr);
	if (substr == 0)
		return (NULL);
	total_len = start + len;
	i = 0;
	while (start < total_len)
	{
		substr[i] = s[start];
		start++;
		i++;
	}
	// printf("freeing memory address %p.\n", s);
	free (s);
	return (substr);
}

char	*ft_substr(char *s, unsigned int start, size_t len)
{
	char	*substr;
	size_t	total_len;
	size_t	i;

	if (!s)
		return (NULL);
	if (!len || start >= (ft_strlen(s)))
		return (NULL);
	substr = calloc(len + 1, 1);
	// printf("Calloc in substr. Memory address is: %p\n", substr);
	if (substr == 0)
		return (NULL);
	total_len = start + len;
	i = 0;
	while (start < total_len)
	{
		substr[i] = s[start];
		start++;
		i++;
	}
	return (substr);
}


char	*get_next_line(int fd)
{
	static char	*remainder = NULL;
	char		*temporary;
	char		*helper;
	static int	count_bytes = 1;

	if (fd < 0 || BUFFER_SIZE < 1)
		return (NULL);
	if (remainder == NULL && count_bytes == 0)
		return (NULL);
	// printf("Remainder at start of program: %s", remainder);
	if (count_bytes == 0 && remainder != NULL && ft_strchr(remainder, '\n') == NULL)
		return (remainder);
	if (count_bytes == 0 && remainder != NULL && ft_strchr(remainder, '\n') != NULL)
	{
		temporary = ft_substr(remainder, 0, (ft_strchr(remainder, '\n') - (remainder - 1)));
		helper = remainder;
		remainder = ft_substr(remainder, (ft_strchr(remainder, '\n') - (remainder - 1)), ft_strlen(remainder) - ft_strlen(temporary));
		free (helper);
		return (temporary);
	}
	while (count_bytes > 0)
	{
		temporary = calloc(BUFFER_SIZE + 1, 1);
		count_bytes = read(fd, temporary, BUFFER_SIZE);
		if (count_bytes == 0 && remainder == NULL)
		{
			free (temporary);
			return (remainder);
		}
		remainder = ft_update_buffer(remainder, temporary); // effectively joining both strings and freeing them.
		if (ft_strchr(remainder, '\n') != NULL)
			break;
	}
	if (count_bytes == 0)
		remainder[ft_strlen(remainder)] = '\n';
	// printf("remainder after while loop: %s", remainder);
	// if (count_bytes == 0 && ft_strchr(remainder, '\n') == NULL)
	// 	return (remainder);
	// if (remainder[0] == '\n')
	// {
	// 	helper = remainder;
	// 	remainder = ft_strdup(helper + 1);
	// 	free (helper);
	// 	printf("%s", remainder);
	// 	temporary = ft_substr(remainder, 0, 1);
	// 	return (temporary);
	// }
	temporary = ft_substr(remainder, 0, (ft_strchr(remainder, '\n') - (remainder - 1)));
	// printf("temporary: %s", temporary);
	helper = remainder;
	remainder = ft_substr(remainder, (ft_strchr(remainder, '\n') - (remainder - 1)), ft_strlen(remainder) - ft_strlen(temporary));
	free (helper);
	// // printf("Remainder: %s", remainder);
	if (count_bytes == 0 && remainder == NULL)
	{
		temporary[ft_strlen(temporary) - 1] = '\0';
	}
	return (temporary);
}

// int	main(void)
// {
// 	int		fd;
// 	int		i;
// 	char	*string;
// 	char	*file_name;

// 	file_name = "readfrom.c";
// 	i = 0;
// 	printf("BUFFER_SIZE: %d\n", BUFFER_SIZE);
// 	fd = open(file_name, O_RDWR);
// 	if (fd == -1)
// 	{
// 		printf("open() failed.");
// 		return (0);
// 	}
// 	printf("File descriptor chosen: %d\n", fd);
// 	while (i < 30)
// 	{
// 		string = get_next_line(fd);
// 		printf("Return string: %s", string);	
// 		i++;
// 		// printf("freeing memory address after gnl call %p.\n", string);
// 		free(string);
// 	}
// 	if (close(fd) == -1)
// 	{
// 		printf("close() failed.");
// 		return (0);
// 	}
// }
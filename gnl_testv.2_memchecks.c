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
	if (s == NULL || s[0] == '\0')
		return (i);
	while (s[i] != '\0')
		i++;
	// printf("Length of string read is: %d\n", i);
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
	printf("mallocing at %p.\n", dest);
	if (dest == 0)
		return (NULL);
	while (i < s_len)
	{
		dest[i] = s[i];
		i++;
	}
	dest[i] = '\0';
	dest[i + 1] = '\0';
	printf("freeing memory address %p.\n", s);
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

char	*ft_add_temp(char *buffer, char *temp)
{
	char	*new;
	int		i;
	int		k;
	int		len;

	if (temp == NULL)
		return (ft_strdup(buffer));
	len = (ft_strlen(buffer) + ft_strlen(temp)); // took out '+1' / 2. and 3. strlen call.
	// printf("2nd. and 3rd call in add_temp.\n");
	new = calloc((len + 2), 1);
	printf("Calloc in add_temp. Memory address is: %p\n", new);
	i = 0;
	if (new == NULL)
		return (NULL);
	while (buffer[i] != '\0')
	{
		new[i] = buffer[i];
		i++;
	}
	k = 0;
	while (temp[k] != '\0')
	{
		new[i + k] = temp[k];
		k++;
	}
	return (new);
}

char	*ft_update_buffer(char *buffer, char *temp)
{
	char	*new;
	int		i;

	i = 0;
	if (buffer == NULL)
	{
		new = calloc(ft_strlen(temp)+ 2, 1);
		printf("Calloc in update_buffer. Memory address is: %p\n", new);
		// printf("update_buffer 1st. call.\n"); // first strlen call
		while (temp[i] != '\0')
		{
			new[i] = temp[i];
			i++;
		}
	}
	else if (temp[0] == '\0')
	{
		printf("freeing memory address %p.\n", temp);
		free(temp);
		return (ft_strdup(buffer));
	}
	else
		new = ft_add_temp(buffer, temp);
	printf("freeing memory address %p.\n", buffer);
	free (buffer);
	return (new);
}

char	*ft_substr_free(char *s, unsigned int start, size_t len)
{
	char	*substr;
	size_t	total_len;
	size_t	i;

	// printf("%s", s);
	// printf("start: %d. len: %ld\n", start, len);
	// printf("strlen: %ld\n", ft_strlen(s));
	if (!s)
		return (NULL);
	if (!len || start >= (ft_strlen(s)))
		return (NULL);
	substr = calloc(len + 1, 1);
	printf("Calloc in substr_free. Memory address is: %p\n", substr);
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
	// substr[i] = 0;
	// printf("%s\n", s);
	printf("freeing memory address %p.\n", s);
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
	printf("Calloc in substr. Memory address is: %p\n", substr);
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
	// substr[i] = '\0';
	return (substr);
}

char	*get_next_line(int fd)
{
	static char	*buffer = NULL;
	char		*temp;
	char		*helper;
	int			count_bytes;

	if (fd < 0 || BUFFER_SIZE < 1)
		return (0);
	temp = calloc(BUFFER_SIZE + 1, 1);
	printf("Calloc in gnl, callocing temp. Memory address is: %p\n", temp);
	count_bytes = read(fd, temp, BUFFER_SIZE);
	if (buffer == NULL && count_bytes == 0)
	{
		printf("free temp at: %p\n", temp);
		free (temp);
		return (NULL);
	}
	while (count_bytes > 0)
	{
		buffer = ft_update_buffer(buffer, temp); // buffer doesn't get freed in the last loop. Buffer holds the entire line now.
		if (ft_strchr(buffer, '\n') != NULL)
			break;
		count_bytes = read(fd, temp, BUFFER_SIZE);
	}
	if (count_bytes == 0)
	{
		buffer[ft_strlen(buffer)] = '\n';
		printf("freeing temp at %p.\n", temp);
		free (temp);
		return (buffer);
	}
	if (count_bytes != 0)
	{
		printf("freeing memory address %p.\n", temp);
		free (temp);
		temp = ft_substr(buffer, 0, (ft_strchr(buffer, '\n') - (buffer - 1)));
		helper = ft_strdup(buffer);
		buffer = ft_substr_free(helper, (ft_strchr(helper, '\n') - (helper - 1)), ft_strlen(helper) - ft_strlen(temp));
		printf("freeing memory address %p.\n", helper);
		free (helper);
	}
	return (temp);
}


int	main(void)
{
	int		fd;
	int		i;
	char	*string;
	char	*file_name;

	file_name = "readfrom.c";
	i = 0;
	printf("BUFFER_SIZE: %d\n", BUFFER_SIZE);
	fd = open(file_name, O_RDWR);
	if (fd == -1)
	{
		printf("open() failed.");
		return (0);
	}
	printf("File descriptor chosen: %d\n", fd);
	while (i < 2)
	{
		string = get_next_line(fd);
		printf("Return string: %s", string);	
		i++;
		printf("freeing memory address %p.\n", string);
		free(string);
	}
	if (close(fd) == -1)
	{
		printf("close() failed.");
		return (0);
	}
}

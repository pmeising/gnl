/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   old_gnl.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pmeising <pmeising@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/05/26 11:02:08 by pmeising          #+#    #+#             */
/*   Updated: 2022/05/31 06:32:32 by pmeising         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "get_next_line.h"
#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

size_t	ft_strlen(const char *s)
{
	int	i;

	i = 0;
	if (s == NULL || s[0] == '\0')
		return (i);
	while (s[i] != '\0')
		i++;
	return (i);
}

// made changes to the original:
// 

char	*ft_strjoin_del(char *s1, char *s2)
{
	char	*dest;
	int		i;
	int		k;
	int		len;

	if (s1 == NULL || s2 == NULL)
		return (NULL);
	len = (ft_strlen(s1) + ft_strlen(s2) + 1);
	dest = (char *)malloc(len);
	i = 0;
	if (dest == NULL)
		return (NULL);
	while (s1[i] != '\0')
	{
		dest[i] = s1[i];
		i++;
	}
	k = 0;
	while (s2[k] != '\0')
	{
		dest[i + k] = s2[k];
		k++;
	}
	dest[i + k] = '\0';
	// free (s1);
	free (s2);
	return (dest);
}

char	*ft_calloc(size_t number, size_t size)
{
	char	*dest;
	size_t	i;

	i = 0;
	if (number == 0 || size == 0)
		return (NULL);
	dest = malloc(number * size);
	if (dest == NULL)
		return (NULL);
	while (i < (number * size))
	{
		dest[i] = '\0';
		i++;
	}
	return (dest);
}

char	*ft_strdup(const char *s)
{
	char	*dest;
	int		i;
	int		s_len;

	i = 0;
	s_len = ft_strlen(s);
	dest = malloc(1 * (s_len + 1));
	if (dest == 0)
		return (NULL);
	while (i < s_len)
	{
		dest[i] = s[i];
		i++;
	}
	dest[i] = '\0';
	return (dest);
}

char	*ft_substr(char const *s, unsigned int start, size_t len)
{
	char	*substr;
	size_t	total_len;
	size_t	i;

	if (!s)
		return (NULL);
	if (!len || start >= (ft_strlen(s)))
		return (ft_strdup(""));
	substr = malloc(len * sizeof(char) + 1);
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
	substr[i] = 0;
	return (substr);
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
	
	// First I need to read the content up to BUFFER_SIZE;
	// This I store in a temporary buffer.
	// Now I need to check, if in that buffersize I find a \n character.
	// If I do, I'll use ft_substr to extract both the actual new line and then 
	// as well the remainder.
	// The remainder I store in a static variable for use in the second gnl call.
	// The next line I can return like it is.
	// If byte_count of read at any point is -1, I return a NULL string.
	// If byte_count of read at any point is 0, I know I've reached the end of the file.

char	*get_next_line(int fd)
{
	static int	byte_count;
	char		*temp_buffer;
	char		*total_buffer;
	static char	*remainder = NULL;

	if (fd < 0)
		return (0);
	byte_count = 1;
	if (remainder == NULL)
		total_buffer = ft_calloc(1, 1);
	else if (remainder != NULL && byte_count != 0)
		total_buffer = remainder;
	while (byte_count > 0 && (ft_strchr(total_buffer, '\n') == NULL))
	{
		temp_buffer = ft_calloc((BUFFER_SIZE + 1), 1);
		byte_count = read(fd, temp_buffer, BUFFER_SIZE);
		total_buffer = ft_strjoin_del(total_buffer, temp_buffer);
	}
	remainder = ft_strchr(total_buffer, '\n');
	printf("%s\n", remainder);
	total_buffer[(remainder - total_buffer)] = '\0';
	remainder = ft_substr(remainder + 1, 1, ft_strlen(remainder));
	// printf("%s\n", remainder);
	if (byte_count == 0 && remainder != NULL)
	{
		printf("I am here.");
		// printf("%s\n", remainder);
		total_buffer[ft_strlen(total_buffer)] = '\n';
		// free(total_buffer);
		return (remainder);
	}
	else if (byte_count == 0 && remainder == NULL)
		return (total_buffer);
	return (total_buffer);
}


int	main(void)
{
	int		fd;
	int		i;
	char	*string;

	i = 0;
	printf("BUFFER_SIZE: %d\n", BUFFER_SIZE);
	fd = open("readfrom.c", O_RDWR);
	if (fd == -1)
	{
		printf("open() failed.");
		return (0);
	}
	printf("File descriptor chosen: %d\n", fd);
	while (i < 6)
	{
		string = get_next_line(fd);
		printf("Return string: %s", string);	
		i++;
	}
	if (close(fd) == -1)
	{
		printf("close() failed.");
		return (0);
	}
	free(string);
}

/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_next_line.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pmeising <pmeising@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/05/30 16:48:14 by pmeising          #+#    #+#             */
/*   Updated: 2022/05/31 16:27:47 by pmeising         ###   ########.fr       */
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
		return (buffer);
	len = (ft_strlen(buffer) + ft_strlen(temp)); // took out '+1' / 2. and 3. strlen call.
	// printf("2nd. and 3rd call in add_temp.\n");
	new = calloc((len + 2), 1);
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
	new[i + k] = '\0';
	return (new);
}

char	*ft_update_buffer(char *buffer, char *temp)
{
	char	*new;
	int		i;

	i = 0;
	if (buffer == NULL)
	{
		new = calloc(ft_strlen(temp)+ 1, 1);
		// printf("update_buffer 1st. call.\n"); // first strlen call
		while (temp[i] != '\0')
		{
			new[i] = temp[i];
			i++;
		}
		free(buffer);
		free(temp);
		return (new);
	}
	else
	{
		new = ft_add_temp(buffer, temp);
		free (buffer);
		free (temp);
		return (new);
	}
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

char	*ft_substr_free(char *s, unsigned int start, size_t len)
{
	char	*substr;
	size_t	total_len;
	size_t	i;

	if (!s)
		return (NULL);
	if (!len || start >= (ft_strlen(s)))
		return (ft_strdup(""));
	substr = calloc(len + 1, 1);
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
		return (ft_strdup(""));
	substr = calloc(len + 1, 1);
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

char	*get_next_line(int fd)
{
	static char	*buffer = NULL;
	char		*temp;
	static int	count_bytes = 1;

	if (fd < 0)
		return (0);
	// if (buffer == NULL && count_bytes == 0)
	// 	return (NULL);
	// printf("Start of file Buffer: %s\n", buffer);
	// count_bytes = 1;
	while (count_bytes != 0 && count_bytes != -1)
	{
		temp = calloc(BUFFER_SIZE + 1, 1);
		count_bytes = read(fd, temp, BUFFER_SIZE);
		// printf("temp stages: %s\n", temp);
		buffer = ft_update_buffer(buffer, temp);
		if (count_bytes == 0)
		{
			buffer[ft_strlen(buffer)] = '\n';
			break;
		}
		if (ft_strchr(buffer, '\n') != NULL)
			break;
	}
	if (ft_strchr(buffer, '\n') != NULL)
		temp = ft_substr(buffer, 0, (ft_strchr(buffer, '\n') + 1 - buffer - 1));
	else
		temp = buffer;
	buffer = ft_substr_free(buffer, (ft_strchr(buffer, '\n') + 1 - buffer), ft_strlen(buffer) - ft_strlen(temp));
	// printf("End of file Buffer: %s\n", buffer);
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
	while (i < 20)
	{
		string = get_next_line(fd);
		printf("Return string: %s", string);	
		i++;
		free(string);
	}
	if (close(fd) == -1)
	{
		printf("close() failed.");
		return (0);
	}
}

/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_next_line.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pmeising <pmeising@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/05/26 11:02:08 by pmeising          #+#    #+#             */
/*   Updated: 2022/05/26 18:25:41 by pmeising         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "get_next_line.h"
#include <fcntl.h>

char	*get_next_line(int fd)
{
	void		*buf;
	int			i;
	static	int	offset;

	while (i < read(fd, buf, BUFFER_SIZE))

	printf("%s\n", buf);
	return (buf);
}

int	main(void)
{
	int		fd;
	char	*string;

	fd = open("/readfrom.c", O_RDWR);
	string = get_next_line(fd);
	printf("%s", string);
}
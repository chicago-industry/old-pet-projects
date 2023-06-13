/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_getline.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yshawn <yshawn@student.21-school.ru>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/11/07 15:15:44 by yshawn            #+#    #+#             */
/*   Updated: 2021/11/07 15:15:48 by yshawn           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libfuck.h"

static char	*ft_getline_error(int *status, int err)
{
	*status = err;
	return (0);
}

static char	*ft_getline_extra_new_line(char **buf, char *nl, int *status)
{
	char	*line;
	char	*temp;
	size_t	len;

	len = nl - *buf;
	line = (char *)ft_memdup(*buf, len + 1);
	if (!line)
		return (ft_getline_error(status, 2));
	line[len] = '\0';
	if ((*buf + len + 1)[0] != '\0')
	{
		temp = ft_strdup(*buf + len + 1);
		if (!temp)
			return (ft_getline_error(status, 2));
		ft_memdel((void **)buf);
		*buf = temp;
	}
	else
		ft_memdel((void **)buf);
	return (line);
}

static char	*ft_getline_extra(char **buf, int *status)
{
	char	*nl;
	char	*line;

	nl = NULL;
	line = NULL;
	if (*buf)
		nl = ft_strchr(*buf, '\n');
	if (nl)
		return (ft_getline_extra_new_line(buf, nl, status));
	else
	{
		if (*buf)
		{
			line = ft_strdup(*buf);
			if (!line)
			{
				*status = 2;
				return (NULL);
			}
			ft_memdel((void **)buf);
		}
	}
	return (line);
}

char	*ft_getline(int fd, int *status)
{
	static char	*arr[GNL_MAX_FD + 1];
	char		buf[GNL_BUFFER_SIZE + 1];
	char		*temp;
	ssize_t		rd;

	*status = 0;
	ft_memset(&buf, 0, GNL_BUFFER_SIZE + 1);
	if (fd < 0 || fd > GNL_MAX_FD)
		return (NULL);
	while (1)
	{
		rd = read(fd, buf, GNL_BUFFER_SIZE);
		if (rd <= 0)
			break ;
		buf[rd] = '\0';
		temp = ft_strjoin(arr[fd], buf);
		if (arr[fd])
			ft_memdel((void **)&arr[fd]);
		arr[fd] = temp;
		if (ft_strchr(arr[fd], '\n'))
			break ;
	}
	if (rd == -1)
		return (ft_getline_error(status, 1));
	return (ft_getline_extra(&arr[fd], status));
}

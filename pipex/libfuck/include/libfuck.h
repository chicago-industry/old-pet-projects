/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   libfuck.h                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yshawn <yshawn@student.21-school.ru>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/11/07 15:17:29 by yshawn            #+#    #+#             */
/*   Updated: 2021/11/07 15:17:30 by yshawn           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef LIBFUCK_H
# define LIBFUCK_H

# include <stdlib.h>
# include <unistd.h>

# define GNL_BUFFER_SIZE	100
# define GNL_MAX_FD			1024

// memes
void	ft_memdel(void **ap);
void	*ft_memset(void *s, int c, size_t len);
void	*ft_memcpy(void *dest, const void *source, size_t size);
void	*ft_memdup(const void *source, size_t size);

// search
int		ft_strcmp(const char *s1_1, const char *s2_2);
char	*ft_strchr(const char *str, int c);
char	*ft_strstr(const char *haystack, const char *needle);
char	*ft_strnstr(const char *haystack, const char *needle, size_t len);

// strings: merge, split, copy 
char	*ft_strdup(const char *s);
char	**ft_strsplit(char const *s, char c);
char	*ft_strjoin(char const *s1, char const *s2);

// strings: other
size_t	ft_strlen(const char *s);
void	ft_free_str_arr(char ***arr);
char	*ft_strsub(char const *s, unsigned int start, size_t len);

// I/O
void	ft_putstr_fd(char *s, int fd);
char	*ft_getline(int fd, int *status);

// cesspool

// # define BASE_HEX		16
// # define BASE_DECIMAL	10
// # define BASE_OCTAL		8
// # define BASE_BINARY	2

// # define SINT_MIN "-2147483648"
// # define SINT_MAX "2147483647"
// # define SINT_MAX_P "+2147483647"

// int		ft_atoi(const char *str);
// int		ft_isnumber_int(const char *str);
// int		ft_isnumber(const char *str);
// char	*ft_itoa_base(int val, int base);
// char	*ft_itoa(int n);
// int		ft_isdigit(int c);
// int		ft_numlen(ssize_t value, int base);
// 
// void	*ft_memccpy(void *dest, const void *src, int c, size_t n);
// void	*ft_memchr(const void *s, int c, size_t n);
// int		ft_memcmp(const void *dest, const void *src, size_t n);
// void	*ft_memcpy(void *dest, const void *src, size_t n);
// void	*ft_memcpy(void *dest, const void *src, size_t n);
// void	*ft_memmove(void *dest, const void *src, size_t len);
// void	*ft_memset(void *b, int c, size_t len);
// void	*ft_memdup(const void *source, size_t size);
// void	ft_memdel(void **p);
// 
// char	*ft_strcat(char *dest, const char *src);
// char	*ft_strchr(const char *s, int c);
// int		ft_strcmp(const char *s1, const char *s2);
// char	*ft_strdup(const char *s);
// int		ft_strncmp(const char *s1, const char *s2, size_t n);
// char	*ft_strnstr(const char *dest, const char *src, size_t len);
// char	*ft_strdup(const char *s);

#endif

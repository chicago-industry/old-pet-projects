#include "../include/malloc.h"

void	print_error_malloc(void *ptr, char *msg)
{
	if (ptr && msg)
	{
		printl(2, E_ERR);
		print_address_hex(2, ptr);
		printl(2, msg);
	}
	else if (!ptr && msg)
	{
		printl(2, E_ERR);
		printl(2, msg);
	}
}

void	*ft_memset(void *s, int c, size_t len)
{
	char	*tmp;
	size_t	i;

	i = 0;
	tmp = (char *)s;
	while (i < len)
	{
		tmp[i] = c;
		i++;
	}
	return (s);
}

void	*ft_memcpy(void *dest, const void *source, size_t size)
{
	char	*dest_copy;
	char	*source_copy;

	if (!size || dest == source)
		return (NULL);
	dest_copy = (char *)dest;
	source_copy = (char *)source;
	while (size)
	{
		*dest_copy = *source_copy;
		dest_copy++;
		source_copy++;
		size--;
	}
	return (dest);
}

static size_t	ft_strlen(const char *s)
{
	size_t i;

	i = 0;
	while (s[i])
		i++;
	return (i);
}

void	printl(int fd, char *s)
{
    write(fd, s, ft_strlen(s));
}

static char	hex_digit(int v)
{
	if (v >= 0 && v < 10)
		return ('0' + v);
	else
		return ('a' + v - 10);
}

void	print_num(int fd, long n)
{
	unsigned long	un;
	char			c;

	if (fd < 0)
		return ;
	if (n < 0)
	{
		printl(fd, "-");
		un = -n;
	}
	else
		un = n;
	if (un > 9)
		print_num(fd, un / 10);
	c = un % 10 + '0';
	write(fd, &c, 1);
}

void	print_address_hex(int fd, void *p0)
{
	uintptr_t	p = (uintptr_t)p0;
	int			i, flag = 0;
	char		ch = '0';

	if (fd < 0)
		return ;
	printl(fd, "0x");
	for (i = (sizeof(p) << 3) - 4; i >= 0; i -= 4)
	{
		if ('0' < (ch = hex_digit((p >> i) & 0xf)))
			flag = 1;
		if (flag)
			write(fd, &ch, 1);
	}
	if (!flag)
		write(fd, &ch, 1);
}

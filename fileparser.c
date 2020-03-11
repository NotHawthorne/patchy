/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   fileparser.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alkozma <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/03/10 18:30:03 by alkozma           #+#    #+#             */
/*   Updated: 2020/03/11 08:50:37 by alkozma          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "patchy.h"

int		patchy_error(int code, char *str)
{
	switch (code)
	{
		case 1:
			return (printf("usage: ./patchy [path_to_bin] [path_to_pac]\n"));
		case 2:
			return (printf("patchy: invalid path: %s\n", str));
		case 3:
			return (printf("patchy: mmap failed\n"));
		case 4:
			return (printf("patchy: bad file\n"));
		default:
			break ;
	}
	return (0);
}

uint32_t	swapend(uint32_t num)
{
	return (((num >> 24) & 0xff) | ((num << 8) & 0xff0000)
		| ((num >> 8) & 0xff00) | ((num << 24) & 0xff000000));
}

void	parse_header(struct p4_header *h)
{
	struct p4_catalog_env	*ce;
	struct p4_version		*v;
	struct p4_file			*f;
	uint32_t				filect;
	uint32_t				verct;

	ce = ((void*)h) + sizeof(struct p4_header);
	v = (void*)(ce + 1);
	f = (void*)(v + 1);
	if (strcmp(h->magic, "BBDL"))
		patchy_error(BADFILE, NULL);
	filect = swapend(h->total_file_count);
	verct = swapend(h->version_count);
	printf("GAME VERSION: %d\n", swapend(h->version_count));
	printf("SIZE OF FILE: %d\n", swapend(h->rounded_size));
	printf("TOTAL FILE COUNT: %d\n", swapend(h->total_file_count));
	printf("TOTAL SIZE OF FILES: %d\n", swapend(h->total_file_size));
	f = (void*)(v + verct);
	printf("\nFILES IN LIST\n==============\n");
	for (uint32_t i = 0; i != filect; i++)
	{
		printf("%d - %s\n", i, f->path);
		f++;
	}
}

int		main(int argc, char **argv)
{
	void		*start;
	struct stat	s;
	int			fd;

	printf("PERSONA 4 BIN PATCHER\n=============\n");
	if (argc != 3)
		return (patchy_error(USAGE, NULL));
	if ((fd = open(argv[1], O_RDONLY)) < 0 || fstat(fd, &s) < 0)
		return (patchy_error(PATH, argv[1]));
	start = mmap(0, s.st_size, PROT_READ, MAP_PRIVATE, fd, 0);
	printf("INPUT FILE: %s\n", argv[1]);
	if (start == MAP_FAILED)
		return (patchy_error(MEMFAIL, NULL));
	parse_header(start);
}

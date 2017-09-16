/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   quartic.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jpeg <jpeg@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2017/09/14 20:25:59 by jpeg              #+#    #+#             */
/*   Updated: 2017/09/17 01:44:49 by jpeg             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
#include <math.h>

typedef struct		s_c
{
	float a;
	float b;
	float c;
	float d;
	float e;
}					t_c;

typedef		struct 	s_res
{
	float x1;
	float x2;
	float x3;
	float x4;
	float real;
	float imag;
}				t_res;

// SOLVING
// see http://1728.org/quartic2.htm
// and http://www.1728.org/cubic2.htm

void div_by_a(t_c *c)
{
	c->b /= c->a;
	c->c /= c->a;
	c->d /= c->a;
	c->e /= c->a;
	c->a /= c->a;
}

float	pw(float x, int pw)
{
	float a = 0;
	if(pw==2)
		a = x*x;
	else if(pw == 3)
		a = x*x*x;
	else if(pw == 4)
		a = x*x*x*x;
	else
	{
		while(a < pw)
		{
			x *= x;
			a++;
		}
		return(x);
	}
	return(a);
}

void solve_cubic(t_c *cub, t_res *res)
{
	float f,g,h;
	printf("Cubic solving \na = %f b= %f c = %f d= %f\n\n", cub->a, cub->b, cub->c, cub->d);
/*	//TESTING
	cub = &(t_c){2, -4, -22, 24, 0}; // 3 REAL
	cub = &(t_c){3, -10, 14, 27, 0}; // 1 REAL - 2 COMPLEX
	cub = &(t_c){1, 6, 12, 8, 0}; //  3 EQUAL
	printf("Cubic solvig \na = %f b= %f c = %f d= %f\n", cub->a, cub->b, cub->c, cub->d);*/
	//
	f = ((3 * cub->c / cub->a) - (pw(cub->b,2)/pw(cub->a,2))) / 3;
	g = ((2 * pw(cub->b,3) / pw(cub->a, 3)) - (9 * cub->b * cub->c / pw(cub->a,2)) + (27 * cub->d / cub->a)) / 27;
	h = (pw(g,2)/4) + (pw(f,3) / 27);
	printf("f = %f\ng = %f \nh = %f\n\n",f,g,h);
	if (h <= 0 && g && f)
	{
		printf("3 REAL ROOTS\n\n");
		float i = pow((((pw(g,2)/4)) - h), (float)1/2);
		float j = pow(i, (float)1/3);
		float k = acos((float)(-(g / (2.0 * i))));
		float l = -j;
		float m = cos(k/3.0f);
		float n = sqrt(3.0) * sin(k/3);
		float p = -(cub->b / (3 * cub->a));
		res->x1 = (2 * j) * cos(k / 3.0f) - (cub->b / (3 * cub->a));
		res->x2 = l * (m + n) + p;
		res->x3 = l * (m - n) + p;
		printf("i = %f\nj= %f\nk = %f\nl = %f\nm = %f\nn = %f\np = %f\n\n", i,j,k,l,m,n,p);
		printf("x1 = %f\nx2 = %f\nx3 = %f\n\n", res->x1,res->x2,res->x3);
	}
	else if (!h && !g && !f)
	{
		printf("3 ROOTS ARE ==\n");
		res->x1 = -pow((cub->d / cub->a), 1.0f/3.0f);
		printf("x1 = x2 = x3 = %f \n\n", res->x1);
	}
	else if(h > 0)
	{
		printf("1 REAL ROOT AND 2 COMPLEX\n");
		float r = -(g/2) + pow(h, 0.5f);
		float s = -pow(-r, 1.0f/3.0f);
		float t = -(g / 2) - pow(h, 0.5f);
		float u = -pow(-t, 1.0f/3.0f);
		res->x1 = (s + u) - (cub->b / (3 * cub->a));
		res->real = (-(s + u))/2 - (cub->b / (3 * cub->a));
		res->imag = (s - u) * (pow(3.0, 0.5f)/2);
		printf("r = %f\ns= %f\nt = %f\nu = %f\nx1 = %f\nreal = %f\nimag = %f\n\n", r,s,t,u,res->x1,res->real,res->imag);
	}
	printf("Cubic solved\n\n");
}

void select_non_zero(t_res res, float *p, float *q)
{
	res.x1 = res.x1 < 0.001 ? 0 : res.x1;
	res.x2 = res.x2 < 0.001 ? 0 : res.x2;
	res.x3 = res.x3 < 0.001 ? 0 : res.x3;
	if(res.x1 && res.x2)
	{
		*p = sqrt(res.x1);
		*q = sqrt(res.x2);
	}
	else if(res.x2 && res.x3)
	{
		*p = sqrt(res.x2);
		*q = sqrt(res.x3);
	}
	else
	{
		*p = sqrt(res.x1);
		*q = sqrt(res.x3);
	}
}

void solve_quartic(t_c *c, t_res *roots)
{
	t_res cub_roots;
	float f,g,h,p,q,r,s;

	printf("Quartic solving \na = %f b = %f c = %f d =  %f e = %f \n\n", c->a, c->b, c->c, c->d, c->e);
	div_by_a(c);
	printf("Simplification by a \na = %f b = %f c = %f d =  %f e = %f \n", c->a, c->b, c->c, c->d, c->e);

	f = c->c - (((3 * c->b * c->b)) / 8);
	g = c->d + ((c->b * c->b * c->b) / 8) - (c->b * c->c / 2);
	h = c->e - ((3 * c->b * c->b * c->b * c->b) / 256) + (((c->b * c->b) * c->c) / 16) - (c->b * c->d / 4);
	printf("f = %f, g = %f, h = %f\n\n",f,g,h);

	t_c cub = (t_c){1, f / 2, ((f * f - 4 * h) / 16), ((-g * -g) / 64), 0};
	solve_cubic(&cub, &cub_roots);

	select_non_zero(cub_roots, &p, &q);
	printf("p = %f, q = %f\n", p, q);

	r = -g / (8 * p * q);
	s = c->b / (4 * c->a);
	printf("r = %f et s = %f\n\n", r,s);

	roots->x1 = p + q + r - s;
	roots->x2 = p - q - r - s;
	roots->x3 = - p + q - r - s;
	roots->x4 = - p - q + r - s;
	//printf("x1 = %f\nx2 = %f\nx3 = %f\nx4 = %f\n", roots->x1, roots->x2, roots->x3, roots->x4);
	// FINISH!
}

int main(int ac, char **av)
{
	if (ac == 6)
	{
		t_res roots;
		t_c c = (t_c){atoi(av[1]), atoi(av[2]), atoi(av[3]), atoi(av[4]), atoi(av[5])};
		solve_quartic(&c, &roots);
		printf("x1 = %f\nx2 = %f\nx3 = %f\nx4 = %f\n\n", roots.x1, roots.x2, roots.x3, roots.x4);
		printf("Quartic solved\n");
	}
	else
		printf("Usage : ./quartic a b c d e\n");
	return(0);
}

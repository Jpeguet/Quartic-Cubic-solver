/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cubic.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jpeg <jpeg@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2017/09/14 20:25:59 by jpeg              #+#    #+#             */
/*   Updated: 2017/09/17 02:32:40 by jpeg             ###   ########.fr       */
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

	if(!cub->a)
	{
		printf("2nd degree equation detected..\n");
		return;
	}
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
		float m = cos(k/3.0);
		float n = sqrt(3.0) * sin(k/3);
		float p = -(cub->b / (3 * cub->a));
		res->x1 = (2 * j) * cos(k / 3.0) - (cub->b / (3.0 * cub->a));
		res->x2 = l * (m + n) + p;
		res->x3 = l * (m - n) + p;
		printf("i = %f\nj= %f\nk = %f\nl = %f\nm = %f\nn = %f\np = %f\n\n", i,j,k,l,m,n,p);
		printf("x1 = %f\nx2 = %f\nx3 = %f\n\n", res->x1,res->x2,res->x3);
	}
	else if (!h && !g && !f)
	{
		printf("3 ROOTS ARE ==\n");
		res->x1 = -pow((cub->d / cub->a), (float)0.3333);
		printf("x1 = x2 = x3 = %f \n\n", res->x1);
	}
	else if(h > 0)
	{
		printf("1 REAL ROOT AND 2 COMPLEX\n");
		float r = -(g/2) + pow(h, 0.5);
		float s = pow(fabs(r), 0.3333);
		float t = -(g / 2) - pow(h, 0.5);
		float u = pow(fabs(t), 0.3333);
		s = r < 0 ? -s : s;
		u = t < 0 ? -u : u;
		res->x1 = (s + u) - (cub->b / (3 * cub->a));
		res->real = (-(s + u))/2 - (cub->b / (3 * cub->a));
		res->imag = (s - u) * (pow(3.0, 0.5)/2);
		printf("r = %f\ns= %f\nt = %f\nu = %f\n\nreal = %f\nimag = %f\n\nx1 = %f\n", r,s,t,u,res->real,res->imag,res->x1);
		printf("x2 = %f - i * %f\nx3 = %f + i * %f\n", res->real, res->imag, res->real, res->imag);
	}
}

int main(int ac, char **av)
{
	if (ac == 5)
	{
		t_res roots = (t_res){0,0,0,0,0,0};
		t_c c = (t_c){atoi(av[1]), atoi(av[2]), atoi(av[3]), atoi(av[4])};
		solve_cubic(&c, &roots);
		printf("Cubic solved\n\n");
	}
	else
		printf("Usage : ./cubic a b c d\n");
	return(0);
}
/*
{2, -4, -22, 24, 0}; // 3 REAL
{3, -10, 14, 27, 0}; // 1 REAL - 2 COMPLEX
{1, 6, 12, 8, 0}; //  3 EQUAL
*/

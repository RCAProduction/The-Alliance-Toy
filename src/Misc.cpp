#include <stdio.h>
#include <sstream>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <cmath>
#include "Config.h"
#include "Misc.h"
#include "icondoc.h"
#ifdef WIN
#include <shlobj.h>
#include <shlwapi.h>
#include <windows.h>
#else
#include <unistd.h>
#include <time.h>
#include <sys/time.h>
#endif
#ifdef MACOSX
#include <mach-o/dyld.h>
#endif

char *exe_name(void)
{
#if defined(WIN)
	char *name= (char *)malloc(64);
	DWORD max=64, res;
	while ((res = GetModuleFileName(NULL, name, max)) >= max)
	{
#elif defined MACOSX
	char *fn=(char*)malloc(64),*name=(char*)malloc(PATH_MAX);
	uint32_t max=64, res;
	if (_NSGetExecutablePath(fn, &max) != 0)
	{
		fn = (char*)realloc(fn, max);
		_NSGetExecutablePath(fn, &max);
	}
	if (realpath(fn, name) == NULL)
	{
		free(fn);
		free(name);
		return NULL;
	}
	res = 1;
#else
	char fn[64], *name=(char *)malloc(64);
	size_t max=64, res;
	sprintf(fn, "/proc/self/exe");
	memset(name, 0, max);
	while ((res = readlink(fn, name, max)) >= max-1)
	{
#endif
#ifndef MACOSX
		max *= 2;
		name = (char *)realloc(name, max);
		memset(name, 0, max);
	}
#endif
	if (res <= 0)
	{
		free(name);
		return NULL;
	}
	return name;
}

//Signum function
int isign(float i) //TODO: INline or macro
{
	if (i<0)
		return -1;
	if (i>0)
		return 1;
	return 0;
}

unsigned clamp_flt(float f, float min, float max) //TODO: Also inline/macro
{
	if (f<min)
		return 0;
	if (f>max)
		return 255;
	return (int)(255.0f*(f-min)/(max-min));
}

float restrict_flt(float f, float min, float max) //TODO Inline or macro or something
{
	if (f<min)
		return min;
	if (f>max)
		return max;
	return f;
}

char *mystrdup(const char *s)
{
	char *x;
	if (s)
	{
		x = (char*)malloc(strlen(s)+1);
		strcpy(x, s);
		return x;
	}
	return NULL;
}

void strlist_add(struct strlist **list, char *str)
{
	struct strlist *item = (struct strlist*)malloc(sizeof(struct strlist));
	item->str = mystrdup(str);
	item->next = *list;
	*list = item;
}

int strlist_find(struct strlist **list, char *str)
{
	struct strlist *item;
	for (item=*list; item; item=item->next)
		if (!strcmp(item->str, str))
			return 1;
	return 0;
}

void strlist_free(struct strlist **list)
{
	struct strlist *item;
	while (*list)
	{
		item = *list;
		*list = (*list)->next;
		free(item);
	}
}

void save_string(FILE *f, char *str)
{
	int li = strlen(str);
	unsigned char lb[2];
	lb[0] = li;
	lb[1] = li >> 8;
	fwrite(lb, 2, 1, f);
	fwrite(str, li, 1, f);
}

int load_string(FILE *f, char *str, int max)
{
	int li;
	unsigned char lb[2];
	fread(lb, 2, 1, f);
	li = lb[0] | (lb[1] << 8);
	if (li > max)
	{
		str[0] = 0;
		return 1;
	}
	fread(str, li, 1, f);
	str[li] = 0;
	return 0;
}

const static char hex[] = "0123456789ABCDEF";
void strcaturl(char *dst, char *src)
{
	char *d;
	unsigned char *s;

	for (d=dst; *d; d++) ;

	for (s=(unsigned char *)src; *s; s++)
	{
		if ((*s>='0' && *s<='9') ||
		        (*s>='a' && *s<='z') ||
		        (*s>='A' && *s<='Z'))
			*(d++) = *s;
		else
		{
			*(d++) = '%';
			*(d++) = hex[*s>>4];
			*(d++) = hex[*s&15];
		}
	}
	*d = 0;
}

void strappend(char *dst, const char *src)
{
	char *d;
	unsigned char *s;

	for (d=dst; *d; d++) ;

	for (s=(unsigned char *)src; *s; s++)
	{
		*(d++) = *s;
	}
	*d = 0;
}

void *file_load(char *fn, int *size)
{
	FILE *f = fopen(fn, "rb");
	void *s;

	if (!f)
		return NULL;
	fseek(f, 0, SEEK_END);
	*size = ftell(f);
	fseek(f, 0, SEEK_SET);
	s = malloc(*size);
	if (!s)
	{
		fclose(f);
		return NULL;
	}
	fread(s, *size, 1, f);
	fclose(f);
	return s;
}

int cpu_check(void)
{
/*#ifdef MACOSX
	return 0;
#else
#ifdef X86
	unsigned af,bf,cf,df;
	x86_cpuid(0, af, bf, cf, df);
	//if (bf==0x68747541 && cf==0x444D4163 && df==0x69746E65)
	//	amd = 1;
	x86_cpuid(1, af, bf, cf, df);
#ifdef X86_SSE
	if (!(df&(1<<25)))
		return 1;
#endif
#ifdef X86_SSE2
	if (!(df&(1<<26)))
		return 1;
#endif
#ifdef X86_SSE3
	if (!(cf&1))
		return 1;
#endif
#endif
#endif*/
	return 0;
}

matrix2d m2d_multiply_m2d(matrix2d m1, matrix2d m2)
{
	matrix2d result = {
		m1.a*m2.a+m1.b*m2.c, m1.a*m2.b+m1.b*m2.d,
		m1.c*m2.a+m1.d*m2.c, m1.c*m2.b+m1.d*m2.d
	};
	return result;
}
vector2d m2d_multiply_v2d(matrix2d m, vector2d v)
{
	vector2d result = {
		m.a*v.x+m.b*v.y,
		m.c*v.x+m.d*v.y
	};
	return result;
}
matrix2d m2d_multiply_float(matrix2d m, float s)
{
	matrix2d result = {
		m.a*s, m.b*s,
		m.c*s, m.d*s,
	};
	return result;
}

vector2d v2d_multiply_float(vector2d v, float s)
{
	vector2d result = {
		v.x*s,
		v.y*s
	};
	return result;
}

vector2d v2d_add(vector2d v1, vector2d v2)
{
	vector2d result = {
		v1.x+v2.x,
		v1.y+v2.y
	};
	return result;
}
vector2d v2d_sub(vector2d v1, vector2d v2)
{
	vector2d result = {
		v1.x-v2.x,
		v1.y-v2.y
	};
	return result;
}

matrix2d m2d_new(float me0, float me1, float me2, float me3)
{
	matrix2d result = {me0,me1,me2,me3};
	return result;
}
vector2d v2d_new(float x, float y)
{
	vector2d result = {x, y};
	return result;
}

void HSV_to_RGB(int h,int s,int v,int *r,int *g,int *b)//convert 0-255(0-360 for H) HSV values to 0-255 RGB
{
	float hh, ss, vv, c, x;
	int m;
	hh = h/60.0f;//normalize values
	ss = s/255.0f;
	vv = v/255.0f;
	c = vv * ss;
	x = c * ( 1 - fabs(fmod(hh,2.0f) -1) );
	if(hh<1){
		*r = (int)(c*255.0);
		*g = (int)(x*255.0);
		*b = 0;
	}
	else if(hh<2){
		*r = (int)(x*255.0);
		*g = (int)(c*255.0);
		*b = 0;
	}
	else if(hh<3){
		*r = 0;
		*g = (int)(c*255.0);
		*b = (int)(x*255.0);
	}
	else if(hh<4){
		*r = 0;
		*g = (int)(x*255.0);
		*b = (int)(c*255.0);
	}
	else if(hh<5){
		*r = (int)(x*255.0);
		*g = 0;
		*b = (int)(c*255.0);
	}
	else if(hh<6){
		*r = (int)(c*255.0);
		*g = 0;
		*b = (int)(x*255.0);
	}
	m = (int)((vv-c)*255.0);
	*r += m;
	*g += m;
	*b += m;
}

void OpenURI(std::string uri) {
#if defined(WIN)
	ShellExecute(0, "OPEN", uri.c_str(), NULL, NULL, 0);
#elif defined(MACOSX)
	char *cmd = (char*)malloc(7+uri.length());
	strcpy(cmd, "open ");
	strappend(cmd, (char*)uri.c_str());
	system(cmd);
#elif defined(LIN)
	char *cmd = (char*)malloc(11+uri.length());
	strcpy(cmd, "xdg-open ");
	strappend(cmd, (char*)uri.c_str());
	system(cmd);
#else
	printf("Cannot open browser\n");
#endif
}

void RGB_to_HSV(int r,int g,int b,int *h,int *s,int *v)//convert 0-255 RGB values to 0-255(0-360 for H) HSV
{
	float rr, gg, bb, a,x,c,d;
	rr = r/255.0f;//normalize values
	gg = g/255.0f;
	bb = b/255.0f;
	a = fmin(rr,gg);
	a = fmin(a,bb);
	x = fmax(rr,gg);
	x = fmax(x,bb);
	if (a==x)//greyscale
	{
		*h = 0;
		*s = 0;
		*v = (int)(a*255.0);
	}
	else
	{
 		c = (rr==a) ? gg-bb : ((bb==a) ? rr-gg : bb-rr);
 		d = (rr==a) ? 3 : ((bb==a) ? 1 : 5);
 		*h = (int)(60.0*(d - c/(x - a)));
 		*s = (int)(255.0*((x - a)/x));
 		*v = (int)(255.0*x);
	}
}

void membwand(void * destv, void * srcv, size_t destsize, size_t srcsize)
{
	size_t i;
	unsigned char * dest = (unsigned char*)destv;
	unsigned char * src = (unsigned char*)srcv;
	for(i = 0; i < destsize; i++){
		dest[i] = dest[i] & src[i%srcsize];
	}
}

void millisleep(long int t)
{
#ifdef WIN
	Sleep(t);
#else
	struct timespec s;
	s.tv_sec = t / 1000;
	s.tv_nsec = (t % 1000) * 10000000;
	nanosleep(&s, NULL);
#endif
}

long unsigned int gettime()
{
#ifdef WIN
	return GetTickCount();
#elif defined(MACOSX)
	struct timeval s;
	gettimeofday(&s, NULL);
	return (unsigned int)(s.tv_sec * 1000 + s.tv_usec / 1000);
#else
	struct timespec s;
	clock_gettime(CLOCK_MONOTONIC, &s);
	return s.tv_sec * 1000 + s.tv_nsec / 1000000;
#endif
}

vector2d v2d_zero = {0,0};
matrix2d m2d_identity = {1,0,0,1};

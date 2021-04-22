#include "dave.h"

void dpRGBtoHSV(int red,int green,int blue,int *hue,int *sat,int *val)
{
 float r,g,b,h,s,v,min,max,delta;
 r = red/255.0;
 g = green/255.0;
 b = blue/255.0;
 if (r>g) if (r>b) max = r; else max = b;
 else if (g>b) max = g; else max = b;
 if (r<g) if (r<b) min = r; else min = b;
 else if (g<b) min = g; else min = b;
 v = max;
 if (max != 0.0) s = (max-min)/max;
 else s = 0.0;
 if (s == 0.0) h = 0.0;
 else {
        delta = max-min;
        if (r == max) h = (g-b)/delta;
        else if (g == max) h = 2.0+(b-r)/delta;
        else h = 4.0+(r-g)/delta;
        h = h*60.0;
        if (h<0) h += 360.0;
        }
 *hue = h*255.0/360.0;
 *sat = s*255.0;
 *val = v*255.0;
}

void dpHSVtoRGB(int hue,int sat,int val,int *red,int *green,int *blue)
{
 float r,g,b,h,s,v,f,p,q,t;
 int i;
 h = hue/255.0*360.0;
 s = sat/255.0;
 v = val/255.0;
 if (s == 0.0) r = g = b = v;
 else {
        if (h == 360.0) h = 0.0;
        h /= 60.0;
        i = h;
        f = h-i;
        p = v*(1.0-s);
        q = v*(1.0-s*f);
        t = v*(1.0-s*(1.0-f));
        switch (i) {
                case 0: r = v;  g = t;  b = p; break;
                case 1: r = q;  g = v;  b = p; break;
                case 2: r = p;  g = v;  b = t; break;
                case 3: r = p;  g = q;  b = v; break;
                case 4: r = t;  g = p;  b = v; break;
                case 5: r = v;  g = p;  b = q; break;
                }
        }
 *red = r*255.0;
 *green = g*255.0;
 *blue = b*255.0;
}


#include <iostream>
#include <vector>
#include <fstream>
#include <math.h>
#include <future>

#include "meshreader.hpp"
#include "vectorpoint.hpp"

vec3 up(0,1,0);
vec3 left(1,0,0);
vec3 fwd(0,0,1);
vec3 none(0,0,0);

vec3 projectscreen(float screendist,vec3* point,float screenmulti) {
    vec3 screen(0,0,screendist);
    vec3 p(point->x,point->y,point->z);
    vec3 diff = screen-p;

    //multiply by arbritary value, acting as the field of view
    vec3 norm = diff.normal()*50;
    vec3 planenorm = fwd*50;

    //basically project the screen to point normal onto a viewport plane, that is then rendered onto screen

    float dot = planenorm.dot(norm); 
    vec3 offset = norm/dot; 
    offset /= 0.02;

    offset *= screenmulti;
    offset.z = diff.magnitude();

    return offset;
}

vec3 rotatex(float t,vec3 normal) {
    vec3 a(1,0,0);
    vec3 b(0,cos(t),-sin(t));
    vec3 c(0,sin(t),cos(t));

    a *= normal;
    b *= normal;
    c *= normal;

    return vec3(a.consolidate(),b.consolidate(),c.consolidate());
}

vec3 rotatey(float t,vec3 normal) {
    vec3 a(cos(t),0,sin(t));
    vec3 b(0,1,0);
    vec3 c(-sin(t),0,cos(t));

    a *= normal;
    b *= normal;
    c *= normal;

    return vec3(a.consolidate(),b.consolidate(),c.consolidate());
}

vec3 rotatez(float t,vec3 normal) {
    vec3 a(cos(t),-sin(t),0);
    vec3 b(sin(t),cos(t),0);
    vec3 c(0,0,1);

    a *= normal;
    b *= normal;
    c *= normal;

    return vec3(a.consolidate(),b.consolidate(),c.consolidate());
}

//this is an optional thing, that uses normal characters instead of the RGB terminal stuff
char lscale[10] = {' ','.','-',':','=','+','*','#','%','@'};
char luminate(int l){
    if (lscale[l]) return lscale[l];
    return '?';
}

bool insidetriangle(vec3 screenpos, vec3 p1, vec3 p2, vec3 p3) {
    vec3 edge12 = (p2-p1).normal();
    vec3 edge23 = (p3-p2).normal();
    vec3 edge34 = (p1-p3).normal();

    vec3 off1 = (p1-screenpos).normal();
    vec3 off2 = (p2-screenpos).normal();
    vec3 off3 = (p3-screenpos).normal();
    
    float d12 = edge12.cross2d(off1);
    float d23 = edge23.cross2d(off2);
    float d34 = edge34.cross2d(off3);

    return (d12<0 && d23<0 && d34<0);
}

vec3 trianglenormal(vec3 p1, vec3 p2, vec3 p3) {

    vec3 a = p2-p1;
    vec3 b = p3-p1;

    return a.cross(b).normal();

}

int main() {

    meshreader::mesh object = {};  
    meshreader::meshFromFile("res/lowmonkey.obj",&object);

    int screensize = 50;
    int screensizew = 50;

    int pixelbuffer[screensizew][screensize] = {0};
    int depthbuffer[screensizew][screensize] = {0};

    float pitch = 1;
    float yaw = 1;
    float roll = 1;

    float rad = 0.0174533;

    vec3 sun(0,1,0);

    int nothinglol;
    std::cin >> nothinglol;

    vec3 p1(0,0,0);
    vec3 p2(0,0,0);   
    vec3 p3(0,0,0);

    while (true) {

        for (int i = 0; i < screensizew; i++){
            for (int j = 0; j < screensize; j++)
            {
                pixelbuffer[i][j] = 0;
                depthbuffer[i][j] = 999;
            }
        }
        
        pitch += 2.5;
        yaw += 1.4;
        roll += 0.3;
 
        for (int tindex = 0; tindex < object.st; tindex++) {
            
            //get each vertex
            vec3 v1 = object.verts[object.tris[tindex][0]]; 
            vec3 v2 = object.verts[object.tris[tindex][1]];
            vec3 v3 = object.verts[object.tris[tindex][2]];
            
            //rotate each vertex around center
            v1 = rotatey(yaw*rad,rotatex(pitch*rad,rotatez(roll*rad,v1)));
            v2 = rotatey(yaw*rad,rotatex(pitch*rad,rotatez(roll*rad,v2)));
            v3 = rotatey(yaw*rad,rotatex(pitch*rad,rotatez(roll*rad,v3)));

            vec3 center = (v1+v2+v3)/3; //get midpoint of each vertex
            float depth = (center-vec3(0,0,8)).magnitude(); //get distance from viewport
          
            vec3 normal = trianglenormal(v1,v2,v3);
          
            //project each vertex to the viewport
            p1 = projectscreen(8,&v1,120); 
            p2 = projectscreen(8,&v2,120);   
            p3 = projectscreen(8,&v3,120);

            //center each vertex
            p1.x += screensizew/2; 
            p1.y += screensize/2;

            p2.x += screensizew/2;
            p2.y += screensize/2;

            p3.x += screensizew/2;
            p3.y += screensize/2;

            float lumen = pow(((normal.dot(sun)+1.5)/2),3); //calculate light luminence from sun dot product
            if (lumen>1) lumen = 1;
            if (lumen<0) lumen = 0;

            int lumenval = lumen*255; //scale up to 0-255 so it works with the RGB stuff

            vec3 sp(0,0,0);
            vec3 screencenter = (p1+p2+p3)/3;
           
            for (int i = screencenter.x-5; i < screencenter.x+5; i++){
                for (int j = screencenter.y-5; j < screencenter.y+5; j++)
                {
                    if (i >= 0 && i < screensizew && j >= 0 && j < screensize) {

                        sp.x = i;
                        sp.y = j;
                        bool inside = insidetriangle(sp,p1,p2,p3);

                        if (inside) {
                            if (depthbuffer[i][j] > depth) {
                                pixelbuffer[i][j] = lumenval;
                                depthbuffer[i][j] = depth;
                            }
                        }
                    }   
                }
            }

        }

        system("clear");
        for (int y = 0; y < screensize; y++) {
            for (int x = 0; x < screensizew; x++) {         
                if (pixelbuffer[x][y] != 0) {
                    int l = pixelbuffer[x][y];
                    printf("\033[48;2;%d;%d;%dm  \033[0m",l,l,l);
                  
                } else {
                    std::cout << "  ";
                }
            }
            std::cout << std::endl;
        }



    }

    return 1;
}   
#pragma once
#include <iostream>
#include <vector>
#include <math.h>
#include <map>
#include <algorithm>

class vec3
{
public:
	float x;
	float y;
	float z;

	vec3(float x,float y,float z) :x(x),y(y),z(z) {};
	
	float dot(vec3 target) {
        return (target.x*x)+(target.y*y)+(target.z*z);
    }
    
    vec3 cross(vec3 target) {
        float nx = y*target.z - z*target.y;
        float ny = z*target.x - x*target.z;
        float nz = x*target.y - y*target.x;
        return vec3(nx,ny,nz);
    }

    float cross2d(vec3 target) {
        return x*target.y - y*target.x;
    }

    vec3 matrix(vec3 target) {
        float nx = x*target.x + x*target.y + x * target.z;
        float ny = y*target.x + y*target.y + y * target.z;
        float nz = z*target.x + z*target.y + z * target.z;
        return vec3(nx,ny,nz);
    }

    void operator*=(vec3 target) {
        x *= target.x;
        y *= target.y;
        z *= target.z;
        
        //return vec3(x*target.x,y*target.y,z*target.z);
    }
    void operator+=(vec3 target) {
        x += target.x;
        y += target.y;
        z += target.z;
        
        //return vec3(x*target.x,y*target.y,z*target.z);
    }
    vec3 operator*(vec3 target) {
        return vec3(x*target.x,y*target.y,z*target.z);
    }

    vec3 operator-(vec3 target) {
        return vec3(x-target.x,y-target.y,z-target.z);
    }

    vec3 operator*(int target) {
        return vec3(x*target,y*target,z*target);
    }

    vec3 operator/(int target) {
        return vec3(x/target,y/target,z/target);
    }

    vec3 operator/(float target) {
        return vec3(x/target,y/target,z/target);
    }

    void operator/=(float target) {
        x /= target;
        y /= target;
        z /= target;
    }

    void operator*=(float target) {
        x *= target;
        y *= target;
        z *= target;
    }

    vec3 operator+(vec3 target) {
        return vec3(x+target.x,y+target.y,z+target.z);
    }

    vec3 difference(vec3 target) {
        return vec3(x-target.x,y-target.y,z-target.z);
    }

    float operator[](int index)
    {
        switch(index) {
            case 0:
                return x;
            case 1:
                return y;
            case 2:
                return z;
            default:
                return 0;
        }
        return 0;
    }

    float magnitude() {
        return sqrt(x*x + y*y + z*z);
    }

    vec3 normal() {
        //float l = sqrt(x*x + y*y + z*z);
        return vec3(x,y,z)/magnitude();
    }

    float consolidate() {
        return x+y+z;
    }

};
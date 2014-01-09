/*
 * ray_trace_oop.cpp
 *
 *  Created on: Nov 25, 2013
 *      Author: mneris
 */
#include <vector>
#include <string>

#include <float.h>
#include "engine/vector3.h"

struct Ray {
	Vector3 origin;
	Vector3 direction;
};

struct Intersection {
	int type;
	float distance;
};

class Shape {
protected:
	std::string name;
	Vector3 color;
	float specularity;
	float reflection;
public:
	Shape(const std::string& name) :
		name(name), color(Vector3::make(1, 1, 1)), specularity(0), reflection(0) {
	}

	virtual Intersection intersect(const Ray& ray) = 0;
	virtual Vector3 normalToPoint(const Vector3& point) = 0;

	void setMaterial(const Vector3& color, float specurality, float reflection) {
		this->color = color;
		this->specularity = specurality;
		this->reflection = reflection;
	}

	virtual ~Shape() {}

	Vector3 getColor() const { return color; }
	float getSpecularity() const { return specularity; }
	float getReflection() const { return reflection; }
};

class Sphere : public Shape {
	Vector3 position;
	float radius;
public:
	Sphere(const std::string& name, const Vector3& position, float radius) :
		Shape(name), position(position), radius(radius) {}

	virtual Intersection intersect(const Ray& ray) {
		float distance = FLT_MAX;
		int type = 0;

		Vector3 diff = position - ray.origin;
		float dot = Vector3::dot(diff, diff);
		float b = Vector3::dot(diff, ray.direction);

		float disc = b*b - dot + radius*radius;

		if(disc > 0) {
			float d = std::sqrt(disc);
			float root1 = b - d;
			float root2 = b + d;

			if(root2 > 0) {
				if(root1 > 0) {
					distance = root2;
					type = -1;
				} else {
					distance = root1;
					type = 1;
				}
			}
		}

		Intersection res = {
				type, distance
		};
		return res;
	}

	virtual Vector3 normalToPoint(const Vector3& point) {
		return (point - position).normalize();
	}
};

class Plane : public Shape {
	Vector3 normal;
	float d;
public:
	Plane(const std::string& name, const Vector3& v0, const Vector3& v1, const Vector3& v2) :
		Shape(name) {
		Vector3 v1v0 = v1 - v0;
		Vector3 v2v0 = v2 - v0;

		normal = Vector3::cross(v1v0, v2v0).normalize();
		d = -Vector3::dot(normal, v0);
	}

	virtual Intersection intersect(const Ray& ray) {
		float distance = FLT_MAX;
		int type = 0;

		float ndotr = Vector3::dot(normal, ray.direction);
		if(ndotr*ndotr > 0.001) {
			float ndoto = Vector3::dot(normal, ray.origin);
			distance = -(ndoto + d) / ndotr;

			if(distance > 0)
				type = 1;
		}

		Intersection res = {
				type, distance
		};
		return res;
	}

	virtual Vector3 normalToPoint(const Vector3& point) {
		return normal;
	}
};

class Light {
	Vector3 position;
	Vector3 color;
public:
	Light(const Vector3& position, const Vector3& color) :
		position(position), color(color) {
	}

	Vector3 getPosition() const { return position; }
	Vector3 getColor() const { return color; }
};

class Scene {
	std::vector<Shape*> objects;
	std::vector<Light*> lights;

public:
	Shape* addObject(Shape* obj) {
		objects.push_back(obj);
		return obj;
	}

	Light* addLight(Light* light) {
		lights.push_back(light);
		return light;
	}

	Vector3 traceRay(const Ray& ray, int depth = 0) {
		Shape* obj = 0;
		Vector3 color = Vector3::make(0, 0, 0);
		float distance = FLT_MAX;

		for(int i = 0; i < objects.size(); i++) {
			Shape* the_obj = objects[i];
			Intersection res = the_obj->intersect(ray);

			if(res.type) {
				if(!obj || res.distance < distance) {
					obj = the_obj;
					distance = res.distance;
				}
			}
		}

		if(!obj)
			return color;

		Vector3 pt = ray.origin + ray.direction*distance;
		Vector3 normal = obj->normalToPoint(pt);

		for(int i = 0; i < lights.size(); i++) {
			Light* the_light = lights[i];

			Vector3 ldir = the_light->getPosition() - pt;
			float len = ldir.length();

			if(len <= 0.0001f)
				len = 1.0f;

			ldir /= len;

			Ray sray = {pt + ldir/10000.0f, ldir};
//			bool shadow = false;
//
//			for(int j = 0; j < objects.size(); j++) {
//				Shape* test_obj = objects[j];
//				Intersection res = test_obj->intersect(sray);
//
//				if(res.type && res.distance < len) {
//					shadow = true;
//					break;
//				}
//			}
//
//			if(shadow)
//				continue;

			float cosine = Vector3::dot(normal, ldir);
			if(cosine < 0)
				cosine = 0;

			color += obj->getColor() * the_light->getColor() * cosine;

			if(obj->getSpecularity() > 0) {
				Vector3 vr = ldir - normal*cosine*2.0f;

				float cosSigma = Vector3::dot(ray.direction, vr);

				if(cosSigma > 0) {
					float s = obj->getSpecularity() * std::pow(cosSigma, 64.0f);
					color += the_light->getColor() * s;
				}
			}
		}
        
        if(obj->getReflection() > 0 && depth < 4) {
            float rdotn = Vector3::dot(ray.direction, normal);
            
            Ray rr = {
                pt, ray.direction - 2.0f * rdotn * normal
            };
            
            rr.origin += rr.direction / 10000.0f;
            
            Vector3 rcolor = traceRay(rr, depth + 1);
            
            color = Vector3::lerp(color, rcolor, obj->getReflection());
        }
        
		if(color.x > 1) color.x = 1;
		if(color.y > 1) color.y = 1;
		if(color.z > 1) color.z = 1;

		return color;
	}

	std::vector<unsigned char> traceScene(int screen_width, int screen_height) {
		std::vector<unsigned char> pixels(3*screen_width*screen_height);

		Vector3 pov = Vector3::make(0, 0, -4);
		Ray ray;

		ray.origin = pov;
		for(int x = 0; x < screen_width; x++) {
			for(int y = 0; y < screen_height; y++) {
				ray.direction = Vector3::make((x - screen_width*0.5f) / 250.0,
											(y - screen_height*0.5f) / 250.0,
											4.0).normalize();

				Vector3 color = traceRay(ray);
				int offset = x*3 + y*3*screen_width;

				pixels[offset+0] = (unsigned char)(255*color.x);
				pixels[offset+1] = (unsigned char)(255*color.y);
				pixels[offset+2] = (unsigned char)(255*color.z);
			}
		}

		return pixels;
	}
};

int main(int argc, char* argv[]) {
	Scene scene;
	int w = 1024;
	int h = 1024;

	Shape* sphere1 = scene.addObject(new Sphere("Sphere1", Vector3::make(0, 0, 0), 0.5f));
	sphere1->setMaterial(Vector3::make(1.0f, 0.3f, 0.3f), 0.5f, 0.09f);

	Shape* sphere2 = scene.addObject(new Sphere("Sphere2", Vector3::make(0, -1, 0), 0.5f));
	sphere2->setMaterial(Vector3::make(0.3f, 1.0f, 0.3f), 0.5f, 0.09f);

	Vector3 v0 = Vector3::make(0, 0.5, -2);
	Vector3 v1 = Vector3::make(0, 0.5, -4);
	Vector3 v2 = Vector3::make(2, 0.5, -2);
	Shape* plane = scene.addObject(new Plane("Ground", v0, v1, v2));
	plane->setMaterial(Vector3::make(0.3f, 0.3f, 0.3f), 0.0f, 0.0f);

    for (int i = 0; i < 800; i++) {
        Shape* shape = scene.addObject(new Sphere("sss", Vector3::make(1, -i, 0), 0.5f));
        shape->setMaterial(Vector3::make(1.0f, 0.3f, 0.3f), 0.5f, 0.09f);
    }
    
	scene.addLight(new Light(Vector3::make(+4, -1, -2), Vector3::make(1.0f, 0.5f, 0.5f)));
	scene.addLight(new Light(Vector3::make(-1, -1, -2), Vector3::make(0.3f, 0.3f, 0.3f)));
	scene.addLight(new Light(Vector3::make(+1, -6, -2), Vector3::make(0.4f, 0.4f, 0.4f)));

	std::vector<unsigned char> pixels = scene.traceScene(w, h);
	printf("P6 %d %d 255\n", w, h);
	fwrite(pixels.data(), sizeof(unsigned char), pixels.size(), stdout);

	return 0;
}

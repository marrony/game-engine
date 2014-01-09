/*
 * ray_trace_oop.cpp
 *
 *  Created on: Nov 25, 2013
 *      Author: mneris
 */
#include <vector>
#include <stack>
#include <string>

#include <float.h>
#include "engine/vector3.h"

struct Ray {
	Vector3 origin;
	Vector3 direction;
};

struct Intersection {
	float distance;
	Vector3 point;
	Vector3 normal;
	size_t material;
};

#define MAX_MATERIAL 1000
struct Materials {
    static int number_of_materials;
	static Vector3 color[MAX_MATERIAL];
	static float specularity[MAX_MATERIAL];
	static float reflection[MAX_MATERIAL];
};

int Materials::number_of_materials = 0;
Vector3 Materials::color[MAX_MATERIAL];
float Materials::specularity[MAX_MATERIAL];
float Materials::reflection[MAX_MATERIAL];

//struct Sphere {
//	Vector3 position;
//	float radius;
//	size_t material;
//};

//struct Plane {
//	Vector3 normal;
//	float d;
//	size_t material;
//};

#define MAX_LIGHTS 100

struct Lights {
    static int number_of_lights;
	static Vector3 position[MAX_LIGHTS];
	static Vector3 color[MAX_LIGHTS];
};

int Lights::number_of_lights = 0;
Vector3 Lights::position[];
Vector3 Lights::color[];

#define MAX_SPHERES 1000

struct Spheres {
    static int number_of_spheres;
	static Vector3 position[MAX_SPHERES];
	static float radius[MAX_SPHERES];
	static size_t material[MAX_SPHERES];
    
    
	static int intersect(const Vector3 position[], const float radius[], size_t material[], size_t spheres_count, const Ray& ray, Intersection* intersection) {
		int outputs = 0;
		float distance = FLT_MAX;
		size_t index = -1;

		for(size_t i = 0; i < spheres_count; i++) {
			Vector3 diff = position[i] - ray.origin;
			float dot = Vector3::dot(diff, diff);
			float b = Vector3::dot(diff, ray.direction);

			float disc = b*b - dot + radius[i]*radius[i];

			if(disc > 0) {
				float d = std::sqrt(disc);
				float root1 = b - d;
				float root2 = b + d;

				if(root2 > 0) {
					if(root1 > 0) {
						if(root2 < distance) {
							distance = root2;
							index = i;
							outputs++;
						}
					} else {
						if(root1 < distance) {
							distance = root1;
							index = i;
							outputs++;
						}
					}
				}
			}
		}

		if(index != -1) {
			intersection->distance = distance;
			intersection->point = ray.origin + ray.direction*distance;
			intersection->normal = (intersection->point - position[index]).normalize();
			intersection->material = material[index];
		}

		return outputs > 0;
	}
};

int Spheres::number_of_spheres = 0;
Vector3 Spheres::position[MAX_SPHERES];
float Spheres::radius[MAX_SPHERES];
size_t Spheres::material[MAX_SPHERES];

#define MAX_PLANES 10

struct Planes {
    static int number_of_planes;
	static Vector3 normal[MAX_PLANES];
	static float d[MAX_PLANES];
	static size_t material[MAX_PLANES];
    
	static int intersect(const Vector3 normal[], const float dx[], const size_t material[], size_t planes_count, const Ray& ray, Intersection* intersection) {
		int outputs = 0;
		float distance = FLT_MAX;
		size_t index = -1;

		for(size_t i = 0; i < planes_count; i++) {
			float ndotr = Vector3::dot(normal[i], ray.direction);

			if(ndotr*ndotr > 0.001) {
				float ndoto = Vector3::dot(normal[i], ray.origin);
				float d = -(ndoto + dx[i]) / ndotr;

				if(d > 0 && d < distance) {
					outputs++;
					distance = d;
					index = i;
				}
			}
		}

		if(index != -1) {
			intersection->distance = distance;
			intersection->point = ray.origin + ray.direction*distance;
			intersection->normal = normal[index];
			intersection->material = material[index];
		}

		return outputs > 0;
	}
};

int Planes::number_of_planes = 0;
Vector3 Planes::normal[MAX_PLANES];
float Planes::d[MAX_PLANES];
size_t Planes::material[MAX_PLANES];

class Scene {
//	std::vector<Material> materials;
//	std::vector<Sphere> spheres;
//	std::vector<Plane> planes;
//	std::vector<Light> lights;
public:
	void addSphere(const Vector3& position, float radius, const Vector3& color, float specurality, float reflection) {
        Spheres::position[Spheres::number_of_spheres] = position;
        Spheres::radius[Spheres::number_of_spheres] = radius;
        Spheres::material[Spheres::number_of_spheres] = Materials::number_of_materials;
        Spheres::number_of_spheres++;
        
        Materials::color[Materials::number_of_materials] = color;
        Materials::specularity[Materials::number_of_materials] = specurality;
        Materials::reflection[Materials::number_of_materials] = reflection;
        Materials::number_of_materials++;
	}

	void addPlane(const Vector3& v0, const Vector3& v1, const Vector3& v2, const Vector3& color, float specurality, float reflection) {
		Vector3 v1v0 = v1 - v0;
		Vector3 v2v0 = v2 - v0;
        
        Planes::normal[Planes::number_of_planes] = Vector3::cross(v1v0, v2v0).normalize();
        Planes::d[Planes::number_of_planes] = -Vector3::dot(Planes::normal[Planes::number_of_planes], v0);
        Planes::material[Planes::number_of_planes] = Materials::number_of_materials;
        Planes::number_of_planes++;

        Materials::color[Materials::number_of_materials] = color;
        Materials::specularity[Materials::number_of_materials] = specurality;
        Materials::reflection[Materials::number_of_materials] = reflection;
        Materials::number_of_materials++;
	}

	void addLight(const Vector3& position, const Vector3& color) {
        Lights::position[Lights::number_of_lights] = position;
        Lights::color[Lights::number_of_lights] = color;
        Lights::number_of_lights++;
	}

	int intersect(const Ray& ray, Intersection* output) {
		Intersection intersections[2];
        
		int count = 0;
		count += Spheres::intersect(Spheres::position, Spheres::radius, Spheres::material, Spheres::number_of_spheres, ray, intersections+count);
		count += Planes::intersect(Planes::normal, Planes::d, Planes::material, Planes::number_of_planes, ray, intersections+count);
        
		Intersection* found = intersections;
		for(int i = 1; i < count; i++) {
			if(intersections[i].distance < found->distance)
				found = intersections+i;
		}

		*output = *found;
		return count > 0;
	}
    
	Vector3 traceRay_no_rec(const Ray& ray) {
		Vector3 color = Vector3::make(0, 0, 0);
        
        float lastReflection = 1;
        Ray lastRay = ray;
        
        for (int j = 0; lastReflection > 0 && j < 3; j++) {
            Intersection intersection;
            
            if(intersect(lastRay, &intersection)) {
                Vector3 pt = intersection.point;
                Vector3 normal = intersection.normal;
                size_t material = intersection.material;
                
                Vector3 light_color = Vector3::make(0, 0, 0);
                
                for(int i = 0; i < Lights::number_of_lights; i++) {
                    Vector3 ldir = Lights::position[i] - pt;
                    float distance_from_light = ldir.length();

                    if(distance_from_light <= 0.0001f)
                        distance_from_light = 1.0f;

                    ldir /= distance_from_light;
                    
                    Ray sray = {pt + ldir/10000.0f, ldir};

#if 1
//                    if(intersect(sray, &intersection)) {
//                        if(intersection.distance < distance_from_light)
//                            continue;
//                    }
                    float in = 1;
#else
                    float in = !intersect(sray, &intersection);
                    in += (1-in) * (intersection.distance >= distance_from_light);
#endif
                    float cosine = std::max(Vector3::dot(normal, ldir), 0.0f);
                    
                    light_color += Materials::color[material] * Lights::color[i] * cosine * in;

                    Vector3 vr = ldir - normal * cosine * 2.0f;
                        
                    float cosSigma = std::max(Vector3::dot(lastRay.direction, vr), 0.0f);
                        
                    float s = Materials::specularity[material] * std::pow(cosSigma, 64.0f);
                    light_color += Lights::color[i] * s * in;
                }
                
//                if(light_color > 1) light_color.x = 1;
//                if(light_color > 1) light_color.y = 1;
//                if(light_color > 1) light_color.z = 1;
                
                color = Vector3::lerp(color, light_color, lastReflection);
                
                float rdotn = Vector3::dot(lastRay.direction, normal);
                Vector3 rdir = lastRay.direction - 2.0f * rdotn * normal;
                
                Ray rray = {pt + rdir/10000.0f, rdir};
                
                lastRay = rray;
                lastReflection = Materials::reflection[material];
            }
        }
        
        if(color.x > 1) color.x = 1;
        if(color.y > 1) color.y = 1;
        if(color.z > 1) color.z = 1;
        
		return color;
	}

	Vector3 traceRay(const Ray& ray, int depth = 0) {
		Vector3 color = Vector3::make(0, 0, 0);
        
		Intersection intersection;
        
		if(intersect(ray, &intersection)) {
			Vector3 pt = intersection.point;
			Vector3 normal = intersection.normal;
			size_t material = intersection.material;
            
			for(int i = 0; i < Lights::number_of_lights; i++) {
				Vector3 ldir = Lights::position[i] - pt;
				float distance_from_light = ldir.length();
                
				if(distance_from_light <= 0.0001f)
					distance_from_light = 1.0f;
                
				ldir /= distance_from_light;
                
				Ray sray = {pt + ldir/10000.0f, ldir};
                
//				if(intersect(sray, &intersection)) {
//					if(intersection.distance < distance_from_light)
//						continue;
//				}
                
				float cosine = std::max(Vector3::dot(normal, ldir), 0.0f);
                
				color += Materials::color[material] * Lights::color[i] * cosine;
                
				if(Materials::specularity[material] > 0) {
					Vector3 vr = ldir - normal * cosine * 2.0f;
                    
					float cosSigma = Vector3::dot(ray.direction, vr);
                    
					if(cosSigma > 0) {
						float s = Materials::specularity[material] * std::pow(cosSigma, 64.0f);
						color += Lights::color[i] * s;
					}
				}
                
//                float in = 1;
//                float cosine = std::max(Vector3::dot(normal, ldir), 0.0f);
//                
//                color += Materials::color[material] * Lights::color[i] * cosine * in;
//                
//                Vector3 vr = ldir - normal * cosine * 2.0f;
//                
//                float cosSigma = std::max(Vector3::dot(ray.direction, vr), 0.0f);
//                
//                float s = Materials::specularity[material] * std::pow(cosSigma, 64.0f);
//                color += Lights::color[i] * s * in;
			}
            
            if(Materials::reflection[material] > 0 && depth < 3) {
                float rdotn = Vector3::dot(ray.direction, normal);
                Vector3 rdir = ray.direction - 2.0f * rdotn * normal;
                
                Ray rr = {pt + rdir/10000.0f, rdir};
                
                Vector3 rcolor = traceRay(rr, depth + 1);
                color = Vector3::lerp(color, rcolor, Materials::reflection[material]);
            }
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

	scene.addSphere(
			Vector3::make(0, 0, 0),
			0.5f,
			Vector3::make(1.0f, 0.3f, 0.3f),
			0.5f,
			0.09f
	);

	scene.addSphere(
			Vector3::make(0, -1, 0),
			0.5f,
			Vector3::make(0.3f, 1.0f, 0.3f),
			0.5f,
			0.09f
	);

	scene.addPlane(
			Vector3::make(0, 0.5, -2),
			Vector3::make(0, 0.5, -4),
			Vector3::make(2, 0.5, -2),
			Vector3::make(0.3f, 0.3f, 0.3f),
			0.0f,
			0.0f
	);
    
    for (int i = 0; i < 800; i++) {
        scene.addSphere(
                        Vector3::make(1, -i, 0),
                        0.5f,
                        Vector3::make(1.0f, 0.3f, 0.3f),
                        0.5f,
                        0.09f
        );
    }

	scene.addLight(Vector3::make(+4, -1, -2), Vector3::make(1.0f, 0.5f, 0.5f));
	scene.addLight(Vector3::make(-1, -1, -2), Vector3::make(0.3f, 0.3f, 0.3f));
	scene.addLight(Vector3::make(+1, -6, -2), Vector3::make(0.4f, 0.4f, 0.4f));

	std::vector<unsigned char> pixels = scene.traceScene(w, h);
	printf("P6 %d %d 255\n", w, h);
	fwrite(pixels.data(), sizeof(unsigned char), pixels.size(), stdout);

	return 0;
}

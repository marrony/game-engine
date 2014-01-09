/*
 * ray_trace_oop.cpp
 *
 *  Created on: Nov 25, 2013
 *      Author: mneris
 */
#include <vector>
#include <string>

#include <float.h>
#include "vector3.h"

struct Ray {
	Vector3 origin;
	Vector3 direction;
};

struct Intersection {
	float distance;
	Vector3 point;
	Vector3 normal;
	int material;
};

struct Material {
	Vector3 color;
	float specularity;
	float reflection;
};

struct Sphere {
	Vector3 position;
	float radius;
	int material;
};

struct Plane {
	Vector3 normal;
	float d;
	int material;
};

struct Light {
	Vector3 position;
	Vector3 color;
};

struct Spheres {
	static int intersect(const Sphere sphere[], int spheres_count, const Ray& ray, Intersection intersections[]) {
		int outputs = 0;
		float distance = FLT_MAX;
		int index = -1;

		for(int i = 0; i < spheres_count; i++) {
			int type = 0;

			Vector3 diff = sphere[i].position - ray.origin;
			float dot = Vector3::dot(diff, diff);
			float b = Vector3::dot(diff, ray.direction);

			float disc = b*b - dot + sphere[i].radius*sphere[i].radius;

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

		if(index >= 0) {
			intersections->distance = distance;
			intersections->point = ray.origin + ray.direction*distance;
			intersections->normal = (intersections->point - sphere[index].position).normalize();
			intersections->material = sphere[index].material;
		}

		return outputs > 0;
	}
};

struct Planes {
	static int intersect(const Plane plane[], int planes_count, const Ray& ray, Intersection intersections[]) {
		int outputs = 0;
		float distance = FLT_MAX;
		int index = -1;

		for(int i = 0; i < planes_count; i++) {
			float ndotr = Vector3::dot(plane[i].normal, ray.direction);

			if(ndotr*ndotr > 0.001) {
				float ndoto = Vector3::dot(plane[i].normal, ray.origin);
				float d = -(ndoto + plane[i].d) / ndotr;

				if(d > 0 && d < distance) {
					outputs++;
					distance = d;
					index = i;
				}
			}
		}

		if(index >= 0) {
			intersections->distance = distance;
			intersections->point = ray.origin + ray.direction*distance;
			intersections->normal = plane[index].normal;
			intersections->material = plane[index].material;
		}

		return outputs > 0;
	}
};

class Scene {
	std::vector<Material> materials;
	std::vector<Sphere> spheres;
	std::vector<Plane> planes;
	std::vector<Light> lights;
public:
	void addSphere(const Vector3& position, float radius, const Vector3& color, float specurality, float reflection) {
		spheres.push_back(Sphere());
		spheres.back().position = position;
		spheres.back().radius = radius;
		spheres.back().material = materials.size();

		materials.push_back(Material());
		materials.back().color = color;
		materials.back().specularity = specurality;
		materials.back().reflection = reflection;
	}

	void addPlane(const Vector3& v0, const Vector3& v1, const Vector3& v2, const Vector3& color, float specurality, float reflection) {
		Vector3 v1v0 = v1 - v0;
		Vector3 v2v0 = v2 - v0;

		planes.push_back(Plane());
		planes.back().normal = Vector3::cross(v1v0, v2v0).normalize();
		planes.back().d = -Vector3::dot(planes.back().normal, v0);
		planes.back().material = materials.size();

		materials.push_back(Material());
		materials.back().color = color;
		materials.back().specularity = specurality;
		materials.back().reflection = reflection;
	}

	void addLight(const Vector3& position, const Vector3& color) {
		lights.push_back(Light());
		lights.back().position = position;
		lights.back().color = color;
	}

	int intersect(const Ray& ray, Intersection* output) {
		int count = 0;
		Intersection intersections[2];
		count += Planes::intersect(planes.data(), planes.size(), ray, intersections+count);
		count += Spheres::intersect(spheres.data(), spheres.size(), ray, intersections+count);

		Intersection* found = intersections;
		for(int i = 1; i < count; i++) {
			if(intersections[i].distance < found->distance)
				found = intersections+i;
		}

		*output = *found;
		return count > 0;
	}

	Vector3 traceRay(const Ray& ray, int depth = 0) {
		Vector3 color = Vector3::make(0, 0, 0);

		Intersection intersection;

		int count = intersect(ray, &intersection);

		if(count) {
			Vector3 pt = intersection.point;
			Vector3 normal = intersection.normal;
			Material* material = &materials[intersection.material];

			for(int i = 0; i < lights.size(); i++) {
				Light* the_light = &lights[i];

				Vector3 ldir = the_light->position - pt;
				float len = ldir.length();

				if(len <= 0.0001f)
					len = 1.0f;

				ldir /= len;

				Ray sray = {pt + ldir/10000.0f, ldir};

				if(intersect(sray, &intersection)) {
					if(intersection.distance < len)
						continue;
				}

				float cosine = Vector3::dot(normal, ldir);
				if(cosine < 0)
					cosine = 0;

				color += material->color * the_light->color * cosine;

				if(material->specularity > 0) {
					Vector3 vr = ldir - normal*cosine*2.0f;

					float cosSigma = Vector3::dot(ray.direction, vr);

					if(cosSigma > 0) {
						float s = material->specularity * std::pow(cosSigma, 64.0f);
						color += the_light->color * s;
					}
				}

				if(material->reflection > 0 && depth < 3) {
					float rdotn = Vector3::dot(ray.direction, normal);

					Ray rr = {
							pt, ray.direction - 2.0f * rdotn * normal
					};

					rr.origin += rr.direction / 10000.0f;

					Vector3 rcolor = traceRay(rr, depth + 1);

					color *= 1.0f - material->reflection;
					color += rcolor * material->reflection;
				}
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

	scene.addLight(Vector3::make(+4, -1, -2), Vector3::make(1.0f, 0.5f, 0.5f));
	scene.addLight(Vector3::make(-1, -1, -2), Vector3::make(0.3f, 0.3f, 0.3f));
	scene.addLight(Vector3::make(+1, -6, -2), Vector3::make(0.4f, 0.4f, 0.4f));

	std::vector<unsigned char> pixels = scene.traceScene(w, h);
	printf("P6 %d %d 255\n", w, h);
	fwrite(pixels.data(), sizeof(unsigned char), pixels.size(), stdout);

	return 0;
}

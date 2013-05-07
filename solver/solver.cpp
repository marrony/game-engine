/*
 * solver.cpp
 *
 *  Created on: 06/05/2013
 *      Author: marrony
 */
////////////////////////////////////////////////////////////////////////////
// Copyright (C) 2008 Insomniac Games
//
// This library is free software; you can redistribute it and/or
// modify it under the terms of the Insomniac Open License
// as published by Insomniac Games.
//
// This library is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even an implied warranty. See the
// Insomniac Open License for more details.
//
// You should have received a copy of the Insomniac Open License
// along with this code; if not, write to the Insomniac Games
// 2255 N. Ontario St Suite 550, Burbank, CA 91504, or email
// nocturnal@insomniacgames.com, or visit
// http://nocturnal.insomniacgames.com.
////////////////////////////////////////////////////////////////////////////
// Simple 3D Solver Example
// Eric Christensen
// Insomniac Games

#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <float.h>
#include <string.h>
#include <assert.h>

#include "vector3.h"

#define USE_VERBOSE

#define NUM_SOLVER_ITERATIONS 20
#define NUM_JACOBIANS         3
#define CONVERGENCE_ESTIMATE  1.0f

struct SimObject {
	Vector3 m_velocity;
	float m_invMass; //todo
};

struct Jacobian {
	Vector3 m_linear;
	float m_constraint_force;
	float m_limit_lower;
	float m_limit_upper;
	float m_mixing_coefficient;
	int m_object_index;
};

void Solve3D(Jacobian *j, int num_jacobians, SimObject *o, int num_objects);
void Solve3DVerbose(Jacobian *j, int num_jacobians, SimObject *o, int num_objects);

#ifndef USE_VERBOSE
#define SOLVE_3D(j, nj, o, no) Solve3D(j, nj, o, no)
#else
#define SOLVE_3D(j, nj, o, no) Solve3DVerbose(j, nj, o, no)
#endif

int main(void) {
	Jacobian jacobians[NUM_JACOBIANS];

	jacobians[0].m_linear = vector::make(0.0f, 1.0f, 0.0f);
	jacobians[1].m_linear = vector::make(0.0f, 0.0f, 1.0f);
	jacobians[2].m_linear = vector::make(1.0f, 0.0f, 0.0f);
	jacobians[0].m_constraint_force = 1.0f;
	jacobians[1].m_constraint_force = 10.0f;
	jacobians[2].m_constraint_force = 1.8f;
	jacobians[0].m_limit_lower = 0.0f;
	jacobians[1].m_limit_lower = 0.0f;
	jacobians[2].m_limit_lower = 0.0f;
	jacobians[0].m_limit_upper = FLT_MAX;
	jacobians[1].m_limit_upper = FLT_MAX;
	jacobians[2].m_limit_upper = FLT_MAX;
	jacobians[0].m_mixing_coefficient = 0.0001f;
	jacobians[1].m_mixing_coefficient = 0.0001f;
	jacobians[2].m_mixing_coefficient = 0.0001f;
	jacobians[0].m_object_index = 0;
	jacobians[1].m_object_index = 0;
	jacobians[2].m_object_index = 0;

	for (int x = 0; x < NUM_JACOBIANS; x++) {
		jacobians[x].m_linear = jacobians[x].m_linear.normalize();
	}

	SimObject sim;
	sim.m_velocity = vector::make(0.0f, -1.0f, 0.0f);
	sim.m_invMass = 1.0f;

	SOLVE_3D(jacobians, NUM_JACOBIANS, &sim, 1);
	return 0;
}

void Solve3D(Jacobian *j, int num_jacobians, SimObject *o, int num_objects) {
	// I r WindoZe pRgrammerer, I uses malloCs n Freez. ICE CREAM!
	float *b = (float *) malloc(sizeof(float) * num_jacobians); // right hand sides
	float *D = (float *) malloc(sizeof(float) * num_jacobians); // diagonal pre-scale
	float *D_ = (float *) malloc(sizeof(float) * num_jacobians); // diagonal post-scale
	float *lam = (float *) malloc(sizeof(float) * num_jacobians); // lambda
	Vector3 *jlin_ = (Vector3 *) malloc(sizeof(Vector3) * num_jacobians); // scaled jacobian
	Vector3 *a = (Vector3 *) malloc(sizeof(Vector3) * num_objects); // corrective acceleration

	memset(lam, 0, sizeof(float) * num_jacobians);
	memset(a, 0, sizeof(Vector3) * num_objects);

	float lam_; // temp lambda
	float d;    // delta

	// preconditioning
	for (int x = 0; x < num_jacobians; x++) {
		D[x] = CONVERGENCE_ESTIMATE / (vector::dot(j[x].m_linear, j[x].m_linear) + j[x].m_mixing_coefficient);
		b[x] = (j[x].m_constraint_force - vector::dot(o[j[x].m_object_index].m_velocity, j[x].m_linear)) * D[x];
		jlin_[x] = j[x].m_linear * D[x];
		D_[x] = D[x] * j[x].m_mixing_coefficient;
	}

	// solving
	for (int iter = 0; iter < NUM_SOLVER_ITERATIONS; iter++) {
		for (int j_index = 0; j_index < num_jacobians; j_index++) {
			d = (b[j_index] - lam[j_index] * D_[j_index]) - vector::dot(a[j[j_index].m_object_index], jlin_[j_index]);
			lam_ = lam[j_index] + d;

			if (lam_ < j[j_index].m_limit_lower) {
				d = j[j_index].m_limit_lower - lam[j_index];
				lam[j_index] = j[j_index].m_limit_lower;
			} else if (lam_ > j[j_index].m_limit_upper) {
				d = j[j_index].m_limit_upper - lam[j_index];
				lam[j_index] = j[j_index].m_limit_upper;
			} else {
				lam[j_index] = lam_;
			}

			a[j[j_index].m_object_index] += (j[j_index].m_linear * d);
		}
	}

	for (int obj_index = 0; obj_index < num_objects; obj_index++) {
		o[obj_index].m_velocity += a[obj_index];
	}

	free(a);
	free(jlin_);
	free(lam);
	free(D_);
	free(D);
	free(b);
}

void Solve3DVerbose(Jacobian *j, int num_jacobians, SimObject *o, int num_objects) {
	float *b = (float *) malloc(sizeof(float) * num_jacobians); // right hand sides
	float *D = (float *) malloc(sizeof(float) * num_jacobians); // diagonal pre-scale
	float *D_ = (float *) malloc(sizeof(float) * num_jacobians); // diagonal post-scale
	float *L = (float *) malloc(sizeof(float) * num_jacobians); // lambda
	Vector3 *Js = (Vector3 *) malloc(sizeof(Vector3) * num_jacobians); // scaled jacobian
	Vector3* JM_1 = (Vector3 *) malloc(sizeof(Vector3) * num_jacobians); // JM^-1
	Vector3 *velocity = (Vector3 *) malloc(sizeof(Vector3) * num_objects); // corrective acceleration

	memset(L, 0, sizeof(float) * num_jacobians);
	memset(velocity, 0, sizeof(Vector3) * num_objects);

	printf("Input:\n");

	for (int x = 0; x < num_jacobians; x++) {
		printf("\tJacobian (%d)\n", x);
		printf("\t\tlinear = (%f %f %f)\n", j[x].m_linear.x, j[x].m_linear.y, j[x].m_linear.z);
		printf("\t\tconstraint force = %f\n", j[x].m_constraint_force);
		printf("\t\tlim lower = %f\n", j[x].m_limit_lower);
		printf("\t\tlim upper = %f\n", j[x].m_limit_upper);
		printf("\t\tmixing coefficient = %f\n", j[x].m_mixing_coefficient);
	}

	for (int x = 0; x < num_objects; x++) {
		printf("\tObject (%d)\n", x);
		printf("\t\tVelocity = (%f %f %f)\n", o[x].m_velocity.x, o[x].m_velocity.y, o[x].m_velocity.z);
	}

	printf("Preconditioning:\n");

	// preconditioning
	for (int x = 0; x < num_jacobians; x++) {
		printf("\tJacobian (%d)\n", x);

		JM_1[x] = j[x].m_linear * o[j[x].m_object_index].m_invMass;

		float u = vector::dot(o[j[x].m_object_index].m_velocity, j[x].m_linear);
		float v = 0;

		b[x] = u + v;
		b[x] = j[x].m_constraint_force - b[x];

		D[x] = vector::dot(JM_1[x], j[x].m_linear);
		D[x] = D[x] + j[x].m_mixing_coefficient;
		D[x] = 1.0f / D[x];

		b[x] = b[x] * D[x];

		Js[x] = j[x].m_linear * D[x];

		D_[x] = D[x] * j[x].m_mixing_coefficient;

//		D[x] = 1.0f / (vector::dot(JM_1, j[x].m_linear) + j[x].m_mixing_coefficient);
//		b[x] = (j[x].m_constraint_force - u+v) * D[x];
//		jlin_[x] = j[x].m_linear * D[x];
//		D_[x] = D[x] * j[x].m_mixing_coefficient;
	}

	// solving
	printf("Solving:\n");
	for (int iter = 0; iter < NUM_SOLVER_ITERATIONS; iter++) {
		printf("\titeration (%d)\n", iter);

		for (int j_index = 0; j_index < num_jacobians; j_index++) {
			printf("\t\tjacobian (%d)\n", j_index);

			float delta = (b[j_index] - L[j_index] * D_[j_index]) - vector::dot(velocity[j[j_index].m_object_index], Js[j_index]);

			printf("\t\t\td = %f\n", delta);

			float L_test = L[j_index] + delta;

			printf("\t\t\tlam_ = %f\n", L_test);

			if (L_test < j[j_index].m_limit_lower) {
				printf("\t\t\tlim: hit lower limit of %f\n", j[j_index].m_limit_lower);
				delta = j[j_index].m_limit_lower - L[j_index];
				printf("\t\t\t\td = %f\n", delta);
				L[j_index] = j[j_index].m_limit_lower;
			} else if (L_test > j[j_index].m_limit_upper) {
				printf("\t\t\tlim: hit upper limit of %f\n", j[j_index].m_limit_upper);
				delta = j[j_index].m_limit_upper - L[j_index];
				printf("\t\t\t\td = %f\n", delta);
				L[j_index] = j[j_index].m_limit_upper;
			} else {
				printf("\t\t\tlim: within bounds\n");
				L[j_index] = L_test;
			}

			velocity[j[j_index].m_object_index] += (JM_1[j_index] * delta);

			printf("\t\t\taccel (object %d) = (%f %f %f)\n", j[j_index].m_object_index, velocity[j[j_index].m_object_index].x, velocity[j[j_index].m_object_index].y, velocity[j[j_index].m_object_index].z);
		}
	}

	for (int obj_index = 0; obj_index < num_objects; obj_index++) {
		o[obj_index].m_velocity += velocity[obj_index];
	}

	printf("Output:\n");
	for (int obj_index = 0; obj_index < num_objects; obj_index++) {
		printf("\tObject (%d)\n", obj_index);
		printf("\t\tVelocity = (%f %f %f)\n", o[obj_index].m_velocity.x, o[obj_index].m_velocity.y, o[obj_index].m_velocity.z);
	}

	free(velocity);
	free(Js);
	free(JM_1);
	free(L);
	free(D_);
	free(D);
	free(b);
}

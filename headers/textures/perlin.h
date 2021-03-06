#ifndef PERLINH
#define PERLINH

#include "../swap.h"

/* Deprecated
inline double trilinear_interp (double c[2][2][2], double u, double v, double w) {
	double accum = 0;
	for (int i = 0; i < 2; i++)
		for (int j = 0; j < 2; j++)
			for (int k = 0; k < 2; k++)
				accum += (i * u + (1 - i) * (1 - u)) * (j * v + (1 - j) * (1 - v)) *(k * w + (1 - k) * (1 - w)) * c[i][j][k];
	return accum;
}
*/

inline double perlin_interp(vec3 c[2][2][2], double u, double v, double w) {
	double uu = u * u * (3 - 2 * u);
	double vv = v * v * (3 - 2 * v);
	double ww = w * w * (3 - 2 * w);
	double accum = 0;
	for (int i = 0; i < 2; i++)
		for (int j = 0; j < 2; j++)
			for (int k = 0; k < 2; k++) {
				vec3 weight_v(u - i, v - j, w - k);
				accum += 	(i * uu + (1 - i) * (1 - uu)) *
					    	(j * vv + (1 - j) * (1 - vv)) *
							(k * ww + (1 - k) * (1 - ww)) *
							dot(c[i][j][k], weight_v);
			}
	return accum;
}

class perlin {
	public :
		double noise (const vec3& p) const {
			double u = p.x() - floor(p.x());
			double v = p.y() - floor(p.y());
			double w = p.z() - floor(p.z());
			
			int i = floor(p.x());
			int j = floor(p.y());
			int k = floor(p.z());

			vec3 c[2][2][2];

			for (int ii = 0; ii < 2; ii++)
				for (int jj = 0; jj < 2; jj++)
					for (int kk = 0; kk < 2; kk++)
						c[ii][jj][kk] = ranvec[perm_x[(i + ii) & 255] ^ perm_y[(j + jj) & 255] ^ perm_z[(k + kk) & 255]];
				
			return perlin_interp(c, u, v, w);
		}

		double turb (const vec3& p, int depth = 7) const {
			double accum = 0;
			vec3 tmp_p = p;
			double weight = 1.0;
			for (int i = 0; i < depth; i++) {
				accum += weight * noise(tmp_p);
				weight *= 0.5;
				tmp_p *= 2;
			}
			return fabs(accum);
		}

		static vec3* ranvec;
		static int*	perm_x;
		static int*	perm_y;
		static int* perm_z;
};

static vec3* perlin_generate () {
	vec3* p = new vec3[256]; 
	for (int i = 0; i < 256; i++) p[i] = unit_vector(vec3(-1 + 2 * drand48(), -1 + 2 * drand48(), -1 + 2 * drand48()));
	return p;
}

void permute (int *p, int n) {
	for (int i = n - 1; i > 0; i--) {
		int target = int(drand48() * (i + 1));
		swap(p[i], p[target]);
	}
}

static int* perlin_generate_perm () {
	int* p = new int[256];
	for (int i = 0; i < 256; i++) p[i] = i;
	permute(p, 256);
	return p;
}

vec3* perlin::ranvec = perlin_generate();

int* perlin::perm_x = perlin_generate_perm();
int* perlin::perm_y = perlin_generate_perm();
int* perlin::perm_z = perlin_generate_perm();

#endif

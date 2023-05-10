#include "system.h"
#include <stdlib.h>

using namespace Eigen;
using namespace std;

System::System(){}

void System::init() {
    /// Initialize water grid
    initWaterGrid();

    /// Initialize ink particles
<<<<<<< Updated upstream
    initParticles();
=======
    if (PART_FILE != "") {
        initFromFile();
    }
    else {
        // Initial particles in a square shape
//        initParticles();

        // Emit particles in a hemisphere shape
        for (int i = 1; i <= 10; i++) {
            emitParticleHemisphere(0.1 * i);
        }
    }
>>>>>>> Stashed changes
}


/**
 * Initializes a (WATERGRID_X x WATERGRID_Y x WATERGRID_Z) waterGrid
 *               ________________+ (WATERGRID_X+1, WATERGRID_Y+1, WATERGRID_Z+1)
 *             /               / |
 *           /               /   | <---- (WATERGRID_Y)
 *          /--------------/     |
 *          |              |    /
 *          |              |  / <----- (WATERGRID_Z)
 * (0,0,0) +|______________|/
 *           (WATERGRID_X)
 */
void System::initWaterGrid() {
    for (int i = 0; i < WATERGRID_X; i++) {
        for (int j = 0; j < WATERGRID_Y; j++) {
            for (int k = 0; k < WATERGRID_Z; k++) {
                /// Create the cell
                Cell cell {
                    .oldVelocity  = Vector3d(0, 0, 0), // CUSTOMIZABLE
                    .currVelocity = Vector3d(0, 0, 0), // CUSTOMIZABLE
                    .forceApplied = false,
                    .neighbors = getGridNeighbors(i, j, k)
                };

                /// Insert into m_waterGrid
                m_waterGrid.insert({Vector3i{i, j, k}, cell});
            }
        }
    }
    double product = WATERGRID_X*WATERGRID_Z*WATERGRID_Y;
    double size = m_waterGrid.size();
    assert(size == WATERGRID_X*WATERGRID_Z*WATERGRID_Y);
    initPressureA();
}

int nonZeroRand() {
    int ret = rand();
    return ret;
//    return (ret != 0) ? ret : 1;
}

void System::emitParticleHemisphere(float radius) {
    // emit outwards from a hemisphere
    float fraction = M_PI / 12.f;
    float twoPi = M_PI * 2.f;
    Vector3f surfaceNormal = Vector3f(0, -1, 0); // sampling point on middle of ceiling, pointing downwards
    Vector3f up = Vector3f(0, 1, 0);
    Vector3f axis = (surfaceNormal + up).normalized();
    AngleAxisf rot(M_PI, axis);
    for (float i = 0; i < twoPi; i += fraction) {
        for (float j = 0; j < M_PI; j += fraction) {
            float x = radius*sin(j) * sin(i);
            float y = radius*cos(j);
            float z = radius*sin(j) * cos(i);

            // starting velocity of particle
            Vector3f vel = rot * Vector3f(x, y, z);

            // starting position of particle
            Vector3f pos = Vector3f(WATERGRID_X/2.f, WATERGRID_Y-0.1f, WATERGRID_Z/2.f) + radius * vel;

            Particle p {
                .position = pos,
                .velocity = vel,
                .opacity = 1.f,
                .lifeTime = 5.f,
            };

            this->m_ink.push_back(p);
        }
    }

}

/// Returns a random position within the specified ranges
Vector3d getRandPosWithinRange(double minX, double maxX,
                               double minY, double maxY,
                               double minZ, double maxZ) {
    double x = minX + ((maxX - minX) / (nonZeroRand() % 100 + 1));
    assert(!isinf(x) || !isnan(x));
    double y = minY + ((maxY - minY) / (nonZeroRand() % 100 + 1));
    assert(!isinf(y) || !isnan(y));
    double z = minZ + ((maxZ - minZ) / (nonZeroRand() % 100 + 1));
    assert(!isinf(z) || !isnan(z));

    return Vector3d(x, y, z);
}

/// Initializes INIT_NUM_PARTICLES Particle structs
void System::initParticles() {
    m_ink.reserve(INIT_NUM_PARTICLES);
    for (int i = 0; i < INIT_NUM_PARTICLES; i++) {
        /// Create the particle
        Particle particle {
            .position = getRandPosWithinRange(WATERGRID_X/2.f-0.2, WATERGRID_X/2.f+0.2,
                                              WATERGRID_Y - 0.1, WATERGRID_Y - 0.1,
                                              WATERGRID_Z/2.f-0.2, WATERGRID_Z/2.f+0.2), // CUSTOMIZABLE
            .velocity = Vector3d{0, 0, 0}, // CUSTOMIZABLE
            .opacity  = 1.f,
            .lifeTime = 5.f // CUSTOMIZABLE
        };

        /// Insert into m_ink
        m_ink.push_back(particle);
    }
    assert(m_ink.size() == INIT_NUM_PARTICLES);
}

/************************** GETTERS ************************************/
const std::vector<Particle>& System::getInkParticles() {
    return m_ink;
}



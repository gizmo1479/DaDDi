#include "system.h"
#include <stdlib.h>

using namespace Eigen;
using namespace std;

System::System(){}

void System::init() {
    /// Initialize water grid
    initWaterGrid();

    /// Initialize ink particles
    initParticles();
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
                    .oldVelocity  = Vector3f(0, 0, 0), // CUSTOMIZABLE
                    .currVelocity = Vector3f(0, 0, 0), // CUSTOMIZABLE
                };

                /// Insert into m_waterGrid
                m_waterGrid.insert({Vector3i{i, j, k}, cell});
            }
        }
    }
    float product = WATERGRID_X*WATERGRID_Z*WATERGRID_Y;
    float size = m_waterGrid.size();
    assert(m_waterGrid.size() == WATERGRID_X*WATERGRID_Z*WATERGRID_Y);
    initPressureA();
}

/// Returns a random position within the specified ranges
Vector3f getRandPosWithinRange(float minX, float maxX,
                               float minY, float maxY,
                               float minZ, float maxZ) {
    return Vector3f{
        minX + ((maxX - minX) / (rand() % 1000)),
        minY + ((maxY - minY) / (rand() % 1000)),
        minZ + ((maxZ - minZ) / (rand() % 1000)),
    };
}

/// Initializes INIT_NUM_PARTICLES Particle structs
void System::initParticles() {
    m_ink.reserve(INIT_NUM_PARTICLES);
    for (int i = 0; i < INIT_NUM_PARTICLES; i++) {
        /// Create the particle
        Particle particle {
            .position = getRandPosWithinRange(WATERGRID_X/4.f, WATERGRID_X*3/4.f, WATERGRID_Y - 0.001, WATERGRID_Y - 0.001, WATERGRID_Z/4.f, WATERGRID_Z*3/4.f), // CUSTOMIZABLE
            .velocity = Vector3f{0, 0, 0}, // CUSTOMIZABLE
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

/************************** PRINTING UTILS *****************************/

/// print a cell
ostream& operator<<(ostream& strm, const Cell& obj) {
    strm << "\tcurrent velocity: (" << obj.currVelocity.x() << ", ";
    strm << obj.currVelocity.y() << ", " << obj.currVelocity.z() << ")\n";
    return strm;
}

/// print a particle
ostream& operator<<(ostream& strm, const Particle& obj) {
    strm << "Particle: \n";
    strm << "\tpos: (" << obj.position.x() << ", ";
    strm << obj.position.y() << ", " << obj.position.z() << ")\n";
    strm << "\tvelocity: (" << obj.velocity.x() << ", ";
    strm << obj.velocity.y() << ", " << obj.velocity.z() << ")\n";
    strm << "\topacity: " << obj.opacity;
    strm << "\tlifetime: " << obj.lifeTime;
    return strm;
}

/// print the whole system
ostream& operator<<(ostream& strm, const System& obj) {
    strm << "********* PRINTING SYSTEM ***********\n";
    strm << "********* PRINTING CELLS ***********\n";
    for (auto& [k, v] : obj.m_waterGrid) {
        strm << "Cell: \n";
        strm << "\tpos in hashmap: (" << k.x() << ", ";
        strm << k.x() << ", " << k.z() << ")\n";
        strm << v << endl;
    }

    strm << "********* PRINTING PARTICLES ***********\n";
    for (auto& el : obj.m_ink) {
        strm << el << endl;
    }
    return strm;
}

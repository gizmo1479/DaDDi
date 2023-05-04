#include "inkSim.h"
#include <fstream>
#include <chrono>

InkSim::InkSim(std::string writeDirectory) {
    this->ink_system.init();
    this->ink_system.checkNanAndInf();
    this->writeDirectory = writeDirectory;
}

void InkSim::simulate(const float renderTimestep, int totalTimesteps) {
    /// For recording time
    auto startTS = std::chrono::system_clock::now();
    auto startTime = std::chrono::system_clock::to_time_t(startTS);
    std::cout << "\e[32mSimulation started at: \e[m" << std::ctime(&startTime) << std::endl;

    int timestepCounter = 1;
    int currFrame = 1;
    float timeSinceLastRender = 0;
    while (timestepCounter <= totalTimesteps) {
        float timeStep = this->ink_system.solve(renderTimestep - timeSinceLastRender);
        std::cout << "TIMESTEP INK: " << timeStep << std::endl;
        std::cout << "NUM TIMESTEPS: " << timestepCounter << std::endl;
        timeSinceLastRender += timeStep;
        if (timeSinceLastRender>=renderTimestep) {
            writeToFile(currFrame);
            currFrame+=1;
            timeSinceLastRender = 0;
        }
        timestepCounter += 1;
    }

    /// For recording time
    auto endTS = std::chrono::system_clock::now();
    std::chrono::duration<double> elapsed_seconds = endTS-startTS;
    int seconds = elapsed_seconds.count();
    std::cout << "\e[32mSimulation & Writing took " << seconds / 60 << " min " << seconds % 60 << " sec\e[m" << std::endl;
}


void InkSim::writeToFile(int frameNum) {
    std::string filename = this->writeDirectory + "/" + std::to_string(frameNum) + ".ply";
    std::ofstream myfile;
    std::cout << "writing to: " + filename << std::endl;
    // Get ink particles
    const std::vector<Particle> inkParticles = this->ink_system.getInkParticles();
    std::string numVertices = std::to_string(inkParticles.size());
    std::cout << "recording " + numVertices + " particles"<< std::endl;
    myfile.open(filename); // based off working directory not current file
    // setup header
    myfile << "ply\n";
    myfile << "format ascii 1.0\n";
    myfile << "element vertex " + numVertices + "\n";
    myfile << "property float x\n";
    myfile << "property float y\n";
    myfile << "property float z\n";
    myfile << "end_header\n";
    // input data
    for(int i = 0; i<inkParticles.size(); i++) {
        Eigen::Vector3d currParticle = inkParticles[i].position;
        std::string position = std::to_string(currParticle.x()) + " " + std::to_string(currParticle.y()) + " " + std::to_string(currParticle.z());
        myfile << position + "\n";
    }
    myfile.close();
}


void InkSim::writeToFile() {
    // TODO: Write to actual file with actualy data
    std::string filename = this->writeDirectory + "/example.ply";
    std::ofstream myfile;
    std::cout << "writing to: " + filename << std::endl;
    myfile.open(filename); // based off working directory not current file
    // setup header
    myfile << "ply\n";
    myfile << "format ascii 1.0\n";
    myfile << "element vertex 3\n";
    myfile << "property float x\n";
    myfile << "property float y\n";
    myfile << "property float z\n";
    myfile << "end_header\n";
    // input data
    myfile << "1 0 0\n";
    myfile << "0 1 0\n";
    myfile << "0 0 1\n";
    myfile.close();
}

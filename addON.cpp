#include <iostream>
#include <cmath>
#include <fstream>
#include <sstream>
#include <vector>

using namespace std;

// menentukan nilai radian
const double PI = 3.14159265359;
#define rad PI / 180.0

// menyimpan nilai dari file input
struct InputData {
    string id;
    double Vx;
    double Vy;
    double omega;
};

class OmniWheelRobot {
private:
    const double R;   // jari-jari roda
    const double Lx;  // jarak horizontal antar roda
    const double Ly;  // jarak vertikal antar roda
    const double angles[4] = {45, 135, 225, 315}; // sudut untuk tiap roda (4 roda)
    double angleRad[4];
    double wheelSpeeds[4];

    // fungsi untuk melakukan penghitungan kecepatan
    void calculateWheelSpeeds(double Vx, double Vy, double omega) {
        for (int i = 0; i < 4; i++) {
            double angleRad = angles[i] * rad; // ubah ke radian untuk menentukan nilai sin & cos
            double cosAngle = cos(angleRad);
            double sinAngle = sin(angleRad);
            wheelSpeeds[i] = (Vx * cosAngle + Vy * sinAngle + (Lx + Ly) * omega) / R;
        }
    }

public:
    OmniWheelRobot(double radius, double lx, double ly) : R(radius), Lx(lx), Ly(ly) {}

    // memanggil fungsi untuk melakukan penghitungan
    void setVelocities(double Vx, double Vy, double omega) {
        calculateWheelSpeeds(Vx, Vy, omega);
    }
    // output
    void printWheelSpeeds() const {
        cout << "Wheel Speeds:" << endl;
        for(int i = 0; i < 4; i++) {
            cout << "Wheel " << i + 1 << ": " << wheelSpeeds[i] << " m/s" << endl;
        }
    }
    // menyimpan output
    void saveWheelSpeedsToFile(const string& filename) const {
        ofstream outFile(filename);
        if (outFile.is_open()) {
            for (int i = 0; i < 4; i++) {
                outFile << "Wheel " << i + 1 << ": " << wheelSpeeds[i] << " m/s" << endl;
            }
            outFile.close();
        } else {
            cerr << "Error opening file for writing." << endl;
        }
    }
    // membuka file input
    void loadVelocitiesFromFile(const string& filename, double& Vx, double& Vy, double& omega) {
        ifstream inFile(filename);
        if (inFile.is_open()) {
            inFile >> Vx >> Vy >> omega;
            inFile.close();
        } else {
            cerr << "Error opening file for reading." << endl;
        }
    }
};

// fungsi untuk membuka file dan menyimpan nilai input
vector<InputData> loadDataFromFile(const string& filename) {
    ifstream inFile(filename);
    vector<InputData> data;
    string line;
    while (getline(inFile, line)) {
        if (line[0] == '#') continue;  // Skip header
        stringstream ss(line);
        string id, temp;
        double Vx, Vy, omega;
        getline(ss, id, ';');
        ss >> Vx;
        getline(ss, temp, ';');  
        ss >> Vy;
        getline(ss, temp, ';');  
        ss >> omega;
        getline(ss, temp, ';');  
        data.push_back({id, Vx, Vy, omega});
    }
    return data;
}

int main() {
    double Vx, Vy, omega;
    string filename;

    // perlu input
    cout << "Enter the input file name (ex: [file name].txt): ";
    cin >> filename;
    
    OmniWheelRobot robot(1.0, 0.5, 0.5); // nilai jari-jari roda dan jarak antar roda

    // memanggil fungsi untuk membuka file
    vector<InputData> inputData = loadDataFromFile(filename);

    for (const auto& data : inputData) {
        cout << "ID: " << data.id << ", Vx: " << data.Vx
             << ", Vy: " << data.Vy << ", Omega: " << data.omega << endl;

        // menggunakan nilai yang didapat sebagai input
        robot.setVelocities(data.Vx, data.Vy, data.omega);
        robot.printWheelSpeeds();

        // menyimpan hasil perhitungan ke dalam sebuah file untuk masing-masing input
        robot.saveWheelSpeedsToFile(data.id + "_wheel_speeds.txt");
    }

    return 0;

}

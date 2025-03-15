#include <fstream>
#include <iostream>
#include <string>
#include <sstream>

// Question 3: This is an extension task that requires you to decode sensor data
// from a CAN log file. CAN (Controller Area Network) is a communication
// standard used in automotive applications (including Redback cars) to allow
// communication between sensors and controllers.
//
// Your Task: Using the definition in the Sensors.dbc file, extract the
// "WheelSpeedRR" values from the candump.log file. Parse these values correctly
// and store them in an output.txt file with the following format:
// (<UNIX_TIME>): <DECODED_VALUE>
// eg:
// (1705638753.913408): 1234.5
// (1705638754.915609): 6789.0
// ...
// The above values are not real numbers; they are only there to show the
// expected data output format. You do not need to use any external libraries.
// Use the resources below to understand how to extract sensor data. Hint: Think
// about manual bit masking and shifting, data types required, what formats are
// used to represent values, etc. Resources:
// https://www.csselectronics.com/pages/can-bus-simple-intro-tutorial
// https://www.csselectronics.com/pages/can-dbc-file-database-intro
//

std::vector<std::string> toVec(std::string s);

int main() {
    std::ifstream in("candump.log");
    std::ofstream out("output.txt");

    /* Get the 'speeds' substring after the '#' */
    auto get_magnitude = [](std::string s) -> std::string { return s.substr(s.find('#') + 1); };
    /* Get the can_id substring before the '#' */
    auto get_id = [](std::string s) -> std::string { return s.substr(0, s.find('#')); };

    /* Only take the rr_speed value; i.e., 32nd -> 47th bit*/
    auto get_rr_speed = [](uint64_t x) -> uint16_t { return (x >> 32 & 0xFFFF); };
    auto print_speed = [&get_magnitude, &get_rr_speed, &out](std::string s) {
        auto val_str = get_magnitude(s);
        auto val_num = std::stoull(val_str, nullptr, 16);
        auto val_num_rr = get_rr_speed(val_num);
        out << val_num_rr * (0.1);
    };

    std::string s;
    std::vector<std::string> s_prime;
    while (getline(in, s)) {
        s_prime = toVec(s);
        try {
            std::string data = s_prime[2];
            std::string unix = s_prime[0];
            /* 0x705 ≡ 1797 decimal (can_id of frame)*/
            if (get_id(data) == "705") {
                out << unix << ": ";
                print_speed(data);
                out << std::endl;
            }
            /* Catching all exceptions is a terrible practice; but in this case this is here purely to tell
             * the compiler we can catch it *if* something happened. */
        } catch (std::exception) {};
    }

}

/* Splitting over ' ' makes it easy to parse the values we care about in candump.log */
std::vector<std::string> toVec(std::string s) {
    std::string t;
    std::vector<std::string> v;
    std::stringstream s_prime(s);

    while (std::getline(s_prime, s, ' ')){
        v.push_back(s);
    }
     return v;
}

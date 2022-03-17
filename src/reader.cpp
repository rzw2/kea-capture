#include <iostream>
#include <map>

#include <chronoptics/tof/kea_camera.hpp>
#include <chronoptics/tof/kea.hpp>
#include <chronoptics/tof/user_config.hpp>
#include <chronoptics/tof/camera_config.hpp>
#include <chronoptics/tof/csf_reader.hpp>

#include "cxxopts.hpp"

namespace tof = chronoptics::tof;

int main(int argc, char **argv)
{
    cxxopts::Options options("Kea on-camera CSF file reader",
                             "Display the CSF capture contents");

    // An example of using the reader to count the number of frames and which types
    std::string csf_file = "capture.csf";

    options.add_options()("h, help", "Help")("csf", "The CSF file to read", cxxopts::value<std::string>(csf_file)->default_value("capture.csf"));

    std::map<int, std::string> frame_map{{0, "RAW_COMMON"}, {1, "COMMON"}, {2, "RAW"}, {3, "REAL"}, {4, "IMAGINARY"}, {5, "PHASE"}, {6, "AMPLITUDE"}, {7, "RADIAL"}, {8, "INTENSITY"}, {9, "X"}, {10, "Y"}, {11, "Z"}, {12, "XYZ"}, {13, "XYZ_AMP"}, {14, "XYZ_BGR"}, {15, "XYZ_BGR_I"}, {18, "BGR"}, {19, "YUV"}, {20, "RAW_AB"}, {21, "MJPEG"}, {22, "BGR_PROJECTED"}};

    try
    {
        tof::CsfReader reader(csf_file);
        size_t nframes = reader.number_of_frames();
        std::cout << "Capture " << csf_file << " has " << nframes << " frames in total" << std::endl;
        auto streams = reader.streams();
        size_t nstreams = streams.size();
        std::cout << "Capture has " << nstreams << " streams in the file" << std::endl;
        size_t n = 0;
        for (auto &stream : streams)
        {
            std::cout << "Stream: " << n << std::endl;
            n++;
            std::cout << "FrameType: " << frame_map[static_cast<int>(stream.frame_type())] << std::endl;
        }
    }
    catch (std::exception &e)
    {
        std::cout << e.what() << std::endl;
        return -1;
    }
    return 0;
}
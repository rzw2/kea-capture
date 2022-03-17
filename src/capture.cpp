
#include <iostream>

#include <chronoptics/tof/kea_camera.hpp>
#include <chronoptics/tof/kea.hpp>
#include <chronoptics/tof/user_config.hpp>
#include <chronoptics/tof/camera_config.hpp>
#include <chronoptics/tof/csf_writer.hpp>

#include "cxxopts.hpp"

namespace tof = chronoptics::tof;

int main(int argc, char **argv)
{
    // Example CSF Capture that runs on the embedded camera.
    cxxopts::Options options("Kea on-camera capture",
                             "Capture the depth camera output to disk");

    std::string output;
    size_t warmups = 40;
    size_t nframes = 10;
    float fps = 20;
    float dmax = 10;
    bool bgr = false;

    // Example of how to capture a CSF file and write to disk on the camera.
    options.add_options()("h, help", "Help")("bgr", "Display the colour image", cxxopts::value<bool>(bgr)->default_value("false"))("dmax", "Maximum distance", cxxopts::value<float>(dmax)->default_value("15.0"))("fps", "Depth frames per second", cxxopts::value<float>(fps)->default_value("20.0"))("output", "The output csf file name", cxxopts::value<std::string>(output)->default_value("capture.csf"))("nframes", "The number of frames to capture", cxxopts::value<size_t>(nframes)->default_value("10"))("warmup", "The number of warmup frames", cxxopts::value<size_t>(warmups)->default_value("40"));
    auto result = options.parse(argc, argv);

    if (result.count("h") || result.count("help"))
    {
        std::cout << options.help() << std::endl;
        return 0;
    }

    std::vector<tof::FrameType> types = {tof::FrameType::RADIAL, tof::FrameType::INTENSITY};
    if (bgr)
    {
        types.push_back(tof::FrameType::BGR);
    }

    try
    {

        tof::EmbeddedKeaCamera cam(tof::ProcessingConfig{});

        // Configure the camera
        tof::UserConfig user{};
        user.set_fps(fps);
        user.set_integration_time(tof::IntegrationTime::MEDIUM);
        user.set_max_distance(dmax);
        user.set_environment(tof::ImagingEnvironment::INSIDE);
        user.set_strategy(tof::Strategy::BALANCED);

        auto config = user.to_camera_config(cam);
        for (size_t n = 0; n < config.frame_size(); n++)
        {
            // Flip the output to be displayed correctly with 1/4" Mounting hole
            config.set_flip(n, true);
        }

        // Generate the ToF ISP filtering settings
        auto proc = config.default_processing();
        proc.set_intensity_scale(5.0f);

        // Set the camera settings
        cam.set_camera_config(config);
        cam.set_process_config(proc);

        tof::CsfWriter writer = tof::create_csf_writer_camera(output, cam);

        size_t nstreams = tof::select_streams(cam, types);

        // Do the warmup frames.
        cam.start();

        std::cout << "Warming Up" << std::endl;
        for (size_t n = 0; n < warmups; n++)
        {
            auto frames = cam.get_frames();
        }

        std::cout << "Capturing Frames" << std::endl;

        for (size_t n = 0; n < nframes; n++)
        {
            auto frames = cam.get_frames();
            for (auto &frame : frames)
            {
                writer.write_frame(frame);
            }
        }

        cam.stop();
    }
    catch (std::exception &e)
    {
        std::cout << e.what() << std::endl;
        return -1;
    }

    return 0;
}
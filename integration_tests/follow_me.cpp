#include <iostream>
#include <cmath>
#include <thread>
#include <chrono>
#include "integration_test_helper.h"
#include "global_include.h"
#include "dronecore.h"

using namespace dronecore;
using namespace std::chrono;
using namespace std::this_thread;
using namespace std::placeholders;

void print(const FollowMe::Config &config);
void fill_follow_target_info(FollowMe::FollowTargetInfo &info);

TEST_F(SitlTest, FollowMe)
{
    DroneCore dc;

    DroneCore::ConnectionResult ret = dc.add_udp_connection();
    ASSERT_EQ(DroneCore::ConnectionResult::SUCCESS, ret);

    // Wait for device to connect via heartbeat.
    sleep_for(seconds(2));
    Device &device = dc.device();

    while (!device.telemetry().health_all_ok()) {
        std::cout << "waiting for device to be ready" << std::endl;
        sleep_for(seconds(1));
    }

    Action::Result action_ret = device.action().arm();
    ASSERT_EQ(Action::Result::SUCCESS, action_ret);

    device.telemetry().flight_mode_async(
    std::bind([](Telemetry::FlightMode flight_mode) {
        std::cout << "FlightMode: " << Telemetry::flight_mode_str(flight_mode)
                  << std::endl;

    }, std::placeholders::_1));

    action_ret = device.action().takeoff();
    ASSERT_EQ(Action::Result::SUCCESS, action_ret);

    sleep_for(seconds(5));

    auto curr_config = device.followme().get_config();
    print(curr_config);

    // Feed Follow target info
    device.followme().register_follow_target_info_callback(std::bind(fill_follow_target_info, _1));

    // start following with default configuration
    FollowMe::Result followme_result = device.followme().start();
    ASSERT_EQ(FollowMe::Result::SUCCESS, followme_result);

    // Let the device follow you (GCS) for half a min, say.
    sleep_for(seconds(30));

    // stop following
    followme_result = device.followme().stop();
    ASSERT_EQ(FollowMe::Result::SUCCESS, followme_result);
    sleep_for(seconds(2)); // to watch flight mode change from "FollowMe" to default "HOLD"

    action_ret = device.action().land();
    ASSERT_EQ(Action::Result::SUCCESS, action_ret);
    sleep_for(seconds(2)); // let the device land
}

TEST_F(SitlTest, FollowMeWithConfig)
{
    DroneCore dc;

    DroneCore::ConnectionResult ret = dc.add_udp_connection();
    ASSERT_EQ(DroneCore::ConnectionResult::SUCCESS, ret);

    // Wait for device to connect via heartbeat.
    sleep_for(seconds(2));
    Device &device = dc.device();

    while (!device.telemetry().health_all_ok()) {
        std::cout << "waiting for device to be ready" << std::endl;
        sleep_for(seconds(1));
    }

    Action::Result action_ret = device.action().arm();
    ASSERT_EQ(Action::Result::SUCCESS, action_ret);

    device.telemetry().flight_mode_async(
    std::bind([](Telemetry::FlightMode flight_mode) {
        std::cout << "FlightMode: " << Telemetry::flight_mode_str(flight_mode)
                  << std::endl;

    }, std::placeholders::_1));

    action_ret = device.action().takeoff();
    ASSERT_EQ(Action::Result::SUCCESS, action_ret);

    sleep_for(seconds(5));

    // configure follow me behaviour
    FollowMe::Config config;
    config.min_height_m = 30.f; // increase min height
    config.follow_dist_m = 20.f; // set distance b/w device and GCS during FollowMe mode
    config.responsiveness = 0.2f; // set to higher responsiveness
    config.follow_dir = FollowMe::Config::FollowDirection::FRONT; // Device follows you from FRONT side

    // Apply configuration
    bool configured = device.followme().set_config(config);
    ASSERT_EQ(true, configured);

    sleep_for(seconds(2)); // untill config is applied

    // Verify your configuration
    auto curr_config = device.followme().get_config();
    print(curr_config);

    // Register callback to feed follow target info
    device.followme().register_follow_target_info_callback(std::bind(fill_follow_target_info, _1));

    // start following
    FollowMe::Result followme_result = device.followme().start();
    ASSERT_EQ(FollowMe::Result::SUCCESS, followme_result);

    // Let the device follow you (GCS) for half a min, say.
    sleep_for(seconds(30));

    // De-register follow target callback.
    device.followme().deregister_follow_target_info_callback();

    // stops following
    followme_result = device.followme().stop();
    ASSERT_EQ(FollowMe::Result::SUCCESS, followme_result);
    sleep_for(seconds(2)); // to watch flight mode change from "FollowMe" to default "HOLD"

    action_ret = device.action().land();
    ASSERT_EQ(Action::Result::SUCCESS, action_ret);
}

void print(const FollowMe::Config &config)
{
    std::cout << "Current FollowMe configuration of the device" << std::endl;
    std::cout << "---------------------------" << std::endl;
    std::cout << "Min Height: " << config.min_height_m << "m" << std::endl;
    std::cout << "Distance: " << config.follow_dist_m << "m" << std::endl;
    std::cout << "Responsiveness: " << config.responsiveness << std::endl;
    std::cout << "Following from: " << FollowMe::Config::to_str(config.follow_dir) << std::endl;
    std::cout << "---------------------------" << std::endl;
}

void fill_follow_target_info(FollowMe::FollowTargetInfo &info)
{
    static FollowMe::FollowTargetInfo spiral_path[] = {
        { 47.39779928779934, 8.54559461331354, 500.0, 50.f, 50.f, 50.f, 10.f, 10.f, 0.f, {4.f, 3.f, 0.f} },
        { 47.39780291, 8.54557048, 489.0, 50.f, 50.f, 50.f, 10.f, 10.f, 0.f, {4.f, 3.f, 0.f} },
        { 47.39779838, 8.5455517, 500.0, 50.f, 50.f, 50.f, 10.f, 10.f, 0.f, {4.f, 3.f, 0.f} },
        { 47.39778748, 8.54554499, 500.0, 50.f, 50.f, 50.f, 10.f, 10.f, 0.f, {4.f, 3.f, 0.f} },
        { 47.39777659, 8.54553561, 500.0, 50.f, 50.f, 50.f, 10.f, 10.f, 0.f, {4.f, 3.f, 0.f} },
        { 47.39776569, 8.54553292, 500.0, 50.f, 50.f, 50.f, 10.f, 10.f, 0.f, {4.f, 3.f, 0.f} },
        { 47.39774663, 8.54552622, 500.0, 50.f, 50.f, 50.f, 10.f, 10.f, 0.f, {4.f, 3.f, 0.f} },
        { 47.39772938, 8.54552488, 500.0, 50.f, 50.f, 50.f, 10.f, 10.f, 0.f, {4.f, 3.f, 0.f} },
        { 47.39771304, 8.54554231, 500.0, 50.f, 50.f, 50.f, 10.f, 10.f, 0.f, {4.f, 3.f, 0.f} },
        { 47.39770578, 8.5455745, 500.0, 50.f, 50.f, 50.f, 10.f, 10.f, 0.f, {4.f, 3.f, 0.f} },
        { 47.39770487, 8.54559596, 500.0, 50.f, 50.f, 50.f, 10.f, 10.f, 0.f, {4.f, 3.f, 0.f} },
        { 47.39770578, 8.54561741, 500.0, 50.f, 50.f, 50.f, 10.f, 10.f, 0.f, {4.f, 3.f, 0.f} },
        { 47.39770669, 8.54563887, 500.0, 50.f, 50.f, 50.f, 10.f, 10.f, 0.f, {4.f, 3.f, 0.f} },
        { 47.39771486, 8.54565765, 500.0, 50.f, 50.f, 50.f, 10.f, 10.f, 0.f, {4.f, 3.f, 0.f} },
        { 47.39773029, 8.54567642, 500.0, 50.f, 50.f, 50.f, 10.f, 10.f, 0.f, {4.f, 3.f, 0.f} },
        { 47.39775026, 8.54568447, 500.0, 50.f, 50.f, 50.f, 10.f, 10.f, 0.f, {4.f, 3.f, 0.f} },
        { 47.39776751, 8.54569118, 500.0, 50.f, 50.f, 50.f, 10.f, 10.f, 0.f, {4.f, 3.f, 0.f} },
        { 47.39778203, 8.54569118, 500.0, 50.f, 50.f, 50.f, 10.f, 10.f, 0.f, {4.f, 3.f, 0.f} },
        { 47.39779838, 8.54568447, 500.0, 50.f, 50.f, 50.f, 10.f, 10.f, 0.f, {4.f, 3.f, 0.f} },
        { 47.39781835, 8.54566972, 500.0, 50.f, 50.f, 50.f, 10.f, 10.f, 0.f, {4.f, 3.f, 0.f} },
        { 47.39782107, 8.54564692, 500.0, 50.f, 50.f, 50.f, 10.f, 10.f, 0.f, {4.f, 3.f, 0.f} },
        { 47.3978247, 8.54561876, 500.0, 50.f, 50.f, 50.f, 10.f, 10.f, 0.f, {4.f, 3.f, 0.f} },
        { 47.3978247, 8.54559193, 500.0, 50.f, 50.f, 50.f, 10.f, 10.f, 0.f, {4.f, 3.f, 0.f} },
        { 47.3978247, 8.54556511, 500.0, 50.f, 50.f, 50.f, 10.f, 10.f, 0.f, {4.f, 3.f, 0.f} },
        { 47.39782107, 8.54553427, 500.0, 50.f, 50.f, 50.f, 10.f, 10.f, 0.f, {4.f, 3.f, 0.f} },
        { 47.39780836, 8.54552756, 500.0, 50.f, 50.f, 50.f, 10.f, 10.f, 0.f, {4.f, 3.f, 0.f} },
        { 47.39779656, 8.54551549, 500.0, 50.f, 50.f, 50.f, 10.f, 10.f, 0.f, {4.f, 3.f, 0.f} },
        { 47.39777568, 8.54550342, 500.0, 50.f, 50.f, 50.f, 10.f, 10.f, 0.f, {4.f, 3.f, 0.f} },
        { 47.3977548, 8.54549671, 500.0, 50.f, 50.f, 50.f, 10.f, 10.f, 0.f, {4.f, 3.f, 0.f} },
        { 47.39773755, 8.54549671, 500.0, 50.f, 50.f, 50.f, 10.f, 10.f, 0.f, {4.f, 3.f, 0.f} },
        { 47.39771849, 8.54550208, 500.0, 50.f, 50.f, 50.f, 10.f, 10.f, 0.f, {4.f, 3.f, 0.f} },
        { 47.39770396, 8.54551415, 500.0, 50.f, 50.f, 50.f, 10.f, 10.f, 0.f, {4.f, 3.f, 0.f} },
        { 47.39769398, 8.54554097, 500.0, 50.f, 50.f, 50.f, 10.f, 10.f, 0.f, {4.f, 3.f, 0.f} },
        { 47.39768762, 8.54556243, 500.0, 50.f, 50.f, 50.f, 10.f, 10.f, 0.f, {4.f, 3.f, 0.f} },
        { 47.39768672, 8.54557852, 500.0, 50.f, 50.f, 50.f, 10.f, 10.f, 0.f, {4.f, 3.f, 0.f} },
        { 47.3976849, 8.54559998, 500.0, 50.f, 50.f, 50.f, 10.f, 10.f, 0.f, {4.f, 3.f, 0.f} },
        { 47.39768399, 8.54562278, 500.0, 50.f, 50.f, 50.f, 10.f, 10.f, 0.f, {4.f, 3.f, 0.f} },
        { 47.39768399, 8.54564155, 500.0, 50.f, 50.f, 50.f, 10.f, 10.f, 0.f, {4.f, 3.f, 0.f} },
        { 47.39769035, 8.54566569, 500.0, 50.f, 50.f, 50.f, 10.f, 10.f, 0.f, {4.f, 3.f, 0.f} },
        { 47.39770759, 8.54568983, 500.0, 50.f, 50.f, 50.f, 10.f, 10.f, 0.f, {4.f, 3.f, 0.f} },
        { 47.39772757, 8.54569922, 500.0, 50.f, 50.f, 50.f, 10.f, 10.f, 0.f, {4.f, 3.f, 0.f}},
        { 47.39774481, 8.54570727, 500.0, 50.f, 50.f, 50.f, 10.f, 10.f, 0.f, {4.f, 3.f, 0.f} },
        { 47.39776025, 8.54572202, 500.0, 50.f, 50.f, 50.f, 10.f, 10.f, 0.f, {4.f, 3.f, 0.f} },
        { 47.39778567, 8.54572336, 500.0, 50.f, 50.f, 50.f, 10.f, 10.f, 0.f, {4.f, 3.f, 0.f} },
        { 47.39780291, 8.54572202, 500.0, 50.f, 50.f, 50.f, 10.f, 10.f, 0.f, {4.f, 3.f, 0.f} },
        { 47.39782107, 8.54571263, 500.0, 50.f, 50.f, 50.f, 10.f, 10.f, 0.f, {4.f, 3.f, 0.f} },
        { 47.39783469, 8.54569788, 500.0, 50.f, 50.f, 50.f, 10.f, 10.f, 0.f, {4.f, 3.f, 0.f} },
        { 47.39783832, 8.54568179, 500.0, 50.f, 50.f, 50.f, 10.f, 10.f, 0.f, {4.f, 3.f, 0.f} },
        { 47.39784104, 8.54566569, 500.0, 50.f, 50.f, 50.f, 10.f, 10.f, 0.f, {4.f, 3.f, 0.f} },
        { 47.39784376, 8.54564424, 500.0, 50.f, 50.f, 50.f, 10.f, 10.f, 0.f, {4.f, 3.f, 0.f} },
        { 47.39784386, 8.54564435, 500.0, 50.f, 50.f, 50.f, 10.f, 10.f, 0.f, {4.f, 3.f, 0.f} },
        { 47.39784396, 8.54564444, 500.0, 50.f, 50.f, 50.f, 10.f, 10.f, 0.f, {4.f, 3.f, 0.f} },
        { 47.39784386, 8.54564454, 500.0, 50.f, 50.f, 50.f, 10.f, 10.f, 0.f, {4.f, 3.f, 0.f} },
        { 47.39784346, 8.54564464, 500.0, 50.f, 50.f, 50.f, 10.f, 10.f, 0.f, {4.f, 3.f, 0.f} },
        { 47.39784336, 8.54564424, 500.0, 50.f, 50.f, 50.f, 10.f, 10.f, 0.f, {4.f, 3.f, 0.f} },
        { 47.39772757, 8.54569922, 500.0, 50.f, 50.f, 50.f, 10.f, 10.f, 0.f, {4.f, 3.f, 0.f} },
        { 47.39774481, 8.54570727, 500.0, 50.f, 50.f, 50.f, 10.f, 10.f, 0.f, {4.f, 3.f, 0.f} },
        { 47.39776025, 8.54572202, 500.0, 50.f, 50.f, 50.f, 10.f, 10.f, 0.f, {4.f, 3.f, 0.f} },
        { 47.39778567, 8.54572336, 500.0, 50.f, 50.f, 50.f, 10.f, 10.f, 0.f, {4.f, 3.f, 0.f} },
        { 47.39770396, 8.54551415, 500.0, 50.f, 50.f, 50.f, 10.f, 10.f, 0.f, {4.f, 3.f, 0.f} },
        { 47.39769398, 8.54554097, 500.0, 50.f, 50.f, 50.f, 10.f, 10.f, 0.f, {4.f, 3.f, 0.f} },
        { 47.39768762, 8.54556243, 500.0, 50.f, 50.f, 50.f, 10.f, 10.f, 0.f, {4.f, 3.f, 0.f} },
        { 47.39768672, 8.54557852, 500.0, 50.f, 50.f, 50.f, 10.f, 10.f, 0.f, {4.f, 3.f, 0.f} },
        { 47.3976849, 8.54559998, 500.0, 50.f, 50.f, 50.f, 10.f, 10.f, 0.f, {4.f, 3.f, 0.f} },
        { 47.39779928779934, 8.54559461331354, 500.0, 50.f, 50.f, 50.f, 10.f, 10.f, 0.f, {4.f, 3.f, 0.f} },
        { 47.39780291, 8.54557048, 489.0, 50.f, 50.f, 50.f, 10.f, 10.f, 0.f, {4.f, 3.f, 0.f} },
        { 47.39779838, 8.5455517, 500.0, 50.f, 50.f, 50.f, 10.f, 10.f, 0.f, {4.f, 3.f, 0.f} },
        { 47.39778748, 8.54554499, 500.0, 50.f, 50.f, 50.f, 10.f, 10.f, 0.f, {4.f, 3.f, 0.f} },
        { 47.39777659, 8.54553561, 500.0, 50.f, 50.f, 50.f, 10.f, 10.f, 0.f, {4.f, 3.f, 0.f} },
        { 47.39776569, 8.54553292, 500.0, 50.f, 50.f, 50.f, 10.f, 10.f, 0.f, {4.f, 3.f, 0.f} },
        { 47.39774663, 8.54552622, 500.0, 50.f, 50.f, 50.f, 10.f, 10.f, 0.f, {4.f, 3.f, 0.f} },
        { 47.39772938, 8.54552488, 500.0, 50.f, 50.f, 50.f, 10.f, 10.f, 0.f, {4.f, 3.f, 0.f} },
        { 47.39771304, 8.54554231, 500.0, 50.f, 50.f, 50.f, 10.f, 10.f, 0.f, {4.f, 3.f, 0.f} }
    };
    static int index = 0;

    info = spiral_path[index++];
    // for now, lets leave others unchanged.
}

#pragma once

#include <functional>
#include <memory>
#include "plugin_base.h"

namespace dronecore {

class GimbalImpl;
class Device;

/**
 * @brief The Gimbal class enables to control a gimbal.
 *
 * Synchronous and asynchronous variants of the gimbal methods are supplied.
 */
class Gimbal : public PluginBase
{
public:
    /**
     * @brief Constructor. Creates the plugin for a specific Device.
     *
     * The plugin is typically created as shown below:
     *
     *     ```cpp
     *     auto gimbal = std::make_shared<Gimbal>(device);
     *     ```
     *
     * @param device The specific device associated with this plugin.
     */
    explicit Gimbal(Device &device);

    /**
     * @brief Destructor (internal use only).
     */
    ~Gimbal();

    /**
     * @brief Possible results returned for gimbal commands.
     */
    enum class Result {
        SUCCESS = 0, /**< @brief Success. The gimbal command was accepted. */
        ERROR, /**< @brief Error. An error occured sending the command. */
        TIMEOUT, /**< @brief Timeout. A timeout occured sending the command. */
        UNKNOWN /**< @brief Unspecified error. */
    };

    /**
     * @brief Returns a human-readable English string for Gimbal::Result.
     *
     * @param result The enum value for which a human readable string is required.
     * @return Human readable string for the Gimbal::Result.
     */
    static const char *result_str(Result result);

    /**
     * @brief Callback type for asynchronous Gimbal calls.
     */
    typedef std::function<void(Result)> result_callback_t;

    /**
     * @brief Set gimbal pitch and yaw angles (synchronous).
     *
     * This sets the desired pitch and yaw angles of a gimbal.
     * The function will return when the command is accepted, however, it might
     * take the gimbal longer to actually be set to the new angles.
     *
     * @param pitch_deg The pitch angle in degrees. Negative to point down.
     * @param yaw_deg The yaw angle in degrees. Positive for clock-wise, range -180..180 or 0..360.
     * @return Result of request.
     */
    Result set_pitch_and_yaw(float pitch_deg, float yaw_deg);

    /**
     * @brief Set gimbal pitch and yaw angles (asynchronous).
     *
     * This sets the desired pitch and yaw angles of a gimbal.
     * The callback will be called when the command is accepted, however, it might
     * take the gimbal longer to actually be set to the new angles.
     *
     * @param pitch_deg The pitch angle in degrees. Negative to point down.
     * @param yaw_deg The yaw angle in degrees. Positive for clock-wise, range -180..180 or 0..360.
     * @param callback Function to call with result of request.
     */
    void set_pitch_and_yaw_async(float pitch_deg, float yaw_deg, result_callback_t callback);

    /**
     * @brief Copy constructor (object is not copyable).
     */
    Gimbal(const Gimbal &) = delete;

    /**
     * @brief Equality operator (object is not copyable).
     */
    const Gimbal &operator=(const Gimbal &) = delete;

private:
    /** @private Underlying implementation, set at instantiation */
    std::unique_ptr<GimbalImpl> _impl;
};

} // namespace dronecore

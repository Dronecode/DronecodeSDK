#include "info_impl.h"
#include "device_impl.h"
#include "global_include.h"
#include <functional>

namespace dronecore {

InfoImpl::InfoImpl() :
    PluginImplBase(),
    _version_mutex(),
    _version()
{
}

InfoImpl::~InfoImpl()
{
}

void InfoImpl::init()
{
    using namespace std::placeholders; // for `_1`

    _parent->register_mavlink_message_handler(
        MAVLINK_MSG_ID_HEARTBEAT,
        std::bind(&InfoImpl::process_heartbeat, this, _1), (void *)this);

    _parent->register_mavlink_message_handler(
        MAVLINK_MSG_ID_AUTOPILOT_VERSION,
        std::bind(&InfoImpl::process_autopilot_version, this, _1), (void *)this);
}

void InfoImpl::deinit()
{
    _parent->unregister_all_mavlink_message_handlers(this);
}

void InfoImpl::process_heartbeat(const mavlink_message_t &message)
{
    UNUSED(message);

    if (!is_complete()) {
        // We try to request more info if not all info is available.
        // We can't rely on DeviceImpl to request the autopilot_version,
        // so we do it here, anyway.
        _parent->request_autopilot_version();
    }
}

void InfoImpl::process_autopilot_version(const mavlink_message_t &message)
{
    mavlink_autopilot_version_t autopilot_version;

    mavlink_msg_autopilot_version_decode(&message, &autopilot_version);

    set_uuid(autopilot_version.uid);

    Info::Version version = {};

    version.flight_sw_major = (autopilot_version.flight_sw_version >> (8 * 3)) & 0xFF;
    version.flight_sw_minor = (autopilot_version.flight_sw_version >> (8 * 2)) & 0xFF;
    version.flight_sw_patch = (autopilot_version.flight_sw_version >> (8 * 1)) & 0xFF;

    // first three bytes of flight_custon_version (little endian) describe vendor version
    translate_binary_to_str(autopilot_version.flight_custom_version + 3,
                            sizeof(autopilot_version.flight_custom_version) - 3,
                            version.flight_sw_git_hash,
                            Info::GIT_HASH_STR_LEN);

    version.flight_sw_vendor_major = autopilot_version.flight_custom_version[2];
    version.flight_sw_vendor_minor = autopilot_version.flight_custom_version[1];
    version.flight_sw_vendor_patch = autopilot_version.flight_custom_version[0];

    version.os_sw_major = (autopilot_version.os_sw_version >> (8 * 3)) & 0xFF;
    version.os_sw_minor = (autopilot_version.os_sw_version >> (8 * 2)) & 0xFF;
    version.os_sw_patch = (autopilot_version.os_sw_version >> (8 * 1)) & 0xFF;

    // Debug() << "flight version: "
    //     << version.flight_sw_major
    //     << "."
    //     << version.flight_sw_minor
    //     << "."
    //     << version.flight_sw_patch;

    // Debug() << "os version: "
    //     << version.os_sw_major
    //     << "."
    //     << version.os_sw_minor
    //     << "."
    //     << version.os_sw_patch;

    translate_binary_to_str(autopilot_version.os_custom_version,
                            sizeof(autopilot_version.os_custom_version),
                            version.os_sw_git_hash,
                            Info::GIT_HASH_STR_LEN);

    set_version(version);
}

void InfoImpl::translate_binary_to_str(uint8_t *binary, unsigned binary_len,
                                       char *str, unsigned str_len)
{
    for (unsigned i = 0; i < binary_len; ++i) {
        // One hex number occupies 2 chars.
        // The binary is in little endian, therefore we need to swap the bytes for us to read.
        snprintf(&str[i * 2], str_len - i * 2,
                 "%02x",
                 ((uint8_t *)binary)[binary_len - 1 - i]);
    }
}

uint64_t InfoImpl::get_uuid() const
{
    std::lock_guard<std::mutex> lock(_uuid_mutex);
    return _uuid;
}

void InfoImpl::set_uuid(uint64_t uuid)
{
    std::lock_guard<std::mutex> lock(_uuid_mutex);
    _uuid = uuid;
}


bool InfoImpl::is_complete() const
{
    {
        std::lock_guard<std::mutex> lock(_version_mutex);

        // TODO: check OS version, it's currently just 0 for SITL
        if (_version.flight_sw_major == 0/* || _version.os_sw_major == 0*/) {
            return false;
        }
    }

    return true;
}

Info::Version InfoImpl::get_version() const
{
    std::lock_guard<std::mutex> lock(_version_mutex);
    return _version;
}

void InfoImpl::set_version(Info::Version version)
{
    std::lock_guard<std::mutex> lock(_version_mutex);
    _version = version;
}


} // namespace dronecore

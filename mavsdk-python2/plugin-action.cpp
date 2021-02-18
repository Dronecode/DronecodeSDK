#include <pybind11/stl.h>
#include <pybind11/pybind11.h>

#include <mavsdk/mavsdk.h>
#include <mavsdk/plugins/action/action.h>

namespace py = pybind11;
void init_action(py::module_& m, py::class_<mavsdk::System>& system)
{
    py::class_<mavsdk::Action> action(m, "Action");

    action.def("arm", &mavsdk::Action::arm)
        .def("disarm", &mavsdk::Action::disarm)
        .def("takeoff", &mavsdk::Action::takeoff)
        .def("land", &mavsdk::Action::land)
        .def("reboot", &mavsdk::Action::reboot)
        .def("shutdown", &mavsdk::Action::shutdown)
        .def("terminate", &mavsdk::Action::terminate)
        .def("kill", &mavsdk::Action::kill)
        .def("return_to_launch", &mavsdk::Action::return_to_launch)
        .def("goto_location", &mavsdk::Action::goto_location)
        .def("do_orbit", &mavsdk::Action::do_orbit)
        .def("transition_to_fixedwing", &mavsdk::Action::transition_to_fixedwing)
        .def("transition_to_multicopter", &mavsdk::Action::transition_to_multicopter)
        .def("get_takeoff_altitude", &mavsdk::Action::get_takeoff_altitude)
        .def("set_takeoff_altitude", &mavsdk::Action::set_takeoff_altitude)
        .def("get_maximum_speed", &mavsdk::Action::get_maximum_speed)
        .def("set_maximum_speed", &mavsdk::Action::set_maximum_speed)
        .def("get_return_to_launch_altitude", &mavsdk::Action::get_return_to_launch_altitude)
        .def("set_return_to_launch_altitude", &mavsdk::Action::set_return_to_launch_altitude);

    py::enum_<mavsdk::Action::OrbitYawBehavior>(action, "OrbitYawBehavior")
        .value("HoldFrontToCircleCenter", mavsdk::Action::OrbitYawBehavior::HoldFrontToCircleCenter)
        .value("HoldInitialHeading", mavsdk::Action::OrbitYawBehavior::HoldInitialHeading)
        .value("Uncontrolled", mavsdk::Action::OrbitYawBehavior::Uncontrolled)
        .value(
            "HoldFrontTangentToCircle", mavsdk::Action::OrbitYawBehavior::HoldFrontTangentToCircle)
        .value("RcControlled", mavsdk::Action::OrbitYawBehavior::RcControlled);

    py::enum_<mavsdk::Action::Result>(action, "Result")
        .value("Unknown", mavsdk::Action::Result::Unknown)
        .value("Success", mavsdk::Action::Result::Success)
        .value("NoSystem", mavsdk::Action::Result::NoSystem)
        .value("ConnectionError", mavsdk::Action::Result::ConnectionError)
        .value("Busy", mavsdk::Action::Result::Busy)
        .value("CommandDenied", mavsdk::Action::Result::CommandDenied)
        .value(
            "CommandDeniedLandedStateUnknown",
            mavsdk::Action::Result::CommandDeniedLandedStateUnknown)
        .value("CommandDeniedNotLanded", mavsdk::Action::Result::CommandDeniedNotLanded)
        .value("Timeout", mavsdk::Action::Result::Timeout)
        .value("VtolTransitionSupportUnknown", mavsdk::Action::Result::VtolTransitionSupportUnknown)
        .value("NoVtolTransitionSupport", mavsdk::Action::Result::NoVtolTransitionSupport)
        .value("ParameterError", mavsdk::Action::Result::ParameterError);

    system.def(
        "action",
        &mavsdk::System::action,
        py::return_value_policy::reference_internal,
        py::keep_alive<1, 0>());
}
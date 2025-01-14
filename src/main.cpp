#include <sbg_device.h>

using sbg::SbgDevice;

int main(int argc, char **argv)
{
  rclcpp::init(argc, argv);
  auto node_handle = std::make_shared<rclcpp::Node>("sbg_device");

  try
  {
    uint32_t loopFrequency;

    RCLCPP_INFO(node_handle->get_logger(), "SBG DRIVER - Init node, load params and connect to the device.");
    SbgDevice sbg_device(*node_handle);

    RCLCPP_INFO(node_handle->get_logger(), "SBG DRIVER - Initialize device for receiving data");
    sbg_device.initDeviceForReceivingData();

    loopFrequency = sbg_device.getUpdateFrequency();
    RCLCPP_INFO(node_handle->get_logger(), "SBG DRIVER - ROS Node frequency : %u Hz", loopFrequency);
    rclcpp::Rate loop_rate(loopFrequency);

    rclcpp::executors::SingleThreadedExecutor executor;
    executor.add_node(node_handle);

    std::chrono::duration<int, std::nano> timeout(1000000);
    while (rclcpp::ok())
    {
      sbg_device.periodicHandle();
      executor.spin_some(timeout);
      loop_rate.sleep();
    }

    rclcpp::shutdown();
    return 0;
  }
  catch (std::exception const& refE)
  {
    RCLCPP_ERROR(node_handle->get_logger(), "SBG_DRIVER - %s", refE.what());
  }

  return 0;
}

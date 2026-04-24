#include <memory>

#include "rclcpp/rclcpp.hpp"
#include "nav_msgs/msg/odometry.hpp"

class OdomFrameFixer : public rclcpp::Node
{
public:
    OdomFrameFixer()
    : Node("odom_frame_fixer")
    {
        sub_ = this->create_subscription<nav_msgs::msg::Odometry>(
            "/Odometry",
            10,
            std::bind(&OdomFrameFixer::callback, this, std::placeholders::_1)
        );

        pub_ = this->create_publisher<nav_msgs::msg::Odometry>(
            "/lio_odom_fixed",
            10
        );

        RCLCPP_INFO(this->get_logger(), "odom_frame_fixer started");
    }

private:
    void callback(const nav_msgs::msg::Odometry::SharedPtr msg)
    {
        auto odom = *msg;

        odom.header.frame_id = "odom";
        odom.child_frame_id = "base_footprint";

        pub_->publish(odom);
    }

    rclcpp::Subscription<nav_msgs::msg::Odometry>::SharedPtr sub_;
    rclcpp::Publisher<nav_msgs::msg::Odometry>::SharedPtr pub_;
};

int main(int argc, char ** argv)
{
    rclcpp::init(argc, argv);

    auto node = std::make_shared<OdomFrameFixer>();

    rclcpp::spin(node);

    rclcpp::shutdown();

    return 0;
}
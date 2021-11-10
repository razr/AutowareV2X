#include "autoware_v2x/v2x_node.hpp"
#include "autoware_v2x/v2x_app.hpp"
#include "autoware_v2x/time_trigger.hpp"
#include "autoware_v2x/router_context.hpp"
#include "autoware_v2x/positioning.hpp"
#include "autoware_v2x/security.hpp"
#include "autoware_v2x/link_layer.hpp"
#include "autoware_v2x/cpm_application.hpp"

#include "rclcpp/rclcpp.hpp"
#include "std_msgs/msg/string.hpp"

#include <vanetza/asn1/cpm.hpp>
#include <vanetza/facilities/cpm_functions.hpp>
#include <sstream>
#include <memory>
#include <boost/thread.hpp>
#include <boost/date_time/posix_time/posix_time.hpp>

namespace gn = vanetza::geonet;

using namespace vanetza;
using namespace vanetza::facilities;
using namespace std::chrono;

namespace v2x
{
  V2XNode::V2XNode(const rclcpp::NodeOptions &node_options) : rclcpp::Node("autoware_v2x_node", node_options)
  // io_service_(),
  //trigger_(io_service_),
  // runtime_(new Runtime(Clock::at(boost::posix_time::microsec_clock::universal_time()))),
  // device_name_("wlp4s0"),
  // device_(device_name_),
  // mac_address_(device_.address()),
  // link_layer_(create_link_layer(io_service_, device_, "ethernet")),
  // positioning_(create_position_provider(io_service_, trigger_.runtime())),
  // security_(create_security_entity(trigger_.runtime(), *positioning_)),
  // mib_(),
  // cp_(new CpmApplication(this, runtime_)),
  // context_(mib_, trigger_, *positioning_, security_.get()),
  // app_()
  {
    using std::placeholders::_1;
    subscription_ = this->create_subscription<autoware_perception_msgs::msg::DynamicObjectArray>("/perception/object_recognition/objects", 10, std::bind(&V2XNode::objectsCallback, this, _1));

    subscription_pos_ = this->create_subscription<tf2_msgs::msg::TFMessage>("/tf", 10, std::bind(&V2XNode::tfCallback, this, _1));
    RCLCPP_INFO(get_logger(), "V2X Node Launched");

    // device_name_ = "wlp4s0";
    // device_(device_name_);
    // mac_address_ = device_.address();

    // std::stringstream sout;
    // sout << mac_address_;
    // RCLCPP_INFO(get_logger(), "MAC Address: '%s'", sout.str().c_str());

    // trigger_(io_service_);

    // link_layer_ = create_link_layer(io_service_, device_, "ethernet");
    // mib_.itsGnLocalGnAddr.mid(mac_address_);
    // mib_.itsGnLocalGnAddr.is_manually_configured(true);
    // mib_.itsGnLocalAddrConfMethod = geonet::AddrConfMethod::Managed;
    // mib_.itsGnSecurity = false;
    // mib_.itsGnProtocolVersion = 1;

    // context_(mib_, trigger_, *positioning_, security_.get()),
    // context_.router_.set_address(mib_.itsGnLocalGnAddr);
    // context_.updateMIB(mib_);

    // positioning_ = create_position_provider(io_service_, trigger_.runtime());
    // security_ = create_security_entity(trigger_.runtime(), *positioning_);

    // RouterContext context_(mib_, trigger_, *positioning_, security_.get());
    // RouterContext context_(mib_, trigger_, *positioning_, security_.get());
    // context_.set_link_layer(link_layer_.get());

    // std::unique_ptr<CpmApplication> cp_ { new CpmApplication(this) };
    // app_ = std::move(cp_);

    // context_.enable(cp_.get());

    // io_service_.run();
    // boost::thread(boost::bind(&boost::asio::io_service::run, &io_service_));
    app = new V2XApp(this);
    boost::thread v2xApp(boost::bind(&V2XApp::start, app));
    // boost::thread v2xApp(V2XApp);
    // boost::thread v2xApp(&V2XApp, this);

    // // Print MAC Address to logger
    // std::stringstream sout;
    // sout << mac_address;
    // RCLCPP_INFO(get_logger(), "MAC Address: '%s'", sout.str().c_str());
  }

  void V2XNode::objectsCallback(const autoware_perception_msgs::msg::DynamicObjectArray::ConstSharedPtr msg)
  {
    // RCLCPP_INFO(get_logger(), "I heard: '%s'", msg->data.c_str());
    RCLCPP_INFO(get_logger(), "V2X: %d objects detected!", msg->objects.size());
    // Send CPM
    // cp_->send(msg, this, pos_lat_, pos_lon_);
    // cp_->updateObjectsStack(msg, this);
    app->objectsCallback(msg);

  }

  void V2XNode::tfCallback(const tf2_msgs::msg::TFMessage::ConstSharedPtr msg)
  {
    // RCLCPP_INFO(get_logger(), "Ego Position: (%f, %f, %f)", msg->transforms[0].transform.translation.x, msg->transforms[0].transform.translation.y, msg->transforms[0].transform.translation.z);

    app->tfCallback(msg);
  }
}

#include "rclcpp_components/register_node_macro.hpp"
RCLCPP_COMPONENTS_REGISTER_NODE(v2x::V2XNode)
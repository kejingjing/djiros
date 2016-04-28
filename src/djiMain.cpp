#include <iostream>
#include <stdio.h>
#include <string.h>
#include <thread>
#include "djiMain.h"
#include "ros_interface.h"
#include <signal.h>

using namespace dji_variable;

// defined in ros_interface.cpp
extern double g_gravity;

int DJI_Setup(std::string serial_port, int baudrate) {
	int ret;
	char uart_name[32];
	strcpy(uart_name, serial_port.c_str());
	printf("Serial port: %s\n", uart_name);
	printf("Baudrate: %d\n", baudrate);
	printf("=========================\n");
	
	//Serial Port Init
	ret = Pro_Hw_Setup(uart_name,baudrate);
	if(ret < 0)
		return ret;

	//Setup Other Things
	DJI_Pro_Setup(NULL);
	return 0;
}

// void update_ros_vars() {

// 	static int frame_id = 0;
// 	frame_id ++;

// 	auto current_time = ros::Time::now();

// 	//update attitude msg
// 	attitude_quad.header.frame_id = "/world";
// 	attitude_quad.header.stamp = current_time;
// 	attitude_quad.q0 = recv_sdk_std_msgs.q.q0;
// 	attitude_quad.q1 = recv_sdk_std_msgs.q.q1;
// 	attitude_quad.q2 = recv_sdk_std_msgs.q.q2;
// 	attitude_quad.q3 = recv_sdk_std_msgs.q.q3;
// 	attitude_quad.wx = recv_sdk_std_msgs.w.x;
// 	attitude_quad.wy = recv_sdk_std_msgs.w.y;
// 	attitude_quad.wz = recv_sdk_std_msgs.w.z;
// 	attitude_quad.ts = recv_sdk_std_msgs.time_stamp;
// 	publishers::att_quad_pub.publish(attitude_quad);

// 	//update global_position msg
// 	global_position.header.frame_id = "/world";
// 	global_position.header.stamp = current_time;
// 	global_position.ts = recv_sdk_std_msgs.time_stamp;
// 	global_position.latitude = recv_sdk_std_msgs.pos.lati;
// 	global_position.longitude = recv_sdk_std_msgs.pos.longti;
// 	global_position.height = recv_sdk_std_msgs.pos.height;
// 	global_position.altitude = recv_sdk_std_msgs.pos.alti;
// 	global_position.health= recv_sdk_std_msgs.pos.health_flag;
// 	global_position_degree = global_position;
// 	global_position_degree.latitude = global_position.latitude * 180.0f /M_PI;
// 	global_position_degree.longitude = global_position.longitude * 180.0f /M_PI;
// 	publishers::gps_pub.publish(global_position);

// 	static int seted = 0;
// 	//TODO:
// 	// FIX BUG about flying at lat = 0
// 	if (global_position.ts != 0 && seted == 0 && global_position.latitude != 0) {
// 		dji_variable::global_position_ref = global_position;
// 		seted = 1;
// 	}


// 	//update velocity msg
// 	velocity.header.frame_id = "/world";
// 	velocity.header.stamp = current_time;
// 	velocity.ts = recv_sdk_std_msgs.time_stamp;
// 	velocity.velx = recv_sdk_std_msgs.v.x;
// 	velocity.vely = recv_sdk_std_msgs.v.y;
// 	velocity.velz = recv_sdk_std_msgs.v.z;
// 	publishers::vel_pub.publish(velocity);

// 	//update accelration msg
// 	acc.header.frame_id = "/world";
// 	acc.header.stamp = current_time;
// 	acc.ts = recv_sdk_std_msgs.time_stamp;
// 	acc.ax = recv_sdk_std_msgs.a.x;
// 	acc.ay = recv_sdk_std_msgs.a.y;
// 	acc.az = recv_sdk_std_msgs.a.z;
// 	publishers::acc_pub.publish(acc);

// 	//update gimbal msg
// 	gimbal_info.header.frame_id = "/gimbal";
// 	gimbal_info.header.stamp= current_time;
// 	gimbal_info.ts = recv_sdk_std_msgs.time_stamp;
// 	gimbal_info.roll = recv_sdk_std_msgs.gimbal.x;
// 	gimbal_info.pitch = recv_sdk_std_msgs.gimbal.y;
// 	gimbal_info.yaw = recv_sdk_std_msgs.gimbal.z;
// 	publishers::gimbal_info_pub.publish(gimbal_info);

// 	//update local_position msg
// 	local_position.header.frame_id = "/world";
// 	local_position.header.stamp = current_time;
// 	dji_variable::gps_convert_ned(
// 			local_position.x,
// 			local_position.y,
// 			global_position.longitude,
// 			global_position.latitude,
// 			dji_variable::global_position_ref.longitude,
// 			dji_variable::global_position_ref.latitude
// 			);
// 	local_position.height = global_position.height;
// 	local_position.ts = global_position.ts;
// 	dji_variable::local_position_ref = local_position;
// 	publishers::local_pos_pub.publish(local_position);

// 	//update odem msg
// 	odem.header.frame_id = "/world";
// 	odem.header.stamp = current_time;
// 	odem.pose.pose.position.x = local_position.x;
// 	odem.pose.pose.position.y = local_position.y;
// 	odem.pose.pose.position.z = local_position.height;
// 	odem.pose.pose.orientation.w = attitude_quad.q0;
// 	odem.pose.pose.orientation.x = attitude_quad.q1;
// 	odem.pose.pose.orientation.y = attitude_quad.q2;
// 	odem.pose.pose.orientation.z = attitude_quad.q3;
// 	odem.twist.twist.angular.x = attitude_quad.wx;
// 	odem.twist.twist.angular.y = attitude_quad.wy;
// 	odem.twist.twist.angular.z = attitude_quad.wz;
// 	odem.twist.twist.linear.x = velocity.velx;
// 	odem.twist.twist.linear.y = velocity.vely;
// 	odem.twist.twist.linear.z = velocity.velz;
// 	publishers::odem_publisher.publish(odem);

// 	//update rc_channel msg
// 	rc_channels.header.frame_id = "/rc";
// 	rc_channels.header.stamp = current_time;
// 	rc_channels.ts = recv_sdk_std_msgs.time_stamp;
// 	rc_channels.pitch = recv_sdk_std_msgs.rc.pitch;
// 	rc_channels.roll = recv_sdk_std_msgs.rc.roll;
// 	rc_channels.mode = recv_sdk_std_msgs.rc.mode;
// 	rc_channels.gear_up = recv_sdk_std_msgs.rc.gear;
// 	rc_channels.throttle = recv_sdk_std_msgs.rc.throttle;
// 	rc_channels.yaw = recv_sdk_std_msgs.rc.yaw;
// 	publishers::rc_channels_pub.publish(rc_channels);

// 	//update compass msg
// 	compass_info.header.frame_id = "/world";
// 	compass_info.header.stamp = current_time;
// 	compass_info.ts = recv_sdk_std_msgs.time_stamp;
// 	compass_info.x = recv_sdk_std_msgs.mag.x;
// 	compass_info.y = recv_sdk_std_msgs.mag.y;
// 	compass_info.z = recv_sdk_std_msgs.mag.z;
// 	publishers::compass_pub.publish(compass_info);


// }
//----------------------------------------------------------
// timer spin_function 50Hz
//----------------------------------------------------------
// void spin_callback(const ros::TimerEvent &)
// {

// 	DJI_Pro_Get_Broadcast_Data(&recv_sdk_std_msgs);

// 	update_ros_vars();
// 	static unsigned int count = 0;
// 	count++;
// 	if (count % 50 == 0) {
// 		std_msgs::Float32 msg;
// 		unsigned char bat = 0;
		
		
// 		//update flight_status 
// 		msg.data = (float) recv_sdk_std_msgs.status;
// 		publishers::flight_status_pub.publish(msg);

// 		//update battery msg
// 		DJI_Pro_Get_Bat_Capacity(&bat);
// 		msg.data = (float)bat;
// 		publishers::battery_pub.publish(msg);

// 		//update ctrl_info
// 		ctrl_info.cur_ctrl_dev_in_navi_mode = recv_sdk_std_msgs.ctrl_info.cur_ctrl_dev_in_navi_mode;
// 		ctrl_info.serial_req_status = recv_sdk_std_msgs.ctrl_info.serial_req_status;
// 		publishers::ctrl_info_pub.publish(ctrl_info);
// 	}
// }

void shutdownSignalHandler(int sig)
{
	api_release_control();
	usleep(500*1000);
	ROS_ERROR("Exit...");
	ros::shutdown();
}


int main(int argc,char **argv) {

	char temp_buf[65];

	ros::init(argc, argv, "DJI_ROS");
	ros::NodeHandle nh;
	ros::NodeHandle nh_private("~");
	
	//publishers::init_publishers(nh);
	// service_handler::init_services(nh);

	nh_private.param("serial_name", serial_name, std::string("/dev/ttyTHS1"));
	nh_private.param("baud_rate", baud_rate, 230400);
	nh_private.param("app_id", app_id, 1022384);
	nh_private.param("app_api_level", app_api_level, 2);
	nh_private.param("app_version", app_version, 1);
	nh_private.param("app_bundle_id", app_bundle_id, std::string("12345678901234567890123456789012"));
	nh_private.param("enc_key", enc_key,
			std::string("e7bad64696529559318bb35d0a8c6050d3b88e791e1808cfe8f7802150ee6f0d"));
	nh_private.param("gravity", g_gravity, 9.80);


	user_act_data.app_id = app_id;
	user_act_data.app_api_level =app_api_level;
	user_act_data.app_ver = SDK_VERSION;
	strncpy((char*)user_act_data.app_bundle_id, app_bundle_id.c_str(),32);

	user_act_data.app_key = temp_buf;
	strcpy(user_act_data.app_key, enc_key.c_str());


	printf("=================================================\n");
	printf("app id: %d\n",user_act_data.app_id);
	printf("api level: %d\n",user_act_data.app_api_level);
	printf("app version: 0x0%X\n",user_act_data.app_ver);
	printf("app key: %s\n",user_act_data.app_key);
	printf("=================================================\n");

	ros::Timer simple_task_timer = nh_private.createTimer(ros::Duration(1.0 / 50.0),  interface_control_timer);
	ros::Subscriber ctrl_sub = nh_private.subscribe("ctrl", 10, interface_control_callback, ros::TransportHints().tcpNoDelay());
	ros::Subscriber gimbal_ctrl_sub = nh_private.subscribe("gimbal_ctrl", 10, interface_gimbal_control_callback, ros::TransportHints().tcpNoDelay());
	ros::Subscriber gimbal_speed_ctrl_sub = nh_private.subscribe("gimbal_speed_ctrl", 10, interface_gimbal_speed_control_callback, ros::TransportHints().tcpNoDelay());

	signal(SIGINT, shutdownSignalHandler);
	if (DJI_Setup(serial_name.c_str(),baud_rate) < 0) {
		printf("Serial Port Cannot Open\n");
		return 0;
	}	
	interface_init(nh_private);

	DJI_Pro_Activate_API(&user_act_data, activate_ack_handle);
	ros::Time activate_time = ros::Time::now();

	ros::Rate r(10);
	while (ros::ok())
	{
		if ((ros::Time::now() - activate_time) > ros::Duration(3.0))
		{
			ROS_ERROR("[ERROR] Activation failed.");
			return -2;
		}
		if (activate_is_successful())
		{
			ROS_WARN("Activate success!");
			break;
		}
	}
	


//just init the service here
//create client in another file

	ros::spin();
	return 0;
}

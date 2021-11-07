#include "ros/ros.h"
//#include "../../../devel/include/serial_node/BoundingBox.h"
#include "serial/serial.h"
#include "stdint.h"
#include <ros/node_handle.h>
#include <../../../devel/include/darknet_ros_msgs//BoundingBox.h>
#include <../../../devel/include/darknet_ros_msgs//BoundingBoxes.h>

#define YOLO_HEADER_HIGH 0x11
#define YOLO_HEADER_LOW 0xa5
#define YOLO_ADDRESS_HIGH 0x00
#define YOLO_ADDRESS_LOW 0x03
#define YOLO_CHANNEL 0x16
#define YOLO_ADD_LENGTH 0
#define  YOLO_HEAD_LENGTH 2
#define  YOLO_PAYLOAD_LENGTH 9
#define YOLO_BAG_LENGTH (YOLO_HEAD_LENGTH+YOLO_ADD_LENGTH+YOLO_PAYLOAD_LENGTH+1)

float center_x;
float center_y;

typedef union {
    uint8_t raw[YOLO_BAG_LENGTH];
    struct {
//        uint8_t ADD[YOLO_ADD_LENGTH];
//        uint8_t CHAN;
        uint8_t head[YOLO_HEAD_LENGTH];
        uint8_t length;
        union {
            uint8_t payload[YOLO_PAYLOAD_LENGTH];
            struct {
                float x;
                float y;
                uint8_t object_num;
            }__attribute__((packed));
        };
    } __attribute__((packed));
} __attribute__((packed)) YOLO_Bag_t;

serial::Serial sp;
YOLO_Bag_t  send_bag;

int serialInit(ros::NodeHandle &nh){
    std::string port;
    int baud_rate;
    nh.param<std::string>("serial_port",port,"/dev/ttyUSB0");
    nh.param<int>("serial_baudrate",baud_rate,9600);
    std::cout << "Try:" << port << "@" << baud_rate << std::endl;
    static serial::Timeout time_out = serial::Timeout::simpleTimeout(100);
    //static do not release time_out until the code finish
    //设置要打开的串口名称
    sp.setPort(port);
    //设置串口通信的波特率
    sp.setBaudrate(baud_rate);
    //串口设置timeout
    sp.setTimeout(time_out);
    try
    {
        //打开串口
        sp.open();
        //判断串口是否打开成功
        if (sp.isOpen())
        {
            //ROS_INFO_STREAM(port<<" is open"<<std::endl);
            std::cout << "[Success] " << port << "@" << baud_rate << " is open" << std::endl;
            return 1;
        }
        else
        {
            std::cerr << "[Error] " << port << "@" << baud_rate << " failed to open" << std::endl;
            return 0;
        }
    }
    catch (serial::IOException &e) //捕捉输入输出异常
    {
        //ROS_ERROR_STREAM("Unable to open port:"<<port<<std::endl);
        std::cerr << e.what() << std::endl;
        return 0;
    }
}

int serialTransmit(uint8_t *buf, int length){
    if (sp.write(buf, length) == length)
    {
        return 1;
    }
    return 0;
}

int serialReceive(uint8_t *buf, uint8_t len = 0)
{
    try
    {
        uint8_t num = len ? len : sp.available();
        if (num && sp.available() >= num)
        {
            return sp.read(buf, num);
        }
        return 0;
    }
    catch (serial::IOException &e)
    {
        std::cerr << e.what() << std::endl;
        return 0;
    }
}

void bag_init(void){
//    send_bag.ADD[0] = YOLO_ADDRESS_HIGH;
//    send_bag.ADD[1] = YOLO_ADDRESS_LOW;
//    send_bag.CHAN = YOLO_CHANNEL;
    send_bag.head[0] = YOLO_HEADER_HIGH;
    send_bag.head[1] = YOLO_HEADER_LOW;
    send_bag.length = YOLO_BAG_LENGTH;
}

void code(void){
    send_bag.x = center_x;
    send_bag.y = center_y;
    send_bag.object_num = 1;
}

void object_callback(const darknet_ros_msgs::BoundingBoxes::ConstPtr &msg)
{

         center_x = (float )((msg->bounding_boxes[0].xmin+msg->bounding_boxes[0].xmax)/2);
         center_y = (float )((msg->bounding_boxes[0].ymin+msg->bounding_boxes[0].ymax)/2);

         code();


}

int flag_ok = 0;
int flag_fail = 0;
//发送给下位机
void stm32_send_handle(const ros::TimerEvent &e){

//    for(int i = 0; i< 2; i++){

        if(serialTransmit(send_bag.raw, YOLO_BAG_LENGTH)){
//            ROS_INFO("successfully!");
                flag_ok++;
        }
        else{
            flag_fail++;
            //ROS_INFO("serial failed");
        }
    ROS_INFO("%d,%d",flag_ok,flag_fail);
//    }

}


int main(int argc, char ** argv){
    ros::init(argc, argv, "serial_node");
    ros::NodeHandle nh("~");
    ros::Subscriber sub = nh.subscribe("/darknet_ros/bounding_boxes",10,object_callback);

    serialInit(nh);
    bag_init();

    //ros::Timer stm32_recv_thread = nh.createTimer( ros::Duration(0.01), stm32_recv_handle );//定时设置收数据频率
    ros::Timer stm32_send_thread = nh.createTimer(ros::Duration(0.0001), stm32_send_handle);//定时设置发数据频率

    ros::AsyncSpinner spinner(0);//多线程订阅，参数为0自动设计线程
    spinner.start();
    ros::waitForShutdown();
    return 0;
}

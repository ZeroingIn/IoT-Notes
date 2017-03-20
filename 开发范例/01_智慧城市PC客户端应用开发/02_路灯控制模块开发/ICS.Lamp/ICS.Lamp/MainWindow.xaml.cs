using ICS.Acquisition;
using ICS.Common;
using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Windows;
using System.Windows.Controls;
using System.Windows.Data;
using System.Windows.Documents;
using System.Windows.Input;
using System.Windows.Media;
using System.Windows.Media.Imaging;
using System.Windows.Navigation;
using System.Windows.Shapes;

namespace ICS.Lamp
{
    /// <summary>
    /// MainWindow.xaml 的交互逻辑
    /// </summary>
    public partial class MainWindow : Window
    {
        public MainWindow()
        {
            InitializeComponent();
        }

        //LED灯开关标志位
        private bool streetLampStatus = false;      //路灯
        private bool corridorLampStatus = false;    //楼道灯

        private void OnOffStreetLamp()
        {
            //修改LED灯开关标志位
            streetLampStatus = !streetLampStatus;
            //调用重载方法去开关LED
            OnOffStreetLamp(streetLampStatus);
        }

        private void OnOffCorridorLamp()
        {
            corridorLampStatus = !corridorLampStatus;
            OnOffCorridorLamp(corridorLampStatus,true);
        }

        /// <summary>
        /// 开关路灯
        /// </summary>
        /// <param name="onOff">true为开false为关</param>
        private void OnOffStreetLamp(bool OnOff)

        {
            //使用Global.ADAM4150Provider.CheckSerialPort来判断相应串口是否打开
            ICS.Common.ResultEntity result = Global.ADAM4150Provider.CheckSerialPort(Global.ADAM4150Provider.ADAM4017Provider);
            //若执行状态（ret.Status）为失败（Result.Failure），则表示串口未打开，设置错误信息用于返回
            if (result.Status == RunStatus.Failure)
            {
                MessageBox.Show(result.ResultMessage);
                return;
            }

            //将ADAM4150数字量控制类赋值给新变量，方便下面的使用
            ADAM4150 adam = Global.ADAM4150Provider;

            //判断是打开还是关闭路灯，adam.OnOff(data);执行命令返回执行结果
            if (OnOff == false)
            {
                //byte[]数组存放关闭路灯命令
                byte[] data = new byte[] { 0x01, 0x05, 0x00, 0x12, 0x00, 0x00, 0x6D, 0xCF };
                OnOff = adam.OnOff(data);
                btnStreetLamp.Content = "开关路灯";
            }
            else
            {
                //byte[]数组存放开启路灯命令
                byte[] data = new byte[] { 0x01, 0x05, 0x00, 0x12, 0xFF, 0x00, 0x2C, 0x3F };
                OnOff = adam.OnOff(data);
                btnStreetLamp.Content = "关闭路灯";
            }

            //判断执行结果是否成功
            if (OnOff == false)
                MessageBox.Show("开启路灯失败。");       
        }

        /// <summary>
        /// 开关楼道灯
        /// </summary>
        /// <param name="onOff">true为开false为关</param>
        /// <param name="infrared">用于控制是否需要红外线感应才开启</param>
        private void OnOffCorridorLamp(bool OnOff, bool infrared)
        {
            //使用Global.ADAM4150Provider.CheckSerialPort来判断相应串口是否打开
            ICS.Common.ResultEntity result = Global.ADAM4150Provider.CheckSerialPort(Global.ADAM4150Provider.ADAM4017Provider);
            //若执行状态（retData.Status）为失败（Result.Failure），则表示串口未打开，设置错误信息用于返回
            if (result.Status == RunStatus.Failure)
            {
                MessageBox.Show(result.ResultMessage);
                return;
            }

            //将ADAM4150数字量控制类赋值给新变量，方便下面的使用
            ADAM4150 adam = Global.ADAM4150Provider;

            //判断是打开还是关闭楼道灯，byte[]数组存放开关灯命令，adam.OnOff(data);执行命令返回执行结果
            if (OnOff == false)
            {
                //byte[]数组存放关闭楼道灯命令
                byte[] data = new byte[] { 0x01, 0x05, 0x00, 0x11, 0x00, 0x00, 0x9D, 0xCF };
                OnOff = adam.OnOff(data);
                btnCorridorLamp.Content = "开关楼道灯";
            }
            else
            {
                //byte[]数组存放开启楼道灯命令
                byte[] data = new byte[] { 0x01, 0x05, 0x00, 0x11, 0xFF, 0x00, 0xDC, 0x3F };
                OnOff = adam.OnOff(data);
                btnCorridorLamp.Content = "关闭楼道灯";
            }

            if (OnOff == false)
                MessageBox.Show("操作楼道灯失败！");
        }

        private void btnStreetLamp_Click(object sender, RoutedEventArgs e)
        {
            OnOffStreetLamp();
        }

        private void btnCorridorLamp_Click(object sender, RoutedEventArgs e)
        {
            OnOffCorridorLamp();
        }
    }
}

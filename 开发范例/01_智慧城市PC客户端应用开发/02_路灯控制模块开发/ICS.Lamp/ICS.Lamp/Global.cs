using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using ICS.Acquisition;

namespace ICS.Lamp
{
    class Global
    {
        public static ADAM4150 ADAM4150Provider
        {
            get
            {
                //使用单例工厂创建ADAM4150数字量控制类
                return (ADAM4150)ClassFactory.GetProvider(equipmentCategory.ADAM4150,Global.ComSetting);
            }
        }
        public static ICS.Models.Com.ComSettingModel _ComSetting = null;
        public static ICS.Models.Com.ComSettingModel ComSetting
        {
            get
            {
                if (_ComSetting == null)
                {
                    //使用ICS.Common.SettingHelper获取串口设置
                    _ComSetting = new ICS.Common.SettingHelper<ICS.Models.Com.ComSettingModel>().GetSetting();
                    //代码直接设置模拟量串口
                    _ComSetting.AnalogQuantityCom = "Com2";
                    //代码直接设置数字量串口
                    _ComSetting.DigitalQuantityCom = "Com2";
                }
                return _ComSetting;
            }
        }
    }
}

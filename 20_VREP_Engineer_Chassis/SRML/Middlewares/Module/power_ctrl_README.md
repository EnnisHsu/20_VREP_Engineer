## power_ctrl

#### 一、使用方法
**初始化**  
阅读构造函数，根据构造函数定义功率控制类  
`PowerCtrl_ClassDef PowerCtrl(CAP_power, ENABLE_power)；`  
加载外部控制器  
`Load_CapChargeController(CapChargeController)；`  
`Load_MotorLimitController(MotorLimitController)；`   

**使用**  
根据需要设置RF最大功率、电机最大功率、剩余能量使用最大值  
`Set_PE_Max(80, 120, 30);`  
运行主控制函数,传入当前RF功率、电机功率、剩余能量以及电机的输出值  
`Control(Power_RF, Power_Motor, RemainEnergy);`   
读取电机的限幅比例，并用自写限幅函数进行限幅  
`Get_LimScale();`  
读取电容允许的充电功率,并进行充电输出  
`Get_ChargePower()；`  

**注意**  
1.如果一些测试机构不需要用到功率控制，不需要改变其他的内容，只需要在构造函数中设置DISABLE就可以了，此时当读取电机限幅比例会一直都是1.0，相当于不不限幅。  
2.配合chassis.cpp模块使用时  
1)由于chassis需要设置不同的速度挡位，所以你得先知道多大的功率有多大的速度，然后将不同的功率对应的挡位初始化到chassis中。  
2)在变换功率时设置相应的合理速度挡位，才能更好地进行功率控制。举个例子，我当前使用当前功率为80W(对应步兵1.9m/s)，那我应该设置的是低速档或者中速档，而不是高速档。

#### 二、内部结构与实现
分为RF供电和CAP供电两种模式进行控制。  
**CAP供电模式**   
输出功率裕度比较大，直接用电流采样板测到的电机总功率进行电机限幅。  
RF给CAP充电，要充分利用RF的最大功率，所以用RF剩余能量进行闭环充电。  
**RF供电模式(待加入)**  
输出功率裕度比较小，用RF的剩余能量进行电机限幅。并判断电机是否收到限幅，以此来决定是否给电容充电，假如电机没有收到限幅，就用剩余能量进行闭环充电；假如电机受到限幅，把电容允许充电功率限制设置为0。   




#### 三、备注
**枚举类型**  
看起来会有点奇怪，如：  
`typedef enum ENABLE_Enum_power
{
	DISABLE_power,
	ENABLE_power	
}ENABLE_e; `   
这是为了防止在其他文件中也有DISABLE、ENABLE等的定义，所以加入模块后缀，防止重定义错误。  



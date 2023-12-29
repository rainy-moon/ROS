# 代码

#### 介绍
一个简单的自制基于内存的图形多道程序32位操作系统。基于单核intel-386及以上架构，支持不同分辨率屏幕模式显卡适配。

#### 软件架构
见其他相关文件

#### 使用准备
+ MinGW或其他windows操作系统c语言编译工具
+ bochs或VMware虚拟机（bochs效果较差）
+ 一个空软盘或空镜像文件
+ imhex或其他类似的16进制修改软件

#### 安装教程&使用说明
> ##### 仅安装使用
>>1.  在演示文件夹下找到rosboot.bin文件，大小为软盘大小
>>2.  打开VMware虚拟机，自行配置一个虚拟机，注意配置单核，且选择稍后安装操作系统。
>>3.  创建虚拟机后添加软盘驱动器，并选择使用rosboot.bin文件
>>4. 正常启动虚拟机即可开机，系统自动选择软盘中的操作系统。

> ##### 修改代码后使用
> >1. 确保有Makefile文件、安装windows操作系统下的make且有路径。保存代码后先使用make clear清除已有.o文件在代码。然后使用make sysinit.bin编译。编译器使用windows操作系统下的c语言编译工具即可（建议使用MinGW）
> >2. 编译后使用16制编辑器（推荐imhex）打开sysinit.bin文件和rosboot.bin文件。
> >3. 将sysinit.bin文件的所有内容复制到rosboot.bin文件0x2E0开始处进行覆盖（0x2E0上方应该有一行0x90数据作为留空）。
> >4. 如果修改了roshead.asm文件，请使用make roshead.bin生成roshead.bin文件。使用16进制编辑器将roshead.bin文件内容全部复制到rosboot.bin文件0x200开始处进行覆盖。（0x200上一行最后有0x55AA主引导程序标记）
> >5. 如果修改了rosboot.asm文件，请使用make rosboot.bin生成新的rosboot.bin文件，然后重复上述流程修改。
> >6. 之后按照上方使用流程使用
> 
> 注意：
> 1. 修改后的总sysinit.bin文件不可超过1020KB
> 2. 现在开机读取磁盘内容仅读取10个柱面、2磁头、18扇区，总计180KB，去掉固定引导程序512B，预留512B系统32位设置程序，sysinit.bin主程序如超过179KB则需要重新修改rosboot.asm读取更大的磁盘内容。
> 3. 使用bochs虚拟机会有明显卡顿，且需要进行设置。设置参数可以参考演示文件夹下bochesout.txt。如无法支持显卡VESA BIOS请上网自行搜索切换教程。


#### 特色功能

# 毫无特色:)

# 内存分配图

|        内存地址段         |      地址段功能       |   地址大小   |
|:-------------------------:|:---------------------:|:------------:|
| 0x0000 0000 - 0x000f ffff |      实模式地址       |      1M      |
| 0X0010 0000 - 0X0010 01ff |         boot          |     512B     |
| 0x0010 0200 - 0x0010 03ff |    32位转换程序块     |     512B     |
| 0x0010 0400 - 0x0010 0fff |        空地址         |     3KB      |
| 0x0010 1000 - 0x001f ffff | rosinit操作系统主程序 |    1020KB    |
| 0x0020 0000 - 0x002f ffff |        空地址         |      1M      |
| 0x0030 0000 - 0x003f ffff |       栈缓冲区        |      1M      |
| 0x0040 0000 - 0x0040 ffff |       页占用表        |     64KB     |
| 0x0041 0000 - 0x004f ffff |        空地址         |   1M-64KB    |
| 0x0050 0000 - 0x0050 ffff |          GDT          |     64K      |
| 0x0051 0000 - 0x0051 0fff |          IDT          |  4K(只用2K)  |
| 0x0051 1000 - 0x0051 ffff |        空地址         |     60K      |
| 0x0052 0000 - 0x1fff ffff |       用户内存        | 512M-5M-128K |

# 特殊地址

| 地址   | 功能                    |
| ------ | ----------------------- |
| 0xff1  | 键盘led指示灯状态       |
| 0xff2  | 色数（bpp）（8位256色） |
| 0xff4  | x分辨率（int）          |
| 0xff8  | y分辨率                 |
| 0xffc  | vram地址                |
| 0x1000 | VESA BIOS分辨率信息            |
| 0x2000 | VBE模式信息             | 

---

# 多任务pid和GDT表项的关系

$prograsses[Index] = pid - 1 = gs - 3$

---

# 任务流程 mesa 语义

$$
\begin{CD}

\boxed{
	\begin{gather}
		初始化多任务控制器(multipc\_ctrl)\\
		注册主程序p0（pid=1；gs+3）\\
		注册任务切换计时器（20ms）	
	\end{gather}
}\\
@VVV\\
\boxed{
	\begin{gather}
		create\_task\\
		预分配16K栈空间\\
		prograsses[Index] = pid - 1 = gs - 3
	\end{gather}
}\\
@VVV\\
\boxed{
	\begin{gather}
		register\_task\\
		设置gdt表项，标记为就绪，进入就绪队列
	\end{gather}
} @>>> 
\boxed{
	就绪
}@<<<\\
@VVV @VVV @AAA\\
\boxed{
	时钟中断int20
}@>>> \boxed{
	运行
}@>>>\\
@VVV @VVV @AAA\\
\boxed{
	semWait->PSleep
}@>>>\boxed{
	不可中断挂起
}\\
@VVV @VVV \quad @AAA\\
\boxed{
	semSignal->PAwake
}@>>>\boxed{
	就绪
}@>>>
\end{CD}
$$






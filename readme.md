# PE64904 SPI 控制程序

## 描述

本程序通过 Arduino 和 SPI 接口控制 PE64904 数字可调电容器。用户可以设置和获取电容器状态，重置设备，并启用自动状态循环测试。

## 功能

- **设置状态**：将电容设置为特定状态（0-31）。
- **获取状态**：获取当前状态以及对应的电容值和电阻值。
- **重置状态**：将电容重置为初始状态（状态 0）。
- **自动步进模式**：每秒自动循环切换状态，用于测试。
- **动态计算**：根据状态动态计算电容和电阻值。

## 指令

通过串口监视器向 Arduino 发送以下指令：

- `SET <state>`：将电容器设置为指定状态（0-31）。
- `GET`：获取当前状态信息。
- `RESET`：将电容器重置为初始状态（状态 0）。
- `START`：启用自动步进模式。
- `STOP`：关闭自动步进模式。

## 使用方法

1. 根据 SPI 接口定义，将 PE64904 连接到 Arduino。
2. 将代码上传到 Arduino。
3. 打开串口监视器（波特率：9600）。
4. 使用上述指令与设备交互。

## 依赖

- Arduino SPI 库

## 示例

### 上电后

```bash
=== PE64904 System Information ===
SPI Settings:
  Clock Divider: 8
  Data Mode: SPI_MODE0
  Bit Order: MSBFIRST

Use commands: SET <state>, GET, RESET, START, STOP.
====================================
```

### 设置状态

输入 `SET 15`

``` bash
=== PE64904 State Update ===
New State: 15
Capacitance: 2.54 pF
Resistance: 1.93 Ω
============================
```

### 获取当前状态

`GET`

```bash
=== PE64904 State Update ===
New State: 15
Capacitance: 2.54 pF
Resistance: 1.93 Ω
============================
```

### 启用自动步进模式

`START`

```bash
Auto step mode started.
```

### 关闭自动步进模式

`STOP`

```bash
Auto step mode stopped.
```

## 许可证

本项目基于 MIT 许可证进行分发。

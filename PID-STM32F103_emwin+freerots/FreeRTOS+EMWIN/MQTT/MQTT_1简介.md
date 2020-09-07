# <font size =11>MQTT协议(一)报文简介</font>
- 来源于网络资料整理
  ## <font size =8>MQTT</font>
  - 由于MQTT是基于TCP的应用层协议。而硬件串口WIFI已经实现了TCP/IP层。所以只需要关注应用层MQTT实现
  - MQTT协议的数据包格式非常简单，由<b>固定报头</b>、<b>可变报头</b>、<b>有效载荷</b>三个部分组成。下面会简要说明三个部分具体有哪些内容:
<table  border="0" align="center">
<tr>
<td align="center"><b>固定报头</b></td>
<td align="center">控制报文类型</td>
<td align="center">标志位</td>
<td align="center">剩余长度</td>
</tr>
<tr>
<td align="center"><b>可变报头</b></td>
<td colspan="3" align="center">可变报头</td>
<tr>
<td align="center"><b>有效载荷</b></td>
<td colspan="3" align="center">业务数据</td>
</tr>
</table>

### <b><font size =5>一、固定报头</font></b>
<table border="1" align="center">
   <tr>
      <td width="27" align="center">bit</td>
      <td width="27" align="center">7</td>
      <td width="25" align="center">6</td>
      <td width="25" align="center">5</td>
      <td width="25" align="center">4</td>
      <td width="25" align="center">3</td>
      <td width="25" align="center">2</td>
      <td width="25" align="center">1</td>
      <td width="25" align="center">0</td>
   </tr>
   <tr >
      <td align="center">byte1</td>
      <td colspan="4" align="center" > MQTT控制报文的类型</td>
      <td colspan="4" align="center" width="200">用于指定报文类型的标志位</td>
   </tr>
   <tr>
      <td  width="111"align="center">byte2、3、4、5</td>
      <td colspan="8" align="center">剩余长度，最大四个字节</td>
   </tr>
</table>

- 其中<b>MQTT控制报文类型</b>(首字节高四位bit7~bit4)如下16种类型:
<table border="1" align="center">
<tr>
<td align="center">报文类型</td>
<td align="center">字段值</td>
<td align="center">数据方向</td>
<td align="center">描述</td>
</tr>
<tr>
<td align="center">保留</td>
<td align="center">0</td>
<td align="center">禁用</td>
<td align="center">保留</td>
</tr>
<tr>
<td align="center">CONNECT</td>
<td align="center">1</td>
<td align="center">Cilent-->Sever</td>
<td align="center">客户端请求链接服务器</td>
</tr>
<tr>
<td align="center">CONNACT</td>
<td align="center">2</td>
<td align="center">Sever-->Cilent</td>
<td align="center">连接确认</td>
</tr>
<tr>
<td align="center">PUBLISH</td>
<td align="center">3</td>
<td align="center">Cilent<-->Sever</td>
<td align="center">发布消息</td>
</tr>
<tr>
<td align="center">PUBACK</td>
<td align="center">4</td>
<td align="center">Cilent<-->Sever</td>
<td align="center">发布确认</td>
</tr>
<tr>
<td align="center">PUBREC</td>
<td align="center">5</td>
<td align="center">Cilent<-->Sever</td>
<td align="center">消息已接收(QoS2第一阶段)</td>
</tr>
<tr>
<td align="center">PUBREL</td>
<td align="center">6</td>
<td align="center">Cilent<-->Sever</td>
<td align="center">消息释放(QoS2第二阶段)</td>
</tr>
<tr>
<td align="center">PUBCOMP</td>
<td align="center">7</td>
<td align="center">Cilent<-->Sever</td>
<td align="center">发布结束(QOS2第三阶段)</td>
</tr>
<tr>
<td align="center">SUBSCRIBE</td>
<td align="center">8</td>
<td align="center">Cilent-->Sever</td>
<td align="center">客户端订阅请求</td>
</tr>
<tr>
<td align="center">SUBACK</td>
<td align="center">9</td>
<td align="center">Sever-->Cilent</td>
<td align="center">服务端订阅确认</td>
</tr>
<tr>
<td align="center">UNSUBSCRIBE</td>
<td align="center">10</td>
<td align="center">Cilent-->Sever</td>
<td align="center">客户端取消订阅</td>
</tr>
<tr>
<td align="center">UNSUBACK</td>
<td align="center">11</td>
<td align="center">Sever-->Cilent</td>
<td align="center">服务端取消订阅确认</td>
</tr>
<tr>
<td align="center">PINGREQ</td>
<td align="center">12</td>
<td align="center">Cilent-->Sever</td>
<td align="center">客户端发送心跳</td>
</tr>
<tr>
<td align="center">PINGRESP</td>
<td align="center">13</td>
<td align="center">Sever-->Cilent</td>
<td align="center">服务端回复心跳</td>
</tr>
<tr>
<td align="center">DISCONNECT</td>
<td align="center">14</td>
<td align="center">Cilent-->Sever</td>
<td align="center">客户端断开连接请求</td>
</tr>
<tr>
<td align="center">保留</td>
<td align="center">15</td>
<td align="center">禁用</td>
<td align="center">保留</td>
</tr>
</table >

- 其中<b>指定控制报文的标志位</b>(首字节低四位bit3~bit0)如下,仅PUBLISH控制报文有使用(其他时候报文缺省):<br>
每种报文都对应了相关的标志位，标志位如下表所列，如果收到非法的标志，接收者 必须关闭网络连接。报文标志列表如下：
<table border="1" align="center">
<tr>
<td align="center">控制报文</td>
<td align="center">固定报头标志</td>
<td align="center">Bit3</td>
<td align="center">Bit2</td>
<td align="center">Bit1</td>
<td align="center">Bit0</td>
</tr>
<tr>
<td align="center">CONNECT</td>
<td align="center">保留</td>
<td align="center">0</td>
<td align="center">0</td>
<td align="center">0</td>
<td align="center">0</td>
</tr>
<tr>
<td align="center">CONNACK</td>
<td align="center">保留</td>
<td align="center">0</td>
<td align="center">0</td>
<td align="center">0</td>
<td align="center">0</td>
</tr>
<tr>
<td align="center">PUBLISH</td>
<td align="center">MQTT3.1.1使用</td>
<td align="center">DUP</td>
<td align="center">Qos</td>
<td align="center">Qos</td>
<td align="center">RETAIN</td>
</tr>
<tr>
<td align="center">PUBACK</td>
<td align="center">保留</td>
<td align="center">0</td>
<td align="center">0</td>
<td align="center">0</td>
<td align="center">0</td>
</tr>
<tr>
<td align="center">PUBREC</td>
<td align="center">保留</td>
<td align="center">0</td>
<td align="center">0</td>
<td align="center">0</td>
<td align="center">0</td>
</tr>
<tr>
<td align="center">PUBREL</td>
<td align="center">保留</td>
<td align="center">0</td>
<td align="center">0</td>
<td align="center">1</td>
<td align="center">0</td>
</tr>
<tr>
<td align="center">PUBCOMP</td>
<td align="center">保留</td>
<td align="center">0</td>
<td align="center">0</td>
<td align="center">0</td>
<td align="center">0</td>
</tr>
<tr>
<td align="center">SUBSCRIBE</td>
<td align="center">保留</td>
<td align="center">0</td>
<td align="center">0</td>
<td align="center">1</td>
<td align="center">0</td>
</tr>
<tr>
<td align="center">SUBACK</td>
<td align="center">保留</td>
<td align="center">0</td>
<td align="center">0</td>
<td align="center">0</td>
<td align="center">0</td>
</tr>
<tr>
<td align="center">UNSUBSCRIBE</td>
<td align="center">保留</td>
<td align="center">0</td>
<td align="center">0</td>
<td align="center">1</td>
<td align="center">0</td>
</tr>
<tr>
<td align="center">UNSUBACK</td>
<td align="center">保留</td>
<td align="center">0</td>
<td align="center">0</td>
<td align="center">0</td>
<td align="center">0</td>
</tr>
<tr>
<td align="center">PINGREQ</td>
<td align="center">保留</td>
<td align="center">0</td>
<td align="center">0</td>
<td align="center">0</td>
<td align="center">0</td>
</tr>
<tr>
<td align="center">PINGRESP</td>
<td align="center">保留</td>
<td align="center">0</td>
<td align="center">0</td>
<td align="center">0</td>
<td align="center">0</td>
</tr>
<tr>
<td align="center">DISCONNECT</td>
<td align="center">保留</td>
<td align="center">0</td>
<td align="center">0</td>
<td align="center">0</td>
<td align="center">0</td>
</tr>
</table>

DUP  = 控制报文的重复分发标志<br>
QoS  = PUBLISH报文的服务质量等级<br>
RETAIN= PUBLISH报文的保留标志<br>

  - <b>剩余长度</b>:MQTT报文的第二字节开始是用于标识MQTT数据包长度的字段，最少一个字节，最大四个字节，每一个字节的低7位用于标识值，范围0~127 。<br>

    剩余长度字段使用一个变长度编码方案，对于小于128的值它使用单字节编码。更大的值下面的方式处理。低7位有效位用于编码数据，最高有效位用于指示是否有更多的字节，且按照大端方式进行编码。因此每个字节可以编码128个数值和一个延续位。剩余长度字节最大4个字节。（剩余长度统计为剩余长度后的所有报文字节数，所以剩余长度=报文总字节-剩余长度字节数-1）
<table border="1" align="center">
<tr>
<td align="center">字节数</td>
<td align="center">最小值</td>
<td align="center">最大值</td>
</tr>
<tr>
<td align="center">1</td>
<td align="center">0(0x00)</td>
<td align="center">127(0x7F)</td>
</tr>
<tr>
<td align="center">2</td>
<td align="center">128(0x80,0x01)</td>
<td align="center">16383(0xFF,0x7F)</td>
</tr>
<tr>
<td align="center">3</td>
<td align="center">16384(0x80,0x80,0x01)</td>
<td align="center">2097151(0xFF,0xFF,0x7F)</td>
</tr>
<tr>
<td align="center">4</td>
<td align="center">2097152(0x80,0x80,0x80,0x01)</td>
<td align="center">268435455(0xFF,0xFF,0xFF,0x7F)</td>
</tr>
</table>


### <b><font size =5>二、可变报头</font></b>

 - 某些MQTT控制报文包含一个可变头部分。它在固定报头和负载间。可变报头的内容根据报文类型的不同而不同（可变头部分在具体报文中具体分析），可变报头的报文标识符字段存在于多个类型的报文里。
 - 报文标识符用于标注报文。如果一个客户端要重发这个特殊的控制报文，在随后重发那个报文时，它必须使用相同的标识符。
<table  border="0" align="center">
<tr>
<td>Bit</td>
<td>7</td>
<td>6</td>
<td>5</td>
<td>4</td>
<td>3</td>
<td>2</td>
<td>1</td>
<td>0</td>
</tr>
<tr>
<td>byte1</td>
<td colspan="8" align="center">报文标识符MSB(字符串长度的最高有效字节)</td>
</tr>
<tr>
<td>byte2</td>
<td colspan="8" align="center">报文标识符LSB(字符串长度的最低有效字节)</td>
</tr>
<tr>
</tr>
</table>

如下列举了哪些MQTT控制报文需要报文标识符：
<table border="0" align="center">
<tr>
<td align="center">控制报文</td>
<td align="center">报文标识符字段</td>
</tr>
<tr>
<td align="center">CONNECT</td>
<td align="center">不需要</td>
</tr>
<tr>
<td align="center">CONNACK</td>
<td align="center">不需要</td>
</tr>
<tr>
<td align="center">PUBLISH</td>
<td align="center">需要（如果QoS>0）</td>
</tr>
<tr>
<td align="center">PUBACK</td>
<td align="center">需要</td>
</tr>
<tr>
<td align="center">PUBREC</td>
<td align="center">需要</td>
</tr>
<tr>
<td align="center">PUBREL</td>
<td align="center">需要</td>
</tr>
<tr>
<td align="center">PUBCOMP</td>
<td align="center">需要</td>
</tr>
<tr>
<td align="center">SUBSCRIBE</td>
<td align="center">需要</td>
</tr>
<tr>
<td align="center">SUBACK</td>
<td align="center">需要</td>
</tr>
<tr>
<td align="center">UNSUBSCRIBE</td>
<td align="center">需要</td>
</tr>
<tr>
<td align="center">UNSUBACK</td>
<td align="center">需要</td>
</tr>
<tr>
<td align="center">PINGREQ</td>
<td align="center">不需要</td>
</tr>
<tr>
<td align="center">PINGRESP</td>
<td align="center">不需要</td>
</tr>
<tr>
<td align="center">DISCONNECT</td>
<td align="center">不需要</td>
</tr>
</table>

### <b><font size =5>三、有效载荷</font></b>
  某些MQTT控制报文在报文的最后部分包含一个有效载荷，如下是包含有效载荷的控制报文列表：
<table border="0" align="center">
<tr>
<td align="center">控制报文</td>
<td align="center">有效载荷</td>
</tr>
<tr>
<td align="center">CONNECT</td>
<td align="center">需要</td>
</tr>
<tr>
<td align="center">CONNACK</td>
<td align="center">不需要</td>
</tr>
<tr>
<td align="center">PUBLISH</td>
<td align="center">可选</td>
</tr>
<tr>
<td align="center">PUBACK</td>
<td align="center">不需要</td>
</tr>
<tr>
<td align="center">PUBREC</td>
<td align="center">不需要</td>
</tr>
<tr>
<td align="center">PUBREL</td>
<td align="center">不需要</td>
</tr>
<tr>
<td align="center">PUBCOMP</td>
<td align="center">不需要</td>
</tr>
<tr>
<td align="center">SUBSCRIBE</td>
<td align="center">需要</td>
</tr>
<tr>
<td align="center">SUBACK</td>
<td align="center">需要</td>
</tr>
<tr>
<td align="center">UNSUBSCRIBE</td>
<td align="center">需要</td>
</tr>
<tr>
<td align="center">UNSUBACK</td>
<td align="center">不需要</td>
</tr>
<tr>
<td align="center">PINGREQ</td>
<td align="center">不需要</td>
</tr>
<tr>
<td align="center">PINGRESP</td>
<td align="center">不需要</td>
</tr>
<tr>
<td align="center">DISCONNECT</td>
<td align="center">不需要</td>
</tr>
</table>
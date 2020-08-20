# <font size =11>MQTT协议(三)CONNECT/DISCONNECT与CONNACK报文</font>
- 来源于网络资料整理
- 之前简单了解了MQTT报文数据包的构成，现在就可以了解每种报文的具体形式。
## <font size =8>CONNECT报文</font>
- 客户端到服务端的网络连接建立后，客户端发送给服务端的第一个报文必须是CONNECT,连接服务器报文。
- 在一个网络连接上，客户端只能发送一次CONNECT报文。服务端必须将客户端发送的第二个CONNECT报文当作协议违规处理并断开客户端的连接。
- 服务端可以检查CONNECT报文的内容是不是满足任何进一步的限制，可以执行身份验证和授权检查。如果任何一项检查没通过，它应该发送一个适当的、返回码非零的CONNACK响应，并且必须关闭这个网络连接。
- 客户端连接服务器的CONNECT报文由<b>固定头</b>，<b>剩余长度</b>，<b>可变报头</b>以及<b>有效载荷</b>组成。以下将简介报文数据包具体内容。（十六进制）
<table  border="0" align="center">
<tr>
<td align="center"><b>固定报头</b></td>
<td align="center">固定头</td>
<td align="center">剩余长度</td>
</tr>
<tr>
<td align="center"><b>可变报头</b></td>
<td colspan="2" align="center">可变报头</td>
<tr>
<td align="center"><b>有效载荷</b></td>
<td colspan="2" align="center">业务数据</td>
</tr>
</table>

### <b><font size =5>一、固定头</font></b>
  CONNECT 报文第一部分为1个字节的<b>固定头</b>：0X10
### <b><font size =5>二、剩余长度</font></b>
  CONNECT报文第二部分为N字节,根据剩余长度大小变化，最小一字节，最大为四字节（见MQTT(一)剩余长度部分）<b>剩余长度</b>=报文数据包总长-1-N
### <b><font size =5>二、可变报头</font></b>
  CONNECT报文可变报头有以下四部分组成
- <b><font size =3>协议名（Protocol Name）</font></b>
- <b><font size =3>协议级别（Protocol Level）</font></b>
- <b><font size =3>连接标志（Connect Flags）</font></b>
- <b><font size =3>保持时间（Keep Alive）</font></b>
  
#### ①<b><font size =3>协议名（Protocol Name）</font></b>
MQTT协议根据不同版本有不同协议名(UTF编码方式)，具体使用什么协议版本需要查阅官方说明文档，如:<br>
MQTT3.1为MQlsdp<br>
MQTT3.1.1为MQTT<br>

在MQTT协议名之前需要有两个字节标注协议名字节数<br>
例如：MQTT3.3.1 版本协议名为
<table border="0" align="center">
   <tr>
      <td rowspan="7" align="center">协议名</td>
      <td></td>
      <td align="center">bit7</td>
      <td align="center">bit6</td>
      <td align="center">bit5</td>
      <td align="center">bit4</td>
      <td align="center">bit3</td>
      <td align="center">bit2</td>
      <td align="center">bit1</td>
      <td align="center">bit0</td>
      <td align="center">说明</td>
   </tr>
   <tr>
      <td align="center">byte1</td>
      <td align="center">0</td>
      <td align="center">0</td>
      <td align="center">0</td>
      <td align="center">0</td>
      <td align="center">0</td>
      <td align="center">0</td>
      <td  align="center">0</td>
      <td  align="center">0</td>
      <td  align="center">长度MSB</td>
   </tr>
   <tr>
      <td align="center">byte2</td>
      <td align="center">0</td>
      <td align="center">0</td>
      <td align="center">0</td>
      <td align="center">0</td>
      <td align="center">0</td>
      <td align="center">1</td>
      <td align="center">0</td>
      <td align="center">0</td>
      <td align="center">长度LSB</td>
   </tr>
   <tr>
      <td align="center">byte3</td>
      <td align="center">0</td>
      <td align="center">1</td>
      <td align="center">0</td>
      <td align="center">0</td>
      <td align="center">1</td>
      <td align="center">1</td>
      <td align="center">0</td>
      <td align="center">1</td>
      <td align="center">M</td>
   </tr>
   <tr>
      <td align="center">byte4</td>
      <td align="center">0</td>
      <td align="center">1</td>
      <td align="center">0</td>
      <td align="center">1</td>
      <td align="center">0</td>
      <td align="center">0</td>
      <td align="center">0</td>
      <td align="center">1</td>
      <td align="center">Q</td>
   </tr>
   <tr>
      <td align="center">byte5</td>
      <td align="center">0</td>
      <td align="center">1</td>
      <td align="center">0</td>
      <td align="center">1</td>
      <td align="center">0</td>
      <td align="center">1</td>
      <td align="center">0</td>
      <td align="center">0</td>
      <td align="center">T</td>
   </tr>
   <tr>
      <td align="center">byte6</td>
      <td align="center">0</td>
      <td align="center">1</td>
      <td align="center">0</td>
      <td align="center">1</td>
      <td align="center">0</td>
      <td align="center">1</td>
      <td align="center">0</td>
      <td align="center">0</td>
      <td align="center">T</td>
   </tr>
</table>

#### ②<b><font size =3>协议级别（Protocol Level）</font></b>
MQTT协议级别根据不同的协议名不同，如:<br>
MQlsdp:级别为3<br>
MQTT:级别为4<br>

例如：MQTT3.3.1 版本协议级别为
<table border="0" align="center">
   <tr>
      <td rowspan="2" align="center">协议级别</td>
      <td></td>
      <td align="center">bit7</td>
      <td align="center">bit6</td>
      <td align="center">bit5</td>
      <td align="center">bit4</td>
      <td align="center">bit3</td>
      <td align="center">bit2</td>
      <td align="center">bit1</td>
      <td align="center">bit0</td>
      <td align="center">说明</td>
   </tr>
   <tr>
      <td align="center">byte7</td>
      <td align="center">0</td>
      <td align="center">0</td>
      <td align="center">0</td>
      <td align="center">0</td>
      <td align="center">0</td>
      <td align="center">1</td>
      <td  align="center">0</td>
      <td  align="center">0</td>
      <td  align="center">Level4</td>
   </tr>
   </table>

#### ③<b><font size =3>连接标志（Connect Flags）</font></b>
连接标志字节包含一些用于指定MQTT连接行为的参数。它还指出playload字段是否存在。（即连接标志中启用的标志，有效载荷中一定要存在）
<table border="0" align="center">
   <tr>
      <td align="center">Bit</td>
      <td align="center">7</td>
      <td align="center">6</td>
      <td align="center">5</td>
      <td align="center">4</td>
      <td align="center">3</td>
      <td align="center">2</td>
      <td align="center">1</td>
      <td align="center">0</td>
   </tr>
   <tr>
      <td align="center">标志名</td>
      <td align="center">UserNameFlag</td>
      <td align="center">PasswordFlag</td>
      <td align="center">Will Retain</td>
      <td colspan="2" align="center">Will Qos </td>
      <td align="center">Will Flag</td>
      <td align="center">CleanSession</td>
      <td align="center">Reserved</td>
   </tr>
   <tr>
      <td align="center">byte8</td>
      <td align="center">x</td>
      <td align="center">x</td>
      <td align="center">x</td>
      <td align="center">x</td>
      <td align="center">x</td>
      <td align="center">x</td>
      <td align="center">x</td>
      <td align="center">0</td>
   </tr>
</table>

- 有效载荷（playload）字段的存在与否并不是reserved字段来指定，而是will flag、user Name等字段。比如：如果will flag为1，则负载中需要包含will tpoic、will message等字段。如果需要用户名密码，则负载中需要包含这些信息。
- 服务端必须验证CONNECT控制报文的保留标志位（第0位reserved）是否为0，如果不为0必须断开客户端连接

<table border="0" align="center">
 <tr>
  <td align="center">标志名</td>
  <td align="center">意义</td>
 </tr>
 <tr>
  <td align="center">Reserved</td>
  <td align="center">保留位，为0</td>
 </tr>
 <tr>
  <td align="center">Clean Session</td>
  <td align="center">告诉服务端否需要基于客户端标识符(Cilent ID),在对应的会话session中保存连接断开过程中收到的级别为Qos1和Qos2的消息并在连接恢复后向客户端进行分发</td>
 </tr align="center">
 <tr>
  <td align="center">Will Flag</td>
  <td align="center">是否开启遗嘱消息模式，如果为开启，则服务端需要存储负载中客户端发送的will topic和will message</td>
 </tr>
 <tr>
  <td align="center">Will Qos</td>
  <td align="center">遗嘱消息的安全级别，占用2字节，标识Qos0、Qos1、Qos2三个级别</td>
 </tr>
 <tr>
  <td align="center">Will Retain</td>
  <td align="center">遗嘱消息是否需要作为保留消息发布</td>
 </tr>
 <tr>
  <td align="center">Password Flag</td>
  <td align="center">是否需要密码</td>
 </tr>
 <tr>
  <td align="center">User Name Flag</td>
  <td align="center">是否需要用户名</td>
 </tr>
</table>

#### ④<b><font size =3>保持时间（Keep Alive）</font></b>
<table border="0" align="center">
 <tr>
  <td align="center">Bit </td>
  <td align="center">7</td>
  <td align="center">6</td>
  <td align="center">5</td>
  <td align="center">4</td>
  <td align="center">3</td>
  <td align="center">2</td>
  <td align="center">1</td>
  <td align="center">0</td>
 </tr>
 <tr>
  <td align="center">byte8</td>
  <td colspan="8" align="center">保持连接 Keep Alive MSB</td>
 </tr>
 <tr>
  <td align="center">byte10</td>
  <td colspan="8" align="center">保持连接 Keep Alive LSB</td>
 </tr>
</table>
保持时间(keep Alive)说明: 

- 客户端负责保证控制报文发送的时间间隔不超过保持连接的值。如果没有任何其它的控制报文可以发送，客户端必须发送一个PINGREQ报文。不管保持连接的值是多少，客户端任何时候都可以发送PINGREQ报文，并且使用PINGRESP报文判断网络和服务端的活动状态。
- 如果保持连接的值非零，并且服务端在一点五倍的保持连接时间内没有收到客户端的控制报文，它必须断开客户端的网络连接，认为网络连接已断开。
- 客户端发送了PINGREQ报文(心跳包)之后，如果在合理的时间内仍没有收到PINGRESP报文，它应该关闭到服务端的网络连接。
- 保持连接的值为零表示关闭保持连接功能。这意味着客户端不断开连接。
- 使用两个字节标识Keep Alive
  Keep Alive允许的最大值是18小时12分15秒，这是从2^16/3600=18.2044得出的

### <b><font size =5>三、有效载荷</font></b>
- 有效载荷在前两字节规定载荷的名称长度，后面字节为载荷名称（十六进制）。可参考可变报头的协议名报文的写法。
- 在连接标志中启用的标志，在有效载荷中一定要出现，否则服务器会与客户端连接不成功
- 除了连接标志中的启用的标志以外，还需要有客户端标识符（Cilent ID）
- 顺序为 Cilent ID ->遗嘱主题(若存在)->遗嘱消息(若存在)->用户名(若存在)->密码(若存在)

## <font size =8>CONNACK报文</font>
- CONNACT 报文为客户端与服务器连接后，服务器响应返回给客户端的报文
- 服务端发送给客户端的第一个报文必须是CONNACK。
- CONNACK报文只有<b>固定头</b>、<b>剩余长度</b>、<b>可变报头</b>部分


### <b><font size =5>一、固定报头</font></b>
- CONNACK报文固定头与剩余长度固定为0x20, 0x02
<table border="0" align="center">
 <tr>
  <td width="27" align="center">Bit</td>
  <td width="27" align="center">7</td>
  <td width="27" align="center">6</td>
  <td width="27" align="center">5</td>
  <td width="27" align="center">4</td>
  <td width="27" align="center">3</td>
  <td width="27" align="center">2</td>
  <td width="27" align="center">1</td>
  <td width="27" align="center">0</td>
 </tr>
 <tr>
  <td>byte1</td>
  <td colspan="4" align="center">MQTT控制报文类型</td>
  <td colspan="4" align="center">指定控制报文标志位</td>
 </tr>
 <tr>
  <td></td>
  <td align="center">0</td>
  <td  align="center">0</td>
  <td  align="center">1</td>
  <td  align="center">0</td>
  <td  align="center">0</td>
  <td  align="center">0</td>
  <td  align="center">0</td>
  <td  align="center">0</td>
 </tr>
 <tr>
  <td  align="center">byte2</td>
  <td colspan="8"  align="center">剩余长度（2）</td>
 </tr>
 <tr>
  <td></td>
  <td align="center">0</td>
  <td align="center">0</td>
  <td align="center">0</td>
  <td align="center">0</td>
  <td align="center">0</td>
  <td align="center">0</td>
  <td align="center">1</td>
  <td align="center">0</td>
 </tr>
</table>

### <b><font size =5>二、可变报头</font></b>
CONNACK报文可变报头由以下部分组成
- <b><font size =3>连接确认标志 (Connect Acknowledge Flags)</font></b>
- <b><font size =3>连接返回码 (Connect Return code)</font></b>

<table border="0" align="center">
 <tr>
  <td align="center">描述</td>
  <td align="center">7</td>
  <td align="center">6</td>
  <td align="center">5</td>
  <td align="center">4</td>
  <td align="center">3</td>
  <td align="center">2</td>
  <td align="center">1</td>
  <td align="center">0</td>
 </tr>
 <tr>
  <td>连接确认标志</td>
  <td colspan="7" align="center">Reserved保留位</td>
  <td align="center">SP</td>
 </tr>
 <tr>
  <td align="center">byte1</td>
  <td align="center">0</td>
  <td align="center">0</td>
  <td align="center">0</td>
  <td align="center">0</td>
  <td align="center">0</td>
  <td align="center">0</td>
  <td align="center">0</td>
  <td align="center">X</td>
 </tr>
 <tr>
  <td align="center">连接返回码</td>
  <td colspan="8" align="center">连接返回码</td>
 </tr>
 <tr>
  <td align="center">byte2</td>
  <td align="center">X</td>
  <td align="center">X</td>
  <td align="center">X</td>
  <td align="center">X</td>
  <td align="center">X</td>
  <td align="center">X</td>
  <td align="center">X</td>
  <td align="center">X</td>
 </tr>
</table>

#### ①<b><font size =3>连接确认标志 (Connect Acknowledge Flags)</font></b>

- 第一个字节是连接确认标志，Bit7-1是保留位且必须设置为0。
- 第0位SP是当前会话(Session Present)标志：<br>
如果服务端收到清理会话（CleanSession）标志为1的连接，除了将CONNACK 报文中的返回码设置为0之外，还必须将CONNACK 报文中的当前会话设置（Session Present）标志为0。<br>
如果服务端收到一个CleanSession 为0 的连接，当前会话标志的值取决于服务端是否已经保存了ClientId 对应客户端的会话状态。<br>

#### ②<b><font size =3>连接返回码 (Connect Return code)</font></b>
<table border="0" align="center">
 <tr>
  <td align="center">值</td>
  <td align="center">返回响应码</td>
  <td align="center">描述</td>
 </tr>
 <tr>
  <td align="center">0</td>
  <td align="center">0x00连接已接受</td>
  <td align="center">连接已被服务器接受</td>
 </tr>
 <tr>
  <td align="center">1</td>
  <td align="center">0x01连接已拒绝，不支持的协议版本</td>
  <td align="center">服务端不支持客户端请求的MQTT协议级别</td>
 </tr>
 <tr>
  <td align="center">2</td>
  <td align="center">0x02连接已拒绝，不合格的客户端标识符（Cilent ID）</td>
  <td align="center">客户端标识符是正确的UTF-8编码，但服务端不允许</td>
 </tr>
 <tr>
  <td align="center">3</td>
  <td align="center">0x03连接已拒绝，服务端不可用</td>
  <td align="center">网络连接已建立，但MQTT服务不可用</td>
 </tr>
 <tr>
  <td align="center">4</td>
  <td align="center">0x04连接已拒绝，无效的用户名或密码</td>
  <td align="center">用户名或密码的数据格式无效</td>
 </tr>
 <tr>
  <td align="center">5</td>
  <td align="center">0x05连接已拒绝，未授权</td>
  <td align="center">客户端未授权连接到此服务器</td>
 </tr>
 <tr>
  <td align="center">6-255 </td>
  <td align="center"></td>
  <td align="center">保留</td>
 </tr>
</table>


## <font size =8>DISCONNECT报文</font>
- DISCONNECT没有可变报头和消息体，其固定头部为两个字节：0xE0 0X00
<table border="0" align="center">
 <tr>
  <td width="27" align="center">Bit</td>
  <td width="27" align="center">7</td>
  <td width="27" align="center">6</td>
  <td width="27" align="center">5</td>
  <td width="27" align="center">4</td>
  <td width="27" align="center">3</td>
  <td width="27" align="center">2</td>
  <td width="27" align="center">1</td>
  <td width="27" align="center">0</td>
 </tr>
 <tr>
  <td>byte1</td>
  <td colspan="4" align="center">MQTT控制报文类型</td>
  <td colspan="4" align="center">指定控制报文标志位</td>
 </tr>
 <tr>
  <td></td>
  <td  align="center">1</td>
  <td  align="center">1</td>
  <td  align="center">1</td>
  <td  align="center">0</td>
  <td  align="center">0</td>
  <td  align="center">0</td>
  <td  align="center">0</td>
  <td  align="center">0</td>
 </tr>
 <tr>
  <td  align="center">byte2</td>
  <td colspan="8"  align="center">剩余长度（0）</td>
 </tr>
 <tr>
  <td></td>
  <td align="center">0</td>
  <td align="center">0</td>
  <td align="center">0</td>
  <td align="center">0</td>
  <td align="center">0</td>
  <td align="center">0</td>
  <td align="center">0</td>
  <td align="center">0</td>
 </tr>
</table>

响应动作如下： 
客户端发送DISCONNECT报文后：
- 必须关闭网络连接
- 不能再通过此网络发送任何控制报文
服务端再收到DISCONNECT报文后：
- 必须丢弃任何与当前连接关联的未发布的遗嘱
- 应该关闭网络连接，如果客户端还没有这么做

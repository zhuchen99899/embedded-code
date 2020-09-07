# <font size =11>MQTT协议(四)PINGREQ&PINGRESP报文</font>
## <font size =8>PINGREQ报文</font>
- PINGREQ为心跳请求报文，由客户端发向服务器：PINGREQ具体流程如下:

```mermaid
graph LR
Cilent--PINGREQ-->Broker;
Broker--PINGRESP-->Cilent;
```
- PINGREQ只有<b>固定报头</b>，没有可变报头和消息体
- 即PINGREQ报文只有两个字节：0xC0,0x00
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
  <td align="center">1</td>
  <td  align="center">1</td>
  <td  align="center">0</td>
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

<b>心跳请求的作用：</b>
客户端发送PINGREQ报文给服务端的，用于：
- 1.在没有任何其他控制报文从客户端发送给服务器时，告知服务端客户端还或者
- 2.请求服务器端发送响应确认它还活着。
- 3.使用网络以确认网络连接没有断开。
  
## <font size =8>PINGRESP报文</font>
- PINGRESP为心跳响应报文，由服务器发向客户端
- PINGRESP只有<b>固定报头</b>，没有可变报头和消息体
- 即PINGRESP只有两个字节：0xD0,0x00

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
  <td align="center">1</td>
  <td  align="center">1</td>
  <td  align="center">0</td>
  <td  align="center">1</td>
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
  <td align="center">0</td>
  <td align="center">0</td>
 </tr>
</table>

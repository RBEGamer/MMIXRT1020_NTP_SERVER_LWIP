# MMIXRT1020_NTP_SERVER_LWIP
An NXP MMIXRT1020-EVB SNTP SERVER IMPLEMENTATION USING LWIP


## USED HARDWARE

* MMIXRT1020-EVB
* PCF85063AT RTC SHIELD

## USED SOFTWARE

* MCUExpresso IDE v11.4
* MMIXRT1020-EVB SDK


## CONFIGURATION

### DHCP FALLBACK ADRRESS

By default, the board is configured to obtain his adress by DHCP. 
The system waits 20 seconds for a valid adress on startup
```c++
//lwip_dhcp_bm.c at line 51
#define configIP_ADDR0 192
#define configIP_ADDR1 168
#define configIP_ADDR2 3
#define configIP_ADDR3 102
```

### NTP PACKET CREATION

```c++
//lwip_dhcp_bm.c at line 138
udpecho_raw_recv
```

## EXAMPLE

### QUERY RESULT


```
Querying Server: 192.168.3.48
Received packet from: 192.168.3.48 (48 bytes)
Leap indictor: 0
Version: 3
Mode: 4
Peer Clock Stratum: 1
Peer Polling Interval: 3
Peer Clock Precision: 0.000000
Root Delay: 0.000000
Root Dispersion: 0.000000
Reference ID: GPS
Reference Clock Update Time: 2021-07-27 23:36:27 (3836417787.000000 sec.)
Originate Time Stamp: 1962-02-23 01:37:49 (1961113069.422561645 sec.)
Receive Time Stamp: 2021-07-27 23:36:27 (3836417787.000000 sec.)
Transmit Time Stamp: 2021-07-27 23:36:27 (3836417787.000000 sec.)
```

### Note
 `Root Delay` and `Root Dispersion` is set to `0.00` by default, due to hardcoded values in the ntp packet!

### Error-Handling

`Leap indictor` will be set to `3 (not synced)` and `Peer Clock Stratum` to `16` to notify the client if there is an error with the system.
This can occur, if no gps or rtc is present and/or no data are recieved from the clock sources.

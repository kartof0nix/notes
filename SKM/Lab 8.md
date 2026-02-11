## Konfiguracja statyczna routowania

### Pomiar przepustowości
B 40%
C 80%

*Serwer*:
B: `iperf -s -p 8010`
C:  `iperf -s -p 8011`

*Klient*
B: `iperf -c 127.0.0.1 -p 8010 -t 10`
C:  `iperf -c 127.0.0.1 -p 8010 -t 10`

## 3. Limitowanie ruchu
### Znakowanie pakietów IP
`/ip firewall mangle add action=mark-connection chain=prerouting dst-port=8010 new-connection-mark=markB passthrough=yes protocol=tcp`
`/ip firewall mangle add action=mark-connection chain=prerouting dst-port=8011 new-connection-mark=markC passthrough=yes protocol=tcp`



### Kolejka
`/queue simple add max-limit=40%A/40%A packet-marks=markB name=rlimit target=192.168.5.53`
`/queue simple add max-limit=80%A/80%A packet-marks=markB name=rlimit target=192.168.5.53`


### Kolejka z wiaderkiem
`/queue simple add max-limit=40%A/40%A packet-marks=markB name=rlimit target=192.168.5.53 bucket-size=10/10`
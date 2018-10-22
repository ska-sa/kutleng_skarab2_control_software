# kutleng_skarab2_control_software
The control software for the skarab2

Provides the software that accompanies the kutleng_skarab2_bsp_firmware HDL repo

The folder ./casperbsp/ethernet/100gtesting/udplinktest/client if for testing
the 100G links

Instructions to run:

1. Create an FPGA bitsream gmactop.bit and program the VCU118/VCU1525 using JTAG
2. Connect the FPGA port 0/1 QSFP to the Mellanox 100G Ethernet card
2. Bring up the Mellanox 100G Ethenet card using ifconfig
   
sudo ifconfig ens6 up

In my machine the ethernet device is ens6 (check on your machine it may be different)
The MAC address should look more like this HWaddr 50:6b:4b:c3:fb:ac

I have set my IP address to 192.168.100.150

3. Change folder to kutleng_skarab2_control_software/casperbsp/ethernet/100gtesting/udplinktest/client
4. Make the test application 
make

This will produce udplinktestclient executable

before running the executable make sure the ethernet links are up and running.
5. Use Wireshark to capture all traffic on ens6

sudo wireshark

6. Start a capture on ens6 [the 100G Mellanox ethernet device]

7. Initialise the ARP tables by doing an ICMP request sequence

ping 192.168.100.10

ping 192.168.100.15

Note that the ping will NOT receive and ICMP reply but it will setup the ARP tables.

8. Run the udplinktestclient application

./udplinktestclient

9. Observe the packets captured on Wireshark to confirm that there was data transfer.

You should see packets from size 1 till the MTU size defined in the udplinktestclient application.

10. Change udplinktestclient.c to a different target IP to test the other QSFP port.


